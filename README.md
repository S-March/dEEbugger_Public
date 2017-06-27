# dEEbugger
[The dEEbugger is an ESP8266 based keychain swiss army knife for electronics](https://gfycat.com/OrneryPlushAbalone)

This patch modifies /improves the graticule: 
![Alt text](/Pictures/Revised_Grat.jpg?raw=true "Revised Graticule")

![Alt text](/Pictures/ON_KEYS.jpg?raw=true "Keychain")

The device was designed with breakouts for the two ADC channels, UART and I2C. It can run off of 5V or 3.3V or USB power. If USB is being used, the 5V and 3.3V pins can be used as outputs.

![Alt text](/Pictures/FULL_CALLOUTS.jpg?raw=true "Callout Main")

## Oscilloscope
To use the dEEbugger, plug in power and navigate to dEEbugger.local in you browser. If your devices does not support mDNS, you can just navigate to the dEEbugger IP address instead.

![Alt text](/Pictures/04.png?raw=true "Oscilloscope Main")
#### Two Channels (ADC and UART Stream)
Chose between multiple inputs for the channels. In the below picture the red is an ADC channel and the blue is a UART stream of numbers.

![Alt text](/Pictures/07.png?raw=true "Two Channels")
#### Peak Detection
The dEEbugger supports real time peak detection as well.

![Alt text](/Pictures/09.png?raw=true "Peak Detection")


## Serial Terminal
#### Local Echo
In additin to acting as an oscilloscope, the dEEbugger can act as a "wireless" serial terminal allowing you to send and receive data over the UART pins through the web application

![Alt text](/Pictures/10.png?raw=true "Local Echo")
#### Terminal Options
Terminal settings: connect/disconnect button, baud rate selection, terminal clear button and enable/disable a local echo.

![Alt text](/Pictures/11.png?raw=true "Terminal Options")


## I2C Controller
#### Find Devices
When "Find Devices" button is pressed, a list of all I2C devices on the bus will be returned.

![Alt text](/Pictures/13.png?raw=true "Find Devices")
#### Read Device
When a device is selected, all readable registers will be returned with their current values.

![Alt text](/Pictures/14.png?raw=true "Read Device")
