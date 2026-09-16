# Licensing and attribution

This repository combines several classes of material that should not be treated as if they share one copyright owner.

## 1. Arduino/project code

The supplied `.ino` file does not contain a clear software-license grant in its header, and the current production record does not establish ownership/licensing terms.

Before this repository is made public:

- confirm who owns the code and has authority to license it;
- choose an explicit software license;
- preserve any author credit requested by the code owner.

### Recommended option if the owner agrees

For tutorial/reference firmware intended to be copied and adapted, **MIT** is a practical permissive license. BSD-2-Clause would also fit.

This is a licensing recommendation, not an assertion that the current source is already MIT-licensed.

`NEEDS_RIGHTSHOLDER_CONFIRMATION`

## 2. Original photographs and original diagrams

Project photographs and diagrams created specifically for this tutorial can be licensed separately from the code.

If the creator wants viewers to reuse documentation assets with attribution, **CC BY 4.0** is a practical choice. If reuse is not intended, leave them under normal copyright rather than attaching a Creative Commons license accidentally.

`NEEDS_RIGHTSHOLDER_CONFIRMATION`

## 3. Manufacturer datasheet

The SANYO/onsemi LC75826 datasheet is third-party documentation. This repository should:

- cite it accurately;
- link to an authorized or reputable discovery source;
- not claim copyright ownership;
- not place it under this repository's code/documentation license.

The current staging repository therefore does **not** copy `LC75826.pdf` into Git.

## 4. Sony service manual

The Sony CDX-A250/A250EE service manual is a Sony publication and should likewise be cited/linked rather than automatically redistributed.

The current staging repository does **not** copy `sony_cdx-a250.pdf` into Git.

## 5. Crops/screenshots derived from third-party manuals

A crop made from a manufacturer/service manual remains derived from third-party copyrighted material. Technical usefulness and copyright status are separate questions.

For a durable public tutorial, prefer:

- original redraws of the small amount of information actually needed;
- textual pin/signal tables with source citation;
- original photographs of the project hardware.

If a manual crop is included later, its purpose and attribution should be explicit and its legal basis should not be implied by the repository's general license.

## 6. Trademarks

`Sony`, `SANYO`, `onsemi`, `Arduino`, and product/part names belong to their respective owners. Their appearance in this repository is for identification/reference and does not imply sponsorship or affiliation.

## Suggested final licensing layout

Once ownership is confirmed, a clean arrangement would be:

```text
LICENSE                 # software license, e.g. MIT
ASSET_LICENSE.md        # license for original project images/diagrams
THIRD_PARTY_NOTICES.md  # manufacturer docs/trademarks/source attribution
```

Until then, `LICENSE-NOTICE.md` at the repository root records that no blanket license is being silently granted.