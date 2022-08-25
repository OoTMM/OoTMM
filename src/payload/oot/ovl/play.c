#include <combo.h>

extern void Play_Init(void*);
extern void comboSwitchToMM(void);

static void spawnSpecial(GameState_Play* play, float x, float y, float z, u8 specialId, u16 gi)
{
    SpawnActor(
        (char*)play + 0x1c24,
        play,
        AC_ITEM_ETCETERA,
        x, y, z,
        0, 0, 0,
        (gi << 8) | 0x40 | specialId
    );
}

static void skipEntranceCutscene(GameState_Play* play)
{
    switch (gSave.entrance)
    {
    case 0x0185:
        SetEventChk(0xa0);
        break;
    case 0x013d:
        SetEventChk(0xa1);
        break;
    case 0x00db:
        SetEventChk(0xa3);
        break;
    case 0x0108:
        SetEventChk(0xa4);
        break;
    case 0x0138:
        if (gSave.age == 1)
            SetEventChk(0xa5);
        else
            SetEventChk(0xba);
        break;
    case 0x014d:
        SetEventChk(0xa6);
        break;
    case 0x0053:
        SetEventChk(0xa7);
        break;
    case 0x0000:
        SetEventChk(0xa8);
        break;
    case 0x0102:
        SetEventChk(0xb1);
        break;
    case 0x0117:
        SetEventChk(0xb2);
        break;
    case 0x0129:
        SetEventChk(0xb3);
        break;
    case 0x0157:
        SetEventChk(0xb4);
        break;
    case 0x0028:
        SetEventChk(0xb5);
        break;
    case 0x00E4:
        SetEventChk(0xb6);
        break;
    case 0x0225:
        SetEventChk(0xb7);
        break;
    case 0x0123:
        SetEventChk(0xb8);
        break;
    case 0x0147:
        SetEventChk(0xb9);
        break;
    default:
        break;
    }
}

void hookPlay_Init(GameState_Play* play)
{
    skipEntranceCutscene(play);

    if ((gSave.entrance & 0xfffc) == 0x0530)
    {
        comboGameSwitch();
        return;
    }

    Play_Init(play);

    /* Saria's Ocarina Check */
    if ((gSave.entrance == 0x05e0 || gSave.entrance == 0x05e1) && (GetEventChk(EV_CHK_SARIA_OCARINA) == 0))
    {
        spawnSpecial(play, -1191.f, -220.f, 1650.f, 0x00, GI_OOT_OCARINA_FAIRY);
    }

    /* Child Zelda checks */
    if (play->sceneId == 0x4a)
    {
        if (!GetEventChk(EV_CHK_ZELDA_LETTER))
            spawnSpecial(play, -460.f, 84.f,  40.f, 0x01, GI_OOT_ZELDA_LETTER);
        if (!GetEventChk(EV_CHK_SONG_ZELDA))
            spawnSpecial(play, -460.f, 84.f, -40.f, 0x02, GI_OOT_SONG_ZELDA);
    }
}
