#ifndef COMBO_COMMON_EVENTS_H
#define COMBO_COMMON_EVENTS_H

#define EV_OOT_CHK_DEKU_MIDO_MEET           0x02
#define EV_OOT_CHK_DEKU_MIDO_SWORD_SHIELD   0x04
#define EV_OOT_CHK_EMERALD_TREE_DEAD        0x07
#define EV_OOT_CHK_GOHMA                    0x09
#define EV_OOT_CHK_DEKU_TREE_MEET           0x0c
#define EV_OOT_CHK_MALON_SPOKEN_HYRULE      0x10
#define EV_OOT_CHK_MALON_EGG                0x12
#define EV_OOT_CHK_TREE_DEAD                0x17
#define EV_OOT_CHK_EPONA                    0x18
#define EV_OOT_CHK_STONE_EMERALD            0x19
#define EV_OOT_CHK_MIDO_TREE_DEAD           0x1c
#define EV_OOT_CHK_BOMBCHU_BOWLING          0x22
#define EV_OOT_CHK_STONE_RUBY               0x25
#define EV_OOT_CHK_RUTO_LETTER              0x32
#define EV_OOT_CHK_STONE_SAPPHIRE           0x37
#define EV_OOT_CHK_ZELDA_LETTER             0x40
#define EV_OOT_CHK_OCARINA_OF_TIME          0x43
#define EV_OOT_CHK_MASTER_SWORD_PULLED      0x45
#define EV_OOT_CHK_MEDALLION_FOREST         0x48
#define EV_OOT_CHK_MEDALLION_FIRE           0x49
#define EV_OOT_CHK_MEDALLION_WATER          0x4a
#define EV_OOT_CHK_DOOR_TIME                0x4b
#define EV_OOT_CHK_RAINBOW_BRIDGE           0x4d
#define EV_OOT_CHK_MASTER_SWORD_CHAMBER     0x4f
#define EV_OOT_CHK_SONG_TP_FOREST           0x50
#define EV_OOT_CHK_SONG_TP_FIRE             0x51
#define EV_OOT_CHK_SONG_TP_WATER            0x52
#define EV_OOT_CHK_SONG_TP_SHADOW           0x54
#define EV_OOT_CHK_SONG_TP_LIGHT            0x55
#define EV_OOT_CHK_SONG_ZELDA               0x59
#define EV_OOT_CHK_SONG_SUN                 0x5a
#define EV_OOT_CHK_SONG_STORMS              0x5b
#define EV_OOT_CHK_WINDMILL_FAST            0x65
#define EV_OOT_CHK_WELL_DRAINED             0x67
#define EV_OOT_CHK_LAKE_HYLIA_WATER         0x69
#define EV_OOT_CHK_ZELDA_FLED               0x80
#define EV_OOT_CHK_ZELDA_FLED_BRIDGE        0x82
#define EV_OOT_CHK_SONG_TIME                0xa9
#define EV_OOT_CHK_BONGO_ESCAPE             0xaa
#define EV_OOT_CHK_SONG_TP_SPIRIT           0xac
#define EV_OOT_CHK_SARIA_OCARINA            0xc1
#define EV_OOT_CHK_LIGHT_ARROW              0xc4
#define EV_OOT_CHK_MEDALLION_SPIRIT         0xc8
#define EV_OOT_CHK_FROGS_GAME               0xd0

#define EV_OOT_CHK_TRIAL_LIGHT              0xbf
#define EV_OOT_CHK_TRIAL_FIRE               0xbe
#define EV_OOT_CHK_TRIAL_SHADOW             0xbd
#define EV_OOT_CHK_TRIAL_WATER              0xbc
#define EV_OOT_CHK_TRIAL_FOREST             0xbb
#define EV_OOT_CHK_TRIAL_SPIRIT             0xad
#define EN_OOT_CHK_GANON_BARRIER            0xc3

#define EV_OOT_ITEM_DEKU_SEEDS              0x13
#define EV_OOT_ITEM_DAMPE_GRAVEYARD_HP      0x1c
#define EV_OOT_ITEM_ANJU_POCKET_EGG         0x2c

#define EV_OOT_INF_KING_ZORA_THAWED         0x138
#define EV_OOT_INF_MAGIC_JAR                0x198

/* Custom flags that seems unused */
#define EV_OOT_CHK_SONG_SARIA               0x58
#define EV_OOT_CHK_MEDALLION_SHADOW         0x61
#define EV_OOT_CHK_SONG_EPONA               0x62
#define EV_OOT_ITEM_GORON_BRACELET          0x20
#define EV_OOT_INF_MEDIGORON                0xb2

#define MM_EV(a, b) (((a) << 3) | (b))

#define EV_MM_WEEK_ENTERED_TERMINA              MM_EV(0x00, 1)
#define EV_MM_WEEK_ENTERED_CLOCK_TOWN_EAST      MM_EV(0x02, 3)
#define EV_MM_WEEK_ENTERED_CLOCK_TOWN_WEST      MM_EV(0x02, 4)
#define EV_MM_WEEK_ENTERED_CLOCK_TOWN_NORTH     MM_EV(0x02, 5)
#define EV_MM_WEEK_MONKEY_PUNISHED              MM_EV(0x09, 7)
#define EV_MM_WEEK_TOWN_GUARDS                  MM_EV(0x0c, 5)
#define EV_MM_WEEK_FOREST_WITCH                 MM_EV(0x10, 4)
#define EV_MM_WEEK_SPIN_UPGRADE                 MM_EV(0x17, 1)
#define EV_MM_WEEK_FIRST_CYCLE                  MM_EV(0x3b, 2)
#define EV_MM_WEEK_BEAVER_RACE1_DONE            MM_EV(0x5d, 0)
#define EV_MM_WEEK_KEETA_DEAD                   MM_EV(23,   2)
#define EV_MM_WEEK_PRINCESS_RETURNED            MM_EV(23,   5)
#define EV_MM_WEEK_BEAVER_RACE_ALT              MM_EV(24,   2)
#define EV_MM_WEEK_PAMELA_FATHER                MM_EV(75,   5)

#define EV_MM_WEEK_WOODFALL_TEMPLE_RISE         MM_EV(20,   0)
#define EV_MM_WEEK_DUNGEON_WF                   MM_EV(20,   1)
#define EV_MM_WEEK_DUNGEON_SH                   MM_EV(33,   7)
#define EV_MM_WEEK_DUNGEON_GB                   MM_EV(55,   7)
#define EV_MM_WEEK_DUNGEON_ST                   MM_EV(52,   5)

#if !defined(__ASSEMBLER__)
# define MM_SET_EVENT_WEEK(x)       (gMmSave.weekEventReg[(x) / 8] |= (1 << ((x) % 8)))
# define MM_CLEAR_EVENT_WEEK(x)     (gMmSave.weekEventReg[(x) / 8] &= ~(1 << ((x) % 8)))
# define MM_GET_EVENT_WEEK(x)       (gMmSave.weekEventReg[(x) / 8] & (1 << ((x) % 8)))
int  GetEventChk(int index);
void SetEventChk(int index);
#endif

#endif
