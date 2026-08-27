# Sistem Deteksi Objek dan Penyortiran Komponen Mekanik Berbasis YOLOv8 pada Konveyor

Sistem penyortiran otomatis untuk komponen mekanik (**bearing**, **bolt**, dan **nut**) yang mengintegrasikan deteksi objek real-time berbasis **YOLOv8** dengan mekanisme **konveyor** dan **aktuator servo**, dikendalikan oleh mikrokontroler **ESP32** melalui komunikasi **HTTP** pada jaringan WiFi lokal.

Proyek ini dikembangkan sebagai Tugas Akhir Program Studi Sarjana Terapan Teknik Elektro, Fakultas Vokasi, Universitas Negeri Yogyakarta.

## ✨ Fitur

- Deteksi dan klasifikasi objek real-time menggunakan model **YOLOv8s** (Ultralytics)
- Mendukung 3 kelas komponen mekanik: `Bearing`, `Bolt`, `Nut`
- Penyortiran otomatis menggunakan motor servo berdasarkan hasil deteksi
- Komunikasi data hasil deteksi dari komputer ke ESP32 melalui **HTTP** (ESP32 sebagai WiFi Access Point)
- Kontrol konveyor otomatis (motor DC) dengan sensor infrared untuk sinkronisasi posisi objek
- Mekanisme *feeder* untuk mengatur pengeluaran objek satu per satu
- Sistem antrean (*queue*) pada ESP32 untuk menangani beberapa objek yang bergerak secara berurutan

## 🖥️ Arsitektur Sistem

```
[Webcam] → [YOLOv8s / Python] → HTTP Request → [ESP32 (WiFi AP + Web Server)]
                                                        │
                        ┌───────────────────────────────┼───────────────────────────────┐
                        ▼                               ▼                               ▼
                 [Motor DC Konveyor]           [Motor Servo Sortir]           [Sensor Infrared]
                                                (Bearing / Bolt / Nut)         (Deteksi posisi objek)
```

1. **Webcam** menangkap citra objek yang berjalan di atas konveyor.
2. **Model YOLOv8s** (dijalankan di komputer/laptop menggunakan Python) mendeteksi dan mengklasifikasikan objek, menghasilkan nama kelas, nilai *confidence*, dan koordinat *bounding box*.
3. Hasil deteksi dikirim ke **ESP32** melalui **HTTP GET request** (ESP32 berjalan sebagai *Access Point* + *Web Server*).
4. **ESP32** memproses perintah, menghentikan konveyor pada posisi sortir, lalu menggerakkan **motor servo** sesuai kelas objek menuju wadah penampungan yang sesuai.
5. **Sensor infrared** digunakan untuk mendeteksi keberadaan objek dan menyinkronkan proses deteksi dengan pergerakan aktuator.

## 🛠️ Perangkat Keras

| Komponen | Fungsi |
|---|---|
| ESP32 | Mikrokontroler utama, WiFi Access Point, Web Server |
| Webcam | Akuisisi citra untuk deteksi objek |
| Motor DC + Driver L298N | Penggerak konveyor |
| Motor Servo (×4) + PCA9685 | Aktuator sortir (Bearing, Bolt, Nut) dan *feeder* |
| Sensor Infrared | Deteksi keberadaan objek pada konveyor |
| Power Supply | Catu daya sistem elektronik |

## 📦 Perangkat Lunak & Library

**Sisi Komputer (Python):**
- Python 3.x
- [Ultralytics YOLOv8](https://github.com/ultralytics/ultralytics)
- OpenCV

**Sisi ESP32 (Arduino/C++):**
- `WiFi.h`
- `WebServer.h`
- `Wire.h`
- `Adafruit_PWMServoDriver.h`

## 📁 Struktur Proyek

```
├── firmware/                  # Kode program ESP32
│   ├── main.ino                 # Program utama (setup & loop)
│   ├── config.h                 # Konfigurasi WiFi, pin, dan parameter servo
│   ├── jaringan.ino              # Komunikasi HTTP (WiFi AP, endpoint /detect)
│   ├── motor_servo.ino           # Kontrol motor DC dan servo sortir/feeder
│   └── sensor.ino               # Pembacaan sensor infrared
├── detection/                  # Script Python deteksi objek (YOLOv8)
│   └── ...
├── model/                      # Model YOLOv8s hasil training (best.pt)
├── dataset/                     # Dataset citra Bearing, Bolt, Nut
└── README.md
```

> 📌 Sesuaikan struktur folder di atas dengan struktur repository Anda yang sebenarnya.

## 🚀 Cara Menjalankan

1. **Upload firmware ke ESP32**
   - Buka folder `firmware/` di Arduino IDE
   - Sesuaikan `ssid` dan `password` pada `config.h`
   - Upload ke board ESP32

2. **Jalankan sistem deteksi di komputer**
   ```bash
   pip install ultralytics opencv-python
   python detect.py
   ```
   Pastikan komputer sudah terhubung ke jaringan WiFi ESP32 (`ESP32_Sortir_Alat`) sebelum menjalankan script.

3. **Nyalakan sistem**
   - Konveyor akan berjalan otomatis
   - Objek yang lewat di depan webcam akan dideteksi dan disortir sesuai kelasnya

## 📊 Performa Model

Model YOLOv8s dievaluasi pada *testing set* dengan hasil sebagai berikut:

| Kelas | Precision | Recall | AP (mAP@0.5) | AP (mAP@0.5:0.95) |
|---|---|---|---|---|
| Bearing | 0,932 | 0,888 | 0,936 | 0,759 |
| Bolt | 0,905 | 0,876 | 0,935 | 0,782 |
| Nut | 0,925 | 0,856 | 0,950 | 0,808 |
| **Rata-rata (All Classes)** | **0,921** | **0,873** | **0,940** | **0,783** |

**Pengujian sistem secara langsung** (30 sampel: 10 Bearing, 10 Bolt, 10 Nut) menghasilkan tingkat keberhasilan deteksi dan penyortiran sebesar **100%**, dengan rata-rata *delay* komunikasi HTTP antara 31–36 ms.

## ⚠️ Keterbatasan

- Pengujian deteksi hanya dilakukan pada kondisi pencahayaan terang dan stabil
- Mekanisme *feeder* belum sepenuhnya otomatis (masih memerlukan bantuan manual)
- Kecepatan konveyor bersifat tetap (belum dapat diatur secara dinamis)

## 👤 Penulis

**Durrotun Nasikhin**
Program Studi Sarjana Terapan Teknik Elektro, Fakultas Vokasi
Universitas Negeri Yogyakarta

## 📄 Lisensi

Proyek ini dibuat untuk keperluan akademik (Tugas Akhir). Silakan sesuaikan lisensi sesuai kebutuhan Anda (misalnya MIT License) jika ingin dipublikasikan secara terbuka.
