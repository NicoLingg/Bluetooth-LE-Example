# Bluetooth-LE-Example


![BluetoothLE](/BluetoothLE.jpg)

This is an example how you can use an Adafruit Bluefruit LE module to communicate with your own written Qt application via the UART service. In this example I use an Arduino Uno which is connected to Adafruit's Bluefruit LE UART Friend and a temperature and humidity sensor. The Arduino gets the temperature and humidity values from the sensor and sends them approximately every 3 seconds via Bluetooth LE to the Qt application which displays the values in a temperature and humidity plot. The app allows you to send a string up to 21 characters long (this is the limitation from BLE) to the Arduino. This allows you to control the Arduino wireless via the Qt-app. In this example we use it to turn on the built-in LED.  

The Adafruit Bluefruit LE module comes with the UART service and it is automatically started during the power-up. This means you don't have to do anything special in order to use it. The UUID are:

Base UUID: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E  
TX UUID: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E  
RX UUID: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E  

More information on the UART service can be found here:  
https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/uart-service  

Additional information about Adafruit's BLE module here:   
https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/introduction

And if you are interested in learning more about Bluetooth LE make sure you check the following links:
- https://en.wikipedia.org/wiki/Bluetooth_low_energy 
- http://doc.qt.io/qt-5/qtbluetooth-le-overview.html
- https://www.bluetooth.com/
- https://www.bluetooth.org/DocMan/handlers/DownloadDoc.ashx?doc_id=286439&_ga=1.161190325.907721094.1478372197 
- https://www.csrsupport.com/download/49793/CS-327738-RP-2-Training%20and%20Tutorials%20-%20Introduction%20to%20Bluetooth%20Smart.pdf
- Robin Heydon at Bluetooth World 2013 - Bluetooth Low Energy Basics (available on youtube.com)

### In order to use this example you need:
1. Qt Creator (Linux should work on Windows but I haven't tested it)
2. Arduino Uno
3. Adafruit Bluefruit LE UART Friend (https://www.adafruit.com/products/2479)
4. Adafruit SHT31-D Temperature & Humidity Sensor Breakout 

Of course you can use a different sensor or no sensor at all and simulate a temperature measurement by sending random numbers via Bluetooth to the Qt application.

In the "Qt-Application" folder you can find two versions of the Qt app, one with plot display and one without (for those who don’t have QWTPlot installed). If you use the version with plots, please make sure you've installed QWTPlot correctly. QWTPlot can be found here: http://qwt.sourceforge.net/index.html

In the "Arduino" folder you can find the Arduino sketch for this example. Please download and install the two following Adafruit libraries:
- Adafruit_BluefruitLE_nRF51:  
https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/software 
- Adafruit_SHT31:   
https://learn.adafruit.com/adafruit-sht31-d-temperature-and-humidity-sensor-breakout/wiring-and-test


### Step by Step Instructions
1. Download "Bluetooth LE UART Example" repository
2. Load Arduino sketch on your Arduino UNO and open the Serial Monitor
3. Open Qt Bluetooth LE application and turn on the Bluetooth on your computer
4. Press the search button and wait until the application finds the Adafruit Bluefriend 
5. If it finds something press connect, and the temperature and humidity measurement will start 

To disconnect please close the application (a better way is on my TODO list!)

In the Qt app example you can send a string to your Arduino, which gets displayed in the IDE serial monitor (if you are connected to your computer). The "Turn LED on" button turns on/off the Arduino's built-in LED. It basically just sends a string with "LED_ON", which can be interpreted by the Arduino. 

Please feel free to use the bluetoothuart class in your next BLE project. All you need to do is to connect the slot "startScan()", "startConnect()" and the signal "changedState(bluetoothleUART::bluetoothleState)" and "newData(QString)".

Slots:
startScan(): starts searching for low energy devices
startConnect(int i): starts connecting to the device i in QList m_qlDevices

Signals:
changedState(): indicates the current state of the bluetooth module
newData(QString): contains new data, which was sent by the Arduino

Functions:
writeData(QString s): Sends string “s” to the Bluetooth module


Let me know if you have any questions!

************************************************************************

# License

See the [LICENSE](LICENSE) file for license rights and limitations (MIT).
