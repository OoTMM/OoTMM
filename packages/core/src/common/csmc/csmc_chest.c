#include <combo.h>
#include <combo/csmc.h>
#include <combo/custom.h>
#include <combo/dungeon.h>

#define CSMC_CHEST_NORMAL     0x00
#define CSMC_CHEST_BOSS_KEY   0x01
#define CSMC_CHEST_MAJOR      0x02
#define CSMC_CHEST_KEY        0x03
#define CSMC_CHEST_SPIDER     0x04
#define CSMC_CHEST_FAIRY      0x05
#define CSMC_CHEST_HEART      0x06
#define CSMC_CHEST_SOUL       0x07

#if defined(GAME_OOT)
# define CHEST_TEX_NORMAL_FRONT     0x06001798
# define CHEST_TEX_NORMAL_SIDE      0x06002798
# define CHEST_TEX_BOSS_KEY_FRONT   0x06003798
# define CHEST_TEX_BOSS_KEY_SIDE    0x06002f98
#else
# define CHEST_TEX_NORMAL_FRONT     0x06001e60
# define CHEST_TEX_NORMAL_SIDE      0x06002e60
# define CHEST_TEX_BOSS_KEY_FRONT   0x06004e60
# define CHEST_TEX_BOSS_KEY_SIDE    0x06005e60
#endif

typedef struct
{
    u32 custom:1;
    u32 segFront:31;
    u32 segSide;
}
ChestCsmcData;

static const ChestCsmcData kCsmcData[] = {
    { 0, CHEST_TEX_NORMAL_FRONT, CHEST_TEX_NORMAL_SIDE },
    { 0, CHEST_TEX_BOSS_KEY_FRONT, CHEST_TEX_BOSS_KEY_SIDE },
    { 1, CUSTOM_CHEST_MAJOR_FRONT_ADDR, CUSTOM_CHEST_MAJOR_SIDE_ADDR },
    { 1, CUSTOM_CHEST_KEY_FRONT_ADDR, CUSTOM_CHEST_KEY_SIDE_ADDR },
    { 1, CUSTOM_CHEST_SPIDER_FRONT_ADDR, CUSTOM_CHEST_SPIDER_SIDE_ADDR },
    { 1, CUSTOM_CHEST_FAIRY_FRONT_ADDR, CUSTOM_CHEST_FAIRY_SIDE_ADDR },
    { 1, CUSTOM_CHEST_HEART_FRONT_ADDR, CUSTOM_CHEST_HEART_SIDE_ADDR },
    { 1, CUSTOM_CHEST_SOUL_FRONT_ADDR, CUSTOM_CHEST_SOUL_SIDE_ADDR },
};

static int csmcChestId(s16 gi)
{
    int csmcId;

    csmcId = csmcFromItem(gi);
    switch (csmcId)
    {
    case CSMC_NORMAL:       return CSMC_CHEST_NORMAL;
    case CSMC_BOSS_KEY:     return CSMC_CHEST_BOSS_KEY;
    case CSMC_MAJOR:        return CSMC_CHEST_MAJOR;
    case CSMC_KEY:          return CSMC_CHEST_KEY;
    case CSMC_SPIDER:       return CSMC_CHEST_SPIDER;
    case CSMC_FAIRY:        return CSMC_CHEST_FAIRY;
    case CSMC_HEART:        return CSMC_CHEST_HEART;
    case CSMC_SOUL:         return CSMC_CHEST_SOUL;
    default:                return CSMC_CHEST_MAJOR;
    }
}

static int csmcEnabledActor(Actor* this, GameState_Play* play)
{
    if (!csmcEnabled())
        return 0;

#if defined(GAME_OOT)
    if (play->sceneId == SCE_OOT_TREASURE_SHOP && (this->variable & 0x1f) != 0x0a && !comboConfig(CFG_OOT_CHEST_GAME_SHUFFLE))
        return 0;
#endif

#if defined(GAME_MM)
    if (play->sceneId == SCE_MM_MOON_GORON)
        return 0;
#endif

    return 1;
}

void csmcChestInit(Actor* this, GameState_Play* play, s16 gi)
{
    int type;

    if (!csmcEnabledActor(this, play))
        return;

    type = csmcFromItem(gi);
    if (type == CSMC_MAJOR || type == CSMC_BOSS_KEY)
    {
        ActorSetScale(this, 0.01f);
        ActorSetUnk(this, 40.f);


#if defined(GAME_OOT)
        /* Fix for IGT chest */
        if (play->sceneId == SCE_OOT_INSIDE_GANON_CASTLE)
        {
            if ((this->variable & 0x1f) == (gComboData.mq & (1 << MQ_GANON_CASTLE) ? 0x04 : 0x11))
                this->world.pos.z -= 10.f;
        }
#endif
    }
    else
    {
#if defined(GAME_OOT)
        ActorSetScale(this, 0.005f);
        /* Fix for spirit temple chest */
        if (play->sceneId == SCE_OOT_TEMPLE_SPIRIT && (this->variable & 0x1f) == 0x04)
        {
            this->world.pos.x += 40.f;
            this->world.pos.z += 40.f;
        }
#else
        ActorSetScale(this, 0.0075f);
#endif
        ActorSetUnk(this, 20.f);
    }

}

void csmcChestPreDraw(Actor* this, GameState_Play* play, s16 gi)
{
    int type;
    const void* listFront;
    const void* listSide;

    if (csmcEnabledActor(this, play))
        type = csmcChestId(gi);
    else
        type = CSMC_CHEST_NORMAL;

    listFront = csmcLoadTexture(kCsmcData[type].custom, kCsmcData[type].segFront, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 1);
    listSide = csmcLoadTexture(kCsmcData[type].custom, kCsmcData[type].segSide, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 1);

    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x0a, listFront);
    gSPSegment(POLY_OPA_DISP++, 0x0b, listSide);
    gSPSegment(POLY_XLU_DISP++, 0x0a, listFront);
    gSPSegment(POLY_XLU_DISP++, 0x0b, listSide);
    CLOSE_DISPS();
}

int csmcChestLarge(s16 gi)
{
    if (!csmcEnabled())
        return -1;
    switch (csmcFromItem(gi))
    {
    case CSMC_MAJOR:
    case CSMC_BOSS_KEY:
        return 1;
    default:
        return 0;
    }
}
