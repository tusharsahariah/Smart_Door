# 🔐 Password Secured Door Lock System (Version 1)

## Overview

The **Password Secured Door Lock System** is an Arduino based embedded security project that controls access to a door using a **4×4 matrix keypad**, **16×2 I2C LCD**, **servo motor**, LEDs, and a buzzer. The system is designed around a **Finite State Machine (FSM)** architecture, making the code modular, scalable, and easy to maintain.

Users initiate authentication by pressing the **#** key and entering a predefined password. Upon successful verification, the servo motor unlocks the door for a fixed duration before automatically locking it again. Incorrect password attempts trigger visual and audible alerts, and after three consecutive failed attempts, the system enters a temporary lockout state to prevent unauthorized access.

---

# ✨ Features

* Finite State Machine (FSM) based architecture
* Password-protected access control
* 4×4 Matrix Keypad for password entry
* Password masking on LCD using `*`
* 16×2 I2C LCD user interface
* Servo motor controlled locking mechanism
* Green LED indication for successful authentication
* Red LED indication for failed authentication
* Buzzer alert for incorrect password attempts
* Automatic door locking after a predefined timeout
* Three-attempt security lockout
* Temporary system lock for 30 seconds after repeated failures
* Password reset during entry using `*`
* Non-blocking state transitions using `millis()`

---

# 🛠 Components Used

| Component                |   Quantity  |
| ------------------------ | :---------: |
| Arduino Uno              |      1      |
| 4×4 Matrix Keypad        |      1      |
| 16×2 LCD with I2C Module |      1      |
| SG90 Servo Motor         |      1      |
| Active/Passive Buzzer    |      1      |
| Green LED                |      1      |
| Red LED                  |      1      |
| 220Ω Resistors           |      2      |
| Breadboard               |      1      |
| Jumper Wires             | As required |

---

# 📌 Pin Configuration

| Arduino Pin | Connected Device |
| ----------- | ---------------- |
| D2          | Keypad Column 4  |
| D3          | Keypad Column 3  |
| D4          | Keypad Column 2  |
| D5          | Keypad Column 1  |
| D6          | Keypad Row 4     |
| D7          | Keypad Row 3     |
| D8          | Keypad Row 2     |
| D9          | Keypad Row 1     |
| D10         | Servo Motor      |
| D11         | Buzzer           |
| D12         | Green LED        |
| D13         | Red LED          |
| A4          | I2C SDA (LCD)    |
| A5          | I2C SCL (LCD)    |

---

# ⚙️ Working Principle

1. When powered on, the system enters the **BOOT** state and initializes all peripherals.

2. After initialization, the door remains locked and the LCD prompts the user to press **#** to begin password entry.

3. The user enters a five-digit password using the keypad. The entered characters are masked with `*` on the LCD.

4. Pressing **#** submits the entered password for verification.

5. If the password is correct:

   * The green LED turns ON.
   * The servo motor unlocks the door.
   * The LCD displays an access granted message.
   * The door remains unlocked for five seconds.
   * The servo automatically locks the door again.

6. If the password is incorrect:

   * The red LED turns ON.
   * The buzzer sounds.
   * The remaining number of attempts is displayed.

7. After three consecutive incorrect attempts:

   * The system enters a security lockout mode.
   * The buzzer and red LED remain active for a short duration.
   * The system ignores all keypad input for 30 seconds.
   * After the timeout, the failed-attempt counter resets and normal operation resumes.

---

# 🔌 Circuit Diagram

<img src="circuit/circuit_diagram.png" width="350">

---

# 🔄 Flowchart

<img src="flowchart/flowchart.png" width="350">

---

# 🚀 Future Versions

## Version 2 – Remote Security Notification

Planned improvements include integrating an **ESP32** with Telegram or WhatsApp messaging services.

### Features

* Telegram or WhatsApp notifications on incorrect password attempts
* Notification after three consecutive failed attempts
* Timestamp of the security event
* Remote monitoring of door access activity

---

## Version 3 – AI Face Recognition Security System

The third version aims to enhance security using an ESP32 camera module and face recognition.

### Planned Features

* Face recognition based door unlocking
* Password free authentication for authorized users
* Capture an image when an incorrect password is entered
* Automatically send the captured image to the administrator via WhatsApp
* Maintain a log of unauthorized access attempts
* Dual authentication using both password and facial recognition for enhanced security
