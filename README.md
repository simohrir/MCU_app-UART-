<img width="908" height="712" alt="image" src="https://github.com/user-attachments/assets/3646fb8a-128e-4811-acf0-752e6508b497" />

This project consists of a simple embedded data logger based on the PIC18F4550 microcontroller, designed to measure, store, and visualize environmental data. The system integrates a real-time clock (RTC) to manage time-stamping, EEPROM memory for data logging, and an LCD to locally display the current time and measured values.

Temperature and humidity measurements are acquired using a DHT11 sensor, with a dedicated driver developed directly from the sensor’s datasheet to ensure accurate timing and reliable communication. The measured data is periodically processed by the microcontroller and transmitted over UART for external monitoring. An LM35 temperature sensor is also supported for analog temperature measurement and calibration purposes.

To enable real-time visualization and analysis on a PC, a Java Swing desktop application was developed. The application uses jSerialComm to handle UART communication with the microcontroller and JFreeChart to plot temperature and humidity data dynamically. The user interface allows the selection of the appropriate COM port, connection and disconnection from the device, and provides visual feedback for successful or failed connections.

In addition to data monitoring, the application includes an LED ON/OFF control to remotely toggle a microcontroller LED, mainly used for debugging and communication verification. A dedicated “Last 1 Hour” feature opens a separate window that displays the temperature and humidity values recorded during the previous hour, allowing short-term trend analysis.
