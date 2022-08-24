#ifndef COMBO_OOT_EVENTS_H
#define COMBO_OOT_EVENTS_H

#define EV_CHK_DEKU_MIDO_MEET           0x02
#define EV_CHK_DEKU_MIDO_SWORD_SHIELD   0x04
#define EV_CHK_DEKU_TREE_MEET           0x0c
#define EV_CHK_SARIA_OCARINA            0xc1

int  GetEventChk(int index);
void SetEventChk(int index);

#endif
