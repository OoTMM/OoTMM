#ifndef COMBO_OBJECT_H
#define COMBO_OBJECT_H

#include <combo/types.h>

typedef struct PACKED ALIGNED(4)
{
    u32 vromStart;
    u32 vromEnd;
}
ObjectData;

extern const ObjectData kObjectsTable[];
extern const ObjectData kExtraObjectsTable[];
extern ObjectData kCustomObjectsTable[];

typedef struct
{
    u16 objectId;
    u16 offsets[8];
}
ObjectPatch;

#define OBJ_OOT_GI_KEY              0x0aa
#define OBJ_OOT_GI_JEWEL            0x0ad
#define OBJ_OOT_GI_MELODY           0x0b6
#define OBJ_OOT_GI_HEART            0x0b7
#define OBJ_OOT_GI_COMPASS          0x0b8
#define OBJ_OOT_GI_BOSSKEY          0x0b9
#define OBJ_OOT_GI_MEDAL            0x0ba
#define OBJ_OOT_GI_NUTS             0x0bb
#define OBJ_OOT_GI_HEARTS           0x0bd
#define OBJ_OOT_GI_ARROWCASE        0x0be
#define OBJ_OOT_GI_BOMBPOUCH        0x0bf
#define OBJ_OOT_GI_BOTTLE           0x0c6
#define OBJ_OOT_GI_STICK            0x0c7
#define OBJ_OOT_GI_MAP              0x0c8
#define OBJ_OOT_GI_SHIELD_1         0x0cb
#define OBJ_OOT_GI_MAGICPOT         0x0cd
#define OBJ_OOT_GI_BOMB_1           0x0ce
#define OBJ_OOT_GI_PURSE            0x0d1
#define OBJ_OOT_GI_GERUDO           0x0d7
#define OBJ_OOT_GI_ARROW            0x0d8
#define OBJ_OOT_GI_BOMB_2           0x0d9
#define OBJ_OOT_GI_EGG              0x0da
#define OBJ_OOT_GI_SCALE            0x0db
#define OBJ_OOT_GI_SHIELD_2         0x0dc
#define OBJ_OOT_GI_HOOKSHOT         0x0dd
#define OBJ_OOT_GI_OCARINA          0x0de
#define OBJ_OOT_GI_MILK             0x0df
#define OBJ_OOT_GI_PACHINKO         0x0e7
#define OBJ_OOT_GI_BOOMERANG        0x0e8
#define OBJ_OOT_GI_BOW              0x0e9
#define OBJ_OOT_GI_GLASSES          0x0ea
#define OBJ_OOT_GI_LIQUID           0x0eb
#define OBJ_OOT_GI_SHIELD_3         0x0ee
#define OBJ_OOT_GI_LETTER           0x0ef
#define OBJ_OOT_GI_CLOTHES          0x0f2
#define OBJ_OOT_GI_BEAN             0x0f3
#define OBJ_OOT_GI_FISH             0x0f4
#define OBJ_OOT_GI_SAW              0x0f5
#define OBJ_OOT_GI_HAMMER           0x0f6
#define OBJ_OOT_GI_GRASS            0x0f7
#define OBJ_OOT_GI_LONGSWORD        0x0f8
#define OBJ_OOT_GI_NIWATORI         0x109
#define OBJ_OOT_GI_BOTTLE_LETTER    0x10b
#define OBJ_OOT_GI_OCARINA_0        0x10e
#define OBJ_OOT_GI_BOOTS_2          0x118
#define OBJ_OOT_GI_SEED             0x119
#define OBJ_OOT_GI_GLOVES           0x12d
#define OBJ_OOT_GI_COIN             0x12e
#define OBJ_OOT_GI_KI_TAN_MASK      0x134
#define OBJ_OOT_GI_REDEAD_MASK      0x135
#define OBJ_OOT_GI_SKJ_MASK         0x136
#define OBJ_OOT_GI_RABIT_MASK       0x137
#define OBJ_OOT_GI_TRUTH_MASK       0x138
#define OBJ_OOT_GI_EYE_LOTION       0x13f
#define OBJ_OOT_GI_POWDER           0x140
#define OBJ_OOT_GI_MUSHROOM         0x141
#define OBJ_OOT_GI_TICKETSTONE      0x142
#define OBJ_OOT_GI_BROKENSWORD      0x143
#define OBJ_OOT_GI_PRESCRIPTION     0x146
#define OBJ_OOT_GI_BRACELET         0x147
#define OBJ_OOT_GI_SOLDOUT          0x148
#define OBJ_OOT_GI_FROG             0x149
#define OBJ_OOT_GI_GOLONMASK        0x150
#define OBJ_OOT_GI_ZORAMASK         0x151
#define OBJ_OOT_GI_GERUDOMASK       0x152
#define OBJ_OOT_GI_HOVERBOOTS       0x157
#define OBJ_OOT_GI_M_ARROW          0x158
#define OBJ_OOT_GI_SUTARU           0x15c
#define OBJ_OOT_GI_GODDESS          0x15d
#define OBJ_OOT_GI_FIRE             0x173
#define OBJ_OOT_GI_INSECT           0x174
#define OBJ_OOT_GI_BUTTERFLY        0x175
#define OBJ_OOT_GI_GHOST            0x176
#define OBJ_OOT_GI_SOUL             0x177
#define OBJ_OOT_GI_DEKUPOUCH        0x17b
#define OBJ_OOT_GI_RUPY             0x17f
#define OBJ_OOT_GI_SWORD_1          0x18d

#define OBJ_MM_LINK_CHILD           0x011
#define OBJ_MM_GI_KEY               0x086
#define OBJ_MM_GI_MELODY            0x08f
#define OBJ_MM_GI_HEART             0x090
#define OBJ_MM_GI_COMPASS           0x091
#define OBJ_MM_GI_BOSSKEY           0x092
#define OBJ_MM_GI_NUTS              0x094
#define OBJ_MM_GI_HEARTS            0x096
#define OBJ_MM_GI_ARROWCASE         0x097
#define OBJ_MM_GI_BOMBPOUCH         0x098
#define OBJ_MM_GI_BOTTLE            0x09e
#define OBJ_MM_GI_STICK             0x09f
#define OBJ_MM_GI_MAP               0x0a0
#define OBJ_MM_GI_MAGICPOT          0x0a4
#define OBJ_MM_GI_BOMB_1            0x0a5
#define OBJ_MM_GI_PURSE             0x0a8
#define OBJ_MM_GI_ARROW             0x0af
#define OBJ_MM_GI_BOMB_2            0x0b0
#define OBJ_MM_GI_SHIELD_2          0x0b3
#define OBJ_MM_GI_HOOKSHOT          0x0b4
#define OBJ_MM_GI_OCARINA           0x0b5
#define OBJ_MM_GI_MILK              0x0b6
#define OBJ_MM_GI_BOW               0x0bf
#define OBJ_MM_GI_GLASSES           0x0c0
#define OBJ_MM_GI_LIQUID            0x0c1
#define OBJ_MM_GI_SHIELD_3          0x0c3
#define OBJ_MM_GI_BEAN              0x0c6
#define OBJ_MM_GI_FISH              0x0c7
#define OBJ_MM_GI_LONGSWORD         0x0cb
#define OBJ_MM_GI_KI_TAN_MASK       0x100
#define OBJ_MM_GI_MASK18            0x102
#define OBJ_MM_GI_RABIT_MASK        0x103
#define OBJ_MM_GI_TRUTH_MASK        0x104
#define OBJ_MM_GI_SOLDOUT           0x113
#define OBJ_MM_GI_GOLONMASK         0x119
#define OBJ_MM_GI_ZORAMASK          0x11a
#define OBJ_MM_GI_M_ARROW           0x121
#define OBJ_MM_GI_SUTARU            0x125
#define OBJ_MM_GI_INSECT            0x137
#define OBJ_MM_GI_GHOST             0x139
#define OBJ_MM_GI_SOUL              0x13a
#define OBJ_MM_GI_RUPY              0x13f
#define OBJ_MM_GI_SWORD_1           0x148
#define OBJ_MM_GI_BOTTLE_RED        0x196
#define OBJ_MM_GI_BOTTLE_15         0x1ae
#define OBJ_MM_GI_RESERVE00         0x1b1
#define OBJ_MM_GI_RESERVE01         0x1b2
#define OBJ_MM_GI_NUTSMASK          0x1bd
#define OBJ_MM_GI_BIGBOMB           0x1ca
#define OBJ_MM_GI_GOLD_DUST         0x1e8
#define OBJ_MM_GI_BOTTLE_16         0x1e9
#define OBJ_MM_GI_BOTTLE_22         0x1ea
#define OBJ_MM_GI_LOACH             0x1ef
#define OBJ_MM_GI_SEAHORSE          0x1f0
#define OBJ_MM_GI_SWORD_2           0x1f9
#define OBJ_MM_GI_SWORD_3           0x1fa
#define OBJ_MM_GI_SWORD_4           0x1fb
#define OBJ_MM_GI_MASK09            0x209
#define OBJ_MM_GI_MASK14            0x20a
#define OBJ_MM_GI_MASK15            0x20b
#define OBJ_MM_GI_RESERVE_B_00      0x20f
#define OBJ_MM_GI_RESERVE_C_00      0x210
#define OBJ_MM_GI_RESERVE_C_01      0x215
#define OBJ_MM_GI_MAGICMUSHROOM     0x21d
#define OBJ_MM_GI_MASK10            0x21f
#define OBJ_MM_GI_MASK12            0x225
#define OBJ_MM_GI_MASK23            0x226
#define OBJ_MM_GI_BOTTLE_21         0x227
#define OBJ_MM_GI_CAMERA            0x228
#define OBJ_MM_GI_MASK03            0x242
#define OBJ_MM_GI_RESERVE_B_01      0x245
#define OBJ_MM_GI_FIELDMAP          0x24d
#define OBJ_MM_GI_SCHEDULE          0x253
#define OBJ_MM_GI_STONEMASK         0x254
#define OBJ_MM_GI_MASK05            0x258
#define OBJ_MM_GI_MASK11            0x259
#define OBJ_MM_GI_MASK20            0x25a
#define OBJ_MM_GI_MASK06            0x265
#define OBJ_MM_GI_MASK16            0x266
#define OBJ_MM_GI_MSSA              0x26b
#define OBJ_MM_GI_MASK21            0x26d
#define OBJ_MM_GI_BOTTLE_04         0x272
#define OBJ_MM_GI_MASK17            0x27d
#define OBJ_MM_GI_MASK22            0x27e
#define OBJ_MM_GI_MASK13            0x282

#endif
