# YOLOv8-Based Mechanical Component Detection and Sorting System

**Real-time detection and automated sorting of bearings, bolts, and nuts using YOLOv8, Python, ESP32, and a conveyor mechanism.**

This project was developed as a final-year Applied Bachelor's (D4) Electrical Engineering project at the Faculty of Vocational Studies, Universitas Negeri Yogyakarta, Indonesia. It combines computer vision with embedded control to identify mechanical components and direct them to their respective sorting paths.

## Overview

The system captures images of mechanical components on a conveyor, detects and classifies each component using a custom YOLOv8 model, and sends the corresponding sorting command to an ESP32 over a local Wi-Fi network using HTTP. The ESP32 coordinates conveyor operation and servo-based sorting. The system is intended for local operation without cloud inference.

### Supported classes

- **Bearing**
- **Bolt**
- **Nut**

## Key features

- Real-time object detection and classification with **YOLOv8s**
- Custom detection model for three mechanical component classes
- Python and OpenCV integration for image capture and inference
- HTTP communication between the computer and ESP32 on a local Wi-Fi network
- Servo-actuated routing and DC-motor-driven conveyor
- Infrared sensing for object-position coordination
- A feeder mechanism for component input (see limitations below)

## System architecture

```text
                  Camera
                    |
                    v
          Python + OpenCV + YOLOv8s
          (detection/classification)
                    |
          HTTP over local Wi-Fi
                    |
                    v
               ESP32
         (access point / server)
                    |
       +------------+-------------+
       |            |             |
       v            v             v
   DC motor     Servo driver   IR sensors
   conveyor       PCA9685      (feedback)
                    |
                    v
          Sorting servos / feeder
                    |
                    v
            Bearing / Bolt / Nut
```

**Processing workflow**

1. A camera captures components passing through the detection area.
2. The Python application runs YOLOv8s inference to identify the component class and confidence score.
3. Based on the detected class, the application sends an HTTP request to the ESP32.
4. The ESP32 controls the sorting actuators and coordinates conveyor motion using the available sensor signals.
5. The component is routed to the appropriate collection bin.

## Hardware

| Component | Purpose |
| --- | --- |
| ESP32 | Embedded controller, local Wi-Fi access point, HTTP server |
| Camera / webcam | Captures component images |
| DC motor and L298N driver | Drives conveyor belt |
| Servo motors and PCA9685 | Controls sorting paths and feeder |
| Infrared sensors | Detects component presence/position |
| Power supply | Supplies system components |

## Software and dependencies

**Python side**
- Python 3.x
- [Ultralytics YOLO](https://github.com/ultralytics/ultralytics)
- OpenCV

**ESP32 side**
- Arduino IDE or a compatible ESP32 development environment
- ESP32 Arduino core (including `WiFi.h`, `WebServer.h`, and `Wire.h`)
- Adafruit PWM Servo Driver library (`Adafruit_PWMServoDriver.h`)

## Repository structure

The top-level layout below reflects the folders currently visible in this repository:

```text
YOLOV8-SORTING/
├── jaringan/               # Communication/network-related project files
├── models/                # Model-related project files
├── python/                # Python project files
├── Manual Book_Final.pdf  # Project manual
└── README.md
```

> **Important:** The exact script, firmware, and weight-file names within these directories have not been verified in this README. Check the corresponding folders before running the software. Earlier documentation referred to `firmware/`, `detection/`, `model/`, and `detect.py`; those locations are **not** the current top-level repository layout.

## Setup and usage

### 1. Clone the repository

```bash
git clone https://github.com/durrotunnasikhin12/YOLOV8-SORTING.git
cd YOLOV8-SORTING
```

### 2. Create a Python environment

```bash
python -m venv .venv
```

Activate it on Windows PowerShell:

```powershell
.\.venv\Scripts\Activate.ps1
```

On Linux/macOS:

```bash
source .venv/bin/activate
```

### 3. Install the core Python packages

```bash
python -m pip install --upgrade pip
python -m pip install ultralytics opencv-python
```

Additional dependencies may be required by the actual scripts in `python/`.

### 4. Configure and upload the ESP32 program

1. Locate the firmware/source files in the repository and open the relevant sketch in the Arduino IDE.
2. Install the ESP32 board package and the required servo driver library.
3. Confirm the hardware wiring, local Wi-Fi configuration, HTTP endpoint, and servo channel mappings against the source code and project manual.
4. Upload the firmware to the ESP32.

### 5. Run the Python application

1. Connect your computer to the ESP32's configured local Wi-Fi network.
2. Locate the **actual entry-point script** under `python/` and confirm the model weight path under `models/`.
3. Start the script using its real filename, for example:

```bash
python python/<entry_point_script>.py
```

The command above is a **template**, not a copy-paste-ready command. Replace `<entry_point_script>` with the script that exists in this repository and review its configuration before starting the conveyor. Do not assume that the old `python detect.py` instruction is valid.

For system operation, wiring and calibration details, refer to [`Manual Book_Final.pdf`](./Manual%20Book_Final.pdf).

## Model evaluation

The YOLOv8s model was evaluated on the project's test set. The following values are reported in the original project README:

| Class | Precision | Recall | AP@0.5 | AP@0.5:0.95 |
| --- | ---: | ---: | ---: | ---: |
| Bearing | 0.932 | 0.888 | 0.936 | 0.759 |
| Bolt | 0.905 | 0.876 | 0.935 | 0.782 |
| Nut | 0.925 | 0.856 | 0.950 | 0.808 |
| **All classes** | **0.921** | **0.873** | **0.940** | **0.783** |

> The class-wise values and aggregate results are preserved from the existing project documentation; they have not been independently recalculated for this README.

### Integrated-system test

The original project documentation reports:

- **30 controlled test samples:** 10 bearings, 10 bolts, and 10 nuts
- **100% successful detection and sorting** in those 30 tests
- **HTTP communication delay:** approximately 31–36 ms

These results describe the documented **controlled test conditions**; they are not a guarantee of performance under all lighting, mechanical, or operating conditions.

## Known limitations

- Detection was tested under relatively bright, stable lighting.
- The feeder was not fully automated and still required manual assistance.
- Conveyor speed was fixed rather than dynamically regulated.
- Additional testing is needed to characterize robustness across changing lighting, object orientations, and operating speeds.

## Portfolio context

This project demonstrates practical integration of **custom YOLO detection**, **Python/OpenCV inference**, **local HTTP communication**, and **embedded actuator control**. It is separate from the author's later YOLOv8 + ByteTrack video-tracking demonstration.

## Author

**Durrotun Nasikhin**  
Applied Bachelor's (D4) in Electrical Engineering  
Faculty of Vocational Studies, Universitas Negeri Yogyakarta, Indonesia

## License

This repository was created for academic project documentation. **No open-source license is stated here.** Reuse and redistribution permissions should not be assumed without an explicit license from the repository owner.
