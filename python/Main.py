import cv2
import requests
import time
from ultralytics import YOLO

# =====================================================
# KONFIGURASI
# =====================================================

ESP32_URL = "http://192.168.4.1/detect"

# Model YOLO
model = YOLO(r"D:\GAS TA\TRAINING TERBARU\weights\best.pt")

# Confidence Threshold
CONFIDENCE_THRESHOLD = 0.561

# Delay setelah area kosong
EMPTY_FRAME_DELAY = 1.0

# Kamera
CAMERA_INDEX = 1

# =====================================================
# VARIABEL SISTEM
# =====================================================

last_seen_time = time.time()
is_ready_to_trigger = True

session = requests.Session()

# =====================================================
# INISIALISASI KAMERA
# =====================================================

cap = cv2.VideoCapture(CAMERA_INDEX)

if not cap.isOpened():
    print("ERROR : Kamera tidak ditemukan!")
    exit()

print("==========================================")
print(" YOLO Sorting System Started")
print("==========================================")

# =====================================================
# LOOP
# =====================================================

while True:

    ret, frame = cap.read()

    if not ret:
        print("Frame gagal dibaca.")
        break

    # -------------------------------
    # DETEKSI YOLO
    # -------------------------------

    results = model(frame, verbose=False)[0]

    detected_items = []
    confidence_list = []

    for box in results.boxes:

        conf = float(box.conf[0])

        if conf < CONFIDENCE_THRESHOLD:
            continue

        cls = int(box.cls[0])
        name = model.names[cls].lower()

        if name not in ["bearing", "bolt", "nut"]:
            continue

        detected_items.append(name)
        confidence_list.append(conf)

    current_time = time.time()

    jumlah_barang = len(detected_items)

    # =====================================================
    # LOGIKA SORTING
    # =====================================================

    if jumlah_barang > 0:

        last_seen_time = current_time

        if is_ready_to_trigger:

            kategori_unik = set(detected_items)

            # ---------------------------------------------
            # Lebih dari satu kategori
            # ---------------------------------------------
            if len(kategori_unik) > 1:

                print(f"[{time.strftime('%H:%M:%S')}] REJECT -> {list(kategori_unik)}")

                try:
                    session.get(
                        f"{ESP32_URL}?item=reject",
                        timeout=1
                    )
                except requests.RequestException as e:
                    print("ESP32 Error :", e)

                is_ready_to_trigger = False

            # ---------------------------------------------
            # Semua kategori sama
            # ---------------------------------------------
            else:

                item = detected_items[0]

                # Satu benda

                if jumlah_barang == 1:

                    conf = confidence_list[0]

                    print(
                        f"[{time.strftime('%H:%M:%S')}] "
                        f"{item.upper()} "
                        f"({conf:.2f})"
                    )

                    try:

                        session.get(
                            f"{ESP32_URL}?item={item}",
                            timeout=1
                        )

                    except requests.RequestException as e:
                        print("ESP32 Error :", e)

                    is_ready_to_trigger = False

                # Lebih dari satu benda

                else:

                    print(
                        f"[{time.strftime('%H:%M:%S')}] "
                        f"{jumlah_barang} {item} terdeteksi bersamaan."
                    )

                    print("Servo tidak dibuka.")

    # =====================================================
    # AREA KOSONG
    # =====================================================

    else:

        if (not is_ready_to_trigger) and \
           ((current_time - last_seen_time) >= EMPTY_FRAME_DELAY):

            is_ready_to_trigger = True

            print(f"[{time.strftime('%H:%M:%S')}] READY")

    # =====================================================
    # DISPLAY
    # =====================================================

    annotated = results.plot()

    cv2.putText(
        annotated,
        f"Jumlah : {jumlah_barang}",
        (10, 70),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.8,
        (255,255,0),
        2
    )

    if confidence_list:

        cv2.putText(
            annotated,
            f"Confidence : {max(confidence_list):.2f}",
            (10,105),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.8,
            (0,255,255),
            2
        )

    if is_ready_to_trigger:

        warna = (0,255,0)
        status = "READY"

    else:

        warna = (0,0,255)
        status = "LOCKED"

    cv2.putText(
        annotated,
        f"STATUS : {status}",
        (10,35),
        cv2.FONT_HERSHEY_SIMPLEX,
        1,
        warna,
        2
    )

    cv2.imshow("YOLOv8 Sorting System", annotated)

    # =====================================================
    # EXIT
    # =====================================================

    key = cv2.waitKey(1)

    if key & 0xFF == ord('q'):
        break

# =====================================================
# SELESAI
# =====================================================

cap.release()
cv2.destroyAllWindows()
