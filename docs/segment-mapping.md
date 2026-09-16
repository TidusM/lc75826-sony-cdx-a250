# Segment-identification and mapping workflow

The LC75826 datasheet defines the electrical correspondence between D1…D208 and the driver's segment/common outputs. It does **not** document the custom Sony LCD glass artwork. Mapping the visible CDX-A250 symbols therefore requires an empirical test.

The supplied Arduino sketch contains an interactive helper for doing exactly that.

## What is source-verified

For segment-output mode, each segment output corresponds to four display-data bits—one per COM output. The manufacturer table runs continuously from:

```text
S1/P1:    D1   D2   D3   D4
...
S13:      D49  D50  D51  D52
S14:      D53  D54  D55  D56
...
S26:      D101 D102 D103 D104
S27:      D105 D106 D107 D108
...
S38:      D149 D150 D151 D152
S39:      D153 D154 D155 D156
...
S50:      D197 D198 D199 D200
S51:      D201 D202 D203 D204
S52/OSCI: D205 D206 D207 D208
```

That gives an electrical address space. The remaining question is which visible LCD element Sony connected to each address.

## What the project sketch does — OBSERVED

`searchOfSegments()` cycles through test-byte positions. For each step it:

1. clears `Aa` through `Ah`;
2. sets one mask bit in one of those bytes;
3. advances a counter called `nSeg`;
4. selects a `blockBit` value;
5. waits for the test pushbutton;
6. calls `segments()` to update the display;
7. prints the counter, block, group, mask-bit index, and byte values over serial.

`segments()` sends the bitwise complement of the eight test bytes:

```cpp
send_char_without(~Aa);
send_char_without(~Ab);
...
send_char_without(~Ah);
```

and then sends a group-ending byte selected by `blockBit`.

The serial monitor therefore gives the experimenter a reproducible identifier for the state visible on the LCD.

## Running the mapping experiment

After the project wiring has been confirmed against `connections.md`:

1. upload the reference sketch;
2. open the serial monitor at **115200 baud**;
3. allow the normal demo sequence to finish and enter `searchOfSegments()`;
4. press the D2 test button once for each step;
5. observe which single LCD stroke/icon changes;
6. record the printed scan information and the visible element;
7. continue until the relevant display has been covered;
8. repeat questionable entries rather than guessing.

A useful worksheet format is:

| Test step printed by sketch | Block | Byte/group | Bit index | Visible LCD element | Confidence / notes |
| ---: | ---: | ---: | ---: | --- | --- |
|  |  |  |  |  |  |

Photographing or filming the LCD while capturing serial output can make later reconciliation much easier.

## Important numbering caveat

The present implementation advances `nSeg` beyond **208**, while the LC75826 manufacturer display-data range is D1…D208. The block-range tests in `searchOfSegments()` also contain boundary values that deserve a deliberate review.

Therefore:

> **Do not equate the sketch's printed `nSeg` counter with manufacturer D1…D208 numbering without first validating the relationship.**

This relationship is `NEEDS_ENGINEER_REVIEW`.

That does not make the scan useless: the printed tuple (counter/block/group/mask/bytes) is still a reproducible experimental identifier. It simply means the polished public mapping should distinguish **test-step number** from **datasheet D-number** unless the engineer confirms they are intentionally identical over a given range.

## Why the bytes are complemented

The current source deliberately transmits `~Aa` … `~Ah` during the segment scan. That inversion is **OBSERVED** in the working code.

The reason and intended visible-state convention should not be reconstructed from guesswork. Record it as:

`NEEDS_ENGINEER_REVIEW`

Until that is clarified, describe the scan operationally ("one test state at a time") rather than asserting that a `1` or `0` directly means "segment on" everywhere in the test helper.

## Existing handwritten mapping material

The project media includes a handwritten segment-identification sheet and segment-identification footage. Those are valuable working evidence, but the current production technical brief does not yet record the numbering scheme as approved.

For that reason the staging repository does **not** publish the handwritten map as an authoritative lookup table yet.

Once reviewed, the best final form would be:

- one clean image of the LCD with segment labels;
- a CSV/Markdown table mapping visible feature → datasheet D-number;
- a second column retaining the original sketch test-step number if it differs;
- enough notes to reproduce ambiguous icons/multi-segment characters.

## Recommended validation pass before publication

For each final mapping entry:

1. confirm the physical LCD element by repeat test;
2. reconcile its scan tuple to the datasheet data group;
3. convert to D1…D208 only when the correspondence is certain;
4. spot-check several entries from each DD group;
5. verify any entries used by `msgHiFolks()`, `msgSONY()`, and `msgCDX()` against the actual displayed result.

This keeps the map useful for future code rather than preserving an unexamined one-off counter convention.