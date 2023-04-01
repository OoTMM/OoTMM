#include <combo.h>
#include <combo/entrance.h>

#if defined(GAME_OOT)
# define ENTRANCES_ADDR 0x03fec000
#else
# define ENTRANCES_ADDR 0x03fed000
#endif

s8  gIsEntranceOverride;
s32 gLastEntrance;
s32 gLastScene;

typedef struct PACKED ALIGNED(4)
{
    s32 key;
    s32 value;
}
Entrance;

ALIGNED(16) static Entrance gEntrances[0x20];

void comboInitEntrances(void)
{
    DMARomToRam(ENTRANCES_ADDR | PI_DOM1_ADDR2, gEntrances, sizeof(gEntrances));
}

s32 comboEntranceOverride(s16 entranceId)
{
    for (int i = 0; i < ARRAY_SIZE(gEntrances); i++)
    {
        if (gEntrances[i].key == -1)
            break;
        if (gEntrances[i].key == (s32)((u16)entranceId & 0xffff))
            return gEntrances[i].value;
    }
    return -1;
}

void comboGetDungeonExit(EntranceDescr* dst, int dungeonId)
{
    static const EntranceDescr kDungeonExits[] = {
        {  0x209, 0 }, /* DUNGEONID_DEKU_TREE */
        {  0x242, 0 }, /* DUNGEONID_DODONGOS_CAVERN */
        {  0x221, 0 }, /* DUNGEONID_JABU_JABU */
        {  0x215, 0 }, /* DUNGEONID_TEMPLE_FOREST */
        {  0x24a, 0 }, /* DUNGEONID_TEMPLE_FIRE */
        {  0x21d, 0 }, /* DUNGEONID_TEMPLE_WATER */
        {  0x205, 0 }, /* DUNGEONID_TEMPLE_SHADOW */
        {  0x1e1, 0 }, /* DUNGEONID_TEMPLE_SPIRIT */
        { 0x8610, 1 }, /* DUNGEONID_TEMPLE_WOODFALL */
        { 0xb210, 1 }, /* DUNGEONID_TEMPLE_SNOWHEAD */
        { 0x6a70, 1 }, /* DUNGEONID_TEMPLE_GREAT_BAY */
        { 0xac10, 1 }, /* DUNGEONID_TEMPLE_STONE_TOWER_INVERTED */
        { 0xaa20, 1 }, /* DUNGEONID_TEMPLE_STONE_TOWER */
        { 0x8480, 1 }, /* DUNGEONID_SPIDER_HOUSE_SWAMP */
        { 0x6880, 1 }, /* DUNGEONID_SPIDER_HOUSE_OCEAN */
        {  0x2a6, 0 }, /* DUNGEONID_BOTTOM_OF_THE_WELL */
        {  0x3d4, 0 }, /* DUNGEONID_ICE_CAVERN */
        {  0x3a8, 0 }, /* DUNGEONID_GERUDO_TRAINING_GROUNDS */
        { 0x2050, 1 }, /* DUNGEONID_BENEATH_THE_WELL */
        { 0x3420, 1 }, /* DUNGEONID_IKANA_CASTLE */
        { 0x20c0, 1 }, /* DUNGEONID_SECRET_SHRINE */
        { 0x3400, 1 }, /* DUNGEONID_BENEATH_THE_WELL_END */
        { 0x6850, 1 }, /* DUNGEONID_PIRATE_FORTRESS */
    };

    memcpy(dst, &kDungeonExits[dungeonId], sizeof(EntranceDescr));
}

void comboTransition(GameState_Play* play, const EntranceDescr* descr)
{
#if defined(GAME_OOT)
    if (!descr->isMM)
    {
        TransitionContext* t;

        t = &play->transition;
        t->type = TRANS_TYPE_NORMAL;
        t->gfx = TRANS_GFX_BLACK;
        t->entrance = descr->id;
    }
    else
    {
        comboGameSwitch(play, descr->id);
    }
#endif

#if defined(GAME_MM)
    if (descr->isMM)
    {
        play->nextEntrance = descr->id;
        play->transitionTrigger = TRANS_TRIGGER_NORMAL;
        play->transitionType = TRANS_TYPE_BLACK;
    }
    else
    {
        comboGameSwitch(play, descr->id);
    }
#endif
}
