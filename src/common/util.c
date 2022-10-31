#include <combo.h>

u32 popcount(u32 x)
{
    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f);
    x = (x & 0x00ff00ff) + ((x >> 8) & 0x00ff00ff);
    x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);
    return x;
}

void* actorAddr(u16 actorId, u32 addr)
{
    ActorOvl* ovl;
    u32 offset;

    ovl = gActorOvl + actorId;
    offset = addr - ovl->vramStart;
    return (char*)ovl->data + offset;
}
