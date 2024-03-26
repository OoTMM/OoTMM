#include <combo.h>
#include <combo/souls.h>
#include <combo/net.h>
#include <combo/menu.h>
#include <combo/entrance.h>

extern void* gMmMag;
GameState_Play* gPlay;

static void debugCheat(GameState_Play* play)
{
#if defined(DEBUG)
    if (!gSaveContext.gameMode && play->gs.input[0].current.buttons & L_TRIG)
    {
        /*MM_SET_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI); */
        SetEventChk(EV_OOT_CHK_MASTER_SWORD_PULLED);
        SetEventChk(EV_OOT_CHK_MASTER_SWORD_CHAMBER);
        gSave.playerData.swordHealth = 8;
        gSave.isBiggoronSword = 1;

        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_ST);

        /*gSave.inventory.quest.gerudoCard = 1; */
        gSave.inventory.dungeonKeys[SCE_OOT_TEMPLE_FIRE] = 8;
        gSave.inventory.dungeonKeys[SCE_OOT_TREASURE_SHOP] = 6;
        gSave.inventory.items[ITS_OOT_STICKS] = ITEM_OOT_STICK;
        gSave.inventory.items[ITS_OOT_NUTS] = ITEM_OOT_NUT;
        gSave.inventory.items[ITS_OOT_BOMBS] = ITEM_OOT_BOMB;
        //gSave.inventory.items[ITS_OOT_BOW] = ITEM_OOT_BOW;
        gSave.inventory.items[ITS_OOT_ARROW_FIRE] = ITEM_OOT_ARROW_FIRE;
        gSave.inventory.items[ITS_OOT_ARROW_ICE] = ITEM_OOT_ARROW_ICE;
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
        gSave.inventory.items[ITS_OOT_SPELL_WIND] = ITEM_OOT_SPELL_WIND;
        gSave.inventory.items[ITS_OOT_SPELL_FIRE] = ITEM_OOT_SPELL_FIRE;
        gSave.inventory.items[ITS_OOT_SPELL_LOVE] = ITEM_OOT_SPELL_LOVE;

        gSave.inventory.equipment.swords = 0x7;
        gSave.inventory.equipment.shields = 0x7;
        gSave.inventory.equipment.tunics = 0x7;
        gSave.inventory.equipment.boots = 0x7;

        gSave.inventory.upgrades.dekuStick = 3;
        gSave.inventory.upgrades.dekuNut = 3;
        /*gSave.inventory.upgrades.bulletBag = 3; */
        gSave.inventory.upgrades.bombBag = 3;
        //gSave.inventory.upgrades.quiver = 3;
        gSave.inventory.upgrades.dive = 2;
        /*gSave.inventory.upgrades.wallet = 3; */
        gSave.inventory.upgrades.strength = 3;
        /*gOotExtraFlags.bottomlessWallet = 1; */
        /*gOotMaxRupees[3] = 9999; */
        /*gWalletDigits[3] = 4; */

        gSave.inventory.ammo[ITS_OOT_STICKS] = 10;
        gSave.inventory.ammo[ITS_OOT_SLINGSHOT] = 50;
        gSave.inventory.ammo[ITS_OOT_NUTS] = 40;
        gSave.inventory.ammo[ITS_OOT_BOMBS] = 40;
        //gSave.inventory.ammo[ITS_OOT_BOW] = 50;
        gSave.inventory.ammo[ITS_OOT_BOMBCHU] = 50;
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

        gSave.inventory.quest.stoneEmerald = 1;
        gSave.inventory.quest.stoneRuby = 0;
        gSave.inventory.quest.stoneSapphire = 0;

        gSave.playerData.magicUpgrade = 1;
        gSave.playerData.magicUpgrade2 = 1;
        gOotSave.playerData.magicSize = 0;
        gSaveContext.magicFillTarget = 0x60;

        gSave.inventory.dungeonKeys[SCE_OOT_TEMPLE_FOREST] = 9;
        gSave.inventory.dungeonKeys[SCE_OOT_INSIDE_GANON_CASTLE] = 9;

        gSave.inventory.quest.medallionShadow = 1;
        gSave.inventory.quest.medallionSpirit = 1;
        gSave.inventory.quest.medallionForest = 1;
        gSave.inventory.quest.medallionFire = 1;
        gSave.inventory.quest.medallionWater = 1;
        gSave.inventory.quest.medallionLight = 1;

        gSave.inventory.quest.stoneRuby = 1;

        gSave.playerData.health = gSave.playerData.healthMax = 20 * 0x10;

        gSave.playerData.rupees = 500;

        gOotExtraTrade.child = 0xffff;
        gOotExtraTrade.adult = 0xffff;
        gOotExtraTrade.adult |= (1 << XITEM_OOT_ADULT_EYEBALL_FROG);
        gOotExtraTrade.adult |= (1 << XITEM_OOT_ADULT_EYE_DROPS);
        gOotExtraTrade.adult |= (1 << XITEM_OOT_ADULT_CLAIM_CHECK);
        gSave.inventory.items[ITS_OOT_TRADE_ADULT] = ITEM_OOT_POCKET_EGG;
        gSave.inventory.items[ITS_OOT_TRADE_CHILD] = ITEM_OOT_MASK_STONE;

        SetEventChk(EV_OOT_CHK_EPONA);

        gSave.equips.buttonItems[1] = ITEM_OOT_MASK_STONE;
        gSave.equips.cButtonSlots[0] = ITS_OOT_TRADE_CHILD;

        gCustomSave.hasElegy = 1;


#if defined(DEBUG_AGE)
        gSave.age = DEBUG_AGE;
#endif

        /*BITMAP16_SET(gSave.eventsMisc, EV_OOT_INF_KING_ZORA_THAWED); */
    }
#endif
}

static int isRainbowBridgeOpen(void)
{
    if (comboConfig(CFG_OOT_BRIDGE_CUSTOM) && !comboSpecialCond(SPECIAL_BRIDGE))
        return 0;

    if (comboConfig(CFG_OOT_BRIDGE_VANILLA) && !(
        gOotSave.inventory.quest.medallionShadow
        && gOotSave.inventory.quest.medallionSpirit
        && gOotSave.inventory.items[ITS_OOT_ARROW_LIGHT] == ITEM_OOT_ARROW_LIGHT
    ))
        return 0;

    if (comboConfig(CFG_OOT_BRIDGE_MEDALLIONS) && !(
        gOotSave.inventory.quest.medallionLight
        && gOotSave.inventory.quest.medallionForest
        && gOotSave.inventory.quest.medallionFire
        && gOotSave.inventory.quest.medallionWater
        && gOotSave.inventory.quest.medallionShadow
        && gOotSave.inventory.quest.medallionSpirit
    ))
        return 0;

    return 1;
}

static void eventFixes(GameState_Play* play)
{
    /* Skip forest temple cutscene */
    if (gSave.entrance == ENTR_OOT_TEMPLE_FOREST)
    {
        u32 tmp;
        tmp = gSave.perm[SCE_OOT_TEMPLE_FOREST].switches;
        if ((tmp & 0xf0000000) != 0xf0000000)
            tmp |= 0x08000000;
        gSave.perm[SCE_OOT_TEMPLE_FOREST].switches = tmp;
    }

    /* Ruto fixes */
    if (gSave.entrance == ENTR_OOT_JABU_JABU)
    {
        /* Skip the intro cutscene */
        u16 tmp;
        tmp = gSave.eventsMisc[20];
        if (!(tmp & 0xff))
            tmp |= 0x1e;
        gSave.eventsMisc[20] = tmp;

        /* Ruto kidnap fixes */
        if (BITMAP16_GET(gSave.eventsMisc, EV_OOT_INF_RUTO_KIDNAPPED) || BITMAP16_GET(gSave.eventsMisc, EV_OOT_INF_RUTO_GOT_SAPPHIRE))
        {
            /* Un-kidnap ruto */
            BITMAP16_CLEAR(gSave.eventsMisc, EV_OOT_INF_RUTO_KIDNAPPED);
            BITMAP16_CLEAR(gSave.eventsMisc, EV_OOT_INF_RUTO_GOT_SAPPHIRE);
        }
    }

    /* Skip Zelda's cutscene when having all the spiritual stones */
    if (gSave.inventory.quest.stoneEmerald && gSave.inventory.quest.stoneRuby && gSave.inventory.quest.stoneSapphire)
    {
        SetEventChk(EV_OOT_CHK_ZELDA_FLED);
        SetEventChk(EV_OOT_CHK_ZELDA_FLED_BRIDGE);
    }

    /* Set the rainbow bridge flag */
    if (isRainbowBridgeOpen())
    {
        SetEventChk(EV_OOT_CHK_RAINBOW_BRIDGE);
    }
}

static void sendSelfTriforce(void)
{
    NetContext* net;
    int npc;
    s16 gi;

    if (!comboConfig(CFG_MULTIPLAYER))
        return;

    gi = GI_OOT_TRIFORCE_FULL;
    npc = NPC_OOT_GANON;

    net = netMutexLock();
    netWaitCmdClear();
    bzero(&net->cmdOut, sizeof(net->cmdOut));
    net->cmdOut.op = NET_OP_ITEM_SEND;
    net->cmdOut.itemSend.playerFrom = gComboData.playerId;
    net->cmdOut.itemSend.playerTo = gComboData.playerId;
    net->cmdOut.itemSend.game = 0;
    net->cmdOut.itemSend.gi = gi;
    net->cmdOut.itemSend.key = ((u32)OV_NPC << 24) | npc;
    net->cmdOut.itemSend.flags = 0;
    netMutexUnlock();

    /* Mark the NPC as obtained */
    BITMAP8_SET(gSharedCustomSave.oot.npc, npc);
}

static void endGame(void)
{
    u8  tmpAge;
    u16 tmpNextCutscene;
    s32 tmpCutscene;
    u16 tmpEntrance;
    u16 tmpSceneId;

    /* Ignore credit warp */
    if (g.isCreditWarp)
        return;

    /* Flag ganon as beaten */
    gOotExtraFlags.ganon = 1;

    /* Send self triforce */
    sendSelfTriforce();

    /* Save tmp gameplay values (in case majora was beaten too) */
    tmpAge = gSave.age;
    tmpNextCutscene = gSaveContext.nextCutscene;
    tmpCutscene = gSave.cutscene;
    tmpEntrance = gSave.entrance;
    tmpSceneId = gSave.sceneId;

    /* Edit gameplay values for end of game save */
    gSave.age = AGE_ADULT;
    gSaveContext.nextCutscene = 0;
    gSave.cutscene = 0;
    if (comboConfig(CFG_ER_ANY))
    {
        gSave.entrance = ENTR_OOT_GANON_TOWER;
        gSave.sceneId = SCE_OOT_GANON_TOWER;
    }
    else
    {
        gSave.entrance = ENTR_OOT_GANON_CASTLE_EXTERIOR_FROM_CASTLE;
        gSave.sceneId = SCE_OOT_GANON_CASTLE_EXTERIOR;
    }

    /* Save */
    comboSave(NULL, SF_PASSIVE);

    /* Restore gameplay values to play the cutscene if majora was beaten too */
    if (comboGoalCond())
    {
        gSave.age = tmpAge;
        gSaveContext.nextCutscene = tmpNextCutscene;
        gSave.cutscene = tmpCutscene;
        gSave.entrance = tmpEntrance;
        gSave.sceneId = tmpSceneId;
    }
}

static u32 entranceForOverride(u32 entrance)
{
    switch (entrance)
    {
    case ENTR_OOT_ZORA_RIVER_FROM_FIELD_WATER:
        /* Water entrance to zora river */
        return ENTR_OOT_ZORA_RIVER_FROM_FIELD;
    case ENTR_OOT_FIELD_FROM_ZORA_RIVER_WATER:
        /* Water entrance to hyrule */
        return ENTR_OOT_FIELD_FROM_ZORA_RIVER;
    case ENTR_OOT_HYRULE_CASTLE_FROM_FAIRY:
        return gLastScene == SCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES ? ENTR_OOT_OUTSIDE_GANON_FROM_FAIRY : ENTR_OOT_HYRULE_CASTLE_FROM_FAIRY;
    case ENTR_OOT_LOST_WOODS_FROM_KOKIRI_FOREST:
        return gLastScene == SCE_OOT_LOST_WOODS ? ENTR_OOT_LOST_WOODS_FROM_LOST_WOODS_NORTH : ENTR_OOT_LOST_WOODS_FROM_KOKIRI_FOREST;
    default:
        return entrance;
    }
}

void preInitTitleScreen(void)
{
    s16 magicCapacity;

    if (gComboCtx.valid)
    {
        /* Disable Title screen */
        gSaveContext.gameMode = 0;

        /* Set file and load */
        gSaveContext.fileIndex = gComboCtx.saveIndex;
        Sram_OpenSave(NULL);
        gSave.cutscene = 0;
        if (gComboCtx.entrance == -1)
            gSave.entrance = 0x1d1;
        else
            gSave.entrance = gComboCtx.entrance;
        gComboCtx.valid = 0;

        /* Set magic */
        magicCapacity = 0;
        if (gSave.playerData.magicUpgrade)
            magicCapacity = gSave.playerData.magicUpgrade2 ? 0x60 : 0x30;
        gSaveContext.magicState = MAGIC_STATE_IDLE;
        gSaveContext.magicCapacity = magicCapacity;
        gSaveContext.magicFillTarget = gSave.playerData.magicAmount;
        gSaveContext.magicTarget = gSave.playerData.magicAmount;
    }
}

void hookPlay_Init(GameState_Play* play)
{
    /* Pre-init */
    gIsEntranceOverride = 0;
    preInitTitleScreen();

    /* Init */
    gActorCustomTriggers = NULL;
    gMultiMarkChests = 0;
    gMultiMarkCollectibles = 0;
    gMultiMarkSwitch0 = 0;
    gMultiMarkSwitch1 = 0;
    comboMultiResetWisps();

    /* Register play */
    gPlay = play;

    /* Handle custom entrance IDs */
    switch (gSave.entrance)
    {
    case ENTR_OOT_OUTSIDE_GANON_FROM_FAIRY:
        gSave.entrance = ENTR_OOT_HYRULE_CASTLE_FROM_FAIRY;
        break;
    case ENTR_OOT_LOST_WOODS_FROM_LOST_WOODS_NORTH:
        gSave.entrance = ENTR_OOT_LOST_WOODS_FROM_KOKIRI_FOREST;
        break;
    }

    if (gSave.entrance == ENTR_OOT_CASTLE_STEALTH)
    {
        /* Entering courtyard */
        if (GetEventChk(EV_OOT_CHK_ZELDA_LETTER))
            gSave.entrance = ENTR_OOT_CASTLE_CAUGHT;
        else
            gSave.entrance = ENTR_OOT_CASTLE_COURTYARD;
    }
    else if (gSave.entrance == ENTR_OOT_CASTLE_STEALTH_FROM_COURTYARD)
    {
        gSave.entrance = ENTR_OOT_CASTLE_FROM_STEALTH;
    }
    else if (gSave.entrance == ENTR_OOT_SAGES_CHAMBER_END)
    {
        endGame();
    }
    else if (gSave.entrance == ENTR_OOT_BOSS_GANON2 && !comboHasSoulOot(GI_OOT_SOUL_NPC_ZELDA))
    {
        gSave.entrance = ENTR_OOT_GANON_TOWER;
    }

    comboCacheClear();
    comboObjectsReset();
    comboExObjectsReset();
    debugCheat(play);
    eventFixes(play);

    Play_Init(play);

    /* Epona fix */
    if (!IsSceneValidEpona(play->sceneId))
        comboClearEpona(play);

    gLastEntrance = gSave.entrance;
    g.inGrotto = (play->sceneId == SCE_OOT_GROTTOS || play->sceneId == SCE_OOT_FAIRY_FOUNTAIN);
    if (!g.inGrotto)
    {
        gLastScene = play->sceneId;
    }

    if (gSave.entrance == ENTR_OOT_SHOP_MASKS)
    {
        gComboCtx.shuffledEntrance = 0;
        comboGameSwitch(play, ENTR_MM_CLOCK_TOWN);
        return;
    }

    /* Spawn Custom Triggers */
    CustomTriggers_Spawn(play);
    comboSpawnCustomWarps(play);

    if (!gCustomKeep)
    {
        comboLoadCustomKeep();
    }

#if defined(DEBUG)
    if (!gSaveContext.gameMode && (play->gs.input[0].current.buttons & R_TRIG))
    {
        gComboCtx.shuffledEntrance = 0;
        comboGameSwitch(play, ENTR_MM_CLOCK_TOWN);
        return;
    }
#endif
}

void Play_UpdateWrapper(GameState_Play* play)
{
    comboMenuTick();
    Debug_Input();
    comboCacheGarbageCollect();
    comboObjectsGC();
    Play_Update(play);
    comboDpadDraw(play);
    Debug_Update();
}

static void Play_LoadKaleidoScopeHook(void* unk)
{
    Play_LoadKaleidoOverlay(unk);
    comboMqKaleidoHook(gPlay);
}

PATCH_CALL(0x8009a06c, Play_LoadKaleidoScopeHook);

void comboClearEpona(GameState_Play* play)
{
    if (AREG(6) != 0)
    {
        /* Link is on Epona, needs to dismount */
        AREG(6) = 0;

        /* Reset the TempB */
        gSave.equips.buttonItems[0] = gSaveContext.buttonStatus[0];
        gSaveContext.buttonStatus[0] = ITEM_NONE;

        /* Reload the B button icon */
        Interface_LoadItemIconImpl(play, 0);
    }
}

NORETURN static void Play_GameSwitch(GameState_Play* play, s32 entrance)
{
    gComboCtx.shuffledEntrance = 1;
    comboClearEpona(play);
    comboGameSwitch(play, entrance);
}

void Play_TransitionDone(GameState_Play* play)
{
    u32 entrance;
    s32 override;

    /* Resolve extended entrance */
    entrance = play->nextEntranceIndex;
    if (entrance == ENTR_EXTENDED)
        entrance = g.nextEntrance;

    /* Handle transition override */
    if (g.inGrotto)
        gIsEntranceOverride = 0;
    if (gIsEntranceOverride)
    {
        gIsEntranceOverride = 0;
        override = comboEntranceOverride(entranceForOverride(entrance));
        if (override != -1)
            entrance = (u32)override;
    }

    /* Check for foreign */
    if (entrance & MASK_FOREIGN_ENTRANCE)
    {
        Play_GameSwitch(play, entrance & ~MASK_FOREIGN_ENTRANCE);
    }
    else
    {
        play->nextEntranceIndex = entrance & 0xffff;
    }
}

static void TimeTravelUpdateEquipment(void)
{
    OotItemEquips* prevAge;
    OotItemEquips* nextAge;

    if (gSave.age == AGE_ADULT)
    {
        prevAge = &gSave.adultEquips;
        nextAge = &gSave.childEquips;
    }
    else
    {
        prevAge = &gSave.childEquips;
        nextAge = &gSave.adultEquips;
    }

    memcpy(prevAge, &gSave.equips, sizeof(*prevAge));
    if (EV_OOT_IS_SWORDLESS())
        prevAge->buttonItems[0] = ITEM_NONE;
    memcpy(&gSave.equips, nextAge, sizeof(*nextAge));

    if (gSave.equips.buttonItems[0] == ITEM_NONE)
        EV_OOT_SET_SWORDLESS();
    else
        EV_OOT_UNSET_SWORDLESS();
}

PATCH_FUNC(0x8006f804, TimeTravelUpdateEquipment);
