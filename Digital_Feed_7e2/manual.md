# Digital Feed Electronic Lead Screw 7e2

## Quick Start

### Moving the tool

- Select `Z` or `X` axis using the corresponding switch
- Select `1x` or `10x` scale using the corresponding switch
- Rotate the handwheel

1 impulse from the handwheel should correspond to 0.01mm movement of the motor in `1x`, 0.1mm in `10x`. If the motor doesn't move, try switching to submode to manual or switching mode to a different value.

### Moving the tool automatically

- Set mode to `Feed mm/min`
- Set submode to manual
- Use joystick to start/stop feeding into any of the 4 directions
- Use the top-left potentiometer to change the speed.

### Moving the tool based on the spindle rotation

- Set mode to `Feed mm/rev`
- Set submode to manual
- Start the spindle
- Use joystick to start/stop feeding into any of the 4 directions
- Use the top-left potentiometer to change the speed

### Cutting a manual thread

Using the right-hand thread cut to the chuck as an example:

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

### Cutting an automatic thread

Using the right-hand thread cut to the chuck as an example:

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

# Manual

Note: this is a slightly redacted automated translation of a manual written by Oleg A. and contributors from ChipMaker.ru. Use with caution.

## Basic concepts and definitions

**Limits** (electronic stops) are virtual positions on the workpiece that limit the machining area (movement ranges tool) and operate in different modes of operation. There are left, right, front and back limits. 

The positions of the limits are set only with the middle (neutral) position of the joystick and the position of the "Submode" switch "Manual". 

Note: the distance between the limits (processing zones) cannot be less than some values determined by your equipment and firmware parameters (the distance between the limits cannot be less than the "acceleration + deceleration" accelerated).

**Feed** is a value that characterizes the speed of the tool along or across the workpiece. Depending on the mode of operation, synchronous or asynchronous, it is measured in millimeters per revolution or millimeters per minute.

**Rapid feed** is the mode of tool movement with the maximum possible speed along one of the axes. It is activated by pressing a special fast travel button on the joystick. Rapid feed is not activated or the movement does not start at all when the distance from the current position of the tool to the stop on the side of movement is less than the distance required for acceleration and deceleration.

## Controls

### Tuning

Intended for selecting the operating mode and setting the mode parameters:

- 8-position mode switch
- 3-position submode switch
- Buttons - up / down / right / left / select to select and change parameter values

### Operational

Directly for operation:

- 4-position joystick with position fixation and its "rapid feed"
- Button 4 buttons for setting / removing limits (left / right / front / rear)
- Potentiometer, which sets the amount of feed to the manual pulse generator (popularly RGI, Handcoder) - 100 pulses per revolution and its satellite switches:
  - 3-position - axis selection: Z / inactive / X
  - 2-position - selection of the scale of movement: 0.01mm / 0.1mm per one "click" of the RGI.

## Basic operating modes

### Thread

- With spindle reversal without opening the mother nut (no limits)
- Without reversing and without opening the nut, switching the direction of the joystick (limits must be set)
- Without reversing with opening the nut and manual return (synchronization by the threaded indicator)
- Automatically (see below)

When preparing and setting the parameters:

- Use the Up-Down buttons to select the thread pitch, can be changed only when the joystick is in a neutral position
- 4 thread directions along the joystick
- 4 limit buttons, limits are set only when the joystick is in neutral position

When the joystick button is pressed, the right stop is ignored, you can arbitrarily ride after it and try on the counterpart, to restore synchronization, it is enough to enter the zone between the limits and reach the right stop, continue threading as usual.

### Automatic threading

1. Prepare outside/inner diameter to the starting size (e.g. 12mm for M12)
2. Set the right and left stops to the required thread length
3. In the "Thread" mode, use the Up / Down buttons to select the required pitch
4. With the "Submode" switch, select "External" Ext or "Internal" Int
5. On the example of a right-hand thread, direct spindle turns, cut to the chuck:
   1. before starting, touch the workpiece with a threaded cutter in diameter and set the carriage to the right stop (limit),
   2. switch the joystick to the left, the process has started, the cross stops light up alternately , the cross stops are extinguished - the process is complete.
6. When done, tool moves to the starting position

**When cutting 55 degree thread, do not use the automatic threading** - or use the machine 2 times in a row, having previously recalculated the height of 55 degree and 60 degree triangles for the current step, before the second pass of the machine, apply the difference in depth manually.

### Multi-start thread

After cutting the 1st thread:

1. Move the cutter to the starting position (in automatic mode, the cutter will take the starting position itself)
2. Stop the spindle
3. Switch to the "Divider" mode
4. Turn the spindle to set the displayed angle to 180 degrees
5. Reset the angle with the "Sel" button
6. Return to the "Thread" mode
7. Start the spindle
8. The 2nd thread will be cut with an offset of 180 degrees relative to the 1st
9. For a 3-way entry - reset accordingly after 120 degrees, etc.

### Synchronous feed (mm / rev)

- 4 directions of feed on the joystick
- 4 directions of rapid movement on the joystick and the button of the accelerated
- 4 buttons of the limit, the limits are set only with the neutral position of the joystick and manual submode

Use the potentiometer on the top-left to change the amount of feed (even during the operation).

### Automatic multi-pass synchronous feed (mm / rev)

Same as "Synchronous feed" but also:

- Use the Up / Down buttons to set the removal rate `Ap`
- Use the Right / Left buttons to set the number of passes `Pq`

Total amount of material removed is `Pq * Ap * 2`.

#### External groove, direction to the chuck

In the "Manual" mode set the right + left limits, move the instrument to the right limit, the "Submode" switch to the "Ext" position, switch the joystick "Left". 

#### External groove, direction from the chuck

In "Manual" mode, set the right + left limits, move the tool to the left limit, switch "Submode" to the "Ext" position, switch the joystick "Right".

#### Internal boring, direction to the chuck

In "Manual" mode set the right + left limits, move the tool to the right limit, switch "Submode" to "Int", switch the joystick "Left".

#### Internal boring, direction from the chuck

In the "Manual" mode set the right + left limits, move the tool to the left limit, switch the "Submode" switch to the "Int" position, switch the joystick "Right".

#### Facing, direction to the spindle axis

In the "Manual" mode, set the front + back limits, move the tool to the back limit, the "Submode" switch to the "Ext" position, switch the "Forward" joystick.

#### Facing, direction from the spindle axis

In the "Manual" mode, set the front + back limits, move the tool to the front limit, the "Submode" switch to the "Ext" position, switch the "Back" joystick.

### Asynchronous feed (mm / min)

- 4 directions of feed along the joystick
- 4 directions of fast movement along the joystick and the accelerated button
- 4 buttons of the limit, the limits are set only with the neutral position of the joystick and neutral the position of the switch "Submode"

Use the potentiometer on the top-left to change the amount of feed (even during the operation).
 
### Automatic multi-pass asynchronous feed (mm / min)

Experimental.

### Cone left and right (Cone < and Cone >)

- Use the Right-Left buttons (with the joystick neutral position) to select the cone.
- The front-back limits (if they were set earlier) are ignored, but their position in memory is retained.
- Cutting is switched on by Joystick Right / Left.
- The joystick button is used to accelerate movement, the trajectory is the selected cone.
- Joystick forward / reverse switching enables synchronized feed in the corresponding direction.

Sharpen the inner cone from a smaller diameter to a larger one, gradually going deeper into the previously prepared hole. Sharpening from the chuck.

Sharpen the outer cone from a larger diameter to a smaller, larger diameter at the chuck.

The exception is the adapter, in this case, too, from a larger diameter to a smaller one, but the chuck has a smaller diameter and is cut in advance.

In this case, the backlash of the screws is taken out and both screws will always be loaded by the feed.

This principle is used to build an Automaton on cones. 

#### Automatic cone

3 variations are available:

- outer cone (thick at the chuck) cut from left to right
- outer cone (thin at the chuck, you need a technological cut along the left end), cut it from right to left, before the technological groove
- inner cone (thin at the chuck) cut it from left to right

### Reserve

Empty for now.

### Rough sphere (Sphr)

Tool - a cut-off tool with a straight cutting edge.

- Using the Up / Down buttons set the diameter of the ball,
- Use the Right / Left buttons to set the residual diameter of the leg,
- With the "Sel" button pressed, set the width of the cutter
- With the "Sel" button pressed, buttons Right / Left set the width steps (step Z)

Procedure:

1. Cut through the workpiece in diameter future sphere + D which must be greater than X backlash and less than REBOUND_X
2. Use parting tool
3. Cut through the workpiece for the convenience of measurements, but no more than ⅓ of the given diameter is not clear in Z or X (with a cutting tool) with measurements in the required diameter of the future ball
4. Engage the rear support
5. Butt the workpiece (dust the end)
6. Return to the back stop
7. Activate the right stop
8. Switch the joystick to the left
9. Wait end of the process
 
### Division of a circle - divider calculator

The first line of the indicator displays the number of division sectors and the required angle for the current sector (Req). The number of division sectors (maximum 255) is set by the Up-Down buttons.

The second line of the indicator displays the number of the current sector, and the real angle of the spindle. Moving between sectors is carried out with the Right-Left buttons.

All that is required is to turn the spindle to match the readings.

Angle zeroing, if necessary, is performed by the 5th menu button.
 
2 short "beeps" in automatic modes means: "I am working", do not put your hands into the rotating mechanism and wait for the end of the process.
