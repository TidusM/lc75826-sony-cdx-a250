# LC75826 communication notes used by this project

This is **not** a replacement for the LC75826 datasheet. It extracts only the protocol/control concepts needed to understand the supplied Sony CDX-A250 Arduino project.

Unless explicitly marked otherwise, the IC behavior on this page is `SOURCE_VERIFIED` from the LC75826E/LC75826W manufacturer datasheet.

## CCB address

The datasheet assigns the LC75826 CCB address:

```text
41H
```

The Arduino source therefore defines:

```cpp
#define addr 0x41
```

## Serial signals

The LC75826 serial-transfer inputs are:

- **CE** — chip enable;
- **CL** — synchronization clock;
- **DI** — transfer data.

On the LC75826W they are pins 62, 63, and 64 respectively.

The Sony CDX-A250 service manual shows the original system controller supplying the equivalent LCD CE, clock, and serial-data signals to IC901.

## Four 72-bit data groups

The manufacturer transfer format places an 8-bit CCB address before each 72-bit data group. The final two bits of the group are `DD`, described by the datasheet as **direction data**.

| DD | Display-data portion | Remaining positions in the 72-bit group |
| --- | --- | --- |
| `00` | D1–D52 (52 bits) | 18 control bits + 2 DD bits |
| `01` | D53–D104 (52 bits) | 18 fixed bits + 2 DD bits |
| `10` | D105–D152 (48 bits) | 22 fixed bits + 2 DD bits |
| `11` | D153–D208 (56 bits) | 14 fixed bits + 2 DD bits |

When **153 or more segments are used**, the datasheet's transfer example says all four 72-bit groups (288 bits total, excluding the repeated 8-bit addresses) are sent.

When fewer than 153 segments are used, 72, 144, or 216 bits may be used depending on the required range, but the first group containing D1–D52 and the control data must always be sent.

![LC75826 data groups](assets/lc75826-data-groups.svg)

## Control data present in the first group

After D1–D52, the first 72-bit group contains the following documented control fields before DD:

```text
P0 P1 P2 P3 DR DN FC0 FC1 FC2 OC SC BU
```

(with fixed zero positions around these fields as shown in the manufacturer transfer diagram).

### P0–P3: segment outputs vs general-purpose outputs

These fields control whether S1/P1 through S8/P8 operate as LCD segment outputs or general-purpose outputs.

The all-zero setting keeps S1 through S8 as segment outputs. Other documented combinations progressively convert S1/P1, S2/P2, etc. to P1, P2, etc.; `1000` configures all eight as P1–P8 general-purpose outputs.

This matters to the Sony panel because some project code comments discuss pins configured as GPIO as well as LCD-segment drive.

### DR: bias selection

| DR | Datasheet drive scheme |
| --- | --- |
| `0` | 1/3-bias drive |
| `1` | 1/2-bias drive |

### DN: 200/208-segment mode

| DN | Datasheet mode |
| --- | --- |
| `0` | up to 200 display segments; S51 is forced low and S52/OSCI is low in internal-oscillator mode or OSCI in external-clock mode |
| `1` | up to 208 display segments; S51 is a segment output and S52/OSCI is a segment output in internal-oscillator mode |

This is why DN appears prominently in the project explanatory material.

### FC0–FC2: display waveform frame frequency

These three bits select the common/segment waveform frame frequency from documented divisors of the internal oscillator or external clock.

The exact frequency choice used by the current project should be stated only after decoding the project's control bytes and confirming the intended setting:

`NEEDS_ENGINEER_REVIEW`

### OC: oscillator mode

| OC | Datasheet mode | S52/OSCI function |
| --- | --- | --- |
| `0` | internal oscillator | S52 segment output |
| `1` | external clock | OSCI external-clock input |

### SC: segment on/off

| SC | Datasheet display state |
| --- | --- |
| `0` | On |
| `1` | Off |

### BU: normal/power-saving mode

| BU | Datasheet mode |
| --- | --- |
| `0` | Normal |
| `1` | Power-saving |

The power-saving mode has additional behavior documented in the datasheet; refer to the primary source rather than relying on this abbreviated table.

## Display data D1–D208

The LC75826 does not decode characters for you. Display data is transferred directly to the segment/common matrix.

The manufacturer correspondence table assigns four display-data bits to each segment output—one for each common output. Examples:

| Segment output | COM1 | COM2 | COM3 | COM4 |
| --- | ---: | ---: | ---: | ---: |
| S1/P1 | D1 | D2 | D3 | D4 |
| S2/P2 | D5 | D6 | D7 | D8 |
| S13 | D49 | D50 | D51 | D52 |
| S14 | D53 | D54 | D55 | D56 |
| S26 | D101 | D102 | D103 | D104 |
| S27 | D105 | D106 | D107 | D108 |
| S38 | D149 | D150 | D151 | D152 |
| S39 | D153 | D154 | D155 | D156 |
| S50 | D197 | D198 | D199 | D200 |
| S51 | D201 | D202 | D203 | D204 |
| S52/OSCI | D205 | D206 | D207 | D208 |

The full table is in the manufacturer datasheet.

This electrical matrix does **not** reveal which visible stroke, icon, or character segment Sony connected to each matrix point. That physical-glass mapping is what the project's interactive segment scan is meant to discover.

## What the Arduino source does — OBSERVED

### Byte shifting

`send_char_without()` iterates its mask from `0b00000001` upward, so the sketch transmits each supplied byte **least-significant bit first**.

### Address strobe

`send_addr()`:

1. drives CE low;
2. clocks out the `0x41` address;
3. raises CE after the address.

The subsequent bytes are then clocked by `send_char_without()` before CE is returned low at the end of the group.

This description is the literal behavior of the supplied source. For full CE/CL timing requirements, use the manufacturer timing diagrams and limits.

### DD/group-ending patterns

The current source uses the following final byte values in several routines:

```cpp
0B00000000  // group/DD 00
0B10000000  // group/DD 01 in the source's byte-shift representation
0B01000000  // group/DD 10
0B11000000  // group/DD 11
```

Because the code transmits each byte LSB-first, visual reading of the C/C++ binary literal from left to right is **not** the same as wire-order reading. This is one reason the repository avoids rewriting these literals into a new abstraction until the engineer confirms the intended final implementation.

## Current-source caveats — NEEDS_ENGINEER_REVIEW

The reference sketch is useful but intentionally not presented as a polished LC75826 library. Before publishing a normalized packet encoder, verify:

1. the exact project control-bit values represented by every final byte in `allON()`, `allOFF()`, and the text-message functions;
2. which control/GPIO state the Sony board requires for its additional panel functions;
3. the intended handling of unused/fixed bit positions;
4. the relationship between the segment-search counter and manufacturer D1…D208 numbering;
5. any source comments whose segment ranges do not exactly match the manufacturer transfer diagram.

Until then, the original code is the reference implementation and this document explains it without silently changing its semantics.