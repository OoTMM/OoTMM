#include <combo.h>

int comboMultibyteCharSize(u8 c)
{
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
        return 2;
    case 0x07:
    case 0x12:
        return 3;
    case 0x15:
        return 4;
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
    *dst++ = 8;

    i = 0;
    while (i < size)
    {
        c = buffer[i++];
        if (c == 0x10)
        {
            *dst++ = 9;
        }
        *dst++ = c;
        len = comboMultibyteCharSize(c);
        while (len-- > 1)
            *dst++ = buffer[i++];
        if (c == 0x04)
        {
            *dst++ = 8;
        }
    }
    return ret;
}

PATCH_CALL(0x800dcc7c, DisplayTextBox_LoadFile);
