# Connections and bench setup

This page deliberately separates **documented signal identities** from the **project-specific bench hookup**. The latter is not yet fully engineer-approved for public reproduction.

## 1. LC75826 serial pins — SOURCE_VERIFIED

From the LC75826E/LC75826W manufacturer datasheet:

| LC75826W pin | Symbol | Function |
| ---: | --- | --- |
| 62 | CE | Chip enable / serial-transfer input |
| 63 | CL | Synchronization clock input |
| 64 | DI | Serial data input |
| 56 | VDD | IC power-supply pin; manufacturer allowable operating range is 4.5–6.0 V |
| 59 | VSS | Ground |
| 61 | INH | Inhibit input; can force the display off |

These are **IC-pin facts**, not instructions to bypass the Sony panel circuitry and power the IC directly.

## 2. Sony CDX-A250 panel interface — SOURCE_VERIFIED

The CDX-A250/A250EE service manual's display block diagram shows the main system controller driving IC901 as follows:

- system-controller `LCD SO` → IC901 `DI`;
- system-controller `LCD CKO` → IC901 `CLK/CL`;
- system-controller `LCD CE` → IC901 `CE`.

The key-board schematic identifies IC901 as `LC75826W-0S-E` and LCD901 as the liquid-crystal display.

The panel connector is CN901. The service-manual schematic names, among others:

| CN901 pin | Service-manual signal name |
| ---: | --- |
| 1 | D-GND |
| 5 | A-GND |
| 11 | DATA-LCD |
| 12 | CE-LCD |
| 13 | CLOCK-LCD |
| 14 | +B |
| 15 | PANEL |

This table reports labels from the service documentation. It does **not** by itself define a safe standalone bench-power recipe.

## 3. Arduino signal assignment in the supplied sketch — OBSERVED

The current reference sketch defines:

```cpp
#define VFD_in 8
#define VFD_clk 9
#define VFD_ce 10
#define BUTTON_PIN 2
```

and uses those pins as follows:

| Arduino pin | Intended serial role in source | Corresponding LC75826 signal |
| ---: | --- | --- |
| D8 | data output | DI |
| D9 | clock output | CL |
| D10 | chip-enable output | CE |
| D2 | segment-test pushbutton interrupt | project helper, not an LC75826 pin |

The `VFD_` names are retained from the original sketch even though the Sony panel is LCD-based.

## 4. Series resistors — OBSERVED IN SOURCE / NEEDS_ENGINEER_REVIEW

Comments next to D8/D9/D10 say that a **1 kΩ resistor can be used to protect each line**. Because that is a project-source recommendation rather than a manufacturer requirement recorded in the current technical brief, the final public wording and exact placement remain:

`NEEDS_ENGINEER_REVIEW`

Do not reinterpret those comments as a validated logic-level converter.

## 5. Pushbutton for segment identification — OBSERVED IN SOURCE

The sketch configures D2 as:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                buttonReleasedInterrupt,
                FALLING);
```

The end-of-file comment also says the segment search uses a button to ground and mentions **2 kΩ**.

Because `INPUT_PULLUP` and the source comment need to be reconciled into one unambiguous construction drawing, the exact published button/resistor hookup is:

`NEEDS_ENGINEER_REVIEW`

The code-level behavior is clear: a falling edge on D2 advances the interactive scan.

## 6. Panel power — NEEDS_ENGINEER_REVIEW

Project media contains an annotated photograph showing a standalone wiring arrangement that labels the panel-side supply as `+12VDC`, and the Sony service schematic labels CN901 pin 14 as `+B`. The panel circuitry then includes its own LCD-driver supply network.

However, the production technical brief does not yet record the exact engineer-approved standalone power procedure, including:

- which CN901 pins are connected on the bench;
- the intended supply voltage/current limit;
- which ground pin(s) are used;
- whether the backlight is powered in the same configuration;
- whether the `PANEL` line needs a defined state;
- protective components and sequencing, if any.

For that reason this repository intentionally does **not** turn the annotated photo into a step-by-step power instruction yet.

### Publication gate

Before public release, replace this section with the actual tested hookup and its conditions after engineer confirmation. Until then:

> **Do not connect a bench supply to the panel based only on connector names or this draft repository.**

## 7. Logic levels — NEEDS_ENGINEER_REVIEW for the project hookup

The manufacturer datasheet defines LC75826 input thresholds relative to VDD; it does not establish that every Arduino/panel combination is automatically safe. The Sony service schematic also shows the original head-unit controller environment, not a generic Arduino interface.

The demonstrated build works, but the final tutorial should explicitly state the tested Arduino board, LC75826/panel supply state, any series protection, and whether level translation is required or merely optional.

`NEEDS_ENGINEER_REVIEW`

## 8. What is safe to prepare now

Before the remaining review is complete you can still:

1. inspect the board and locate CN901 / IC901;
2. read the primary documents in `references.md`;
3. inspect the Arduino source and understand its CE/CL/DI assignments;
4. study the packet structure in `protocol.md`;
5. prepare a segment-mapping worksheet.

The missing information is deliberately concentrated here rather than scattered as assumptions throughout the tutorial.