#include <combo.h>
#include <combo/entrance.h>
#include <combo/dma.h>

#if defined(GAME_OOT)
# define ENTRANCES_ADDR 0x03ff4000
#else
# define ENTRANCES_ADDR 0x03ff5000
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

static Entrance* gEntrances;

void comboInitEntrances(void)
{
    size_t size;

    size = comboDmaLoadFile(NULL, COMBO_VROM_ENTRANCES);
    gEntrances = malloc(size);
    comboDmaLoadFile(gEntrances, COMBO_VROM_ENTRANCES);
}

s32 comboEntranceOverride(s16 entranceId)
{
    int i;

    i = 0;
    for (;;)
    {
        if (gEntrances[i].key == -1)
            return -1;
        if (gEntrances[i].key == (s32)((u16)entranceId & 0xffff))
            return gEntrances[i].value;
        i++;
    }
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
        {  0x23d, 0 }, /* DUNGEONID_GANON_CASTLE */
        {  0x534, 0 }, /* DUNGEONID_GANON_TOWER */
        { 0xd880, 1 }, /* DUNGEONID_MOON */
    };

    memcpy(dst, &kDungeonExits[dungeonId], sizeof(EntranceDescr));
}

void comboTransition(GameState_Play* play, u32 entrance)
{
#if defined(GAME_OOT)
    if (!(entrance & MASK_FOREIGN_ENTRANCE))
    {
        play->transitionTrigger = TRANS_TYPE_NORMAL;
        play->transitionType = TRANS_GFX_BLACK;
        play->nextEntranceIndex = (u16)entrance;
    }
    else
    {
        comboGameSwitch(play, entrance ^ MASK_FOREIGN_ENTRANCE);
    }
#endif

#if defined(GAME_MM)
    gNoTimeFlow = 0;
    if (gSave.playerForm == MM_PLAYER_FORM_FIERCE_DEITY && !comboConfig(CFG_MM_FD_ANYWHERE))
    {
        switch (play->sceneId)
        {
        case SCE_MM_LAIR_ODOLWA:
        case SCE_MM_LAIR_GOHT:
        case SCE_MM_LAIR_GYORG:
        case SCE_MM_LAIR_TWINMOLD:
        case SCE_MM_LAIR_MAJORA:
            gSave.playerForm = MM_PLAYER_FORM_HUMAN;
            gSave.equippedMask = PLAYER_MASK_NONE;
            break;
        }
    }

    if (!(entrance & MASK_FOREIGN_ENTRANCE))
    {
        play->nextEntrance = (u16)entrance;
        play->transitionTrigger = TRANS_TRIGGER_NORMAL;
        play->transitionType = TRANS_TYPE_FADE_BLACK;
    }
    else
    {
        comboGameSwitch(play, entrance ^ MASK_FOREIGN_ENTRANCE);
    }
#endif
}

void comboTransitionDescr(GameState_Play* play, const EntranceDescr* descr)
{
    u32 entrance;

    entrance = descr->id;

#if defined(GAME_OOT)
    if (descr->isMM)
        entrance |= MASK_FOREIGN_ENTRANCE;
#endif

#if defined(GAME_MM)
    if (!descr->isMM)
        entrance |= MASK_FOREIGN_ENTRANCE;
#endif

    comboTransition(play, entrance);
}
