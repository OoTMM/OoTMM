#include "multi.h"

static const u32 kCrc32Table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
};

u32 Multi_CRC32(const void* data, int size)
{
    const u8* p = (const u8*)data;
    u32 crc = 0xffffffff;

    for (size_t i = 0; i < size; i++)
    {
        crc ^= p[i];
        crc = (crc >> 4) ^ kCrc32Table[crc & 0xf];
        crc = (crc >> 4) ^ kCrc32Table[crc & 0xf];
    }

    return ~crc;
}
