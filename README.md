# 🔐 Password Secured Door Lock System (Version 2)

## Overview

The **IoT Smart Door Lock System** is an ESP32-based embedded security project designed to provide secure password-based access control with real-time remote notifications. The system combines **embedded systems**, **IoT**, and **Finite State Machine (FSM)** concepts to build a reliable and scalable smart door locking solution.

The user initiates authentication by pressing the **#** key on a **4×4 matrix keypad** and enters a predefined password. The entered password is displayed as masked characters (`*`) on a **0.96" OLED display**. If authentication is successful, the system unlocks the door using a servo motor (currently represented by a green LED during development), while incorrect password attempts trigger visual and audible alerts.

The ESP32 connects to a Wi-Fi network and communicates with a **Telegram Bot**, allowing the administrator to receive instant notifications whenever important security events occur, such as successful access, incorrect password attempts, and temporary security lockouts.

The entire project is implemented using a **Finite State Machine (FSM)** architecture, making the firmware modular, maintainable, and easy to expand with future IoT and AI-based security features.

---

# ✨ Features

## Embedded Features

- ESP32-based Smart Door Lock
- Finite State Machine (FSM) architecture
- Password-protected access control
- 4×4 Matrix Keypad interface
- Password masking using `*`
- 0.96" OLED display interface
- Green LED indication for successful authentication
- Red LED indication for failed authentication
- Buzzer alert for unauthorized access
- Automatic door locking after a predefined timeout
- Three-attempt security lockout
- Temporary 30-second system lock
- Password reset during entry using `*`
- Non-blocking state transitions using `millis()`
  
---

## IoT Features

- ESP32 Wi-Fi connectivity
- Telegram Bot integration
- Instant startup notification
- Telegram notification on successful authentication
- Telegram notification on incorrect password attempts
- Telegram notification during security lockout

---

# 🛠 Components Used

| Component | Quantity |
|-----------|:--------:|
| ESP32 NodeMCU-32S | 1 |
| 4×4 Matrix Keypad | 1 |
| 0.96" OLED Display (SSD1306 I2C) | 1 |
| SG90 Servo Motor *(Development currently uses Green LED)* | 1 |
| Active Buzzer | 1 |
| Green LED | 1 |
| Red LED | 1 |
| 220Ω Resistors | 2 |
| Breadboard | 1 |
| Jumper Wires | As required |

---

# 📌 Pin Configuration

| ESP32 GPIO | Connected Device |
|------------|------------------|
| GPIO12 | Keypad Row 1 |
| GPIO14 | Keypad Row 2 |
| GPIO27 | Keypad Row 3 |
| GPIO26 | Keypad Row 4 |
| GPIO25 | Keypad Column 1 |
| GPIO33 | Keypad Column 2 |
| GPIO32 | Keypad Column 3 |
| GPIO13 | Keypad Column 4 |
| GPIO23 | Green LED |
| GPIO19 | Red LED |
| GPIO18 | Buzzer |
| GPIO5 | Servo Motor *(Future Hardware)* |
| GPIO21 | OLED SDA |
| GPIO22 | OLED SCL |

---

# ⚙️ Working Principle

1. The ESP32 powers on and initializes all peripherals.
2. The OLED displays the boot screen.
3. The ESP32 connects to the configured Wi-Fi network.
4. After a successful Wi-Fi connection, a startup notification is sent to the administrator through Telegram.
5. The system enters the **LOCKED** state and prompts the user to press **#**.
6. The user enters a five-digit password using the keypad.
7. Each entered digit is masked using `*` on the OLED.
8. Pressing **#** submits the entered password.
9. The system verifies the password.

### If the password is correct

- Green LED turns ON.
- Servo motor unlocks the door *(currently simulated using the Green LED)*.
- OLED displays **Access Granted**.
- Telegram notification is sent.
- Door remains unlocked for the configured duration.
- Door automatically locks again.

### If the password is incorrect

- Red LED turns ON.
- Buzzer sounds.
- Remaining attempts are displayed.
- Telegram notification is sent.

### After three consecutive incorrect attempts

- System enters **Security Lock Mode**.
- Keypad input is disabled for 30 seconds.
- Red LED and buzzer indicate the security event.
- Telegram security alert is sent.
- Failed attempt counter resets after timeout.

---

<!-- # 🔌 Circuit Diagram

<img src="circuit/circuit_diagram.png" width="350">

---
-->

# 🔄 Flowchart

<img src="flowchart/flowchart.png" width="350">

---

# 🚀 Future Versions

## Version 3 — Remote Control & Monitoring

- Telegram Commands
  - `/status`
  - `/unlock`
  - `/lock`
  - `/restart`
  - `/help`
- Remote Door Unlock
- Remote Door Lock
- Door Status Monitoring
- Event Logs
- Password Change via Telegram
- OTA Firmware Updates

---

## Version 4 — AI Smart Security

- ESP32-CAM Integration
- Face Recognition
- Image Capture on Unauthorized Access
- Send Captured Image via Telegram
- Cloud Event Logging
- Dual Authentication (Face + Password)
- Visitor Recognition
- AI-based Intrusion Detection

---

# 📈 Version History

| Version | Description | Status |
|----------|-------------|:------:|
| Version 1 | Arduino-based Password Door Lock | ✅ Completed |
| Version 2 | ESP32 IoT Smart Door Lock with Telegram Notifications | ✅ Completed |
| Version 3 | Remote Monitoring & Telegram Commands | 🚧 Planned |
| Version 4 | AI Face Recognition Security | 🔜 Planned |

