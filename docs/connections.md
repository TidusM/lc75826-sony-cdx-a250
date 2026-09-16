# Connections and bench setup

This page describes the interface used by the Sony CDX-A250 panel in the demonstrated project.

## LC75826 serial interface

The LC75826W serial inputs are:

| LC75826W pin | Symbol | Function |
| ---: | --- | --- |
| 62 | CE | chip enable |
| 63 | CL | serial clock |
| 64 | DI | serial data |
| 56 | VDD | IC supply |
| 59 | VSS | ground |
| 61 | INH | inhibit input |

These are IC-pin definitions from the LC75826 datasheet. In this project the Arduino communicates through the **Sony panel connector and panel circuitry**, not by bypassing the board and wiring directly to the IC supply pins.

## Sony panel connector

The Sony CDX-A250 service schematic identifies the front-panel connector as **CN901**.

Relevant signals are:

| CN901 pin | Sony signal | Project role |
| ---: | --- | --- |
| 1 | D-GND | panel digital ground |
| 5 | A-GND | panel analog ground |
| 11 | DATA-LCD | serial data path to LC75826 DI |
| 12 | CE-LCD | chip-enable path to LC75826 CE |
| 13 | CLOCK-LCD | serial clock path to LC75826 CL |
| 14 | +B | panel supply input |
| 15 | PANEL | panel-detection/control signal in the original radio |

## Arduino signal wiring

The reference sketch uses:

```cpp
#define VFD_in 8
#define VFD_clk 9
#define VFD_ce 10
#define BUTTON_PIN 2
```

For the demonstrated serial interface:

| Arduino | Sony panel | LC75826 function |
| ---: | --- | --- |
| D8 | CN901 pin 11, DATA-LCD | DI |
| D9 | CN901 pin 13, CLOCK-LCD | CL |
| D10 | CN901 pin 12, CE-LCD | CE |
| GND | panel ground | common reference |

The sketch comments recommend **1 kΩ series resistors** on the three serial lines as simple protection. They are not a substitute for a level translator in a design that genuinely requires level translation.

## Panel power

The demonstrated project powers the **panel assembly** through its board-level supply path. The Sony schematic labels CN901 pin 14 as `+B`; the project material uses a **12 VDC bench supply** at the panel side.

Use a current-limited bench supply when first reproducing the setup and verify the exact board revision before applying power.

The LC75826 itself operates from the panel's local supply circuitry. Do not treat the IC's VDD specification as permission to connect the external bench supply directly to the IC VDD pin.

## Segment-test pushbutton

The sketch configures D2 as:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                buttonReleasedInterrupt,
                FALLING);
```

The button therefore advances the scan by creating a falling edge on D2. The reference source comments also mention a resistor in the test-button wiring; reproduce the demonstrated wiring rather than adding an unnecessary external pull-up when `INPUT_PULLUP` is enabled.

## Backlight and panel functions

The key/display board contains circuitry beyond the LC75826 LCD interface, including LED illumination and panel controls. A working LCD serial interface does not automatically imply that every backlight or key-related function is controlled through the same three LC75826 serial wires.

When debugging, separate:

1. panel power;
2. LCD driver communication;
3. LCD segment data;
4. illumination/backlight behavior;
5. key/panel logic.

That distinction avoids diagnosing a lighting problem as a serial-protocol failure.

## First power-up checklist

Before sending display data:

1. verify the panel board revision and CN901 orientation;
2. use a current-limited bench supply;
3. establish a common ground between Arduino and panel;
4. connect D8/D9/D10 to DATA-LCD/CLOCK-LCD/CE-LCD;
5. check for accidental shorts;
6. upload the unmodified reference sketch;
7. confirm serial output at 115200 baud;
8. observe CE/CL/DI with a logic analyzer or oscilloscope if the display remains inactive.

For protocol-level debugging continue with [`protocol.md`](protocol.md).
