# Heart-Rate-Monitoring-System
# Biopotential ECG Signal Acquisition System with Edge-Level Exception Handling

A high-resolution biomedical instrumentation system designed to capture, process, and stream real-time single-lead electrocardiogram (ECG) data. The system utilizes an AD8232 analog front-end integrated with an Arduino Uno R3 (ATmega328P) to handle hardware signal conditioning, baseline drift rejection, and active lead-off error detection at the edge. 

Data is packaged and streamed over high-speed serial communication ($115200$ Baud) to the Processing IDE interface for real-time waveform visualization.

## 🛠️ Hardware Interface & Pin Mapping

* **Microcontroller:** Arduino Uno R3 (ATmega328P)
* **Analog Front-End (AFE):** AD8232 Single-Lead Heart Rate Monitor
* **Electrodes:** 3-Pad Disposable Hydrogel ECG Electrodes (RA, LA, RL Reference)

| AD8232 Pin | Arduino Uno R3 Pin | Function |
| :--- | :--- | :--- |
| **OUTPUT** | `A0` | Analog Biopotential Waveform Input (10-bit ADC) |
| **LO+** | `D10` | Digital Lead-Off Detection Plus Flag |
| **LO-** | `D11` | Digital Lead-Off Detection Minus Flag |
| **3.3V** | `3.3V` | DC Power Supply Input |
| **GND** | `GND` | Common System Ground Reference |

---

## ⚙️ Firmware Architecture & Logic Execution

The system executes a tight polling loop with a $1\text{ ms}$ baseline delay to maintain structural data consistency without flooding the serial buffer. 

### 1. High-Speed Serial Transmission
The UART link is clocked at 115200 bps to match the high refresh rate requirements of the visualization software, ensuring smooth real-time rendering of the cardiac trace.

### 2. Defensive Exception Handling (Leads-Off Logic)
To prevent erratic data plots when an electrode loses physical body contact, the edge device implements an out-of-bounds tracking mechanism:
* The system constantly monitors digital inputs `D10` and `D11`.
* If either pin registers a `HIGH` state (signaling poor contact or disconnection), the microcontroller immediately suppresses raw ADC readings and injects a constant error token of **`1100`** into the serial pipeline.
* Because the standard 10-bit ADC window only spans `0 to 1023`, this value acts as an explicit error flag for the receiving Processing script.

### 3. Native Data Streaming
When contact integrity is verified, the system samples the raw biopotential data from analog channel `A0` and publishes the `0-1023` integer matrix straight to the communication line.

---

