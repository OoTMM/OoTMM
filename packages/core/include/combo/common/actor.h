#ifndef COMBO_COMMON_ACTOR_H
#define COMBO_COMMON_ACTOR_H

#if !defined(__ASSEMBLER__)
# include <combo/types.h>
# include <combo/util.h>
#endif

#if defined(GAME_OOT)
# define AC_PLAYER                  0x000
# define AC_EN_TEST                 0x002
# define AC_ITEM_GIVER              0x003
# define AC_EN_GIRLA                0x004
# define AC_CUSTOM_WARP             0x005
# define AC_CUSTOM_TRIGGERS         0x006
# define AC_EN_PART                 0x007
# define AC_EN_LIGHT                0x008
# define AC_EN_DOOR                 0x009
# define AC_EN_BOX                  0x00a
# define AC_BG_DY_YOSEIZO           0x00b
# define AC_BG_HIDAN_FIREWALL       0x00c
# define AC_EN_POH                  0x00d
# define AC_EN_OKUTA                0x00e
# define AC_BG_YDAN_SP              0x00f
# define AC_EN_BOM                  0x010
# define AC_EN_WALLMAS              0x011
# define AC_EN_DODONGO              0x012
# define AC_EN_FIREFLY              0x013
# define AC_EN_HORSE                0x014
# define AC_EN_ITEM00               0x015
# define AC_EN_ARROW                0x016
# define AC_CUSTOM_TORCH2           0x017
# define AC_EN_ELF                  0x018
# define AC_EN_NIW                  0x019
# define AC_EN_TITE                 0x01b
# define AC_EN_REEBA                0x01c
# define AC_EN_PEEHAT               0x01d
# define AC_EN_BUTTE                0x01e
# define AC_EN_INSECT               0x020
# define AC_EN_FISH                 0x021
# define AC_EN_HOLL                 0x023
# define AC_EN_SCENE_CHANGE         0x024
# define AC_EN_ZF                   0x025
# define AC_EN_HATA                 0x026
# define AC_BOSS_DODONGO            0x027
# define AC_BOSS_GOMA               0x028
# define AC_EN_ZL1                  0x029
# define AC_EN_VIEWER               0x02a
# define AC_EN_GOMA                 0x02b
# define AC_BG_PUSHBOX              0x02c
# define AC_EN_BUBBLE               0x02d
# define AC_DOOR_SHUTTER            0x02e
# define AC_EN_DODOJR               0x02f
# define AC_EN_BDFIRE               0x030
# define AC_EN_BOOM                 0x032
# define AC_EN_TORCH2               0x033
# define AC_EN_BILI                 0x034
# define AC_EN_TP                   0x035
# define AC_EN_ST                   0x037
# define AC_EN_BW                   0x038
# define AC_EN_A_OBJ                0x039
# define AC_EN_EIYER                0x03a
# define AC_EN_RIVER_SOUND          0x03b
# define AC_EN_HORSE_NORMAL         0x03c
# define AC_EN_OSSAN                0x03d
# define AC_BG_TREEMOUTH            0x03e
# define AC_BG_DODOAGO              0x03f
# define AC_BG_HIDAN_DALM           0x040
# define AC_BG_HIDAN_HROCK          0x041
# define AC_EN_HORSE_GANON          0x042
# define AC_BG_HIDAN_ROCK           0x043
# define AC_BG_HIDAN_RSEKIZOU       0x044
# define AC_BG_HIDAN_SEKIZOU        0x045
# define AC_BG_HIDAN_SIMA           0x046
# define AC_BG_HIDAN_SYOKU          0x047
# define AC_EN_XC                   0x048
# define AC_BG_HIDAN_CURTAIN        0x049
# define AC_BG_SPOT00_HANEBASI      0x04a
# define AC_EN_MB                   0x04b
# define AC_EN_BOMBF                0x04c
# define AC_EN_ZL2                  0x04d
# define AC_BG_HIDAN_FSLIFT         0x04e
# define AC_EN_OE2                  0x04f
# define AC_BG_YDAN_HASI            0x050
# define AC_BG_YDAN_MARUTA          0x051
# define AC_BOSS_GANONDROF          0x052
# define AC_EN_AM                   0x054
# define AC_EN_DEKUBABA             0x055
# define AC_EN_M_FIRE1              0x056
# define AC_EN_M_THUNDER            0x057
# define AC_BG_DDAN_JD              0x058
# define AC_BG_BREAKWALL            0x059
# define AC_EN_JJ                   0x05a
# define AC_EN_HORSE_ZELDA          0x05b
# define AC_BG_DDAN_KD              0x05c
# define AC_DOOR_WARP1              0x05d
# define AC_OBJ_SYOKUDAI            0x05e
# define AC_ITEM_B_HEART            0x05f
# define AC_EN_DEKUNUTS             0x060
# define AC_BG_MENKURI_KAITEN       0x061
# define AC_BG_MENKURI_EYE          0x062
# define AC_EN_VALI                 0x063
# define AC_BG_MIZU_MOVEBG          0x064
# define AC_BG_MIZU_WATER           0x065
# define AC_ARMS_HOOK               0x066
# define AC_EN_FHG                  0x067
# define AC_BG_MORI_HINERI          0x068
# define AC_EN_BB                   0x069
# define AC_BG_TOKI_HIKARI          0x06a
# define AC_EN_YUKABYUN             0x06b
# define AC_BG_TOKI_SWD             0x06c
# define AC_EN_FHG_FIRE             0x06d
# define AC_BG_MJIN                 0x06e
# define AC_BG_HIDAN_KOUSI          0x06f
# define AC_DOOR_TOKI               0x070
# define AC_BG_HIDAN_HAMSTEP        0x071
# define AC_EN_BIRD                 0x072
# define AC_EN_WOOD02               0x077
# define AC_EN_LIGHTBOX             0x07c
# define AC_EN_PU_BOX               0x07d
# define AC_EN_TRAP                 0x080
# define AC_EN_AROW_TRAP            0x081
# define AC_EN_VASE                 0x082
# define AC_EN_TA                   0x084
# define AC_EN_TK                   0x085
# define AC_BG_MORI_BIGST           0x086
# define AC_BG_MORI_ELEVATOR        0x087
# define AC_BG_MORI_KAITENKABE      0x088
# define AC_BG_MORI_RAKKATENJO      0x089
# define AC_EN_VM                   0x08a
# define AC_DEMO_EFFECT             0x08b
# define AC_DEMO_KANKYO             0x08c
# define AC_BG_HIDAN_FWBIG          0x08d
# define AC_EN_FLOORMAS             0x08e
# define AC_EN_HEISHI1              0x08f
# define AC_EN_RD                   0x090
# define AC_EN_PO_SISTERS           0x091
# define AC_BG_HEAVY_BLOCK          0x092
# define AC_BG_PO_EVENT             0x093
# define AC_OBJ_MURE                0x094
# define AC_EN_SW                   0x095
# define AC_BOSS_FD                 0x096
# define AC_OBJECT_KANKYO           0x097
# define AC_EN_DU                   0x098
# define AC_EN_FD                   0x099
# define AC_EN_HORSE_LINK_CHILD     0x09a
# define AC_DOOR_ANA                0x09b
# define AC_BG_SPOT02_OBJECTS       0x09c
# define AC_BG_HAKA                 0x09d
# define AC_MAGIC_WIND              0x09e
# define AC_MAGIC_FIRE              0x09f
# define AC_EN_RU1                  0x0a1
# define AC_BOSS_FD2                0x0a2
# define AC_EN_FD_FIRE              0x0a3
# define AC_EN_DH                   0x0a4
# define AC_EN_DHA                  0x0a5
# define AC_EN_RL                   0x0a6
# define AC_EN_ENCOUNT1             0x0a7
# define AC_DEMO_DU                 0x0a8
# define AC_DEMO_IM                 0x0a9
# define AC_DEMO_TRE_LGT            0x0aa
# define AC_EN_FW                   0x0ab
# define AC_BG_VB_SIMA              0x0ac
# define AC_EN_VB_BALL              0x0ad
# define AC_BG_HAKA_MEGANE          0x0ae
# define AC_BG_HAKA_MEGANEBG        0x0af
# define AC_BG_HAKA_SHIP            0x0b0
# define AC_BG_HAKA_SGAMI           0x0b1
# define AC_EN_HEISHI2              0x0b3
# define AC_EN_ENCOUNT2             0x0b4
# define AC_EN_FIRE_ROCK            0x0b5
# define AC_EN_BROB                 0x0b6
# define AC_MIR_RAY                 0x0b7
# define AC_BG_SPOT09_OBJ           0x0b8
# define AC_BG_SPOT18_OBJ           0x0b9
# define AC_BOSS_VA                 0x0ba
# define AC_BG_HAKA_TUBO            0x0bb
# define AC_BG_HAKA_TRAP            0x0bc
# define AC_BG_HAKA_HUTA            0x0bd
# define AC_BG_HAKA_ZOU             0x0be
# define AC_BG_SPOT17_FUNEN         0x0bf
# define AC_EN_SYATEKI_ITM          0x0c0
# define AC_EN_SYATEKI_MAN          0x0c1
# define AC_EN_TANA                 0x0c2
# define AC_EN_NB                   0x0c3
# define AC_BOSS_MO                 0x0c4
# define AC_EN_SB                   0x0c5
# define AC_EN_BIGOKUTA             0x0c6
# define AC_EN_KAREBABA             0x0c7
# define AC_BG_BDAN_OBJECTS         0x0c8
# define AC_DEMO_SA                 0x0c9
# define AC_DEMO_GO                 0x0ca
# define AC_EN_IN                   0x0cb
# define AC_EN_TR                   0x0cc
# define AC_BG_SPOT16_BOMBSTONE     0x0cd
# define AC_BG_HIDAN_KOWARERUKABE   0x0cf
# define AC_BG_BOMBWALL             0x0d0
# define AC_BG_SPOT08_ICEBLOCK      0x0d1
# define AC_EN_RU2                  0x0d2
# define AC_OBJ_DEKUJR              0x0d3
# define AC_BG_MIZU_UZU             0x0d4
# define AC_BG_SPOT06_OBJECTS       0x0d5
# define AC_BG_ICE_OBJECTS          0x0d6
# define AC_BG_HAKA_WATER           0x0d7
# define AC_EN_MA2                  0x0d9
# define AC_EN_BOM_CHU              0x0da
# define AC_EN_HORSE_GAME_CHECK     0x0db
# define AC_BOSS_TW                 0x0dc
# define AC_EN_RR                   0x0dd
# define AC_EN_BA                   0x0de
# define AC_EN_BX                   0x0df
# define AC_EN_ANUBICE              0x0e0
# define AC_EN_ANUBICE_FIRE         0x0e1
# define AC_BG_MORI_HASHIGO         0x0e2
# define AC_BG_MORI_HASHIRA4        0x0e3
# define AC_BG_MORI_IDOMIZU         0x0e4
# define AC_BG_SPOT16_DOUGHNUT      0x0e5
# define AC_BG_BDAN_SWITCH          0x0e6
# define AC_EN_MA1                  0x0e7
# define AC_BOSS_GANON              0x0e8
# define AC_BOSS_SST                0x0e9
# define AC_EN_NY                   0x0ec
# define AC_EN_FR                   0x0ed
# define AC_ITEM_SHIELD             0x0ee
# define AC_BG_ICE_SHELTER          0x0ef
# define AC_EN_ICE_HONO             0x0f0
# define AC_ITEM_OCARINA            0x0f1
# define AC_MAGIC_DARK              0x0f4
# define AC_DEMO_6K                 0x0f5
# define AC_EN_ANUBICE_TAG          0x0f6
# define AC_BG_HAKA_GATE            0x0f7
# define AC_BG_SPOT15_SAKU          0x0f8
# define AC_BG_JYA_GOROIWA          0x0f9
# define AC_BG_JYA_ZURERUKABE       0x0fa
# define AC_BG_JYA_COBRA            0x0fc
# define AC_BG_JYA_KANAAMI          0x0fd
# define AC_EN_FISHING              0x0fe
# define AC_OBJ_OSHIHIKI            0x0ff
# define AC_BG_GATE_SHUTTER         0x100
# define AC_EFF_DUST                0x101
# define AC_BG_SPOT01_FUSYA         0x102
# define AC_BG_SPOT01_IDOHASHIRA    0x103
# define AC_BG_SPOT01_IDOMIZU       0x104
# define AC_BG_PO_SYOKUDAI          0x105
# define AC_BG_GANON_OTYUKA         0x106
# define AC_BG_SPOT15_RRBOX         0x107
# define AC_BG_UMAJUMP              0x108
# define AC_ARROW_FIRE              0x10a
# define AC_ARROW_ICE               0x10b
# define AC_ARROW_LIGHT             0x10c
# define AC_ITEM_ETCETERA           0x10f
# define AC_OBJ_KIBAKO              0x110
# define AC_OBJ_TSUBO               0x111
# define AC_EN_WONDER_ITEM          0x112
# define AC_EN_IK                   0x113
# define AC_DEMO_IK                 0x114
# define AC_EN_SKJ                  0x115
# define AC_EN_SKJNEEDLE            0x116
# define AC_EN_G_SWITCH             0x117
# define AC_DEMO_EXT                0x118
# define AC_DEMO_SHD                0x119
# define AC_EN_DNS                  0x11a
# define AC_ELF_MSG                 0x11b
# define AC_EN_HONOTRAP             0x11c
# define AC_EN_TUBO_TRAP            0x11d
# define AC_OBJ_ICE_POLY            0x11e
# define AC_BG_SPOT03_TAKI          0x11f
# define AC_BG_SPOT07_TAKI          0x120
# define AC_EN_FZ                   0x121
# define AC_EN_PO_RELAY             0x122
# define AC_BG_RELAY_OBJECTS        0x123
# define AC_EN_DIVING_GAME          0x124
# define AC_EN_KUSA                 0x125
# define AC_OBJ_BEAN                0x126
# define AC_OBJ_BOMBIWA             0x127
# define AC_OBJ_SWITCH              0x12a
# define AC_OBJ_ELEVATOR            0x12b
# define AC_OBJ_LIFT                0x12c
# define AC_OBJ_HSBLOCK             0x12d
# define AC_EN_OKARINA_TAG          0x12e
# define AC_EN_YABUSAME_MARK        0x12f
# define AC_EN_GOROIWA              0x130
# define AC_EN_EX_RUPPY             0x131
# define AC_EN_TORYO                0x132
# define AC_EN_DAIKU                0x133
# define AC_EN_NWC                  0x135
# define AC_EN_BLKOBJ               0x136
# define AC_ITEM_INBOX              0x137
# define AC_EN_GE1                  0x138
# define AC_OBJ_BLOCKSTOP           0x139
# define AC_EN_SDA                  0x13a
# define AC_EN_CLEAR_TAG            0x13b
# define AC_EN_NIW_LADY             0x13c
# define AC_EN_GM                   0x13d
# define AC_EN_MS                   0x13e
# define AC_EN_HS                   0x13f
# define AC_BG_INGATE               0x140
# define AC_EN_KANBAN               0x141
# define AC_EN_HEISHI3              0x142
# define AC_EN_SYATEKI_NIW          0x143
# define AC_EN_ATTACK_NIW           0x144
# define AC_BG_SPOT01_IDOSOKO       0x145
# define AC_EN_SA                   0x146
# define AC_EN_WONDER_TALK          0x147
# define AC_BG_GJYO_BRIDGE          0x148
# define AC_EN_DS                   0x149
# define AC_EN_MK                   0x14a
# define AC_EN_BOM_BOWL_MAN         0x14b
# define AC_EN_BOM_BOWL_PIT         0x14c
# define AC_EN_OWL                  0x14d
# define AC_EN_ISHI                 0x14e
# define AC_OBJ_HANA                0x14f
# define AC_OBJ_LIGHTSWITCH         0x150
# define AC_OBJ_MURE2               0x151
# define AC_EN_GO                   0x152
# define AC_EN_FU                   0x153
# define AC_EN_CHANGER              0x155
# define AC_BG_JYA_MEGAMI           0x156
# define AC_BG_JYA_LIFT             0x157
# define AC_BG_JYA_BIGMIRROR        0x158
# define AC_BG_JYA_BOMBCHUIWA       0x159
# define AC_BG_JYA_AMISHUTTER       0x15a
# define AC_BG_JYA_BOMBIWA          0x15b
# define AC_BG_SPOT18_BASKET        0x15c
# define AC_EN_GANON_ORGAN          0x15e
# define AC_EN_SIOFUKI              0x15f
# define AC_EN_STREAM               0x160
# define AC_EN_MM                   0x162
# define AC_EN_KO                   0x163
# define AC_EN_KZ                   0x164
# define AC_EN_WEATHER_TAG          0x165
# define AC_BG_SST_FLOOR            0x166
# define AC_EN_ANI                  0x167
# define AC_EN_EX_ITEM              0x168
# define AC_BG_JYA_IRONOBJ          0x169
# define AC_EN_JS                   0x16a
# define AC_EN_JSJUTAN              0x16b
# define AC_EN_CS                   0x16c
# define AC_EN_MD                   0x16d
# define AC_EN_HY                   0x16e
# define AC_EN_GANON_MANT           0x16f
# define AC_EN_OKARINA_EFFECT       0x170
# define AC_EN_MAG                  0x171
# define AC_DOOR_GERUDO             0x172
# define AC_ELF_MSG2                0x173
# define AC_DEMO_GT                 0x174
# define AC_EN_PO_FIELD             0x175
# define AC_EFC_ERUPC               0x176
# define AC_BG_ZG                   0x177
# define AC_EN_HEISHI4              0x178
# define AC_EN_ZL3                  0x179
# define AC_BOSS_GANON2             0x17a
# define AC_EN_KAKASI               0x17b
# define AC_EN_TAKARA_MAN           0x17c
# define AC_OBJ_MAKEOSHIHIKI        0x17d
# define AC_OCEFF_SPOT              0x17e
# define AC_END_TITLE               0x17f
# define AC_EN_TORCH                0x181
# define AC_DEMO_EC                 0x182
# define AC_SHOT_SUN                0x183
# define AC_EN_DY_EXTRA             0x184
# define AC_EN_WONDER_TALK2         0x185
# define AC_EN_GE2                  0x186
# define AC_OBJ_ROOMTIMER           0x187
# define AC_EN_SSH                  0x188
# define AC_EN_STH                  0x189
# define AC_OCEFF_WIPE              0x18a
# define AC_OCEFF_STORM             0x18b
# define AC_EN_WEIYER               0x18c
# define AC_BG_SPOT05_SOKO          0x18d
# define AC_BG_JYA_1FLIFT           0x18e
# define AC_BG_JYA_HAHENIRON        0x18f
# define AC_BG_SPOT12_GATE          0x190
# define AC_BG_SPOT12_SAKU          0x191
# define AC_EN_HINTNUTS             0x192
# define AC_EN_NUTSBALL             0x193
# define AC_BG_SPOT00_BREAK         0x194
# define AC_EN_SHOPNUTS             0x195
# define AC_EN_IT                   0x196
# define AC_EN_GELDB                0x197
# define AC_OCEFF_WIPE2             0x198
# define AC_OCEFF_WIPE3             0x199
# define AC_EN_NIW_GIRL             0x19a
# define AC_EN_DOG                  0x19b
# define AC_EN_SI                   0x19c
# define AC_BG_SPOT01_OBJECTS2      0x19d
# define AC_OBJ_COMB                0x19e
# define AC_BG_SPOT11_BAKUDANKABE   0x19f
# define AC_OBJ_KIBAKO2             0x1a0
# define AC_EN_DNT_DEMO             0x1a1
# define AC_EN_DNT_JIJI             0x1a2
# define AC_EN_DNT_NOMAL            0x1a3
# define AC_EN_GUEST                0x1a4
# define AC_BG_BOM_GUARD            0x1a5
# define AC_EN_HS2                  0x1a6
# define AC_DEMO_KEKKAI             0x1a7
# define AC_BG_SPOT08_BAKUDANKABE   0x1a8
# define AC_BG_SPOT17_BAKUDANKABE   0x1a9
# define AC_OBJ_MURE3               0x1ab
# define AC_EN_TG                   0x1ac
# define AC_EN_MU                   0x1ad
# define AC_EN_GO2                  0x1ae
# define AC_EN_WF                   0x1af
# define AC_EN_SKB                  0x1b0
# define AC_DEMO_GJ                 0x1b1
# define AC_DEMO_GEFF               0x1b2
# define AC_BG_GND_FIREMEIRO        0x1b3
# define AC_BG_GND_DARKMEIRO        0x1b4
# define AC_BG_GND_SOULMEIRO        0x1b5
# define AC_BG_GND_NISEKABE         0x1b6
# define AC_BG_GND_ICEBLOCK         0x1b7
# define AC_EN_GB                   0x1b8
# define AC_EN_GS                   0x1b9
# define AC_BG_MIZU_BWALL           0x1ba
# define AC_BG_MIZU_SHUTTER         0x1bb
# define AC_EN_DAIKU_KAKARIKO       0x1bc
# define AC_BG_BOWL_WALL            0x1bd
# define AC_EN_WALL_TUBO            0x1be
# define AC_EN_PO_DESERT            0x1bf
# define AC_EN_CROW                 0x1c0
# define AC_DOOR_KILLER             0x1c1
# define AC_BG_SPOT11_OASIS         0x1c2
# define AC_BG_SPOT18_FUTA          0x1c3
# define AC_BG_SPOT18_SHUTTER       0x1c4
# define AC_EN_MA3                  0x1c5
# define AC_EN_COW                  0x1c6
# define AC_BG_ICE_TURARA           0x1c7
# define AC_BG_ICE_SHUTTER          0x1c8
# define AC_EN_KAKASI2              0x1c9
# define AC_EN_KAKASI3              0x1ca
# define AC_OCEFF_WIPE4             0x1cb
# define AC_EN_EG                   0x1cc
# define AC_BG_MENKURI_NISEKABE     0x1cd
# define AC_EN_ZO                   0x1ce
# define AC_OBJ_MAKEKINSUTA         0x1cf
# define AC_EN_GE3                  0x1d0
# define AC_OBJ_TIMEBLOCK           0x1d1
# define AC_OBJ_HAMISHI             0x1d2
# define AC_EN_ZL4                  0x1d3
# define AC_EN_MM2                  0x1d4
# define AC_BG_JYA_BLOCK            0x1d5
# define AC_OBJ_WARP2BLOCK          0x1d6
#endif

#if defined(GAME_MM)
# define AC_PLAYER              0x000
# define AC_EN_TEST             0x001
# define AC_EN_GIRLA            0x002
# define AC_EN_PART             0x003
# define AC_EN_LIGHT            0x004
# define AC_EN_DOOR             0x005
# define AC_EN_BOX              0x006
# define AC_EN_PAMETFROG        0x007
# define AC_EN_OKUTA            0x008
# define AC_EN_BOM              0x009
# define AC_EN_WALLMAS          0x00a
# define AC_EN_DODONGO          0x00b
# define AC_EN_FIREFLY          0x00c
# define AC_EN_HORSE            0x00d
# define AC_EN_ITEM00           0x00e
# define AC_EN_ARROW            0x00f
# define AC_EN_ELF              0x010
# define AC_EN_NIW              0x011
# define AC_EN_TITE             0x012
# define AC_EN_PEEHAT           0x014
# define AC_EN_BUTTE            0x015
# define AC_EN_INSECT           0x016
# define AC_EN_FISH             0x017
# define AC_EN_HOLL             0x018
# define AC_EN_DINOFOS          0x019
# define AC_EN_HATA             0x01a
# define AC_EN_ZL1              0x01b
# define AC_EN_VIEWER           0x01c
# define AC_EN_BUBBLE           0x01d
# define AC_DOOR_SHUTTER        0x01e
# define AC_ITEM_GIVER          0x01f
# define AC_EN_BOOM             0x020
# define AC_EN_TORCH2           0x021
# define AC_EN_MINIFROG         0x022
# define AC_CUSTOM_WARP         0x023
# define AC_CUSTOM_TRIGGERS     0x025
# define AC_EN_ST               0x024
# define AC_EN_A_OBJ            0x026
# define AC_OBJ_WTURN           0x027
# define AC_EN_RIVER_SOUND      0x028
# define AC_EN_OSSAN            0x02a
# define AC_CUSTOM_SPELL_FIRE   0x02b
# define AC_CUSTOM_SPELL_WIND   0x02c
# define AC_EN_FAMOS            0x02d
# define AC_CUSTOM_SPELL_LOVE   0x02e
# define AC_EN_BOMBF            0x02f
# define AC_EN_AM               0x032
# define AC_EN_DEKUBABA         0x033
# define AC_EN_M_FIRE1          0x034
# define AC_EN_M_THUNDER        0x035
# define AC_BG_BREAKWALL        0x036
# define AC_DOOR_WARP1          0x038
# define AC_OBJ_SYOKUDAI        0x039
# define AC_ITEM_B_HEART        0x03a
# define AC_EN_DEKUNUTS         0x03b
# define AC_EN_BBFALL           0x03c
# define AC_ARMS_HOOK           0x03d
# define AC_EN_BB               0x03e
# define AC_BG_KEIKOKU_SPR      0x03f
# define AC_EN_WOOD02           0x041
# define AC_EN_DEATH            0x043
# define AC_EN_MINIDEATH        0x044
# define AC_EN_VM               0x047
# define AC_DEMO_EFFECT         0x048
# define AC_DEMO_KANKYO         0x049
# define AC_EN_FLOORMAS         0x04a
# define AC_EN_RD               0x04c
# define AC_BG_F40_FLIFT        0x04d
# define AC_BG_HEAVY_BLOCK      0x04e
# define AC_OBJ_MURE            0x04f
# define AC_EN_SW               0x050
# define AC_OBJECT_KANKYO       0x051
# define AC_EN_HORSE_LINK_CHILD 0x054
# define AC_DOOR_ANA            0x055
# define AC_EN_ENCOUNT1         0x05b
# define AC_DEMO_TRE_LGT        0x05c
# define AC_EN_ENCOUNT2         0x05f
# define AC_EN_FIRE_ROCK        0x060
# define AC_BG_CTOWER_ROT       0x061
# define AC_MIR_RAY             0x062
# define AC_EN_SB               0x064
# define AC_EN_BIGSLIME         0x065
# define AC_EN_KAREBABA         0x066
# define AC_EN_IN               0x067
# define AC_EN_RU               0x069
# define AC_EN_BOM_CHU          0x06a
# define AC_EN_HORSE_GAME_CHECK 0x06b
# define AC_EN_RR               0x06c
# define AC_EN_FR               0x073
# define AC_EN_FISHING          0x079
# define AC_OBJ_OSHIHIKI        0x07a
# define AC_EFF_DUST            0x07b
# define AC_BG_UMAJUMP          0x07c
# define AC_ARROW_FIRE          0x07d
# define AC_ARROW_ICE           0x07e
# define AC_ARROW_LIGHT         0x07f
# define AC_ITEM_ETCETERA       0x080
# define AC_OBJ_KIBAKO          0x081
# define AC_OBJ_TSUBO           0x082
# define AC_EN_IK               0x084
# define AC_DEMO_SHD            0x089
# define AC_EN_DNS              0x08a
# define AC_ELF_MSG             0x08b
# define AC_EN_HONOTRAP         0x08c
# define AC_EN_TUBO_TRAP        0x08d
# define AC_OBJ_ICE_POLY        0x08e
# define AC_EN_FZ               0x08f
# define AC_EN_KUSA             0x090
# define AC_OBJ_BEAN            0x091
# define AC_OBJ_BOMBIWA         0x092
# define AC_OBJ_SWITCH          0x093
# define AC_OBJ_LIFT            0x095
# define AC_OBJ_HSBLOCK         0x096
# define AC_EN_OKARINA_TAG      0x097
# define AC_EN_GOROIWA          0x099
# define AC_EN_DAIKU            0x09c
# define AC_EN_NWC              0x09d
# define AC_ITEM_INBOX          0x09e
# define AC_EN_GE1              0x09f
# define AC_OBJ_BLOCKSTOP       0x0a0
# define AC_EN_SDA              0x0a1
# define AC_EN_CLEAR_TAG        0x0a2
# define AC_EN_GM               0x0a4
# define AC_EN_MS               0x0a5
# define AC_EN_HS               0x0a6
# define AC_BG_INGATE           0x0a7
# define AC_EN_KANBAN           0x0a8
# define AC_EN_ATTACK_NIW       0x0aa
# define AC_EN_MK               0x0ae
# define AC_EN_OWL              0x0af
# define AC_EN_ISHI             0x0b0
# define AC_OBJ_HANA            0x0b1
# define AC_OBJ_LIGHTSWITCH     0x0b2
# define AC_OBJ_MURE2           0x0b3
# define AC_EN_FU               0x0b5
# define AC_EN_STREAM           0x0b8
# define AC_EN_MM               0x0b9
# define AC_EN_WEATHER_TAG      0x0bc
# define AC_EN_ANI              0x0bd
# define AC_EN_JS               0x0bf
# define AC_EN_OKARINA_EFFECT   0x0c4
# define AC_EN_MAG              0x0c5
# define AC_ELF_MSG2            0x0c6
# define AC_BG_F40_SWLIFT       0x0c7
# define AC_EN_KAKASI           0x0ca
# define AC_OBJ_MAKEOSHIHIKI    0x0cb
# define AC_OCEFF_SPOT          0x0cc
# define AC_EN_TORCH            0x0ce
# define AC_SHOT_SUN            0x0d0
# define AC_OBJ_ROOMTIMER       0x0d3
# define AC_EN_SSH              0x0d4
# define AC_OCEFF_WIPE          0x0d6
# define AC_OCEFF_STORM         0x0d7
# define AC_OBJ_DEMO            0x0d8
# define AC_EN_MINISLIME        0x0d9
# define AC_EN_NUTSBALL         0x0da
# define AC_OCEFF_WIPE2         0x0df
# define AC_OCEFF_WIPE3         0x0e0
# define AC_EN_DG               0x0e2
# define AC_EN_SI               0x0e3
# define AC_OBJ_COMB            0x0e4
# define AC_OBJ_KIBAKO2         0x0e5
# define AC_EN_HS2              0x0e7
# define AC_OBJ_MURE3           0x0e8
# define AC_EN_TG               0x0e9
# define AC_EN_WF               0x0ec
# define AC_EN_SKB              0x0ed
# define AC_EN_GS               0x0ef
# define AC_OBJ_SOUND           0x0f0
# define AC_EN_CROW             0x0f1
# define AC_EN_COW              0x0f3
# define AC_OCEFF_WIPE4         0x0f6
# define AC_EN_ZO               0x0f8
# define AC_OBJ_MAKEKINSUTA     0x0f9
# define AC_EN_GE3              0x0fa
# define AC_OBJ_HAMISHI         0x0fc
# define AC_EN_ZL4              0x0fd
# define AC_EN_MM2              0x0fe
# define AC_DOOR_SPIRAL         0x100
# define AC_OBJ_PZLBLOCK        0x102
# define AC_OBJ_TOGE            0x103
# define AC_OBJ_ARMOS           0x105
# define AC_OBJ_BOYO            0x106
# define AC_EN_GRASSHOPPER      0x109
# define AC_OBJ_GRASS           0x10b
# define AC_OBJ_GRASS_CARRY     0x10c
# define AC_OBJ_GRASS_UNIT      0x10d
# define AC_BG_FIRE_WALL        0x110
# define AC_EN_BU               0x111
# define AC_EN_ENCOUNT3         0x112
# define AC_EN_JSO              0x113
# define AC_OBJ_CHIKUWA         0x114
# define AC_EN_KNIGHT           0x115
# define AC_EN_WARP_TAG         0x116
# define AC_EN_AOB_01           0x117
# define AC_EN_BOJ_01           0x118
# define AC_EN_BOJ_02           0x119
# define AC_EN_BOJ_03           0x11a
# define AC_EN_ENCOUNT4         0x11b
# define AC_EN_BOM_BOWL_MAN     0x11c
# define AC_EN_SYATEKI_MAN      0x11d
# define AC_BG_ICICLE           0x11f
# define AC_EN_SYATEKI_CROW     0x120
# define AC_EN_BOJ_04           0x121
# define AC_EN_CNE_01           0x122
# define AC_EN_BBA_01           0x123
# define AC_EN_BJI_01           0x124
# define AC_BG_SPDWEB           0x125
# define AC_EN_MT_TAG           0x128
# define AC_BOSS_01             0x129
# define AC_BOSS_02             0x12a
# define AC_BOSS_03             0x12b
# define AC_BOSS_04             0x12c
# define AC_BOSS_05             0x12d
# define AC_BOSS_06             0x12e
# define AC_BOSS_07             0x12f
# define AC_BG_DY_YOSEIZO       0x130
# define AC_EN_BOJ_05           0x132
# define AC_EN_SOB1             0x135
# define AC_EN_GO               0x138
# define AC_EN_RAF              0x13a
# define AC_OBJ_FUNEN           0x13b
# define AC_OBJ_RAILLIFT        0x13c
# define AC_BG_NUMA_HANA        0x13d
# define AC_OBJ_FLOWERPOT       0x13e
# define AC_OBJ_SPINYROLL       0x13f
# define AC_DM_HINA             0x140
# define AC_EN_SYATEKI_WF       0x141
# define AC_OBJ_SKATEBLOCK      0x142
# define AC_OBJ_ICEBLOCK        0x143
# define AC_EN_BIGPAMET         0x144
# define AC_EN_SYATEKI_DEKUNUTS 0x145
# define AC_ELF_MSG3            0x146
# define AC_EN_FG               0x147
# define AC_DM_RAVINE           0x148
# define AC_DM_SA               0x149
# define AC_EN_SLIME            0x14a
# define AC_EN_PR               0x14b
# define AC_OBJ_TOUDAI          0x14c
# define AC_OBJ_ENTOTU          0x14d
# define AC_OBJ_BELL            0x14e
# define AC_EN_SYATEKI_OKUTA    0x14f
# define AC_OBJ_SHUTTER         0x151
# define AC_DM_ZL               0x152
# define AC_EN_ELFGRP           0x153
# define AC_DM_TSG              0x154
# define AC_EN_BAGUO            0x155
# define AC_OBJ_VSPINYROLL      0x156
# define AC_OBJ_SMORK           0x157
# define AC_EN_TEST2            0x158
# define AC_EN_TEST3            0x159
# define AC_EN_TEST4            0x15a
# define AC_EN_BAT              0x15b
# define AC_EN_SEKIHI           0x15c
# define AC_EN_WIZ              0x15d
# define AC_EN_WIZ_BROCK        0x15e
# define AC_EN_WIZ_FIRE         0x15f
# define AC_EFF_CHANGE          0x160
# define AC_DM_STATUE           0x161
# define AC_OBJ_FIRESHIELD      0x162
# define AC_BG_LADDER           0x163
# define AC_EN_MKK              0x164
# define AC_DEMO_GETITEM        0x165
# define AC_EN_DNB              0x167
# define AC_EN_DNH              0x168
# define AC_EN_DNK              0x169
# define AC_EN_DNQ              0x16a
# define AC_BG_KEIKOKU_SAKU     0x16c
# define AC_OBJ_HUGEBOMBIWA     0x16d
# define AC_EN_FIREFLY2         0x16e
# define AC_EN_RAT              0x16f
# define AC_EN_WATER_EFFECT     0x170
# define AC_EN_KUSA2            0x171
# define AC_BG_SPOUT_FIRE       0x172
# define AC_BG_DBLUE_MOVEBG     0x174
# define AC_EN_DY_EXTRA         0x175
# define AC_EN_BAL              0x176
# define AC_EN_GINKO_MAN        0x177
# define AC_EN_WARP_UZU         0x178
# define AC_OBJ_DRIFTICE        0x179
# define AC_EN_LOOK_NUTS        0x17a
# define AC_EN_MUSHI2           0x17b
# define AC_EN_FALL             0x17c
# define AC_EN_MM3              0x17d
# define AC_BG_CRACE_MOVEBG     0x17e
# define AC_EN_DNO              0x17f
# define AC_EN_PR2              0x180
# define AC_EN_PRZ              0x181
# define AC_EN_JSO2             0x182
# define AC_OBJ_ETCETERA        0x183
# define AC_EN_EGOL             0x184
# define AC_OBJ_MINE            0x185
# define AC_OBJ_PURIFY          0x186
# define AC_EN_TRU              0x187
# define AC_EN_TRT              0x188
# define AC_EN_TEST5            0x18b
# define AC_EN_TEST6            0x18c
# define AC_EN_AZ               0x18d
# define AC_EN_ESTONE           0x18e
# define AC_BG_HAKUGIN_POST     0x18f
# define AC_DM_OPSTAGE          0x190
# define AC_DM_STK              0x191
# define AC_DM_CHAR00           0x192
# define AC_DM_CHAR01           0x193
# define AC_DM_CHAR02           0x194
# define AC_DM_CHAR03           0x195
# define AC_DM_CHAR04           0x196
# define AC_DM_CHAR05           0x197
# define AC_DM_CHAR06           0x198
# define AC_DM_CHAR07           0x199
# define AC_DM_CHAR08           0x19a
# define AC_DM_CHAR09           0x19b
# define AC_OBJ_TOKEIDAI        0x19c
# define AC_EN_MNK              0x19e
# define AC_EN_EGBLOCK          0x19f
# define AC_EN_GUARD_NUTS       0x1a0
# define AC_BG_HAKUGIN_BOMBWALL 0x1a1
# define AC_OBJ_TOKEI_TOBIRA    0x1a2
# define AC_BG_HAKUGIN_ELVPOLE  0x1a3
# define AC_EN_MA4              0x1a4
# define AC_EN_TWIG             0x1a5
# define AC_EN_PO_FUSEN         0x1a6
# define AC_EN_DOOR_ETC         0x1a7
# define AC_EN_BIGOKUTA         0x1a8
# define AC_BG_ICEFLOE          0x1a9
# define AC_OBJ_OCARINALIFT     0x1aa
# define AC_EN_TIME_TAG         0x1ab
# define AC_BG_OPEN_SHUTTER     0x1ac
# define AC_BG_OPEN_SPOT        0x1ad
# define AC_BG_FU_KAITEN        0x1ae
# define AC_OBJ_AQUA            0x1af
# define AC_EN_ELFORG           0x1b0
# define AC_EN_ELFBUB           0x1b1
# define AC_EN_FU_MATO          0x1b3
# define AC_EN_FU_KAGO          0x1b4
# define AC_EN_OSN              0x1b5
# define AC_BG_CTOWER_GEAR      0x1b6
# define AC_EN_TRT2             0x1b7
# define AC_OBJ_TOKEI_STEP      0x1b8
# define AC_BG_LOTUS            0x1b9
# define AC_EN_KAME             0x1ba
# define AC_OBJ_TAKARAYA_WALL   0x1bb
# define AC_BG_FU_MIZU          0x1bc
# define AC_EN_SELLNUTS         0x1bd
# define AC_BG_DKJAIL_IVY       0x1be
# define AC_OBJ_VISIBLOCK       0x1c0
# define AC_EN_TAKARAYA         0x1c1
# define AC_EN_TSN              0x1c2
# define AC_EN_DS2N             0x1c3
# define AC_EN_FSN              0x1c4
# define AC_EN_SHN              0x1c5
# define AC_EN_STOP_HEISHI      0x1c7
# define AC_OBJ_BIGICICLE       0x1c8
# define AC_EN_LIFT_NUTS        0x1c9
# define AC_EN_TK               0x1ca
# define AC_BG_MARKET_STEP      0x1cc
# define AC_OBJ_LUPYGAMELIFT    0x1cd
# define AC_EN_TEST7            0x1ce
# define AC_MIR_RAY2            0x1d0
# define AC_EN_WDHAND           0x1d1
# define AC_EN_GAMELUPY         0x1d2
# define AC_BG_DANPEI_MOVEBG    0x1d3
# define AC_EN_SNOWWD           0x1d4
# define AC_EN_PM               0x1d5
# define AC_EN_GAKUFU           0x1d6
# define AC_ELF_MSG4            0x1d7
# define AC_ELF_MSG5            0x1d8
# define AC_EN_COL_MAN          0x1d9
# define AC_EN_TALK_GIBUD       0x1da
# define AC_EN_GIANT            0x1db
# define AC_OBJ_SNOWBALL        0x1dc
# define AC_BOSS_HAKUGIN        0x1dd
# define AC_EN_GB2              0x1de
# define AC_EN_ONPUMAN          0x1df
# define AC_BG_TOBIRA01         0x1e0
# define AC_EN_TAG_OBJ          0x1e1
# define AC_OBJ_DHOUSE          0x1e2
# define AC_OBJ_HAKAISI         0x1e3
# define AC_BG_HAKUGIN_SWITCH   0x1e4
# define AC_EN_SNOWMAN          0x1e6
# define AC_TG_SW               0x1e7
# define AC_EN_PO_SISTERS       0x1e8
# define AC_EN_PP               0x1e9
# define AC_EN_HAKUROCK         0x1ea
# define AC_EN_HANABI           0x1eb
# define AC_OBJ_DOWSING         0x1ec
# define AC_OBJ_WIND            0x1ed
# define AC_EN_RACEDOG          0x1ee
# define AC_EN_KENDO_JS         0x1ef
# define AC_BG_BOTIHASIRA       0x1f0
# define AC_EN_FISH2            0x1f1
# define AC_EN_PST              0x1f2
# define AC_EN_POH              0x1f3
# define AC_OBJ_SPIDERTENT      0x1f4
# define AC_EN_ZORAEGG          0x1f5
# define AC_EN_KBT              0x1f6
# define AC_EN_GG               0x1f7
# define AC_EN_MARUTA           0x1f8
# define AC_OBJ_SNOWBALL2       0x1f9
# define AC_EN_GG2              0x1fa
# define AC_OBJ_GHAKA           0x1fb
# define AC_EN_DNP              0x1fc
# define AC_EN_DAI              0x1fd
# define AC_BG_GORON_OYU        0x1fe
# define AC_EN_KGY              0x1ff
# define AC_EN_INVADEPOH        0x200
# define AC_EN_GK               0x201
# define AC_EN_AN               0x202
# define AC_EN_BEE              0x204
# define AC_EN_OT               0x205
# define AC_EN_DRAGON           0x206
# define AC_OBJ_DORA            0x207
# define AC_EN_BIGPO            0x208
# define AC_OBJ_KENDO_KANBAN    0x209
# define AC_OBJ_HARIKO          0x20a
# define AC_EN_STH              0x20b
# define AC_BG_SINKAI_KABE      0x20c
# define AC_BG_HAKA_CURTAIN     0x20d
# define AC_BG_KIN2_BOMBWALL    0x20e
# define AC_BG_KIN2_FENCE       0x20f
# define AC_BG_KIN2_PICTURE     0x210
# define AC_BG_KIN2_SHELF       0x211
# define AC_EN_RAIL_SKB         0x212
# define AC_EN_JG               0x213
# define AC_EN_TRU_MT           0x214
# define AC_OBJ_UM              0x215
# define AC_EN_NEO_REEBA        0x216
# define AC_BG_MBAR_CHAIR       0x217
# define AC_BG_IKANA_BLOCK      0x218
# define AC_BG_IKANA_MIRROR     0x219
# define AC_BG_IKANA_ROTARYROOM 0x21a
# define AC_BG_DBLUE_BALANCE    0x21b
# define AC_BG_DBLUE_WATERFALL  0x21c
# define AC_EN_KAIZOKU          0x21d
# define AC_EN_GE2              0x21e
# define AC_EN_MA_YTS           0x21f
# define AC_EN_MA_YTO           0x220
# define AC_OBJ_TOKEI_TURRET    0x221
# define AC_BG_DBLUE_ELEVATOR   0x222
# define AC_OBJ_WARPSTONE       0x223
# define AC_EN_ZOG              0x224
# define AC_OBJ_ROTLIFT         0x225
# define AC_OBJ_JG_GAKKI        0x226
# define AC_BG_INIBS_MOVEBG     0x227
# define AC_EN_ZOT              0x228
# define AC_OBJ_TREE            0x229
# define AC_OBJ_Y2LIFT          0x22a
# define AC_OBJ_Y2SHUTTER       0x22b
# define AC_OBJ_BOAT            0x22c
# define AC_OBJ_TARU            0x22d
# define AC_OBJ_HUNSUI          0x22e
# define AC_EN_JC_MATO          0x22f
# define AC_MIR_RAY3            0x230
# define AC_EN_ZOB              0x231
# define AC_ELF_MSG6            0x232
# define AC_OBJ_NOZOKI          0x233
# define AC_EN_TOTO             0x234
# define AC_EN_RAILGIBUD        0x235
# define AC_EN_BABA             0x236
# define AC_EN_SUTTARI          0x237
# define AC_EN_ZOD              0x238
# define AC_EN_KUJIYA           0x239
# define AC_EN_GEG              0x23a
# define AC_OBJ_KINOKO          0x23b
# define AC_OBJ_YASI            0x23c
# define AC_EN_TANRON1          0x23d
# define AC_EN_TANRON2          0x23e
# define AC_EN_TANRON3          0x23f
# define AC_OBJ_CHAN            0x240
# define AC_EN_ZOS              0x241
# define AC_EN_S_GORO           0x242
# define AC_EN_NB               0x243
# define AC_EN_JA               0x244
# define AC_BG_F40_BLOCK        0x245
# define AC_BG_F40_SWITCH       0x246
# define AC_EN_PO_COMPOSER      0x247
# define AC_EN_GURUGURU         0x248
# define AC_OCEFF_WIPE5         0x249
# define AC_EN_STONE_HEISHI     0x24a
# define AC_OCEFF_WIPE6         0x24b
# define AC_EN_SCOPENUTS        0x24c
# define AC_EN_SCOPECROW        0x24d
# define AC_OCEFF_WIPE7         0x24e
# define AC_EFF_KAMEJIMA_WAVE   0x24f
# define AC_EN_HG               0x250
# define AC_EN_HGO              0x251
# define AC_EN_ZOV              0x252
# define AC_EN_AH               0x253
# define AC_OBJ_HGDOOR          0x254
# define AC_BG_IKANA_BOMBWALL   0x255
# define AC_BG_IKANA_RAY        0x256
# define AC_BG_IKANA_SHUTTER    0x257
# define AC_BG_HAKA_BOMBWALL    0x258
# define AC_BG_HAKA_TOMB        0x259
# define AC_EN_SC_RUPPE         0x25a
# define AC_BG_IKNV_DOUKUTU     0x25b
# define AC_BG_IKNV_OBJ         0x25c
# define AC_EN_PAMERA           0x25d
# define AC_OBJ_HSSTUMP         0x25e
# define AC_EN_HIDDEN_NUTS      0x25f
# define AC_EN_ZOW              0x260
# define AC_EN_TALK             0x261
# define AC_EN_AL               0x262
# define AC_EN_TAB              0x263
# define AC_EN_NIMOTSU          0x264
# define AC_EN_HIT_TAG          0x265
# define AC_EN_RUPPECROW        0x266
# define AC_EN_TANRON4          0x267
# define AC_EN_TANRON5          0x268
# define AC_EN_TANRON6          0x269
# define AC_EN_DAIKU2           0x26a
# define AC_EN_MUTO             0x26b
# define AC_EN_BAISEN           0x26c
# define AC_EN_HEISHI           0x26d
# define AC_EN_DEMO_HEISHI      0x26e
# define AC_EN_DT               0x26f
# define AC_EN_CHA              0x270
# define AC_OBJ_DINNER          0x271
# define AC_EFF_LASTDAY         0x272
# define AC_BG_IKANA_DHARMA     0x273
# define AC_EN_AKINDONUTS       0x274
# define AC_EFF_STK             0x275
# define AC_EN_IG               0x276
# define AC_EN_RG               0x277
# define AC_EN_OSK              0x278
# define AC_EN_STH2             0x279
# define AC_EN_YB               0x27a
# define AC_EN_RZ               0x27b
# define AC_EN_SCOPECOIN        0x27c
# define AC_EN_BJT              0x27d
# define AC_EN_BOMJIMA          0x27e
# define AC_EN_BOMJIMB          0x27f
# define AC_EN_BOMBERS          0x280
# define AC_EN_BOMBERS2         0x281
# define AC_EN_BOMBAL           0x282
# define AC_OBJ_MOON_STONE      0x283
# define AC_OBJ_MU_PICT         0x284
# define AC_BG_IKNINSIDE        0x285
# define AC_EFF_ZORABAND        0x286
# define AC_OBJ_KEPN_KOYA       0x287
# define AC_OBJ_USIYANE         0x288
# define AC_EN_NNH              0x289
# define AC_OBJ_KZSAKU          0x28a
# define AC_OBJ_MILK_BIN        0x28b
# define AC_EN_KITAN            0x28c
# define AC_BG_ASTR_BOMBWALL    0x28d
# define AC_BG_IKNIN_SUSCEIL    0x28e
# define AC_EN_BSB              0x28f
# define AC_EN_RECEPGIRL        0x290
# define AC_EN_THIEFBIRD        0x291
# define AC_EN_JGAME_TSN        0x292
# define AC_OBJ_JGAME_LIGHT     0x293
# define AC_OBJ_YADO            0x294
# define AC_DEMO_SYOTEN         0x295
# define AC_DEMO_MOONEND        0x296
# define AC_BG_LBFSHOT          0x297
# define AC_BG_LAST_BWALL       0x298
# define AC_EN_AND              0x299
# define AC_EN_INVADEPOH_DEMO   0x29a
# define AC_OBJ_DANPEILIFT      0x29b
# define AC_EN_FALL2            0x29c
# define AC_DM_AL               0x29d
# define AC_DM_AN               0x29e
# define AC_DM_AH               0x29f
# define AC_DM_NB               0x2a0
# define AC_EN_DRS              0x2a1
# define AC_EN_ENDING_HERO      0x2a2
# define AC_DM_BAL              0x2a3
# define AC_EN_PAPER            0x2a4
# define AC_EN_HINT_SKB         0x2a5
# define AC_DM_TAG              0x2a6
# define AC_EN_BH               0x2a7
# define AC_EN_ENDING_HERO2     0x2a8
# define AC_EN_ENDING_HERO3     0x2a9
# define AC_EN_ENDING_HERO4     0x2aa
# define AC_EN_ENDING_HERO5     0x2ab
# define AC_EN_ENDING_HERO6     0x2ac
# define AC_DM_GM               0x2ad
# define AC_OBJ_SWPRIZE         0x2ae
# define AC_EN_INVISIBLE_RUPPE  0x2af
# define AC_OBJ_ENDING          0x2b0
# define AC_EN_RSN              0x2b1

#endif

#if !defined(__ASSEMBLER__)
typedef struct GameState_Play GameState_Play;
typedef struct ActorContext ActorContext;
typedef struct Actor Actor;

typedef void (*ActorCallback)(Actor*, GameState_Play*);

typedef struct
{
    Vec3f pos;
    Vec3s rot;
}
PosRot;

typedef struct Actor
{
    u16         id;
    u8          type;
    u8          room;
    s32         flags;
    PosRot      home;
    u16         variable;
    s8          objTableIndex;
    s8          targetMode;

#if defined(GAME_OOT)
    u16        sfx;
    PosRot     world;
    PosRot     focus;
#endif

#if defined(GAME_MM)
    s16        halfDaysBits;
    PosRot     world;
    s8         csId;
    u8         audioFlags;
    PosRot     focus;
    u16        sfx;
#endif

    float       targetArrowOffset;
    Vec3f       scale;
    Vec3f       velocity;
    float       speedXZ;
    float       gravity;
    float       minVelocityY;
    CollisionPoly* wallPoly; // Wall polygon the actor is touching
    CollisionPoly* floorPoly; // Floor polygon directly below the actor
    u8          wallBgId; // Bg ID of the wall polygon the actor is touching
    u8          floorBgId; // Bg ID of the floor polygon directly below the actor
    s16         wallYaw; // Y rotation of the wall polygon the actor is touching
    f32         floorHeight; // Y position of the floor polygon directly below the actor
    f32         yDistToWater; // Distance to the surface of active waterbox. Negative value means above water
    u16         bgCheckFlags; // Flags indicating how the actor is interacting with collision
    s16         yawTowardsPlayer; // Y rotation difference between the actor and the player
    f32         xyzDistToPlayerSq; // Squared distance between the actor and the player
    f32         xzDistanceFromLink;
    f32         yDistanceFromLink;
    CollisionCheckInfo colChkInfo;
    Vec3s       rot2;
    char        unk_ba[2];
    float       modelOffsetY;
    char        unk_c0[0x4e];
    u16         messageId;
    u16         freezeTimer;
    char        unk_11A[0x6];
    Actor*      parent;
    Actor*      child;
    Actor*      prev;
    Actor*      next;
    void*       init;
    void*       fini;
    void*       update;
    void*       draw;
    void*       ovl;
}
Actor;

#if defined(GAME_MM)
typedef struct
{
    u32     count;
    Actor*  first;
    char    unk_8[0x4];
}
ActorList;
#endif

#if defined(GAME_OOT)
# define X(x) (x)
#else
# define X(x) (x + 8)
#endif

ASSERT_OFFSET(Actor, id,        0x000);
ASSERT_OFFSET(Actor, type,      0x002);
ASSERT_OFFSET(Actor, room,      0x003);
ASSERT_OFFSET(Actor, home,      0x008);
ASSERT_OFFSET(Actor, variable,  0x01c);
ASSERT_OFFSET(Actor, parent, X(0x118));
ASSERT_OFFSET(Actor, child, X(0x11c));
ASSERT_OFFSET(Actor, prev,      X(0x120));
ASSERT_OFFSET(Actor, next,      X(0x124));
ASSERT_OFFSET(Actor, init,      X(0x128));
ASSERT_OFFSET(Actor, fini,      X(0x12c));
ASSERT_OFFSET(Actor, update,    X(0x130));
ASSERT_OFFSET(Actor, draw,      X(0x134));
ASSERT_OFFSET(Actor, ovl,       X(0x138));

#undef X


#if defined(GAME_OOT)
_Static_assert(sizeof(Actor) == 0x13c, "OoT Actor size is wrong");
#endif

#if defined(GAME_MM)
_Static_assert(sizeof(Actor) == 0x144, "MM Actor size is wrong");
_Static_assert(sizeof(ActorList) == 0xC, "MM ActorList size is wrong");
#endif

typedef struct BlinkInfo {
    /* 0x0 */ s16 eyeTexIndex;
    /* 0x2 */ s16 blinkTimer;
} BlinkInfo; // size = 0x4

Actor* comboSpawnActor(ActorContext* actorCtx, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable);
Actor* comboSpawnActorEx(ActorContext* actorCtx, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3);

typedef enum {
    /* 0x00 */ ACTORCAT_SWITCH,
    /* 0x01 */ ACTORCAT_BG,
    /* 0x02 */ ACTORCAT_PLAYER,
    /* 0x03 */ ACTORCAT_EXPLOSIVE,
    /* 0x04 */ ACTORCAT_NPC,
    /* 0x05 */ ACTORCAT_ENEMY,
    /* 0x06 */ ACTORCAT_PROP,
    /* 0x07 */ ACTORCAT_ITEMACTION,
    /* 0x08 */ ACTORCAT_MISC,
    /* 0x09 */ ACTORCAT_BOSS,
    /* 0x0A */ ACTORCAT_DOOR,
    /* 0x0B */ ACTORCAT_CHEST,
    /* 0x0C */ ACTORCAT_MAX
} ActorCategory;

#endif

#endif
