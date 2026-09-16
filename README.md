# Driving the Sony CDX-A250 LCD with an LC75826 and Arduino

This repository documents a reverse-engineering project built around the detachable front-panel/display board from a **Sony CDX-A250** car radio. The board uses an **LC75826W LCD driver** (Sony designator **IC901**) to control the front-panel LCD.

The supplied Arduino sketch demonstrates direct control of the display-driver interface and includes example display patterns plus a segment-identification routine.

> **Technical review status**
>
> Manufacturer/service-manual facts in this repository are cited to their primary source. Behavior visible in the supplied project code or media is described as observation. A few project-specific reproduction details—especially the final bench-power connection, exact connector wiring, and interpretation of some control bytes—still require engineer confirmation and are marked `NEEDS_ENGINEER_REVIEW` rather than guessed.

## What the project demonstrates

Project media shows the removed CDX-A250 display/key board operating independently and displaying:

- an all-segments test;
- `HI FOLKS`;
- `SONY`;
- `CDX-A250`;
- individual segment tests used while mapping the LCD.

The current Arduino source also contains those demo patterns and the interactive segment-search routine.

## Hardware identification

The Sony service manual identifies the front-panel LCD driver as **IC901, LC75826W-0S-E**, connected to **LCD901**. In the display block diagram, the system controller supplies the LCD driver with three serial-interface signals: data, clock, and chip enable.

The LC75826 manufacturer datasheet describes the LC75826E/LC75826W as 1/4-duty general-purpose LCD drivers. Relevant documented capabilities include:

- direct drive for up to **208 display segments**;
- up to **8 general-purpose output ports** by reconfiguring S1/P1 through S8/P8;
- a SANYO CCB-format serial interface;
- CCB address **41H** (`0x41` in the Arduino source);
- serial input pins **CE (pin 62)**, **CL (pin 63)**, and **DI (pin 64)** on the LC75826W;
- control fields for display data, segment/GPIO selection, bias selection, 200/208-segment selection, frame frequency, oscillator mode, display on/off, power-saving mode, and data-group selection.

See [docs/protocol.md](docs/protocol.md) for the subset that matters to this project.

## Repository layout

```text
.
├── README.md
├── arduino/
│   └── LC75826_Sony_CDX_A250_panel_Car_Radio_V2.ino
├── docs/
│   ├── connections.md
│   ├── protocol.md
│   ├── segment-mapping.md
│   ├── troubleshooting.md
│   ├── references.md
│   ├── licensing.md
│   └── assets/
│       ├── lc75826-data-groups.svg
│       └── sony-display-interface.svg
└── LICENSE-NOTICE.md
```

The manufacturer datasheet and Sony service manual are **not redistributed in this repository**. They are cited in [docs/references.md](docs/references.md).

## Current Arduino sketch

The source is preserved as the project reference sketch rather than silently rewritten into a library. Its own header explicitly describes it as example/reference code intended to be adapted.

### Pin names used by the sketch

| Arduino pin | Sketch name | Role in the sketch | Documentation status |
| --- | --- | --- | --- |
| D8 | `VFD_in` | Serial data output toward LC75826 | Code-observed; LC75826 input is DI |
| D9 | `VFD_clk` | Serial clock output | Code-observed; LC75826 input is CL |
| D10 | `VFD_ce` | Chip-enable output | Code-observed; LC75826 input is CE |
| D2 | `BUTTON_PIN` | Falling-edge interrupt used to advance segment testing | Code-observed |

The `VFD_` variable names are historical names in the sketch. The Sony panel documented here is an **LCD** panel; the source file is kept unchanged so that this repository does not silently alter the engineer's working reference.

### Important: power and connector hookup

The service manual identifies the panel connector signals and the project material includes an annotated bench-wiring photograph. However, the exact public reproduction recipe for panel power, ground, connector pin choice, backlight behavior, and any protective/series components has not yet been recorded as engineer-approved project scope.

**Do not infer a bench hookup from the table above alone.** See [docs/connections.md](docs/connections.md). The unresolved items are explicitly marked `NEEDS_ENGINEER_REVIEW`.

## Uploading and running the sketch

1. Open `arduino/LC75826_Sony_CDX_A250_panel_Car_Radio_V2.ino` in the Arduino IDE.
2. Select the board/port appropriate to the Arduino hardware used for the project. The source comments use Arduino Uno pin numbering.
3. Confirm the physical panel connections against [docs/connections.md](docs/connections.md) **after the `NEEDS_ENGINEER_REVIEW` items there have been resolved**.
4. Upload the sketch.
5. Open the serial monitor at **115200 baud** if you want to follow the segment-identification output.

At startup/loop time the current sketch exercises `allOFF()`, `allON()`, `msgHiFolks()`, `msgSONY()`, and `msgCDX()`, then enters `searchOfSegments()`.

## How communication is organized

The manufacturer documentation divides the LC75826 transfer into an 8-bit CCB address followed by one of four 72-bit data groups selected by the two DD bits:

| DD group | Display data represented in the group | Other bits in that 72-bit group |
| --- | --- | --- |
| `00` | D1–D52 | control data + DD |
| `01` | D53–D104 | fixed/control positions + DD |
| `10` | D105–D152 | fixed/control positions + DD |
| `11` | D153–D208 | fixed/control positions + DD |

The current sketch sends address `0x41`, shifts bytes **least-significant bit first**, and uses four group-ending byte patterns corresponding to DD `00`, `01`, `10`, and `11`.

That is enough to explain the structure of the working code without claiming that every comment or byte-boundary annotation in this early sketch is a canonical implementation of the datasheet. See [docs/protocol.md](docs/protocol.md) for the exact distinction.

## Segment identification

The LC75826 datasheet tells us how display data D1…D208 maps to driver output/common combinations, but it cannot tell us which physical symbol or stroke Sony assigned to each combination on the custom LCD glass. That part must be mapped experimentally.

The sketch includes `searchOfSegments()` and `segments()` for this purpose. In broad terms:

1. one test bit is selected at a time;
2. a button press advances to the next test step;
3. the display is updated;
4. the serial monitor prints the scan counter, selected block, byte group, bit index, and test-byte values;
5. the visible LCD element can then be recorded against that test step.

There is an important caveat: the present scan counter reaches values beyond the datasheet's D1…D208 display-data range and `segments()` transmits complemented test bytes (`~Aa` … `~Ah`). Therefore the sketch's printed `nSeg` value must **not yet be presented as a universally verified D-number mapping**. The exact numbering convention remains `NEEDS_ENGINEER_REVIEW` before a polished segment map is published.

See [docs/segment-mapping.md](docs/segment-mapping.md).

## Source-backed facts vs project-specific facts

To make the tutorial auditable, this repository uses a simple distinction:

- **SOURCE_VERIFIED** — directly supported by the LC75826 manufacturer datasheet or the Sony CDX-A250/A250EE service manual.
- **OBSERVED** — directly present in the supplied working source or project media.
- **NEEDS_ENGINEER_REVIEW** — a project-specific interpretation, hookup detail, discrepancy, or technical statement that has not yet been explicitly confirmed for publication.

This is especially important for old/reverse-engineered hardware: a plausible generic hookup is not a substitute for the connection actually tested on this panel revision.

## Troubleshooting

Start with [docs/troubleshooting.md](docs/troubleshooting.md). The first checks are the serial-interface path (CE/CL/DI), the LC75826 inhibit state, the selected data group/control state, and—only after the project hookup is confirmed—the panel supply/ground arrangement.

## Documentation and external references

The main primary sources are:

- **SANYO LC75826E / LC75826W datasheet**, ordering/document family `EN*A0159` / `No.A0161`.
- **Sony CDX-A250 / CDX-A250EE Service Manual**, Ver. 1.1 (2006-01), Sony publication **9-879-865-02**.

They are cited rather than copied. Links and source-identification details are in [docs/references.md](docs/references.md).

## YouTube video

This repository accompanies the LC75826 / Sony CDX-A250 tutorial video from the channel. The permanent video URL will be added after publication.

`VIDEO_URL_PENDING_RELEASE`

## Licensing and attribution

The current project source arrived without a recorded software license in the production material. This staging repository therefore does **not** silently assign a license to code that may belong to another contributor/rightsholder.

Before the repository is made public, ownership should be confirmed and an explicit code license selected. A permissive license such as MIT is a sensible option **if the code owner agrees**. Original project photographs/diagrams can be licensed separately (for example CC BY 4.0) if their creator wants reuse.

Manufacturer datasheets, Sony service documentation, trademarks, logos, and any crops derived from those documents are not relicensed here. See [docs/licensing.md](docs/licensing.md) and [LICENSE-NOTICE.md](LICENSE-NOTICE.md).

## Current release blockers

Before changing this staging repository to public, the smallest useful technical-review pass is to confirm:

1. the exact panel connector/power/ground wiring used in the demonstrated build;
2. whether any level shifting or series protection is required/recommended in the published build;
3. the final meaning of the control-byte values actually used by the example sketch;
4. the intended numbering convention for the interactive segment scan;
5. code and image ownership/licensing.

Everything else in the tutorial should remain traceable either to the primary documentation or to the supplied project itself.