#ifndef COMBO_COMMON_EVENTS_H
#define COMBO_COMMON_EVENTS_H

#define EV_OOT_CHK_DEKU_MIDO_MEET           0x02
#define EV_OOT_CHK_DEKU_MIDO_SWORD_SHIELD   0x04
#define EV_OOT_CHK_EMERALD_TREE_DEAD        0x07
#define EV_OOT_CHK_GOHMA                    0x09
#define EV_OOT_CHK_DEKU_TREE_MEET           0x0c
#define EV_OOT_CHK_TREE_DEAD                0x17
#define EV_OOT_CHK_STONE_EMERALD            0x19
#define EV_OOT_CHK_MIDO_TREE_DEAD           0x1c
#define EV_OOT_CHK_STONE_RUBY               0x25
#define EV_OOT_CHK_RUTO_LETTER              0x32
#define EV_OOT_CHK_STONE_SAPPHIRE           0x37
#define EV_OOT_CHK_ZELDA_LETTER             0x40
#define EV_OOT_CHK_OCARINA_OF_TIME          0x43
#define EV_OOT_CHK_MEDALLION_FOREST         0x48
#define EV_OOT_CHK_MEDALLION_FIRE           0x49
#define EV_OOT_CHK_MEDALLION_WATER          0x4a
#define EV_OOT_CHK_SONG_FOREST              0x50
#define EV_OOT_CHK_SONG_FIRE                0x51
#define EV_OOT_CHK_SONG_WATER               0x52
#define EV_OOT_CHK_SONG_SHADOW              0x54
#define EV_OOT_CHK_SONG_ZELDA               0x59
#define EV_OOT_CHK_SONG_SUN                 0x5a
#define EV_OOT_CHK_SONG_STORMS              0x5b
#define EV_OOT_CHK_ZELDA_FLED               0x80
#define EV_OOT_CHK_ZELDA_FLED_BRIDGE        0x82
#define EV_OOT_CHK_SONG_TIME                0xa9
#define EV_OOT_CHK_SARIA_OCARINA            0xc1

/* Custom flags that seems unused */
#define EV_OOT_CHK_SONG_SARIA               0x58
#define EV_OOT_CHK_ARROW_FIRE               0x60
#define EV_OOT_ITEM_GORON_BRACELET          0x20

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

int  GetEventChk(int index);
void SetEventChk(int index);

#define MM_SET_EVENT_WEEK(x)    (gMmSave.weekEventReg[(x) / 8] |= (1 << ((x) % 8)))

#endif
