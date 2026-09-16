# Troubleshooting

This page starts with checks that are supported by the primary documentation or the supplied source. It intentionally avoids inventing an unreviewed power/wiring recipe.

## Nothing appears on the LCD

Check in this order:

1. **Do not assume the panel power hookup.** Confirm the standalone CN901 supply/ground arrangement against the engineer-approved build first. See `connections.md`.
2. Verify that the Arduino pins used by the sketch really reach the LC75826 serial path:
   - D8 → DI/data path;
   - D9 → CL/clock path;
   - D10 → CE/chip-enable path.
3. Check **INH**. The manufacturer datasheet states that the INH input can force the display off. The exact standalone-panel INH state is part of the project hookup and should be confirmed rather than improvised.
4. Check that the first LC75826 group is actually transmitted. The datasheet requires the D1–D52/control-data group even when fewer than 153 display segments are used.
5. Confirm that the control data does not select `SC = 1` (segments off) or an unintended power-saving state.

## Some segments work but others never change

The LC75826 display data is split across four DD groups:

- `00`: D1–D52;
- `01`: D53–D104;
- `10`: D105–D152;
- `11`: D153–D208.

For a panel using data above D152, the datasheet example sends all four groups. A missing/wrong group selection can therefore look like a partially working display.

Also check the **DN** control bit. The datasheet uses DN to select up-to-200 vs up-to-208 segment operation; in the 200-segment mode S51/S52 are not both available as normal segment outputs.

## First eight segment outputs behave like ordinary digital outputs

S1/P1 through S8/P8 can be switched between LCD segment outputs and general-purpose outputs by P0–P3 control data. Confirm that the control state matches the intended use.

## Display stays off despite data activity

Primary-source checks include:

- INH state;
- SC state (`0` = display on, `1` = display off);
- BU state (`0` = normal mode, `1` = power-saving mode);
- oscillator mode/control state;
- valid panel/IC power.

The exact project values and physical hookup remain subject to the review notes in `connections.md` and `protocol.md`.

## Serial data looks reversed on a logic analyzer

The supplied `send_char_without()` function starts with mask `0b00000001` and shifts the mask left, so **each C/C++ byte is sent least-significant bit first**.

For example, do not visually read a literal such as `0B10000000` left-to-right and assume that is its order on the DI wire.

## Segment-search number does not match D1…D208

That is currently expected to require reconciliation.

The sketch's `nSeg` counter reaches values beyond 208, while the LC75826 datasheet defines display data D1…D208. Treat the printed counter as a **test-step identifier**, not an authoritative datasheet D-number, until the mapping convention is engineer-approved.

See `segment-mapping.md`.

## Segment search seems inverted

`segments()` sends complemented values (`~Aa` through `~Ah`). That behavior is present in the reference source. The reason/visible-state convention has not yet been recorded as an approved technical explanation.

`NEEDS_ENGINEER_REVIEW`

## The demo messages differ after code cleanup

The binary constants in `msgHiFolks()`, `msgSONY()`, and `msgCDX()` are the actual project pattern data. Refactoring them into character fonts, tables, or a generalized display library can be useful later, but it should be tested against the hardware before replacing the reference version.

For the current repository, preserve the original sketch as the known reference rather than treating a prettier rewrite as equivalent without validation.

## Backlight is off while LCD segments work

The Sony service documentation shows separate panel/backlight circuitry around the key/display board. The exact standalone backlight hookup and whether it is controlled by one of the LC75826-configured general-purpose outputs are project-specific details that still require final engineer confirmation.

`NEEDS_ENGINEER_REVIEW`

Do not diagnose a dark backlight as an LC75826 serial failure without separating LCD operation from illumination circuitry.

## Arduino upload succeeds but the panel remains unchanged

Separate the problem into layers:

1. Arduino program is running (serial reset/start message appears).
2. CE/CL/DI signals are toggling.
3. Address `0x41` and group traffic are present.
4. LC75826 is not inhibited/off/power-saving unexpectedly.
5. Panel supply and common ground are correct for the tested build.
6. LCD glass segment changes are observed.

This prevents changing pattern bytes when the actual failure is at power, inhibit, or serial-interface level.

## Before opening an issue / comparing results

Record:

- exact Arduino board;
- exact sketch commit/file version;
- which CDX-A250/A250EE panel/board revision you have;
- power source and current limit (after the public hookup is approved);
- logic-analyzer capture if available;
- whether all-on/all-off works;
- whether any of the three demo messages work;
- serial-monitor output from the failing segment-search step.

Those details are far more useful than a generic "display does not work" report.