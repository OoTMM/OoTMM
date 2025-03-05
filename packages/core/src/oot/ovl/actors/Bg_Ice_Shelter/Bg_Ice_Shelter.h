#ifndef Z_BG_ICE_SHELTER_H
#define Z_BG_ICE_SHELTER_H

#include <combo.h>

struct Actor_BgIceShelter;

typedef void (*Actor_BgIceShelterActionFunc)(struct Actor_BgIceShelter*, struct PlayState*);

typedef enum RedIceType {
    /* 0 */ RED_ICE_LARGE,    // Large red ice block
    /* 1 */ RED_ICE_SMALL,    // Small red ice block
    /* 2 */ RED_ICE_PLATFORM, // Complex structure that can be climbed and walked on. Unused in vanilla OoT, used in MQ to cover the Ice Cavern Map chest
    /* 3 */ RED_ICE_WALL,     // Vertical ice sheets blocking corridors
    /* 4 */ RED_ICE_KING_ZORA // Giant red ice block covering King Zora
} RedIceType;

typedef struct Actor_BgIceShelter {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ Actor_BgIceShelterActionFunc actionFunc;
    /* 0x0168 */ ColliderCylinder cylinder1; // Used to detect blue fire and also as OC for no-dynapoly types
    /* 0x01B4 */ ColliderCylinder cylinder2; // Only used by no-dynapoly types to make weapons bounce off
    /* 0x0200 */ s16 alpha;
} Actor_BgIceShelter; // size = 0x0204

#endif
