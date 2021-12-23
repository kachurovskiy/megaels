**This software and instructions are provided as is, without warranty of any kind. This is a hobby project. Using this might damage your equipment, cause injury or death. Use at your own risk. Original author didn't state the lisence for the code so it remains unknown.**

While the PCB is confirmed to work overall, it's very early days for this project and don't expect an easy ride if you go for it.

# MegaEls

Metal lathe electronic lead screw project built on top of Russian-speaking [DigitalFeed ELS system by Oleg A. and contributors from ChipMaker.ru](https://www.chipmaker.ru/topic/118083/).

![megaels-example](https://user-images.githubusercontent.com/517919/146578580-5ed5b854-83fb-4121-9611-b14a97ffb6ef.jpg)

## Code

Original code of DigitalFeed 7e2 is provided in the [folder above](https://github.com/kachurovskiy/megaels/tree/main/Digital_Feed_7e2). It requires tuning a few constants at the top of the main file to fit your lathe and motors.

Notably, it doesn't support belt connection for Z or X motors yet.

## Manual

There are [some instructions explaining how to operate the ELS](https://github.com/kachurovskiy/megaels/blob/main/Digital_Feed_7e2/manual.md) that I auto-translated and corrected a bit but expect to invest some time here :)

## PCB

![megaels_2012-12-05](https://user-images.githubusercontent.com/517919/146578632-993fc083-6fc4-4a29-a345-051907fbcaa5.jpg)

[Gerber files for production](https://github.com/kachurovskiy/megaels/raw/main/PCB_20211205/Gerber_PCB_MegaEls_20211205.zip)

Some PCB production techniques use lead and other nasty stuff. Order at your own risk and dispose properly.

Ordering it from JLCPCB.com takes just a few clicks - upload the ZIP file, use all default settings, choose shipping and checkout. Costs around 20 euro with delivery for 5 PCBs (minumum order).

## Components

- [Arduino Mega 2560](https://www.aliexpress.com/item/32850843888.html) (micro USB)
- [Connector pins](https://www.aliexpress.com/item/1005002577212594.html) that would get soldered to the board
- [1602 LCD 5V](https://www.aliexpress.com/item/32326489466.html)
- [9 6\*6 push buttons](https://www.aliexpress.com/item/32862548214.html)
- [4 eight-position switches](https://www.aliexpress.com/item/4000700310059.html)
- [Manual pulse generator 5V 4-pin 100 pulses](https://www.aliexpress.com/item/32949618549.html)
- [4-position joystick with latching and a button](https://www.aliexpress.com/item/4000893010190.html) though this one is a little too big and hard to switch
- [2 RV09 10kOhm potentiometers](https://www.aliexpress.com/item/4000078097697.html)
- [KF2EDG connectors with 3.5mm pitch](https://www.aliexpress.com/item/1005003085954456.html) - 5x4 pins, 1x6 pins, make sure to order both plug and needle
- [4 1kOhm resistor arrays with 9 resistors each](https://www.aliexpress.com/item/32840103807.html)
- 2 1kOhm resistors and 4 680 Ohm resistors
- 5V buzzer
- 2 ceramic capacitors e.g. 10 uF
- 4 LEDs

## Cases

![megaels_2012-12-05](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-case.png)

Download: [STL body](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-case-body.stl) and [STL body](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-case-lid.stl) or [f3d](https://github.com/kachurovskiy/megaels/raw/main/cases/MegaElsCase.f3d)

![megaels_2012-12-05](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-mpg.png)

Download: [STL body](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-mpg-body.stl) and [STL body](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-mpg-lid.stl) or [f3d](https://github.com/kachurovskiy/megaels/raw/main/cases/MegaElsMPG.f3d)

## Schematics

![MegaEls schematics](https://github.com/kachurovskiy/megaels/raw/main/PCB_20211205/Schematic_MegaEls_2021-12-05.png)
