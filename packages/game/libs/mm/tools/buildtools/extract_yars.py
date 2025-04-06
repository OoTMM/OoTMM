#!/usr/bin/env python3

# SPDX-FileCopyrightText: © 2023 ZeldaRET
# SPDX-License-Identifier: MIT

# yar (Yaz0 ARchive) decompressor
#
# This program decompresses every raw yar binary file listed in
# `tools/filelists/{version}/archives.csv` to a corresponding
# `extracted/{version}/baserom/{file}.unarchive` raw file.
#
# It works by decompressing every Yaz0 block and appending them one by one into
# a new raw binary file so it can be processed normally by other tools.


from __future__ import annotations

import argparse
import crunch64
import dataclasses
from pathlib import Path
import struct


PRINT_XML = False


@dataclasses.dataclass
class ArchiveMeta:
    start: int
    end: int


def readFileAsBytearray(filepath: Path) -> bytearray:
    with filepath.open(mode="rb") as f:
        return bytearray(f.read())


def readBytesAsWord(array_of_bytes: bytearray, offset: int) -> int:
    return struct.unpack_from(f">I", array_of_bytes, offset)[0]


def getOffsetsList(archiveBytes: bytearray) -> list[ArchiveMeta]:
    archivesOffsets: list[ArchiveMeta] = []

    firstEntryOffset = readBytesAsWord(archiveBytes, 0)
    firstEntrySize = readBytesAsWord(archiveBytes, 4)

    archivesOffsets.append(
        ArchiveMeta(firstEntryOffset, firstEntryOffset + firstEntrySize)
    )

    offset = 4
    while offset < firstEntryOffset - 4:
        entry = readBytesAsWord(archiveBytes, offset)
        nextEntry = readBytesAsWord(archiveBytes, offset + 4)
        entryStart = entry + firstEntryOffset
        entryEnd = nextEntry + firstEntryOffset
        archivesOffsets.append(ArchiveMeta(entryStart, entryEnd))

        offset += 4

    return archivesOffsets


def extractArchive(archivePath: Path, outPath: Path):
    archiveBytes = readFileAsBytearray(archivePath)

    if readBytesAsWord(archiveBytes, 0) == 0:
        # Empty file, ignore it
        return

    print(f"Extracting '{archivePath}' -> '{outPath}'")
    archivesOffsets = getOffsetsList(archiveBytes)

    if PRINT_XML:
        print("<Root>")
        print(f'    <File Name="{outPath.stem}">')

    with outPath.open("wb") as out:
        currentOffset = 0
        for meta in archivesOffsets:
            decompressedBytes = crunch64.yaz0.decompress(
                archiveBytes[meta.start : meta.end]
            )
            decompressedSize = len(decompressedBytes)
            out.write(decompressedBytes)

            if PRINT_XML:
                print(
                    f'        <Blob Name="{archivePath.stem}_Blob_{currentOffset:06X}" Size="0x{decompressedSize:04X}" Offset="0x{currentOffset:X}" />'
                )

            currentOffset += decompressedSize

    if PRINT_XML:
        print(f"    </File>")
        print("</Root>")


def main():
    parser = argparse.ArgumentParser(description="MM archives extractor")
    parser.add_argument(
        "baserom_segments_dir",
        type=Path,
        help="Directory of uncompressed ROM segments",
    )
    parser.add_argument(
        "-v",
        "--version",
        help="version to process",
        default="n64-us",
    )
    parser.add_argument("--xml", help="Generate xml to stdout", action="store_true")

    args = parser.parse_args()

    baseromSegmentsDir: Path = args.baserom_segments_dir
    version: str = args.version

    global PRINT_XML
    PRINT_XML = args.xml

    archivesCsvPath = Path(f"tools/filelists/{version}/archives.csv")

    with archivesCsvPath.open() as f:
        for line in f:
            archiveName = line.strip().split(",")[1]
            archivePath = baseromSegmentsDir / archiveName

            extractedPath = Path(str(archivePath) + ".unarchive")
            extractArchive(archivePath, extractedPath)


if __name__ == "__main__":
    main()
