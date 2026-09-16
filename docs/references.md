# References and source provenance

This project relies on the original LC75826 manufacturer documentation, the Sony CDX-A250 service documentation, and the demonstrated hardware/firmware.

Third-party PDFs are cited rather than redistributed.

## LC75826 manufacturer datasheet

**Document:** `LC75826E / LC75826W — 1/4-Duty General-Purpose LCD Display Driver`  
**Original manufacturer:** SANYO Electric Co., Ltd.  
**Document identifiers:** `EN*A0159`, `No.A0161`  
**Project source filename:** `LC75826.pdf`

Topics used by this repository include:

- 208-segment capability;
- CE / CL / DI serial interface;
- CCB address `41H`;
- four transfer groups selected by DD;
- P0–P3, DR, DN, FC0–FC2, OC, SC, and BU;
- D1…D208 to segment/common correspondence;
- pin functions and serial timing.

Discovery links:

- onsemi technical documentation: https://www.onsemi.com/design/technical-documentation
- Datasheet Archive LC75826 search: https://www.datasheetarchive.com/?q=lc75826

Verify the document identifiers before using a downloaded copy.

## Sony CDX-A250 / CDX-A250EE service manual

**Document:** `CDX-A250 / CDX-A250EE Service Manual`  
**Version:** `Ver. 1.1`  
**Date:** `2006.01`  
**Sony publication number:** `9-879-865-02`  
**Project source filename:** `sony_cdx-a250.pdf`

Material used by the tutorial includes:

- display-section block diagram;
- key-section board and schematic;
- IC901 identification and connections;
- LCD901 context;
- CN901 interface signal names.

Sony support:

https://www.sony.co.uk/electronics/support/mobile-cd-players-digital-media-players-cdx-series/cdx-a250

Sony's public support page does not expose the service manual used here. Search by publication number `9-879-865-02` and verify model/version metadata when locating an archival copy.

A third-party catalog page identifying the exact service manual is:

https://remont-aud.net/load/car_audio/72-1-0-4830

This is included as a discovery aid, not as an endorsement of redistribution terms.

## Arduino reference source

**File:** `arduino/LC75826_Sony_CDX_A250_panel_Car_Radio_V2.ino`

The repository preserves the project source as the reference baseline used for the demonstrated implementation.

Reference SHA-256:

```text
2357d9ba8443bff3d322f1299a11764cd86c8f34f37af7daad159d069078d256
```

## Project media

The repository includes original project photographs and original explanatory diagrams where they help the tutorial. These are licensed as described in [`../ASSET_LICENSE.md`](../ASSET_LICENSE.md).

Manufacturer/service-manual screenshots and PDFs are deliberately not bundled as normal project assets.

## Related video

The complete hardware demonstration and explanation:

https://youtu.be/Oe9wPHL-y7k
