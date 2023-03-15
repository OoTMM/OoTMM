#include <combo.h>

extern void* gMmMag;
GameState_Play* gPlay;

static void debugCheat(GameState_Play* play)
{
#if defined(DEBUG)
    if (play->gs.input[0].current.buttons & L_TRIG)
    {
        gSave.inventory.dungeonKeys[SCE_OOT_TEMPLE_FIRE] = 8;
        gSave.inventory.items[ITS_OOT_STICKS] = ITEM_OOT_STICK;
        gSave.inventory.items[ITS_OOT_NUTS] = ITEM_OOT_NUT;
        gSave.inventory.items[ITS_OOT_BOMBS] = ITEM_OOT_BOMB;
        gSave.inventory.items[ITS_OOT_ARROW_FIRE] = ITEM_OOT_ARROW_FIRE;
        gSave.inventory.items[ITS_OOT_ARROW_LIGHT] = ITEM_OOT_ARROW_LIGHT;
        gSave.inventory.items[ITS_OOT_SLINGSHOT] = ITEM_OOT_SLINGSHOT;
        gSave.inventory.items[ITS_OOT_OCARINA] = ITEM_OOT_OCARINA_TIME;
        gSave.inventory.items[ITS_OOT_BOOMERANG] = ITEM_OOT_BOOMERANG;
        gSave.inventory.items[ITS_OOT_BOTTLE] = ITEM_OOT_RUTO_LETTER;
        gSave.inventory.items[ITS_OOT_BOTTLE2] = ITEM_OOT_FISH;
        gSave.inventory.items[ITS_OOT_BOTTLE3] = ITEM_OOT_BIG_POE;
        gSave.inventory.items[ITS_OOT_BOTTLE4] = ITEM_OOT_BIG_POE;
        gSave.inventory.items[ITS_OOT_BOMBCHU] = ITEM_OOT_BOMBCHU_10;
        gSave.inventory.items[ITS_OOT_TRADE_CHILD] = ITEM_OOT_ZELDA_LETTER;
        gSave.inventory.items[ITS_OOT_HAMMER] = ITEM_OOT_HAMMER;
        gSave.inventory.items[ITS_OOT_HOOKSHOT] = ITEM_OOT_LONGSHOT;
        gSave.inventory.items[ITS_OOT_LENS] = ITEM_OOT_LENS;
        gSave.inventory.equipment.swords = 0x7;
        gSave.inventory.equipment.shields = 0x7;
        gSave.inventory.equipment.tunics = 0x7;
        gSave.inventory.equipment.boots = 0x7;
        gSave.inventory.upgrades.dekuStick = 3;
        gSave.inventory.upgrades.dekuNut = 3;
        gSave.inventory.upgrades.bulletBag = 3;
        gSave.inventory.upgrades.bombBag = 3;
        gSave.inventory.upgrades.quiver = 3;
        gSave.inventory.upgrades.dive = 2;
        gSave.inventory.upgrades.wallet = 2;
        gSave.inventory.upgrades.strength = 3;
        gSave.inventory.ammo[ITS_OOT_STICKS] = 10;
        gSave.inventory.ammo[ITS_OOT_SLINGSHOT] = 50;
        gSave.inventory.ammo[ITS_OOT_BOMBS] = 40;
        gSave.inventory.quest.songZelda = 1;
        gSave.inventory.quest.songSaria = 1;
        gSave.inventory.quest.songTime = 1;
        gSave.inventory.quest.songSun = 1;
        gSave.inventory.quest.songEpona = 1;
        gSave.inventory.quest.songStorms = 1;
        gSave.inventory.quest.songTpLight = 1;
        gSave.inventory.quest.songTpShadow = 1;
        gSave.inventory.quest.songTpWater = 1;
        gSave.inventory.quest.songTpFire = 1;
        gSave.inventory.quest.songTpForest = 1;
        gSave.inventory.quest.songTpSpirit = 1;

        gSave.inventory.quest.stoneEmerald = 0;
        gSave.inventory.quest.stoneRuby = 0;
        gSave.inventory.quest.stoneSapphire = 0;

        gMmExtraFlags2.majora = 1;

        gSave.playerData.magicUpgrade = 1;
        gSave.playerData.magicUpgrade2 = 1;
        gSave.playerData.magicAmount = 0x60;

        gSave.inventory.dungeonKeys[SCE_OOT_TEMPLE_FOREST] = 9;
        gSave.inventory.dungeonKeys[SCE_OOT_INSIDE_GANON_CASTLE] = 9;

        //gSave.inventory.quest.medallionShadow = 1;
        //gSave.inventory.quest.medallionSpirit = 1;
        //gSave.inventory.quest.medallionForest = 1;
        //gSave.inventory.quest.medallionFire = 1;
        //gSave.inventory.quest.medallionWater = 1;
        //gSave.inventory.quest.medallionLight = 1;

        gSave.inventory.quest.stoneRuby = 1;

        gSave.playerData.health = gSave.playerData.healthMax = 20 * 0x10;

        gSave.playerData.rupees = 500;

        gOotExtraTrade.child = 0xffff;
        gOotExtraTrade.adult = 0xffff;
        gOotExtraTrade.adult |= (1 << XITEM_OOT_ADULT_EYEBALL_FROG);
        gOotExtraTrade.adult |= (1 << XITEM_OOT_ADULT_EYE_DROPS);
        gOotExtraTrade.adult |= (1 << XITEM_OOT_ADULT_CLAIM_CHECK);
        gSave.inventory.items[ITS_OOT_TRADE_ADULT] = ITEM_OOT_EYEBALL_FROG;

        SetEventChk(EV_OOT_CHK_EPONA);

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
    if (gSave.inventory.quest.stoneEmerald && gSave.inventory.quest.stoneRuby && gSave.inventory.quest.stoneSapphire)
    {
        SetEventChk(EV_OOT_CHK_ZELDA_FLED);
        SetEventChk(EV_OOT_CHK_ZELDA_FLED_BRIDGE);
    }

    /* Set the rainbow bridge flag */
    if (gSave.inventory.quest.medallionLight && gSave.inventory.quest.medallionForest && gSave.inventory.quest.medallionFire && gSave.inventory.quest.medallionWater && gSave.inventory.quest.medallionShadow && gSave.inventory.quest.medallionSpirit)
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

static u8 sInGrotto;

void hookPlay_Init(GameState_Play* play)
{
    s32 override;

    /* Register play */
    gPlay = play;

    /* Handle transition override */
    if (sInGrotto)
        gIsEntranceOverride = 0;
    if (gIsEntranceOverride)
    {
        gIsEntranceOverride = 0;
        override = comboEntranceOverride(gSave.entrance);
        if (override != -1)
        {
            if (override >= 0)
                gSave.entrance = override;
            else
            {
                gSave.entrance = gLastEntrance;
                Play_Init(play);
                gComboCtx.shuffledEntrance = 1;
                comboGameSwitch(play, override);
                return;
            }
        }
    }

    /* Handle swordless */
    if (gSave.equips.equipment.swords == 0 || (gSave.inventory.equipment.swords & (1 << (gSave.equips.equipment.swords - 1))) == 0)
    {
        /* Set swordless */
        gSave.equips.equipment.swords = 0;
        switch (gSave.equips.buttonItems[0])
        {
        case ITEM_OOT_KOKIRI_SWORD:
        case ITEM_OOT_MASTER_SWORD:
        case ITEM_OOT_GIANT_KNIFE:
        case ITEM_OOT_GIANT_KNIFE_BROKEN:
            gSave.equips.buttonItems[0] = ITEM_NONE;
            break;
        }
        gSave.eventsMisc[29] = 1;
    }
    else
    {
        /* Set sworded */
        gSave.eventsMisc[29] = 0;
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

    comboObjectsReset();
    debugCheat(play);
    eventFixes(play);

    Play_Init(play);
    gLastEntrance = gSave.entrance;
    sInGrotto = (play->sceneId == SCE_OOT_GROTTOS || play->sceneId == SCE_OOT_FAIRY_FOUNTAIN);

    if (gSave.entrance == 0x0530)
    {
        gComboCtx.shuffledEntrance = 0;
        comboGameSwitch(play, -1);
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
        gComboCtx.shuffledEntrance = 0;
        comboGameSwitch(play, -1);
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

static void Play_LoadKaleidoScopeHook(void* unk)
{
    Play_LoadKaleidoOverlay(unk);
    comboMqKaleidoHook(gPlay);
}

PATCH_CALL(0x8009a06c, Play_LoadKaleidoScopeHook);
