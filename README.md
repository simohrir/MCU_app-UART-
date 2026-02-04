# UART Temperature & Humidity Data Logger (Java)

## Description
This project is a simple data logger that uses UART communication to monitor temperature and humidity data from a microcontroller using a Java desktop application.

The application does not display any data until the correct serial port is selected and a successful connection is established. Once connected, the user can monitor real-time data, control an LED on the microcontroller, and visualize sensor values using dynamic graphs.

---

## Features
- UART serial communication between microcontroller and Java application
- Manual selection of the correct COM port
- Connection status handling:
  - Success panel displayed when connection is successful
  - Error panel displayed if connection fails
- LED control:
  - Button to turn the microcontroller LED ON or OFF (used mainly for debugging)
- Real-time data visualization:
  - Temperature graph
  - Humidity graph
- Live status display:
  - Current temperature
  - Current humidity
  - Current time
- Last 1 Hour data:
  - A dedicated window opens to display temperature and humidity data recorded during the last hour

---
