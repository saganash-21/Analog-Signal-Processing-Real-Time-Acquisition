# Analog-Signal-Processing-Real-Time-Acquisition

# IoT Environmental Monitor: Analog Signal Processing & Real-Time Acquisition

![Platform](https://img.shields.io/badge/Platform-ESP32-blue) ![Language](https://img.shields.io/badge/Language-C%2B%2B-orange) ![License](https://img.shields.io/badge/License-MIT-green)

## 📖 Overview
This project demonstrates the design and implementation of a mixed-signal environmental monitoring system using an **ESP32**. The system captures and processes two distinct types of physical signals:

1.  **Dynamic AC Signals (Audio):** High-frequency sound intensity using a custom hardware band-pass filter and burst sampling.
2.  **Static DC Signals (Light):** Low-frequency ambient light levels using an LDR with hardware smoothing and hysteresis-based control.

The goal was to move beyond basic "digital read" logic and implement **Analog Signal Conditioning** to improve Signal-to-Noise Ratio (SNR) and system stability before data reaches the microcontroller.

---

## 🔧 Hardware Architecture: Signal Conditioning

### 1. The Audio Path (AC Signal Processing)
**Sensor:** KY-038 Microphone Module (Analog Mode)
* **Challenge:** The raw output contained significant DC offset and high-frequency electronic noise, making "quiet" vs "loud" distinction unreliable.
* **Solution: Passive RC Band-Pass Filter & DC Bias**
    * **High-Pass Stage:** Removes the sensor's DC offset, isolating the pure AC sound wave.
    * **Low-Pass Stage:** Filters out high-frequency noise and prevents aliasing during ADC sampling.
    * **DC Bias Network:** A voltage divider ($2 \times 10k\Omega$) re-centers the clean AC signal to **1.65V** (midpoint of the ESP32's 3.3V ADC), ensuring the full negative half of the sound wave is readable without clipping.

### 2. The Light Path (DC Signal Processing)
**Sensor:** Light Dependent Resistor (LDR)
* **Challenge:** LDRs can be "jittery" due to flickering artificial light sources (50Hz/60Hz mains hum) or passing shadows, leading to erratic LED switching.
* **Solution: Passive Low-Pass Filter**
    * Implemented a hardware RC Low-Pass Filter with a time constant of $\tau \approx 0.1s$.
    * **Result:** This hardware "averaging" smooths out rapid voltage spikes before the ESP32 even reads the pin, providing a stable DC voltage representing true ambient light.

![Schematic Diagram](docs/schematic_diagram.png)
*Figure 1: Complete System Schematic designed in EAGLE, showing the Signal Conditioning stages.*

---

## 💻 Firmware Architecture: Windowed Burst Sampling

To manage the conflicting timing requirements of high-speed audio and slow-speed light monitoring, the firmware utilizes a **Time-Division Multiplexing** strategy.

### 1. Audio "Burst" Integration Window
* **Logic:** The CPU enters a dedicated **50ms Integration Window** (blocking loop).
* **Sampling Rate:** Inside this window, the ADC oversamples the microphone at approximately **10kHz**.
* **Purpose:** This high-speed burst captures the transient peaks and valleys of the AC waveform to calculate the **Peak-to-Peak Amplitude** (Loudness), which would be missed by standard slow sampling.

### 2. Telemetry & Control Cycle
* **Logic:** Immediately following the 50ms audio burst, the system performs a single "housekeeping" cycle.
* **Update Rate:** This results in an effective **20Hz refresh rate** for the LDR and LED outputs.
* **Benefit:** Keeps the UI responsive without interrupting the critical audio signal analysis.

---

## 🚀 Key Technical Highlights

### 🛡️ Noise Immunity & Hysteresis
* **Hardware:** Integrated filters on both inputs (Band-Pass for Audio, Low-Pass for Light) ensure signal integrity at the source.
* **Software:** Implemented **Software Hysteresis (Schmitt Trigger logic)** for the LED indicators.
    * **Margin:** $\pm 0.05V$.
    * **Effect:** Prevents actuator "chattering" (rapid on/off flickering) when the LDR voltage hovers near the critical thresholds (1.1V and 2.2V).

### 📊 Data Validation
* **Verification:** Signal integrity was verified visually using **Teleplot**.
    * Confirmed the centering of the Audio AC wave at 1.65V (DC Bias check).
    * Verified the "Peak-to-Peak" algorithm correctly identified loudness spikes vs. background noise.

![Teleplot Analysis](images/teleplot_audio_capture.png)
*Figure 2: Real-time waveform analysis showing the Raw Audio Signal (Left) and Calculated Intensity (Right).*

---

## ⚖️ Design Trade-offs
* **Passive vs. Active Filtering:** I chose **Passive RC Filters** over Active Op-Amp filters to reduce component count and power consumption. While this results in slight signal attenuation (Gain < 1), it is acceptable for relative loudness detection and simplifies the circuit complexity.
* **Blocking vs. Non-Blocking:** A **Windowed Burst** approach (blocking for 50ms) was chosen over a purely interrupt-driven approach. This simplifies the DSP math for peak-detection while still maintaining a refresh rate (20Hz) that is faster than human perception for the LED indicators.

---

## 🔌 Pinout & Wiring

| Component | ESP32 Pin | Function |
| :--- | :--- | :--- |
| **Sound Input** | GPIO 32 (ADC) | Filtered & Biased Audio Signal |
| **LDR Input** | GPIO 35 (ADC) | Filtered Light Level Voltage |
| **Red LED** | GPIO 5 | Low Light Indicator (< 1.1V) |
| **Blue LED** | GPIO 18 | Medium Light Indicator (1.1V - 2.2V) |
| **Green LED** | GPIO 19 | High Light Indicator (> 2.2V) |

---

## 🛠️ Getting Started

### Prerequisites
* ESP32 Development Board
* PlatformIO (VS Code Extension) OR Arduino IDE

### Installation
1.  **Clone the Repo:**
    ```bash
    git clone [https://github.com/yourusername/ESP32-Analog-Signal-Processing.git](https://github.com/yourusername/ESP32-Analog-Signal-Processing.git)
    ```
2.  **Hardware Setup:** * Wire the filters according to `docs/schematic_diagram.png`.
3.  **Flash Firmware:** * Open `src/main.cpp` in your IDE.
    * Build and Upload to the ESP32.
4.  **Visualize:** * Open **Teleplot** (VS Code Extension) or Serial Plotter.
    * Set Baud Rate to `115200`.

---

## 🔮 Future Improvements
* **FFT Analysis:** Move from "Peak-to-Peak" amplitude detection to **Fast Fourier Transform (FFT)** to isolate specific sound frequencies.
* **IoT Integration:** Send the conditioned data to a cloud dashboard (e.g., Blynk or AWS IoT) for long-term noise pollution logging.
* **PCB Design:** Convert the breadboard prototype into a custom PCB shield for the ESP32.

---

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
