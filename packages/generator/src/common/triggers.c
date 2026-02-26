#include <combo.h>
#include <combo/souls.h>
#include <combo/inventory.h>

static Actor sFakeActor;
static s16 sTriggerSongGi;
static u8 sTrigger;
static u8 sTriggerAcc;

enum
{
    TRIGGER_NONE = 0,
    TRIGGER_SONG,
    TRIGGER_GANON_BK,
    TRIGGER_TRIFORCE,

#if defined(GAME_OOT)
    TRIGGER_OOT_SHEIK_COLOSSUS,
    TRIGGER_OOT_SHEIK_KAKARIKO,
    TRIGGER_OOT_SARIA_OCARINA,
    TRIGGER_OOT_ZELDA_LIGHT_ARROW,
    TRIGGER_OOT_WEIRD_EGG,
    TRIGGER_OOT_POCKET_EGG,
#endif

#if defined(GAME_MM)
    TRIGGER_MM_SONG_HEALING,
#endif
};

static int Triggers_GiveItem(PlayState* play, const ComboItemQuery* q)
{
    Player* link;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return 0;

    if (Actor_HasParentZ(&sFakeActor))
    {
        sFakeActor.parent = NULL;
        return 1;
    }

    comboGiveItem(&sFakeActor, play, q, 4096.f, 4096.f);
    return 0;
}

static int Triggers_GiveItemNpc(PlayState* play, s16 gi, int npc)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.ovType = OV_NPC;
    q.gi = gi;
    q.id = npc;

    return Triggers_GiveItem(play, &q);
}

static int Triggers_GiveItemDirect(PlayState* play, s16 gi)
{
    ComboItemQuery q = ITEM_QUERY_INIT;
    q.gi = gi;
    return Triggers_GiveItem(play, &q);
}


static int Triggers_ItemSafe(Player* player, PlayState* play)
{
    if (player->stateFlags1 & (PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN | PLAYER_ACTOR_STATE_GROTTO))
    {
        sTriggerAcc = 0;
        return 0;
    }

#if defined(GAME_OOT)
    if (play->sceneId == SCE_OOT_BOMBCHU_BOWLING_ALLEY)
        return 0;
#endif

    sTriggerAcc++;
    if (sTriggerAcc > 3)
    {
        sTriggerAcc = 3;
        return 1;
    }
    return 0;
}

static s16 Triggers_NextGivenSong(void)
{
    const u8* notes = gSharedCustomSave.notes;

    if (notes[NOTES_SONG_OOT_TP_FOREST] >= 6 && !gOotSave.info.inventory.quest.songTpForest)
        return GI_OOT_SONG_TP_FOREST;
    if (notes[NOTES_SONG_OOT_TP_FIRE] >= 8 && !gOotSave.info.inventory.quest.songTpFire)
        return GI_OOT_SONG_TP_FIRE;
    if (notes[NOTES_SONG_OOT_TP_WATER] >= 5 && !gOotSave.info.inventory.quest.songTpWater)
        return GI_OOT_SONG_TP_WATER;
    if (notes[NOTES_SONG_OOT_TP_SPIRIT] >= 6 && !gOotSave.info.inventory.quest.songTpSpirit)
        return GI_OOT_SONG_TP_SPIRIT;
    if (notes[NOTES_SONG_OOT_TP_SHADOW] >= 7 && !gOotSave.info.inventory.quest.songTpShadow)
        return GI_OOT_SONG_TP_SHADOW;
    if (notes[NOTES_SONG_OOT_TP_LIGHT] >= 6 && !gOotSave.info.inventory.quest.songTpLight)
        return GI_OOT_SONG_TP_LIGHT;
    if (notes[NOTES_SONG_OOT_ZELDA] >= 6 && !gOotSave.info.inventory.quest.songZelda)
        return GI_OOT_SONG_ZELDA;
    if (notes[NOTES_SONG_OOT_EPONA] >= 6 && !gOotSave.info.inventory.quest.songEpona)
        return GI_OOT_SONG_EPONA;
    if (notes[NOTES_SONG_OOT_SARIA] >= 6 && !gOotSave.info.inventory.quest.songSaria)
        return GI_OOT_SONG_SARIA;
    if (notes[NOTES_SONG_OOT_SUN] >= 6 && !gOotSave.info.inventory.quest.songSun)
        return GI_OOT_SONG_SUN;
    if (notes[NOTES_SONG_OOT_TIME] >= 6 && !gOotSave.info.inventory.quest.songTime)
        return GI_OOT_SONG_TIME;
    if (notes[NOTES_SONG_OOT_STORMS] >= 6 && !gOotSave.info.inventory.quest.songStorms)
        return GI_OOT_SONG_STORMS;
    if (notes[NOTES_SONG_OOT_EMPTINESS] >= 7 && !gSharedCustomSave.oot.hasElegy)
        return GI_OOT_SONG_EMPTINESS;

    if (notes[NOTES_SONG_MM_AWAKENING] >= 7 && !gMmSave.info.inventory.quest.songAwakening)
        return GI_MM_SONG_AWAKENING;
    if (notes[NOTES_SONG_MM_GORON] >= 8 && !gMmSave.info.inventory.quest.songLullaby)
        return GI_MM_SONG_GORON;
    if (notes[NOTES_SONG_MM_GORON] >= 6 && !gMmSave.info.inventory.quest.songLullabyIntro && Config_Flag(CFG_MM_PROGRESSIVE_LULLABY))
        return GI_MM_SONG_GORON_HALF;
    if (notes[NOTES_SONG_MM_ZORA] >= 7 && !gMmSave.info.inventory.quest.songNewWave)
        return GI_MM_SONG_ZORA;
    if (notes[NOTES_SONG_MM_EMPTINESS] >= 7 && !gMmSave.info.inventory.quest.songEmpty)
        return GI_MM_SONG_EMPTINESS;
    if (notes[NOTES_SONG_MM_ORDER] >= 6 && !gMmSave.info.inventory.quest.songOrder)
        return GI_MM_SONG_ORDER;
    if (notes[NOTES_SONG_MM_TIME] >= 6 && !gMmSave.info.inventory.quest.songTime)
        return GI_MM_SONG_TIME;
    if (notes[NOTES_SONG_MM_HEALING] >= 6 && !gMmSave.info.inventory.quest.songHealing)
        return GI_MM_SONG_HEALING;
    if (notes[NOTES_SONG_MM_EPONA] >= 6 && !gMmSave.info.inventory.quest.songEpona)
        return GI_MM_SONG_EPONA;
    if (notes[NOTES_SONG_MM_SOARING] >= 6 && !gMmSave.info.inventory.quest.songSoaring)
        return GI_MM_SONG_SOARING;
    if (notes[NOTES_SONG_MM_STORMS] >= 6 && !gMmSave.info.inventory.quest.songStorms)
        return GI_MM_SONG_STORMS;
    if (notes[NOTES_SONG_MM_SUN] >= 6 && !gMmSave.info.inventory.quest.songSun)
        return GI_MM_SONG_SUN;

    return GI_NONE;
}

static void Trigger_Set(u8 trigger)
{
    sTrigger = trigger;
    sTriggerAcc = 0;
}

static void Triggers_Check(PlayState* play)
{
    s16 gi;

    /* Songs */
    gi = Triggers_NextGivenSong();
    if (gi != GI_NONE)
    {
        sTriggerSongGi = gi;
        Trigger_Set(TRIGGER_SONG);
        return;
    }

    /* Ganon BK */
    if (Config_Flag(CFG_OOT_GANON_BK_CUSTOM) && !gOotExtraFlags.ganonBossKey && Config_SpecialCond(SPECIAL_GANON_BK))
    {
        Trigger_Set(TRIGGER_GANON_BK);
        return;
    }

    /* Triforce (Hunt) */
    if (Config_Flag(CFG_GOAL_TRIFORCE) && !gOotExtraFlags.triforceWin && gTriforceCount >= gComboConfig.triforceGoal)
    {
        Trigger_Set(TRIGGER_TRIFORCE);
        return;
    }

    /* Triforce (Quest) */
    if (Config_Flag(CFG_GOAL_TRIFORCE3) && !gOotExtraFlags.triforceWin && gTriforceCount >= 3)
    {
        Trigger_Set(TRIGGER_TRIFORCE);
        return;
    }

    /* Endgame Item Win */
    if (gOotExtraFlags.endgameItemIsWin && !gOotExtraFlags.triforceWin)
    {
        Trigger_Set(TRIGGER_TRIFORCE);
        return;
    }


#if defined(GAME_OOT)
    /* Sheik in colossus */
    if (comboHasSoulOot(GI_OOT_SOUL_NPC_SHEIK) && gSave.entrance == ENTR_OOT_DESERT_COLOSSUS_FROM_TEMPLE_SPIRIT && !GetEventChk(EV_OOT_CHK_SONG_TP_SPIRIT))
    {
        Trigger_Set(TRIGGER_OOT_SHEIK_COLOSSUS);
        return;
    }

    /* Sheik in Kakariko */
    if (comboHasSoulOot(GI_OOT_SOUL_NPC_SHEIK) && play->sceneId == SCE_OOT_KAKARIKO_VILLAGE && gSave.info.inventory.quest.medallionForest && gSave.info.inventory.quest.medallionFire && gSave.info.inventory.quest.medallionWater && gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_SONG_TP_SHADOW))
    {
        Trigger_Set(TRIGGER_OOT_SHEIK_KAKARIKO);
        return;
    }

    /* Saria's Ocarina */
    if (comboHasSoulOot(GI_OOT_SOUL_NPC_SARIA) && gSave.entrance == ENTR_OOT_LOST_WOODS_BRIDGE_FROM_FOREST && !GetEventChk(EV_OOT_CHK_SARIA_OCARINA))
    {
        Trigger_Set(TRIGGER_OOT_SARIA_OCARINA);
        return;
    }

    /* Zelda Light Arrows */
    if (comboHasSoulOot(GI_OOT_SOUL_NPC_SHEIK) && comboHasSoulOot(GI_OOT_SOUL_NPC_ZELDA) && play->sceneId == SCE_OOT_TEMPLE_OF_TIME && gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_LIGHT_ARROW))
    {
        int shouldTrigger;
        if (Config_Flag(CFG_OOT_LACS_CUSTOM))
            shouldTrigger = Config_SpecialCond(SPECIAL_LACS);
        else
            shouldTrigger = (gSave.info.inventory.quest.medallionSpirit && gSave.info.inventory.quest.medallionShadow);

        if (shouldTrigger)
        {
            Trigger_Set(TRIGGER_OOT_ZELDA_LIGHT_ARROW);
            return;
        }
    }

    /* Weird egg */
    if (g.triggerWeirdEgg)
    {
        Trigger_Set(TRIGGER_OOT_WEIRD_EGG);
        return;
    }

    /* Pocket egg */
    if (g.triggerPocketEgg)
    {
        Trigger_Set(TRIGGER_OOT_POCKET_EGG);
        return;
    }
#endif

#if defined(GAME_MM)
    if (!gMmExtraFlags2.songHealing && gSave.entrance == ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER && g.isNextEntranceInitialSong)
    {
        Trigger_Set(TRIGGER_MM_SONG_HEALING);
        return;
    }
#endif
}

static int Triggers_Run(PlayState* play)
{
    switch (sTrigger)
    {
    case TRIGGER_SONG:
        if (Triggers_GiveItemDirect(play, sTriggerSongGi))
            return 1;
        return 0;
    case TRIGGER_GANON_BK:
        if (Triggers_GiveItemDirect(play, GI_OOT_BOSS_KEY_GANON))
        {
            gOotExtraFlags.ganonBossKey = 1;
            return 1;
        }
        return 0;
    case TRIGGER_TRIFORCE:
        gOotExtraFlags.triforceWin = 1;
        comboCreditWarp(play);
        return 1;

#if defined(GAME_OOT)
    case TRIGGER_OOT_SHEIK_COLOSSUS:
        if (Triggers_GiveItemNpc(play, GI_OOT_SONG_TP_SPIRIT, NPC_OOT_SHEIK_SPIRIT))
        {
            SetEventChk(EV_OOT_CHK_SONG_TP_SPIRIT);
            return 1;
        }
        return 0;
    case TRIGGER_OOT_SHEIK_KAKARIKO:
        if (Triggers_GiveItemNpc(play, GI_OOT_SONG_TP_SHADOW, NPC_OOT_SHEIK_SHADOW))
        {
            SetEventChk(EV_OOT_CHK_SONG_TP_SHADOW);
            SetEventChk(EV_OOT_CHK_BONGO_ESCAPE);
            return 1;
        }
        return 0;
    case TRIGGER_OOT_SARIA_OCARINA:
        if (Triggers_GiveItemNpc(play, GI_OOT_OCARINA_FAIRY, NPC_OOT_SARIA_OCARINA))
        {
            SetEventChk(EV_OOT_CHK_SARIA_OCARINA);
            return 1;
        }
        return 0;
    case TRIGGER_OOT_ZELDA_LIGHT_ARROW:
        if (Triggers_GiveItemNpc(play, GI_OOT_ARROW_LIGHT, NPC_OOT_ZELDA_LIGHT_ARROW))
        {
            SetEventChk(EV_OOT_CHK_LIGHT_ARROW);
            return 1;
        }
        return 0;
    case TRIGGER_OOT_WEIRD_EGG:
        if (Triggers_GiveItemNpc(play, GI_OOT_CHICKEN, NPC_OOT_WEIRD_EGG))
        {
            comboRemoveTradeItemChild(XITEM_OOT_CHILD_WEIRD_EGG);
            g.triggerWeirdEgg = 0;
            return 1;
        }
        return 0;
    case TRIGGER_OOT_POCKET_EGG:
        if (Triggers_GiveItemNpc(play, GI_OOT_POCKET_CUCCO, NPC_OOT_POCKET_EGG))
        {
            comboRemoveTradeItemAdult(XITEM_OOT_ADULT_POCKET_EGG);
            g.triggerPocketEgg = 0;
            return 1;
        }
        return 0;
#endif

#if defined(GAME_MM)
    case TRIGGER_MM_SONG_HEALING:
        if (Triggers_GiveItemNpc(play, GI_MM_SONG_HEALING, NPC_MM_SONG_HEALING))
        {
            gMmExtraFlags2.songHealing = 1;
            return 1;
        }
        return 0;
#endif
    }

    return 0;
}

void Triggers_Reset(void)
{
    bzero(&sFakeActor, sizeof(sFakeActor));
    sFakeActor.update = (void*)1;

    sTrigger = TRIGGER_NONE;
    sTriggerAcc = 0;
    sTriggerSongGi = GI_NONE;
}

void Triggers_Update(PlayState* play)
{
    Player* player;

    if (gSaveContext.gameMode != GAMEMODE_NORMAL)
        return;

    player = GET_PLAYER(play);
    if (!player)
        return;

    /* Copy position, so that distance checks will work */
    sFakeActor.world.pos.x = player->actor.world.pos.x;
    sFakeActor.world.pos.y = player->actor.world.pos.y;
    sFakeActor.world.pos.z = player->actor.world.pos.z;

    /* Check for a new trigger, stop there if there isn't anything to do */
    if (sTrigger == TRIGGER_NONE)
    {
        Triggers_Check(play);
        if (sTrigger == TRIGGER_NONE)
            return;
    }

    if (!Triggers_ItemSafe(player, play))
        return;

    if (Triggers_Run(play))
    {
        sTrigger = TRIGGER_NONE;
        sTriggerAcc = 0;
        sTriggerSongGi = GI_NONE;
    }
}
