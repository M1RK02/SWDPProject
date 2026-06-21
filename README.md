# LUMIA — Multimodal Edge-AI Context-Aware Smart Glasses

### Laboratory of Smart Wearables Design & Prototyping — Academic Year 2025-26 (Group A08)

---

## 📌 Project Overview

**LUMIA** is an autonomous, context-aware smart glasses platform designed to process environmental telemetry locally at the edge. The system functions entirely without cloud reliance, performing real-time multimodal sensor fusion and inference execution directly on an ultra-low-power microcontroller.

The platform integrates two custom hardware subsystems: the wearable mainboard housing an **STM32U575** MCU and a **Bluetooth Low Energy (BLE)** transmitter, and the custom 4-layer **LUMOS Probe** containing an **AS7341** multispectral light sensor and an **IMP34DT05TR** digital PDM MEMS microphone. 

On-chip, an 8-bit integer-quantized Artificial Neural Network (ANN) trained via TensorFlow/Keras and converted via STM32Cube.AI (X-CUBE-AI) monitors environmental context in real-time. The system classifies current surroundings into one of five distinct categories: `UNDERGROUND`, `OUTDOOR`, `INDOOR_QUIET`, `INDOOR_NORMAL`, and `COVERED_DARK`. Resulting context predictions are dispatched via BLE to trigger dynamic theme and feature adaptations within a Flutter companion smartphone application.

---

## 📂 Repository Structure

The project is structured into modular layers spanning hardware design, firmware, data processing pipelines, mobile integration, and documentation:

```text
.
├── altium/                          # Hardware Design (LUMOS Sensor Probe)
│   ├── Lumos_Probe.PrjPcb           # Altium Designer project file
│   ├── Lumos_Probe.SchDoc           # Schematic document (AS7341, IMP34DT05TR, DF40C connector)
│   ├── Lumos_Probe.PcbDoc           # 4-layer PCB routing and layout definition
│   ├── Lumos_Probe.BomDoc           # Production Bill of Materials
│   └── Lumos_Probe.OutJob           # Fabrication outputs job configuration
├── firmware/                        # Embedded STM32 C Codebases
│   ├── data_logger/                 # Event-Driven Data Logger Firmware
│   │   ├── Core/Src/main.c          # MDF1-DMA double buffering and NAND SPI logging
│   │   └── data_logger.ioc          # STM32CubeMX hardware initialization profile
│   └── inference/                   # Edge TinyML Inference Firmware
│       ├── Core/Src/main.c          # On-device inference loop & BLE packet dispatch
│       ├── Core/Src/features.c      # Temporal & spectral feature extraction routines
│       ├── X-CUBE-AI/               # STM32Cube.AI neural network execution runtime
│       └── inference.ioc            # STM32CubeMX inference setup profile
├── pipeline/                        # Python Data Engine & Machine Learning
│   ├── data/                        # Local raw acquisition storage
│   ├── output/                      # Output of the model training
│   ├── download_data.py             # Tkinter GUI serial download utility
│   ├── sync_normalization.py        # Automatic training parameter sync tool
│   ├── data_analysis.ipynb          # Telemetry visualizer & feature profiling
│   ├── model_training.ipynb         # ANN architecture training & INT8 quantization
│   └── requirements.txt             # Python pipeline package dependencies
├── app/                             # Flutter Mobile Application
│   ├── lib/
│   │   ├── main.dart                # App entrypoint and BLE client initialization
│   │   ├── connection/              # BLE connection manager & telemetry streams
│   │   ├── models/                  # Struct definition for parsed environment logs
│   │   └── services/
│   │       └── packet_parser.dart   # BLE stream byte-packet parser
│   └── pubspec.yaml                 # Flutter dependencies and assets configurations
└── report/                          # Documentation
    ├── main.tex                     # Technical project report in LaTeX
    ├── report.pdf                   # Technical project report in PDF
    └── figures/                     # Report diagrams and PCB schematics
```

---

## 🛠️ System Architecture

### 1. Hardware Design: LUMOS Probe
The custom **LUMOS Probe** is built on a 4-layer FR4 stack-up (Signal-GND-VCC-Signal) designed in Altium. It interfaces with the wearable mainboard via a 10-pin `DF40C-10DP-0.4V` board-to-board connector.
* **AS7341 Multispectral Sensor**: Captures ambient light across 8 spectral channels ($F_1$–$F_8$, clear, and NIR) via I2C3 operating at 1.8 V with dedicated pull-ups.
* **IMP34DT05TR MEMS Microphone**: Captures ambient acoustics using a digital PDM interface, clocked directly by the MCU.
* **BOM Production Cost**: €8.8 per unit base for a small batch run of 125 units (including SMT assembly).

### 2. Dual-Firmware Embedded Architecture
* **Data Logger**: Designed as an event-driven Finite State Machine (FSM). Light data is read asynchronously via the AS7341 physical interrupt pin. PDM audio is streamed at a 16 kHz sampling rate via a circular `GPDMA1` double-buffer scheme using half-transfer and full-transfer interrupts to structure packets and write raw logs directly to the SPI NAND Flash memory.
* **Edge Inference**: Ingests features from raw telemetry using a sliding window. It computes a 39-dimensional vector (light intensity, spectral ratios, darkness level, flicker frequency distribution, audio energy, silence ratio, zero-crossing rate, and frequency-band energy) computed on-chip, normalizes the inputs, quantizes them to `int8`, and executes the TinyML runtime.

### 3. TinyML Model Details
* **Topology**: Multilayer Perceptron (MLP) consisting of 39 input features, two hidden layers (32 and 16 neurons with ReLU activation), and a 5-class Softmax output layer.
* **Quantization**: Fully quantized to `int8` (inputs/outputs type `int8`) using post-training integer quantization.
* **Performance**: Achieved a macro $F_1$-score of 0.990 during testing on the target baseline environments.

### 4. BLE Synchronization Protocol
Classification outcomes and raw spectral telemetry are bundled into a 24-byte packet:
* **Byte 0**: Start Byte (`0x7B` / `{`)
* **Byte 1**: Packet Type (`0x4C` / `L`)
* **Byte 2**: Class Label (`0` to `4`)
* **Bytes 3–22**: Spectral light channels ($F_1$–$F_8$, clear, and NIR) encoded as 16-bit little-endian values.
* **Byte 23**: End Byte (`0x7D` / `}`)

The companion Flutter app dynamically decodes this payload using a dedicated byte stream parser to update the context state.

---

## 🚀 Getting Started

### 📋 Prerequisites
* **Firmware Development**: STM32CubeIDE (v1.13.0+) with `X-CUBE-AI` extension installed.
* **Python Pipeline**: Python 3.11+ environment.
* **Mobile Companion**: Flutter SDK configured for mobile execution.

### 🔌 Step 1: Data Logger and Hardware Ingestion
1. Compile and flash the [data_logger](firmware/data_logger) firmware to the STM32U575.
2. Run the acquisition state machine to record environmental signals onto the NAND flash.
3. Activate the python virtual environment in the pipeline directory:
   ```bash
   cd pipeline
   python -m venv .venv
   source .venv/bin/activate  # On Windows: .venv\Scripts\activate
   pip install -r requirements.txt
   ```
4. Run the data downloader to stream bin packets via USB Virtual COM Port (VCP), plot features, and export dataset files (`.csv`, `.wav`, `.npz` format):
   ```bash
   python download_data.py
   ```

### 🧠 Step 2: TinyML Training & Sync
1. Open and run [model_training.ipynb](pipeline/model_training.ipynb) to process features and compile the quantized TinyML model.
2. Run [sync_normalization.py](pipeline/sync_normalization.py) to automatically parse the resulting `model_metadata.json` and inject updated normalization arrays and quantization scales directly into the MCU inference files ([features.c](firmware/inference/Core/Src/features.c) and [main.c](firmware/inference/Core/Src/main.c)):
   ```bash
   python sync_normalization.py
   ```
3. Load the [inference](firmware/inference) workspace, compile the code, and flash the device to enable real-time edge predictions.

### 📱 Step 3: Run Mobile Application
1. Connect a Flutter-compatible smartphone or launch an emulator.
2. Build and launch the mobile application:
   ```bash
   cd app
   flutter pub get
   flutter run
   ```
3. Turn on the smart glasses' BLE module, establish a connection via the app interface, and watch the UI transition dynamically as you change environmental environments.

---

## 🤝 Contributors
* **Mirko Pica** — Embedded Firmware, State Machine Logging, Serial Retrieval Protocol, Edge Inference Integration.
* **Elena Dalle Nogare** — Hardware Design, Schematic Capture, 4-Layer PCB Routing of LUMOS Probe.
* **Christian Prendin** — Companion App Development, BLE State Transmission/Decoding, Firmware-App Integration.
* **Leonardo Grossi** — Machine Learning Architecture, Feature Extraction Optimization, Model Quantization.
* **Jaime Iriarte** — Hardware Validation, Schematics Integration, Application Scenarios Analysis.

---

## 📄 License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.
