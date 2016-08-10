## Credits
Port of modified Adafruit library for ILI9341-based TFT displays.

Original lib: https://github.com/adafruit/Adafruit_ILI9341

The library is extended with text rendering routines got from here: http://www.instructables.com/id/Arduino-TFT-display-and-font-library/?ALLSTEPS

Original hspi code: https://github.com/Perfer/esp8266_ili9341

The sample project is to be built with ESP8266 Unofficial Development Kit aka UDK. 
http://esp8266.ru/forum/threads/obsuzhdenie-unofficial-development-kit-for-espressif-esp8266.32/

## ILI9341 driver
The driver itself and needed dependencies are in /driver and /include/driver.
The driver is written in C++ which is not well supported by ESP8266 toolchain and sdk, so some dirty hack is needed to properly contstruct C++ objects. The code for this is in user/routines.cpp.

## SPI staff
In spite of the fact that according to the datasheet max ILI9341's clock speed is 10MHz mine robustly works at up to 40MHz so I added SPI speed prescaler macro at the beginning of hspi.c.
Defining it to 1 means HSPI will be clocked at 40MHz, 4 means 10 MHz.

## Sample code
The sample code consists of two parts:

1) Rotating cube https://youtu.be/kEWThKicO0Q

2) Sample HVAC controller UI (for the extended version of this: http://habrahabr.ru/post/214257/) https://youtu.be/VraLl8XK1CI

What to demonstrate is controlled by macro UIDEMO defined at the beginning of user_main.cpp. If it's defined then the sample HVAC controller UI is shown, else, the rotating cube is rendered.

## Wiring
The code uses hardware HSPI with hardware controlled CS, so the wiring shall be as follows: 

ILI9341 pin -->	ESP8266 pin

MOSI 	-->	GPIO13

CLK 	-->	GPIO14

CS 	-->	GPIO15

DC 	-->	GPIO2

Reset to 3.3v

GND to ground

MISO may be left unconnected


