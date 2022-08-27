#ifndef COMBO_OOT_EVENTS_H
#define COMBO_OOT_EVENTS_H

#define EV_CHK_DEKU_MIDO_MEET           0x02
#define EV_CHK_DEKU_MIDO_SWORD_SHIELD   0x04
#define EV_CHK_EMERALD_TREE_DEAD        0x07
#define EV_CHK_GOHMA                    0x09
#define EV_CHK_DEKU_TREE_MEET           0x0c
#define EV_CHK_TREE_DEAD                0x17
#define EV_CHK_STONE_EMERALD            0x19
#define EV_CHK_MIDO_TREE_DEAD           0x1c
#define EV_CHK_STONE_RUBY               0x25
#define EV_CHK_STONE_SAPPHIRE           0x37
#define EV_CHK_ZELDA_LETTER             0x40
#define EV_CHK_SONG_ZELDA               0x59
#define EV_CHK_SONG_SUN                 0x5a
#define EV_CHK_SONG_STORMS              0x5b
#define EV_CHK_SARIA_OCARINA            0xc1

/* Custom flags that seems unused */
#define EV_CHK_SONG_SARIA               0x58
#define EV_ITEM_GORON_BRACELET          0x20

int  GetEventChk(int index);
void SetEventChk(int index);

#endif
