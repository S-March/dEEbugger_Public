# dEEbugger for NODEMCU hardware
[The original dEEbugger is an ESP8266 based keychain swiss army knife for electronics](https://gfycat.com/OrneryPlushAbalone)

I have tried to keep the same github format as the original dEEbugger Git in case S-March wishes to use any of my updates.

This patch modifies /improves the graticule: and adds other features..
It is compatible with NodeMCU hardware. 

My primary use for this software is as a filament force display for 3D printers see: [Filament Force sensor](https://www.thingiverse.com/thing:2429390)

![Alt text](/Pictures/30Hz%20sine%20wave.png " 30Hz signal on Revised Graticule")
Note that the ADC cannot be read whilst the ESP is updating the websocket to update the scope display , so my code deliberately displays a blank line where the ADC data is missing. It takes samples for about 2 seconds, then updates the scope display. 

This example below shows a HX711 interface for strain gauges so that weight/force measurements can be made, and with some force applied to the sensor:The non sampled gaps are visible as blanks in the display
![Alt text](/Pictures/HX711%20channel%20showing%20signal%20gaps.png)

I have also added an OSD for slowly chganging signals:
![Alt text](/Pictures/OSD.png)






