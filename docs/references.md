# References and source provenance

The public tutorial should remain reproducible without copying documents whose redistribution rights are not ours.

## Primary source 1 — LC75826 manufacturer datasheet

**Document:** `LC75826E / LC75826W — 1/4-Duty General-Purpose LCD Display Driver`  
**Original manufacturer:** SANYO Electric Co., Ltd.  
**Document/order identifiers visible in the supplied source:** `EN*A0159`, `No.A0161`  
**Project source name:** `LC75826.pdf`

Topics used by this repository include:

- device overview and 208-segment capability;
- CE/CL/DI serial-interface pins;
- CCB address `41H`;
- four serial data formats selected by DD;
- P0–P3, DR, DN, FC0–FC2, OC, SC, and BU control fields;
- display-data D1…D208 to Sx/COMx correspondence;
- VDD/VSS and other IC pin functions;
- serial timing specifications.

### Online discovery links

- onsemi technical-documentation portal: https://www.onsemi.com/design/technical-documentation
- Datasheet Archive LC75826 search/catalog page: https://www.datasheetarchive.com/?q=lc75826

The repository does **not** bundle the manufacturer PDF. A user should verify that a downloaded copy matches the document identifiers above before relying on page/field descriptions here.

## Primary source 2 — Sony CDX-A250/A250EE service manual

**Document:** `CDX-A250 / CDX-A250EE Service Manual`  
**Version:** `Ver. 1.1`  
**Date:** `2006.01`  
**Sony publication number:** `9-879-865-02`  
**Project source name:** `sony_cdx-a250.pdf`

Sections materially used by this repository include:

- display-section block diagram;
- key-section printed wiring board;
- key-section schematic diagram;
- IC901 identification and connections;
- LCD901/panel context;
- CN901 interface signal names.

### Sony support page

Sony still hosts the CDX-A250 product-support page and operating/installation manuals:

https://www.sony.co.uk/electronics/support/mobile-cd-players-digital-media-players-cdx-series/cdx-a250

Sony's public support page does not currently expose the service manual used by this project. For the exact service document, search by the publication number **9-879-865-02** and verify model/version metadata before using a third-party copy.

One third-party catalog page that identifies the exact CDX-A250/A250EE Ver.1.1 service manual is:

https://remont-aud.net/load/car_audio/72-1-0-4830

That link is supplied as a discovery/reference aid, not as an endorsement of redistribution terms or download conditions.

## Project source — Arduino sketch

**File:** `LC75826_Sony_CDX_A250_panel_Car_Radio_V2.ino`

The repository copy is intended to preserve the supplied project reference source. The source header itself describes it as rough/reference code that can be adapted rather than a polished library.

At the time this staging repository was prepared, the project copy had SHA-256:

```text
2357d9ba8443bff3d322f1299a11764cd86c8f34f37af7daad159d069078d256
```

This hash is useful for confirming that later cleanup has not silently replaced the reference baseline.

## Project media / observations

The private production asset set contains:

- photographs of the original Sony head unit and removed panel board;
- photographs/video of the standalone LCD running all-on/all-off and text patterns;
- annotated board/wiring images;
- a handwritten segment-identification map;
- segment-identification footage;
- derived crops of the manufacturer/service documents used during the video.

Only media whose ownership and technical annotations are appropriate for public redistribution should be copied into this public repository.

## Why the PDFs and service-manual crops are not committed here

Manufacturer datasheets and service manuals are third-party copyrighted works. Having a local copy for research does not imply permission to redistribute it from this repository.

The default policy here is therefore:

1. cite the exact document and revision;
2. link to an official source when one is available;
3. otherwise provide enough publication metadata for the reader to locate a legitimate copy;
4. create original explanatory diagrams where possible instead of reproducing full manufacturer figures;
5. include a crop/screenshot only when there is a concrete educational need and its use/licensing has been considered.

## Technical authority note

A citation establishes where a statement came from; it does not make a project-specific interpretation automatically correct. Where this repository moves from documented IC behavior to the exact Sony-panel bench configuration, unresolved points are marked `NEEDS_ENGINEER_REVIEW`.