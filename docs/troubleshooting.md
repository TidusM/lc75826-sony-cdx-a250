# Troubleshooting

Work from the electrical layers upward. Most failures are easier to isolate when power, transport, control state, and segment data are checked separately.

## Nothing appears on the LCD

Check, in order:

1. panel power and current draw;
2. common ground between Arduino and panel;
3. continuity of D8 → DATA-LCD, D9 → CLOCK-LCD, D10 → CE-LCD;
4. CE/CL/DI activity with a scope or logic analyzer;
5. transmission of CCB address `0x41`;
6. LC75826 inhibit/control state;
7. first data group (D1–D52 + control fields);
8. display data.

Relevant control states:

- `SC = 0` — segments enabled;
- `BU = 0` — normal mode;
- `INH` must not be forcing the driver/display inactive.

## Only part of the display responds

Check the DD groups:

- `00` → D1–D52
- `01` → D53–D104
- `10` → D105–D152
- `11` → D153–D208

If the display uses data above D152, all four groups are needed.

Also check `DN`, which selects 200- vs 208-segment operation.

## S1/P1 through S8/P8 behave unexpectedly

These outputs can be configured as LCD segment outputs or as general-purpose outputs. Check P0–P3 in the first transfer group.

## Logic-analyzer data looks reversed

The source shifts each byte **least-significant bit first**.

For example, `0B10000000` is not observed on DI in the same left-to-right order in which the literal is written.

## Segment scan looks inverted

The reference `segments()` routine sends `~Aa` through `~Ah`. Keep that inversion when comparing your result with the original project.

## Segment number and datasheet D-number do not line up

Treat the scan's `nSeg` value as a test-step counter. Use the printed DD block, byte/group, and bit position to reconcile the state to D1…D208.

See [`segment-mapping.md`](segment-mapping.md).

## Backlight is off but LCD data works

The illumination circuitry and LCD serial interface are separate diagnostic layers. A working LC75826 transfer does not guarantee that the panel LEDs/backlight are powered or controlled as expected.

Verify the panel power/illumination path before changing LCD packet data.

## Arduino uploads successfully but nothing changes

An upload only proves that the Arduino accepted the sketch. Confirm:

1. the serial reset/start message at 115200 baud;
2. CE/CL/DI toggling;
3. address `0x41`;
4. group transfers;
5. valid control state;
6. panel power and common ground;
7. actual segment activity.

## When comparing results

Record:

- Arduino board;
- exact sketch version/commit;
- panel/board revision;
- bench supply voltage and current limit;
- whether all-on/all-off works;
- whether `HI FOLKS`, `SONY`, or `CDX-A250` works;
- logic-analyzer capture if available;
- serial output from the relevant segment-test step.

Those details make faults reproducible and much easier to diagnose.
