#include <combo.h>
#include <assets/mm/objects/gameplay_field_keep.h>
#include <assets/mm/objects/object_ishi.h>

Gfx gOriginalFieldSmallRockOpaDL[] = {
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsDPLoadTextureBlock(0x05006810, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPBranchList(gFieldSmallRockOpaDL),
};

Gfx gOriginalSmallRockDL[] = {
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsDPLoadTextureBlock(0x060001b0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPBranchList(gSmallRockDL),
};
