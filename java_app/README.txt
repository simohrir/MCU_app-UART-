## Application Behavior
1. The application starts with no data shown.
2. The user selects the correct serial port and clicks **Connect**.
3. If the connection is successful:
   - A confirmation panel is displayed
   - Data reception and plotting start
4. If the connection fails:
   - An error panel is shown
5. Temperature and humidity values are plotted in real time.
6. The **LED ON/OFF** button controls the MCU LED.
7. The **LAST_1H** button opens a new window showing temperature and humidity data for the last hour.

---
## Requirements
- Java JDK 8 or higher
- jSerialComm library
- JFreeChart library
