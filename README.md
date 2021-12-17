# MegaEls

Metal lathe electronic lead screw project built on top of Russian-speaking [DigitalFeed ELS system by Oleg A. and contributors from ChipMaker.ru](https://www.chipmaker.ru/topic/118083/).

![megaels-example](https://user-images.githubusercontent.com/517919/146578580-5ed5b854-83fb-4121-9611-b14a97ffb6ef.jpg)

**This software and instructions are provided as is, without warranty of any kind. This is a hobby project. Using this might damage your equipment, cause injury or death. Use at your own risk. Original author didn't state the lisence for the code so it remains unknown.**

## Code

Original code of DigitalFeed 7e2 is provided in the folder above. It requires tuning a few constants at the top of the main file to fit your lathe and motors.

## PCB

![megaels_2012-12-05](https://user-images.githubusercontent.com/517919/146578632-993fc083-6fc4-4a29-a345-051907fbcaa5.jpg)

[Gerber files for production](https://github.com/kachurovskiy/megaels/raw/main/Gerber_PCB_MegaEls_20211205.zip)

Some PCB production techniques use lead and other nasty stuff. Order at your own risk and dispose properly.

Ordering it from JLCPCB.com takes just a few clicks - upload the ZIP file, use all default settings, choose shipping and checkout.

## Components

- Arduino Mega 2560
- 1602 LCD
- 9 6\*6 push buttons
- 4 LEDs
- 4 eight-position switches (model SR16)
- Manual pulse generator
- 4-position joystick with latching and a button
- 2 RV09 10kOhm potentiometers
- KF2EDG connectors with 3.5mm pitch - 5 with 4 pins, 1 with 6 pins (make sure to order both parts of each connector)
- 4 1kOhm resistor arrays with 9 resistors each
- 2 1kOhm resistors and 4 680 Ohm resistors
