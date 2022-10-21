#include <combo.h>

/* Instant text */
static int DisplayTextBox_LoadFile(u8* dst, u32 vromAddr, u32 size)
{
    u8 buffer[1024];
    int ret;
    u32 i;
    u8 c;

    ret = LoadFile(buffer, vromAddr, size);
    *dst++ = 8;

    i = 0;
    while (i < size)
    {
        c = buffer[i++];
        *dst++ = c;

        switch (c)
        {
        case 0x05:
        case 0x13:
        case 0x0e:
        case 0x0c:
        case 0x1e:
        case 0x06:
        case 0x14:
        case 0x11:
            /* Skip 1 byte */
            *dst++ = buffer[i++];
            break;
        case 0x07:
        case 0x12:
            /* Skip 2 bytes */
            *dst++ = buffer[i++];
            *dst++ = buffer[i++];
            break;
        case 0x15:
            /* Skip 3 bytes */
            *dst++ = buffer[i++];
            *dst++ = buffer[i++];
            *dst++ = buffer[i++];
            break;
        case 0x04:
            /* Inject extra dfast text marker */
            *dst++ = 8;
            break;
        }
    }
    return ret;
}

PATCH_CALL(0x800dcc7c, DisplayTextBox_LoadFile);
