#include <combo.h>

int comboMultibyteCharSize(u8 c)
{
    switch (c)
    {
    case 0x14:
        return 2;
    case 0x1b:
    case 0x1c:
    case 0x1d:
    case 0x1e:
    case 0x1f:
        return 3;
    }
    return 1;
}

/* Instant text */
static int DisplayTextBox_LoadFile(u8* dst, u32 vromAddr, u32 size)
{
    u8 buffer[1024];
    int ret;
    u32 i;
    u8 c;
    int len;

    ret = LoadFile(buffer, vromAddr, size);
    i = 0;

    /* Copy header */
    while (i < 11)
        *dst++ = buffer[i++];

    /* Set fast text */
    *dst++ = 0x17;
    while (i < size)
    {
        c = buffer[i++];
        if (c == 0x1e)
        {
            /* Disable fast text */
            *dst++ = 0x18;
        }
        *dst++ = c;

        len = comboMultibyteCharSize(c);
        while (len-- > 1)
            *dst++ = buffer[i++];

        switch (c)
        {
        case 0x10:
        case 0x12:
        case 0x1e:
            /* Inject extra fast text marker */
            *dst++ = 0x17;
            break;
        }
    }
    return ret;
}

PATCH_CALL(0x80151200, DisplayTextBox_LoadFile);
PATCH_CALL(0x8015126c, DisplayTextBox_LoadFile);
