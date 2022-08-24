#include <combo.h>

extern void Play_Init(void*);
extern void comboSwitchToMM(void);

void hookPlay_Init(GameState_Play* play)
{
    if ((gSave.entrance & 0xfffc) == 0x0530)
    {
        comboGameSwitch();
        return;
    }

    Play_Init(play);

    /* Saria's Ocarina Check */
    if ((gSave.entrance == 0x05e0 || gSave.entrance == 0x05e1) && (GetEventChk(EV_CHK_SARIA_OCARINA) == 0))
    {
        SpawnActor((char*)play + 0x1c24, play, AC_ITEM_ETCETERA, -1191.f, -220.f, 1626.f, 0, 0, 0, (GI_OOT_FAIRY_OCARINA << 8) | 0x40);
    }
}
