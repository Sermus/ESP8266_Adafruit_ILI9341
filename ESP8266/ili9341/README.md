Port of modified Adafruit library for ILI9341-based TFT displays.
Original lib: https://github.com/adafruit/Adafruit_ILI9341

The library is extended with text rendering routines got from here: http://www.instructables.com/id/Arduino-TFT-display-and-font-library/?ALLSTEPS

The sample code consists of two parts:
1) Rotating cube
2) Sample HVAC controller UI (for the extended version of this: http://habrahabr.ru/post/214257/)

What to demonstrate is controlled by macro UIDEMO defined at the beginning of user_main.cpp. If it's defined then the sample HVAC controller UI is shown, else, the rotating cube is rendered.

Wiring:
The code uses hardware HSPI with hardware controlled CS, so the wiring shall be as follows: 

ILI9341 pin	ESP8266 pin
MISO 		GPIO12
CLK 		GPIO14
CS 		GPIO0
DC 		GPIO2
Reset to 3.3v
GND to ground
MOSI may be left unconnected


