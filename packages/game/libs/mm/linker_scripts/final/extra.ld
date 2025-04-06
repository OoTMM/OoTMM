ENTRY(entrypoint);

ASSERT ((_bootSegmentRomEnd <= 0x101000), "Error: _bootSegmentRomEnd is larger than 1 MB");
