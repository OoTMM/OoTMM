#include <combo.h>
#include <combo/csmc.h>

#define FMT_KEY(fmt, bpp)  (((fmt) << 4) | (bpp))

static int log2Pot(int pot)
{
    switch (pot)
    {
    case 1: return 0;
    case 2: return 1;
    case 4: return 2;
    case 8: return 3;
    case 16: return 4;
    case 32: return 5;
    case 64: return 6;
    }

    UNREACHABLE();
}

static u32 csmcResolveTexture(u8 custom, u32 seg)
{
    void* addr;

    if (!custom)
        return seg;

    addr = comboCacheGetFile(seg);
    return ((u32)addr - 0x80000000);
}

void* csmcLoadTexture(u8 custom, u32 addr, int fmt, int bpp, int w, int h, int mirror)
{
    Gfx buffer[16];
    Gfx* dlist;
    Gfx* dst;
    int l2w;
    int l2h;
    int mirrorFlag;

    addr = csmcResolveTexture(custom, addr);
    dlist = buffer;
    l2w = log2Pot(w);
    l2h = log2Pot(h);
    mirrorFlag = mirror ? G_TX_MIRROR : G_TX_NOMIRROR;

    switch (FMT_KEY(fmt, bpp))
    {
    /* RGBA16 */
    case FMT_KEY(G_IM_FMT_RGBA, G_IM_SIZ_16b):
        gDPLoadTextureBlock(dlist++, addr, G_IM_FMT_RGBA, G_IM_SIZ_16b, w, h, 0, mirrorFlag | G_TX_WRAP, mirrorFlag | G_TX_WRAP, l2w, l2h, 0, 0);
        break;
    }

    gSPEndDisplayList(dlist++);

    /* Alloc the new display list on the OPA */
    dst = gPlay->gs.gfx->polyOpa.end - (dlist - buffer);
    gPlay->gs.gfx->polyOpa.end = dst;
    memcpy(dst, buffer, (dlist - buffer) * sizeof(Gfx));

    return dst;
}

void* csmcLoadTextureEx(const CsmcDisplayList* d)
{
    Gfx buffer[16];
    Gfx* dlist;
    Gfx* dst;
    int l2w;
    int l2h;
    int mirrorFlag;
    int clampFlag;
    u32 addr;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    addr = csmcResolveTexture(!!(d->flags & CTF_CUSTOM_TEXTURE), d->addr);
    dlist = buffer;
    l2w = log2Pot(d->w);
    l2h = log2Pot(d->h);
    mirrorFlag = (d->flags & CTF_MIRROR) ? G_TX_MIRROR : G_TX_NOMIRROR;
    clampFlag = (d->flags & CTF_CLAMP) ? G_TX_CLAMP : G_TX_WRAP;

    switch (FMT_KEY(d->fmt, d->bpp))
    {
    /* RGBA16 */
    case FMT_KEY(G_IM_FMT_RGBA, G_IM_SIZ_16b):
        gDPLoadTextureBlock(dlist++, addr, G_IM_FMT_RGBA, G_IM_SIZ_16b, d->w, d->h, 0, mirrorFlag | clampFlag, mirrorFlag | clampFlag, l2w, l2h, 0, 0);
        break;
    }

    if (d->flags & CTF_COLOR)
    {
        r = (d->color >> 24) & 0xff;
        g = (d->color >> 16) & 0xff;
        b = (d->color >>  8) & 0xff;
        a = (d->color >>  0) & 0xff;
        gDPSetPrimColor(dlist++, 0, 0, r, g, b, a);
    }

    gSPEndDisplayList(dlist++);

    /* Alloc the new display list on the OPA */
    dst = gPlay->gs.gfx->polyOpa.end - (dlist - buffer);
    gPlay->gs.gfx->polyOpa.end = dst;
    memcpy(dst, buffer, (dlist - buffer) * sizeof(Gfx));

    return dst;
}
