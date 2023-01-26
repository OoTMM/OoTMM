#include <combo.h>

extern void* gMmMag;

static void debugCheat(GameState_Play* play)
{
#if defined(DEBUG)
    if (play->gs.input[0].current.buttons & L_TRIG)
    {
        gSave.inventory[ITS_OOT_STICKS] = ITEM_OOT_STICK;
        gSave.inventory[ITS_OOT_NUTS] = ITEM_OOT_NUT;
        gSave.inventory[ITS_OOT_BOMBS] = ITEM_OOT_BOMB;
        gSave.inventory[ITS_OOT_BOW] = ITEM_OOT_BOW;
        gSave.inventory[ITS_OOT_ARROW_FIRE] = ITEM_OOT_ARROW_FIRE;
        gSave.inventory[ITS_OOT_ARROW_LIGHT] = ITEM_OOT_ARROW_LIGHT;
        gSave.inventory[ITS_OOT_SLINGSHOT] = ITEM_OOT_SLINGSHOT;
        gSave.inventory[ITS_OOT_OCARINA] = ITEM_OOT_OCARINA_TIME;
        gSave.inventory[ITS_OOT_BOOMERANG] = ITEM_OOT_BOOMERANG;
        gSave.inventory[ITS_OOT_BOTTLE] = ITEM_OOT_RUTO_LETTER;
        gSave.inventory[ITS_OOT_BOTTLE2] = ITEM_OOT_FISH;
        gSave.inventory[ITS_OOT_BOTTLE3] = ITEM_OOT_BIG_POE;
        gSave.inventory[ITS_OOT_BOTTLE4] = ITEM_OOT_BIG_POE;
        gSave.inventory[ITS_OOT_BOMBCHU] = ITEM_OOT_BOMBCHU_10;
        gSave.inventory[ITS_OOT_TRADE_CHILD] = ITEM_OOT_ZELDA_LETTER;
        gSave.inventory[ITS_OOT_HAMMER] = ITEM_OOT_HAMMER;
        gSave.inventory[ITS_OOT_HOOKSHOT] = ITEM_OOT_LONGSHOT;
        gSave.inventory[ITS_OOT_LENS] = ITEM_OOT_LENS;
        gSave.equipment.swords = 0x3;
        gSave.equipment.shields = 0x7;
        gSave.equipment.tunics = 0x7;
        gSave.equipment.boots = 0x7;
        gSave.upgrades.dekuStick = 3;
        gSave.upgrades.dekuNut = 3;
        gSave.upgrades.bulletBag = 3;
        gSave.upgrades.bombBag = 3;
        gSave.upgrades.quiver = 3;
        gSave.upgrades.dive = 2;
        gSave.upgrades.wallet = 2;
        gSave.upgrades.strength = 3;
        gSave.ammo[ITS_OOT_STICKS] = 10;
        gSave.ammo[ITS_OOT_SLINGSHOT] = 50;
        gSave.ammo[ITS_OOT_BOMBS] = 40;
        gSave.ammo[ITS_OOT_BOW] = 50;
        gSave.quest.songZelda = 1;
        gSave.quest.songSaria = 1;
        gSave.quest.songTime = 1;
        gSave.quest.songSun = 1;
        gSave.quest.songEpona = 1;
        gSave.quest.songStorms = 1;
        gSave.quest.songTpLight = 1;
        gSave.quest.songTpShadow = 1;
        gSave.quest.songTpWater = 1;
        gSave.quest.songTpFire = 1;
        gSave.quest.songTpForest = 1;
        gSave.quest.songTpSpirit = 1;

        gSave.quest.stoneEmerald = 0;
        gSave.quest.stoneRuby = 0;
        gSave.quest.stoneSapphire = 0;

        gMmExtraFlags2.majora = 1;

        gSave.magicUpgrade = 1;
        gSave.magicUpgrade2 = 1;
        gSave.magicAmount = 0x60;

        gSave.dungeonKeys[SCE_OOT_TEMPLE_FOREST] = 9;
        gSave.dungeonKeys[SCE_OOT_INSIDE_GANON_CASTLE] = 9;

        //gSave.quest.medallionShadow = 1;
        //gSave.quest.medallionSpirit = 1;
        //gSave.quest.medallionForest = 1;
        //gSave.quest.medallionFire = 1;
        //gSave.quest.medallionWater = 1;
        //gSave.quest.medallionLight = 1;

        gSave.health = gSave.healthMax = 20 * 0x10;

        gSave.rupees = 500;

        gOotExtraTrade.child = 0xffff;
        gOotExtraTrade.adult = 0;
        gOotExtraTrade.adult |= (1 << XITEM_OOT_ADULT_EYEBALL_FROG);
        gOotExtraTrade.adult |= (1 << XITEM_OOT_ADULT_EYE_DROPS);
        gOotExtraTrade.adult |= (1 << XITEM_OOT_ADULT_CLAIM_CHECK);
        gSave.inventory[ITS_OOT_TRADE_ADULT] = ITEM_OOT_EYEBALL_FROG;

#if defined(DEBUG_AGE)
        gSave.age = DEBUG_AGE;
#endif

        BITMAP16_SET(gSave.eventsMisc, EV_OOT_INF_KING_ZORA_THAWED);
    }
#endif
}

static void eventFixes(GameState_Play* play)
{

    /* Skip forest temple cutscene */
    if (gSave.entrance == 0x169)
    {
        u32 tmp;
        tmp = gSave.perm[SCE_OOT_TEMPLE_FOREST].switches;
        if ((tmp & 0xf0000000) != 0xf0000000)
            tmp |= 0x08000000;
        gSave.perm[SCE_OOT_TEMPLE_FOREST].switches = tmp;
    }

    /* Ruto fixes */
    if (gSave.entrance == 0x028)
    {
        /* Skip the intro cutscene */
        u16 tmp;
        tmp = gSave.eventsMisc[20];
        if (!(tmp & 0xff))
            tmp |= 0x1e;
        gSave.eventsMisc[20] = tmp;
    }

    /* Skip Zelda's cutscene when having all the spiritual stones */
    if (gSave.quest.stoneEmerald && gSave.quest.stoneRuby && gSave.quest.stoneSapphire)
    {
        SetEventChk(EV_OOT_CHK_ZELDA_FLED);
        SetEventChk(EV_OOT_CHK_ZELDA_FLED_BRIDGE);
    }

    /* Set the rainbow bridge flag */
    if (gSave.quest.medallionLight && gSave.quest.medallionForest && gSave.quest.medallionFire && gSave.quest.medallionWater && gSave.quest.medallionShadow && gSave.quest.medallionSpirit)
    {
        SetEventChk(EV_OOT_CHK_RAINBOW_BRIDGE);
    }

    /* Open Trials */
    if (gSave.entrance == 0x0467)
    {
        SetEventChk(EV_OOT_CHK_TRIAL_LIGHT);
        SetEventChk(EV_OOT_CHK_TRIAL_FIRE);
        SetEventChk(EV_OOT_CHK_TRIAL_SHADOW);
        SetEventChk(EV_OOT_CHK_TRIAL_WATER);
        SetEventChk(EV_OOT_CHK_TRIAL_FOREST);
        SetEventChk(EV_OOT_CHK_TRIAL_SPIRIT);
        SetEventChk(EN_OOT_CHK_GANON_BARRIER);
    }
}

static void endGame(void)
{
    u8  tmpAge;
    u16 tmpNextCutscene;
    s32 tmpCutscene;
    u16 tmpEntrance;
    u16 tmpSceneId;

    /* Flag ganon as beaten */
    gOotExtraFlags.ganon = 1;

    /* Save tmp gameplay values (in case majora was beaten too) */
    tmpAge = gSave.age;
    tmpNextCutscene = *(u16*)((char*)&gSaveContext + 0x1412);
    tmpCutscene = gSave.cutscene;
    tmpEntrance = gSave.entrance;
    tmpSceneId = gSave.sceneId;

    /* Edit gameplay values for end of game save */
    gSave.age = AGE_ADULT;
    *(u16*)((char*)&gSaveContext + 0x1412) = 0;
    gSave.cutscene = 0;
    gSave.entrance = 0x023d;
    gSave.sceneId = SCE_OOT_GANON_CASTLE_EXTERIOR;

    /* Save */
    comboWriteSave();

    /* Restore gameplay values to play the cutscene if majora was beaten too */
    if (gMmExtraFlags2.majora)
    {
        gSave.age = tmpAge;
        *(u16*)((char*)&gSaveContext + 0x1412) = tmpNextCutscene;
        gSave.cutscene = tmpCutscene;
        gSave.entrance = tmpEntrance;
        gSave.sceneId = tmpSceneId;
    }
}

void hookPlay_Init(GameState_Play* play)
{
    /* Handle swordless */
    if (gSave.currentEquipment.swords == 0 || (gSave.equipment.swords & (1 << (gSave.currentEquipment.swords - 1))) == 0)
    {
        /* Set swordless */
        gSave.currentEquipment.swords = 0;
        switch (gSave.buttons[0])
        {
        case ITEM_OOT_KOKIRI_SWORD:
        case ITEM_OOT_MASTER_SWORD:
        case ITEM_OOT_GIANT_KNIFE:
        case ITEM_OOT_GIANT_KNIFE_BROKEN:
            gSave.buttons[0] = 0xff;
            break;
        }
        gSave.eventsMisc[29] = 1;
    }

    if (gSave.entrance == 0x007a)
    {
        /* Entering courtyard */
        if (GetEventChk(EV_OOT_CHK_ZELDA_LETTER))
            gSave.entrance = 0x0594;
        else
            gSave.entrance = 0x0400;
    }
    else if (gSave.entrance == 0x006b)
    {
        endGame();
    }

    if (gMmMag)
    {
        free(gMmMag);
    }
    comboObjectsReset();
    debugCheat(play);
    eventFixes(play);

    Play_Init(play);
    comboSpawnItemGivers(play);

    if (gSave.entrance == 0x0530)
    {
        PlayStoreFlags(play);
        gSave.sceneId = play->sceneId;
        comboGameSwitch();
        return;
    }

    /* Title screen transition skip */
    if (gComboCtx.valid)
    {
        play->transition.gfx = 11;
    }

    if (!gCustomKeep)
    {
        comboLoadCustomKeep();
    }

#if defined(DEBUG)
    if (play->gs.input[0].current.buttons & R_TRIG)
    {
        comboGameSwitch();
        return;
    }
#endif
}

void Play_DrawWrapper(GameState_Play* play)
{
    comboObjectsGC();
    Play_Draw(play);

    if (gComboCtx.valid)
    {
        OPEN_DISPS(play->gs.gfx);
        gDPSetCycleType(OVERLAY_DISP++, G_CYC_FILL);
        gDPSetRenderMode(OVERLAY_DISP++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetFillColor(OVERLAY_DISP++, 0);
        gDPFillRectangle(OVERLAY_DISP++, 0, 0, 0xfff, 0xfff);
        CLOSE_DISPS();
    }
    else
    {
        /* Need to draw dpad */
        comboDpadDraw(play);
    }
}
