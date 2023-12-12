#include <combo.h>
#include <combo/text.h>
#include <combo/dungeon.h>
#include <combo/item.h>

static int mapDungeonId(GameState_Play* play, s16 gi)
{
    switch (gi)
    {
    case GI_OOT_MAP_DT:
        return DUNGEONID_DEKU_TREE;
    case GI_OOT_MAP_DC:
        return DUNGEONID_DODONGOS_CAVERN;
    case GI_OOT_MAP_JJ:
        return DUNGEONID_JABU_JABU;
    case GI_OOT_MAP_FOREST:
        return DUNGEONID_TEMPLE_FOREST;
    case GI_OOT_MAP_FIRE:
        return DUNGEONID_TEMPLE_FIRE;
    case GI_OOT_MAP_WATER:
        return DUNGEONID_TEMPLE_WATER;
    case GI_OOT_MAP_SHADOW:
        return DUNGEONID_TEMPLE_SHADOW;
    case GI_OOT_MAP_SPIRIT:
        return DUNGEONID_TEMPLE_SPIRIT;
    case GI_OOT_MAP_BOTW:
        return DUNGEONID_BOTTOM_OF_THE_WELL;
    case GI_OOT_MAP_IC:
        return DUNGEONID_ICE_CAVERN;
    case GI_MM_MAP_WF:
        return DUNGEONID_TEMPLE_WOODFALL;
    case GI_MM_MAP_SH:
        return DUNGEONID_TEMPLE_SNOWHEAD;
    case GI_MM_MAP_GB:
        return DUNGEONID_TEMPLE_GREAT_BAY;
    case GI_MM_MAP_ST:
        return DUNGEONID_TEMPLE_STONE_TOWER;
    case GI_OOT_MAP:
    case GI_MM_MAP:
        return comboCurrentDungeon(play);
    default:
        return -1;
    }
}

static int compassDungeonId(GameState_Play* play, s16 gi)
{
    switch (gi)
    {
    case GI_OOT_COMPASS_DT:
        return DUNGEONID_DEKU_TREE;
    case GI_OOT_COMPASS_DC:
        return DUNGEONID_DODONGOS_CAVERN;
    case GI_OOT_COMPASS_JJ:
        return DUNGEONID_JABU_JABU;
    case GI_OOT_COMPASS_FOREST:
        return DUNGEONID_TEMPLE_FOREST;
    case GI_OOT_COMPASS_FIRE:
        return DUNGEONID_TEMPLE_FIRE;
    case GI_OOT_COMPASS_WATER:
        return DUNGEONID_TEMPLE_WATER;
    case GI_OOT_COMPASS_SHADOW:
        return DUNGEONID_TEMPLE_SHADOW;
    case GI_OOT_COMPASS_SPIRIT:
        return DUNGEONID_TEMPLE_SPIRIT;
    case GI_OOT_COMPASS_BOTW:
        return DUNGEONID_BOTTOM_OF_THE_WELL;
    case GI_OOT_COMPASS_IC:
        return DUNGEONID_ICE_CAVERN;
    case GI_MM_COMPASS_WF:
        return DUNGEONID_TEMPLE_WOODFALL;
    case GI_MM_COMPASS_SH:
        return DUNGEONID_TEMPLE_SNOWHEAD;
    case GI_MM_COMPASS_GB:
        return DUNGEONID_TEMPLE_GREAT_BAY;
    case GI_MM_COMPASS_ST:
        return DUNGEONID_TEMPLE_STONE_TOWER;
    case GI_OOT_COMPASS:
    case GI_MM_COMPASS:
        return comboCurrentDungeon(play);
    default:
        return -1;
    }
}

static void comboTextMap(char** b, GameState_Play* play, s16 gi)
{
    int dungeonId;

    if (!comboConfig(CFG_ER_DUNGEONS))
        return;

    dungeonId = mapDungeonId(play, gi);
    if (dungeonId < 0)
        return;

    /* Display the entrance */
    if (dungeonId == DUNGEONID_TEMPLE_STONE_TOWER || dungeonId == DUNGEONID_TEMPLE_STONE_TOWER_INVERTED)
    {
        comboTextAppendStr(b, TEXT_NL "The entrances are at ");
        comboTextAppendDungeonName(b, gComboData.dungeons[DUNGEONID_TEMPLE_STONE_TOWER]);
        comboTextAppendStr(b, " and ");
        comboTextAppendDungeonName(b, gComboData.dungeons[DUNGEONID_TEMPLE_STONE_TOWER_INVERTED]);
        comboTextAppendStr(b, ".");
    }
    else
    {
        comboTextAppendStr(b, TEXT_NL "The entrance is at ");
        comboTextAppendDungeonName(b, gComboData.dungeons[dungeonId]);
        comboTextAppendStr(b, ".");
    }
}


static void comboTextCompass(char** b, GameState_Play* play, s16 gi)
{
    int dungeonId;
    int bossId;

    if (!comboConfig(CFG_ER_BOSS))
        return;

    dungeonId = compassDungeonId(play, gi);
    if (dungeonId < 0)
        return;
    if (dungeonId == DUNGEONID_TEMPLE_STONE_TOWER)
        dungeonId = DUNGEONID_TEMPLE_STONE_TOWER_INVERTED;
    bossId = comboBossDungeon(dungeonId);
    if (bossId < 0)
        return;

    /* Display the boss */
    comboTextAppendStr(b, TEXT_NL "It points at ");
    comboTextAppendBossName(b, bossId);
    comboTextAppendStr(b, ".");
}

void comboTextExtra(char** b, GameState_Play* play, s16 gi)
{
    if (gi < 0)
        gi = -gi;

    comboTextMap(b, play, gi);
    comboTextCompass(b, play, gi);
}

void comboTextHijackItemEx(GameState_Play* play, const ComboItemOverride* o, int count)
{
    char* b;
    char* start;
    int isSelf;

    isSelf = (o->player == PLAYER_SELF) || (o->player == PLAYER_ALL) || (o->player == gComboData.playerId);

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->msgCtx.font.textBuffer.schar;
#endif
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "You got ");
    comboTextAppendItemNameOverride(&b, o, 0);
    comboTextAppendStr(&b, "!");
    if (isSelf)
    {
        if (count)
        {
            comboTextAppendStr(&b, TEXT_NL "This is your " TEXT_COLOR_RED);
            comboTextAppendOrd(&b, count);
            comboTextAppendClearColor(&b);
            comboTextAppendStr(&b, ".");
        }

        comboTextExtra(&b, play, o->gi);
    }
    comboTextAppendStr(&b, TEXT_END);
    comboTextAutoLineBreaks(start);
}

void comboTextHijackItem(GameState_Play* play, s16 gi, int count)
{
    ComboItemOverride o;
    memset(&o, 0, sizeof(o));
    o.gi = gi;
    comboTextHijackItemEx(play, &o, count);
}
