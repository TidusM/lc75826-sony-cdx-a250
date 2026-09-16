# Segment identification and mapping

The LC75826 datasheet defines D1…D208 electrically. The Sony LCD glass defines what those data positions look like to the viewer. The second part must be discovered experimentally.

## Electrical mapping

For segment-output mode, each segment output corresponds to four display-data bits, one for each COM output:

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

The full correspondence table is in the LC75826 datasheet.

## Interactive mapping routine

The reference sketch contains `searchOfSegments()` and `segments()`.

The test routine:

1. clears the working bytes `Aa` through `Ah`;
2. selects one bit position;
3. chooses the appropriate data block;
4. waits for the D2 pushbutton event;
5. updates the display;
6. prints the current test information over serial.

Open the Arduino serial monitor at **115200 baud** while running the scan.

## Practical mapping workflow

Use a worksheet such as:

| Test step | DD block | Byte | Bit | Visible LCD element | Notes |
| ---: | ---: | ---: | ---: | --- | --- |
|  |  |  |  |  |  |

For each button press:

1. observe the visible element that changes;
2. record the serial output;
3. photograph ambiguous symbols if useful;
4. repeat uncertain steps;
5. convert the recorded test position to the datasheet D-number using the active DD group and byte/bit position.

Recording video of the panel while capturing serial output is especially useful because the visual state and the software state can be reconciled later.

## About the scan counter

The sketch's internal `nSeg` counter is a **test-step counter used by this reference routine**. For documentation work, the safest identifier is the complete tuple printed by the sketch—block, byte/group, bit index, and test value—then the corresponding D1…D208 position can be derived explicitly.

This avoids treating an implementation counter as if it were itself the manufacturer's formal D-number.

## Complemented test bytes

During the segment scan, `segments()` transmits the bitwise complement of the working bytes:

```cpp
send_char_without(~Aa);
send_char_without(~Ab);
...
send_char_without(~Ah);
```

That inversion is part of the demonstrated reference routine. Preserve it when reproducing the original scan.

## Turning a map into useful code

Once the physical mapping is known, avoid hard-coding only complete words. A more reusable second-stage implementation can define named LCD features or character segments and then compose text/icons from those mappings.

For example, a future mapping table could contain:

```text
visible feature -> D-number -> DD group -> byte -> bit
```

Keep that future abstraction separate from the original reference sketch so that the working baseline remains easy to compare with the video and hardware.
