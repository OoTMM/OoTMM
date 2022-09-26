#include <combo.h>

extern void Play_Init(void*);

static void debugCheat(GameState_Play* play)
{
    if (play->gs.input[0].current.buttons & 0x20)
    {
        gSave.inventory[ITS_OOT_STICKS] = ITEM_OOT_STICK;
        gSave.inventory[ITS_OOT_NUTS] = ITEM_OOT_NUT;
        gSave.inventory[ITS_OOT_BOMBS] = ITEM_OOT_BOMB;
        gSave.inventory[ITS_OOT_BOW] = ITEM_OOT_BOW;
        gSave.inventory[ITS_OOT_ARROW_FIRE] = ITEM_OOT_ARROW_FIRE;
        gSave.inventory[ITS_OOT_SLINGSHOT] = ITEM_OOT_SLINGSHOT;
        gSave.inventory[ITS_OOT_OCARINA] = ITEM_OOT_OCARINA_TIME;
        gSave.inventory[ITS_OOT_BOOMERANG] = ITEM_OOT_BOOMERANG;
        gSave.inventory[ITS_OOT_BOTTLE] = ITEM_OOT_RUTO_LETTER;
        gSave.inventory[ITS_OOT_BOTTLE2] = ITEM_OOT_EMPTY_BOTTLE;
        gSave.equipment.swords = 0x7;
        gSave.equipment.shields = 0x7;
        gSave.equipment.tunics = 0x7;
        gSave.equipment.boots = 0x7;
        gSave.upgrades.bulletBag = 3;
        gSave.upgrades.bombBag = 3;
        gSave.upgrades.quiver = 3;
        gSave.upgrades.dive = 2;
        gSave.ammo[ITS_OOT_SLINGSHOT] = 50;
        gSave.ammo[ITS_OOT_BOMBS] = 40;
        gSave.ammo[ITS_OOT_BOW] = 50;
        gSave.quest.songZelda = 1;
        gSave.quest.songSaria = 1;
        gSave.quest.songTime = 1;

        gSave.quest.stoneEmerald = 1;
        gSave.quest.stoneRuby = 1;
        gSave.quest.stoneSapphire = 1;
    }
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
    comboObjectsReset();
    debugCheat(play);
    skipEntranceCutscene(play);

    Play_Init(play);

    /* Saria's Ocarina Check */
    if (gSave.entrance == 0x05e0 || gSave.entrance == 0x04de)
    {
        comboSpawnSpecial(play, -1191.f, -220.f, 1650.f, EV_OOT_CHK_SARIA_OCARINA, GI_OOT_OCARINA_FAIRY);
    }

    /* Child Zelda checks */
    if (play->sceneId == SCE_OOT_CASTLE_COURTYARD)
    {
        comboSpawnSpecial(play, -460.f, 84.f,  40.f, EV_OOT_CHK_ZELDA_LETTER, GI_OOT_ZELDA_LETTER);
        comboSpawnSpecial(play, -460.f, 84.f, -40.f, EV_OOT_CHK_SONG_ZELDA, GI_OOT_SONG_ZELDA);
    }

    /* Sun Song */
    if (play->sceneId == SCE_OOT_TOMB_ROYAL)
    {
        comboSpawnSpecial(play, 0.f, 70.f, -1160.f, EV_OOT_CHK_SONG_SUN, GI_OOT_SONG_SUN);
    }

    /* Saria's Song */
    if (play->sceneId == SCE_OOT_SACRED_FOREST_MEADOW && gSave.age == AGE_CHILD && GetEventChk(EV_OOT_CHK_ZELDA_LETTER))
    {
        comboSpawnSpecial(play, 125.f, 500.f, -2970.f, EV_OOT_CHK_SONG_SARIA, GI_OOT_SONG_SARIA);
    }

    /* Skip Zelda's cutscene when having all the spiritual stones */
    if (gSave.quest.stoneEmerald && gSave.quest.stoneRuby && gSave.quest.stoneSapphire)
    {
        SetEventChk(EV_OOT_CHK_ZELDA_FLED);
        SetEventChk(EV_OOT_CHK_ZELDA_FLED_BRIDGE);

        if (play->sceneId == SCE_OOT_HYRULE_FIELD && gSave.age == AGE_CHILD)
        {
            comboSpawnSpecial(play, 299.f, -136.f, 884.f, EV_OOT_CHK_OCARINA_OF_TIME, GI_OOT_OCARINA_TIME);
            comboSpawnSpecial(play, 499.f, -136.f, 884.f, EV_OOT_CHK_SONG_TIME, GI_OOT_SONG_TIME);
        }
    }

    if ((gSave.entrance & 0xfffc) == 0x0530)
    {
        comboGameSwitch();
        return;
    }
}
