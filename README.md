**This software and instructions are provided as is, without warranty of any kind. This is a hobby project. Using this might damage your equipment, cause injury or death. Use at your own risk. Original author didn't state the lisence for the code so it remains unknown.**

# MegaEls

Metal lathe electronic lead screw project built on top of Russian-speaking [DigitalFeed ELS system by Oleg A. and contributors from ChipMaker.ru](https://www.chipmaker.ru/topic/118083/).

![megaels-example](https://user-images.githubusercontent.com/517919/147279909-92772466-749f-495c-bee4-87cd6dff6e9c.jpg)

## ⚡ Looking for resellers and manufacturers 💲

If you're willing to manufacture this device and/or sell it as a kit, we'd be happy to link to your page for free - especially if it makes the build cheaper than buying all the components separately.

## Code

Code is provided in the [folder above](https://github.com/kachurovskiy/megaels/tree/main/Digital_Feed_7e2). It requires tuning a few constants at the top of `Digital_Feed_7e2.ino` to fit your lathe and motors.

Verify that hardware parameters are set correctly:

- Use `mm/min` mode and a clock to ensure each axis moves in the right direction and by the expected amount of mm within 60 seconds
- Use "Dividing head" mode (last one) to check that when you turn the spindle by hand in the "forward" direction, angle on screen increments from 0 to 360

Every time hardware parameters change, update `Cone_Info` and `Thread_Info` tables in the middle of that file using calculator in [`tables.html`](http://kachurovskiy.com/megaels/Digital_Feed_7e2/tables.html). If you don't recalculate those tables, your threads and cones will come out wrong even when `mm/min` and dividing head tests above pass.

To upload the code to your device, install Arduino IDE, connect your Arduino Mega to the computer via a USB cable, open `Digital_Feed_7e2.ino`, select `Arduino Mega 2560` in `Tools > Board` and click `Sketch > Upload`.

## PCB

![megaels_2012-12-05](https://user-images.githubusercontent.com/517919/146578632-993fc083-6fc4-4a29-a345-051907fbcaa5.jpg)

[Gerber files for production](https://github.com/kachurovskiy/megaels/raw/main/PCB_20211205/Gerber_PCB_MegaEls_20211205.zip)

Some PCB production techniques use lead and other nasty stuff. Order at your own risk and dispose properly.

Ordering it from JLCPCB.com takes just a few clicks - upload the ZIP file, use all default settings, choose shipping and checkout. Costs around 20 euro with delivery for 5 PCBs (minumum order).

Cheaper, country-specific options: [Ukraine](https://www.olx.ua/d/uk/obyavlenie/elektronna-gtara-dlya-tokarnogo-verstata-na-arduino-megaels-nanoels-IDPnBwt.html) (feel free to add more)

## Components

- [Arduino Mega 2560](https://www.aliexpress.com/item/32850843888.html) (micro USB)
- [Connector pins](https://www.aliexpress.com/item/1005002577212594.html) that would get soldered to the board
- [1602 LCD 5V](https://www.aliexpress.com/item/32326489466.html)
- [9 6\*6 push buttons](https://www.aliexpress.com/item/32862548214.html)
- [8-position switches](https://www.aliexpress.com/item/4000700310059.html)
- [Manual pulse generator 5V 4-pin 100 pulses](https://www.aliexpress.com/item/32949618549.html) 60mm
- [4-position joystick with latching and a button](https://www.aliexpress.com/item/4000893010190.html) though this one is a little too big and hard to switch
- [2 RV09 10kOhm potentiometers](https://www.aliexpress.com/item/4000078097697.html)
- [KF2EDG connectors with 3.5mm pitch](https://www.aliexpress.com/item/1005003085954456.html) - 5 with 4 pins, 1 with 6 pins, make sure to order both plug and straight needle
- [4 1kOhm resistor arrays with 9 resistors each](https://www.aliexpress.com/item/32840103807.html)
- 2 1kOhm resistors and 4 680 Ohm resistors
- 5V buzzer
- 2 ceramic capacitors e.g. 10 uF
- 4 LEDs

## Cases

The following cases are designed to be used with [M3 threaded inserts](https://www.aliexpress.com/item/32430148405.html) that would be pushed into the 4 designated holes using your soldering iron tip and receive M3 [nylon screws (for MPG) or 20mm standoffs (for the main case)](https://www.aliexpress.com/item/32617866125.html). 

### Main case

![megaels_2012-12-05](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-case.png)

Here the lid is connected using nylon screws into 20mm standoffs that work as screws holding the PCB down. It's designed to be mounted using two M5 bolts 135mm apart on centers.

Cut 1 line on the right side of the case and push the small part of the wall to the outside to get access to the mini USB port of the Arduino.

Download: [STL body](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-case-body.stl) and [STL lid](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-case-lid.stl) or [f3d](https://github.com/kachurovskiy/megaels/raw/main/cases/MegaElsCase.f3d)

[Printable SVG](https://github.com/kachurovskiy/megaels/raw/main/megaels-face.svg) with labels for the controls.

### MPG case

![megaels_2012-12-05](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-mpg.png)

Download: [STL body](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-mpg-body.stl) and [STL lid](https://github.com/kachurovskiy/megaels/raw/main/cases/megaels-mpg-lid.stl) or [f3d](https://github.com/kachurovskiy/megaels/raw/main/cases/MegaElsMPG.f3d)

## Schematics

![MegaEls schematics](https://github.com/kachurovskiy/megaels/raw/main/PCB_20211205/Schematic_MegaEls_2021-12-05.png)

## Quick Start

### Moving the tool

- Select `Z` or `X` axis using the corresponding switch
- Select `1x` or `10x` scale using the corresponding switch
- Rotate the handwheel

https://user-images.githubusercontent.com/517919/150657985-6fe1d571-e0b5-475b-9b13-4543c2c39b69.mp4

1 impulse from the handwheel should correspond to 0.01mm movement of the motor in `1x`, 0.1mm in `10x`. If the motor doesn't move, try switching to submode to manual or switching mode to a different value.

### Turning of facing

https://user-images.githubusercontent.com/517919/150658372-34b87c6c-d6d8-46ef-8bb0-31651cf59bb8.mp4

- Set mode to `Feed mm/rev`
- Set submode to manual
- Start the spindle
- Use joystick to start/stop feeding into any of the 4 directions
- Use the top-left potentiometer to change the speed

### Asynchronous feed (grinding)

https://user-images.githubusercontent.com/517919/150658051-4663ad40-8815-415f-8046-cac0e11eda6a.mp4

- Set mode to `Feed mm/min`
- Set submode to manual
- Use joystick to start/stop feeding into any of the 4 directions
- Use the top-left potentiometer to change the speed.

### Threading

Using the right-hand thread cut to the chuck as an example:

https://user-images.githubusercontent.com/517919/150672370-4f7bad88-6a53-406b-a333-fa034ceace00.mp4

- Set mode to `Thrd`
- Set submode to manual
- Use up/down menu buttons to select the pitch. For TPI pitches, scroll up through all the metric pitches.
- Move the tool using the handwheel to the left-most `Z` position
- Set the left limit using the corresponding button
- Move the tool using the handwheel to the right-most `Z` position
- Set the right limit using the corresponding button
- Start the spindle
- Move in the cutter (manually or using the handwheel)
- Set joystick to "left" and wait for the tool to reach the left limit (don't stop the spindle)
- Move the cutter out
- Set joystick to "right" and wait for the tool to reach the right limit (don't stop the spindle), use joystick button for faster movement
- Move in the cutter
- Set joystick to "left" and so on

### Automatic threading

Using the right-hand thread cut to the chuck as an example:

https://user-images.githubusercontent.com/517919/150673123-4970087e-299d-4873-bf87-a3d265480593.mp4

- Set mode to `Thrd`
- Set submode to manual
- Use up/down menu buttons to select the pitch. For TPI pitches, scroll up through all the metric pitches.
- Move the tool using the handwheel to the left-most `Z` position
- Set the left limit using the corresponding button
- Start the spindle
- Move the tool using the handwheel so that the cutter just barely touches the material
- Move the tool using the handwheel to the right-most `Z` position
- Set the right limit using the corresponding button
- Set submode to external
- Set joystick to "left" and wait for the operation to complete

When done, set joystick to neutral and submode to manual to be able to use the handwheel again.

#### Manual follow-up after automatic threading

When automatic threading is finished you can test fit and potentially make a few more passes:

https://user-images.githubusercontent.com/517919/150681412-96608c69-1e91-40f1-9b8f-9cf68e4d7d45.mp4

- Move joystick to neutral
- Set submode to manual
- Set joystick to "right" and press the joystick button to move past the right limit
- Turn on the spindle (no need to reverse it) and wait for the cutter to move out of the way, turn off the spindle
- Test fit the part
- Set joystick to "left"
- Turn on the spindle, move in the cutter, make another pass, it should stop at your existing left stop
- Move out the cutter, no need to stop the spindle
- Set joystick to "right", it should stop at your existing right stop, repeat until the part fits

### Cones

https://user-images.githubusercontent.com/517919/151621173-53c0f2e1-b2ff-46d9-8cfa-94ba974be943.mp4

- Set mode to `>` or `<`
- Set submode to manual
- Pick the cone angle using the menu buttons
- Turn on the spindle
- Use joystick "left" and "right" positions to move the cutter along the cone
- Use joystick "forward" and "back" positions to move the cutter in or out

## Manual

There are [some instructions explaining how to operate the ELS](https://github.com/kachurovskiy/megaels/blob/main/Digital_Feed_7e2/manual.md) that I auto-translated and corrected a bit but expect to invest some time here :)
