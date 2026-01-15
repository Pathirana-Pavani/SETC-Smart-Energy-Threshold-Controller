# SETC - Smart Energy Threshold Controller 🔌⚡

**Third Year Project II | Department of Electrical & Electronic Engineering**
**University of Peradeniya**

![Project Status](https://img.shields.io/badge/Status-Completed-success)
![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![License](https://img.shields.io/badge/License-MIT-green)

## 📌 Project Overview
The **Smart Energy Threshold Controller (SETC)** is an intelligent, offline plug-and-play device designed to monitor real-time energy consumption and automatically disconnect loads when a user-defined energy threshold is exceeded.

Built using an **ESP32** microcontroller and the **PZEM-004T** industrial-grade energy sensor, this system provides precise tracking of Voltage, Current, Power, and Energy. It operates entirely **offline** (no Wi-Fi/Cloud required), ensuring data privacy, reliability, and safety in environments like laboratories, hostels, and smart homes.

## 🚀 Key Features
* **Real-Time Monitoring:** Live display of Voltage ($V$), Current ($A$), Power ($W$), and Accumulated Energy ($Wh/kWh$) on a 0.96" OLED screen.
* **Auto Load Cut-Off:** Automatically disconnects the appliance via a relay when the energy limit is reached to prevent overuse.
* **Multi-Threshold Control:** User-selectable energy limits (Low, Medium, High) tailored for different appliance types.
* **Offline Privacy:** Operates without internet connectivity, eliminating cyber risks.
* **Safety Mechanisms:** Includes overload protection, electrical isolation, and manual reset requirements.

## 🛠️ Hardware Specifications

| Component | Specification |
| :--- | :--- |
| **Controller** | ESP32 Development Board (Doit DevKit V1) |
| **Sensor** | PZEM-004T V3.0 (100A variant) |
| **Display** | 0.96" OLED (128x64 I2C) |
| **Actuator** | 5V Relay Module (Active Low) |
| **Max Load** | 15A / 3.5kW |
| **Input** | 230V AC / 50Hz |

## 🔌 Circuit & Pinout
Based on the `ESP_Controller.cpp` source code:

| Component Pin | ESP32 GPIO | Description |
| :--- | :--- | :--- |
| **PZEM RX** | `GPIO 16` | UART2 RX (Serial2) |
| **PZEM TX** | `GPIO 17` | UART2 TX (Serial2) |
| **OLED SDA** | `GPIO 21` | I2C Data |
| **OLED SCL** | `GPIO 22` | I2C Clock |
| **Relay IN** | `GPIO 5` | Active Low Control |
| **Button** | *GPIO 4 (Configurable)* | Threshold Selection Input |

> **Note:** The relay logic is **Active Low** (`LOW` = ON, `HIGH` = OFF). The system performs a stabilization check (10 seconds) on startup before accumulating energy.

## ⚙️ Software Setup
1.  **Dependencies:**
    Install the following libraries in your Arduino IDE / PlatformIO:
    * `PZEM004Tv30` (by Mandulaj)
    * `Adafruit_SSD1306`
    * `Adafruit_GFX`

2.  **Upload:**
    * Select Board: **DOIT ESP32 DEVKIT V1**
    * Connect the ESP32 via USB.
    * Upload the `ESP_Controller.cpp` code.

## 📖 Usage Guide
The device features a simple interface to manage energy limits.

### Threshold Levels
| Level | Energy Limit | Recommended For |
| :--- | :--- | :--- |
| **Level 1** | **0.50 kWh** | Phone chargers, Routers, Lamps |
| **Level 2** | **1.50 kWh** | Laptops, Fans, TVs |
| **Level 3** | **3.00 kWh** | Fridges, Kettles, Irons |

### Operation Logic
1.  **Startup:** The device waits 10 seconds to stabilize sensors.
2.  **Monitoring:** Displays live V, I, P, and E values.
3.  **Cut-Off:** If `Energy > Threshold`, the Relay turns **OFF**.
4.  **Reset:** Press the reset button (or power cycle) to clear the energy counter and restore power.

## 📸 Gallery
<p align="center">
  <img src="https://github.com/Pathirana-Pavani/SETC-Smart-Energy-Threshold-Controller/blob/main/images/device_iso.jpeg" width="30%" alt="Device Iso">
  <img src="https://github.com/Pathirana-Pavani/SETC-Smart-Energy-Threshold-Controller/blob/main/images/device_internal.jpeg" width="30%" alt="Internal Wiring">
  <img src="https://github.com/Pathirana-Pavani/SETC-Smart-Energy-Threshold-Controller/blob/main/images/device_working.jpeg" width="30%" alt="Working Device">
</p>

## 👥 Contributors
This project was developed by:

| Name | Email |
| --- | --- |
| **Dodandeniya D. G. J. K.** | [e20085@eng.pdn.ac.lk](mailto:e20085@eng.pdn.ac.lk) |
| **Edirisinghe H. K. D.** | [e20091@eng.pdn.ac.lk](mailto:e20091@eng.pdn.ac.lk) |
| **Pathirana B. P. P.** | [e20281@eng.pdn.ac.lk](mailto:e20281@eng.pdn.ac.lk) |

## ⚠️ Safety Warning
**High Voltage Hazard:** This project involves working with **230V AC mains electricity**. Improper handling can result in serious injury, shock, or fire. 
* Always unplug the device before opening the enclosure.
* Ensure the PCB/Wiring is properly isolated from the user-accessible case.
* Do not exceed the rated load of 15A.

---
*© 2026 Dept. of Electrical & Electronic Engineering, University of Peradeniya*
