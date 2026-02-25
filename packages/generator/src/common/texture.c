#include <combo.h>

void Grayscale(void* dst, u16 size)
{
    Color_RGBA8* c;
    u16 acc;

    for (u16 i = 0; i < size; ++i)
    {
        c = (Color_RGBA8*)dst + i;
        acc = (c->r + c->g * 2 + c->b) / 7;
        c->r = acc & 0xff;
        c->g = acc & 0xff;
        c->b = acc & 0xff;
    }
}
