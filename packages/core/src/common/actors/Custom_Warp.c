#include <combo.h>
#include <combo/dungeon.h>

typedef struct
{
    Actor base;
}
Actor_CustomWarp;

#if defined(GAME_OOT)
static void CustomWarp_OnTrigger(Actor_CustomWarp* this, GameState_Play* play)
{
}
#endif

#if defined(GAME_MM)

#define SWITCH_SPRING       0
#define SWITCH_SWAMP_CLEAR  1
#define SWITCH_COAST_CLEAR  2

static void CustomWarp_OnTrigger(Actor_CustomWarp* this, GameState_Play* play)
{
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_BLACK;

    switch (this->base.variable)
    {
    case SWITCH_SPRING:
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH);
        play->nextEntrance = 0xae70;
        break;
    case SWITCH_SWAMP_CLEAR:
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF);
        play->nextEntrance = 0x0ca0;
        break;
    case SWITCH_COAST_CLEAR:
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB);
        play->nextEntrance = ENTR_MM_GREAT_BAY_COAST_FROM_LABORATORY;
        break;
    }
}
#endif

static void CustomWarp_Init(Actor_CustomWarp* this, GameState_Play* play)
{

}

static void CustomWarp_Update(Actor_CustomWarp* this, GameState_Play* play)
{
    if (ActorTalkedTo(&this->base))
    {
        CustomWarp_OnTrigger(this, play);
    }
    else
    {
        ActorEnableTalk(&this->base, play, 30.f);
    }
}

#if defined(GAME_OOT)
static const u32 kMatTransformOffset = 0x11da0;
#else
static const u32 kMatTransformOffset = 0x187fc;
#endif

/* TODO: Move this into a helper */
static void shaderFlameEffect(GameState_Play* play)
{
#if defined(GAME_OOT)
    static const u32 kFlameDlist = 0x52a10;
#else
    static const u32 kFlameDlist = 0x7d590;
#endif

    OPEN_DISPS(play->gs.gfx);
    ModelViewUnkTransform((float*)((char*)play + kMatTransformOffset));
    gSPSegment(POLY_XLU_DISP++, 0x08, DisplaceTexture(play->gs.gfx, 0, 0, 0, 0x20, 0x40, 1, 0, (-play->gs.frameCount & 0x7f) << 2, 0x20, 0x80));
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 0xff, 0x00, 0xff, 0xff);
    gDPSetEnvColor(POLY_XLU_DISP++, 0xff, 0x00, 0x00, 0xff);
    gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | kFlameDlist);
    CLOSE_DISPS();
}

static void CustomWarp_Draw(Actor_CustomWarp* this, GameState_Play* play)
{
    static const float scale = 0.003f;

    /* Transform */
    ModelViewTranslate(this->base.position.x, this->base.position.y, this->base.position.z, MAT_SET);
    ModelViewScale(scale, scale, scale, MAT_MUL);

    /* Draw */
    OPEN_DISPS(play->gs.gfx);
    InitListPolyXlu(play->gs.gfx);
    shaderFlameEffect(play);
    CLOSE_DISPS();
}

ActorInit CustomWarp_gActorInit = {
    AC_CUSTOM_WARP,
    0x7,
    0x11,
    0x1,
    sizeof(Actor_CustomWarp),
    (ActorFunc)CustomWarp_Init,
    NULL,
    (ActorFunc)CustomWarp_Update,
    (ActorFunc)CustomWarp_Draw,
};

void comboSpawnCustomWarps(GameState_Play* play)
{
    int variable;
    float x;
    float y;
    float z;

    variable = -1;

#if defined(GAME_MM)
    if ((comboConfig(CFG_ER_MAJOR_DUNGEONS) || gComboData.preCompleted & (1 << DUNGEONID_TEMPLE_SNOWHEAD)) && play->sceneId == SCE_MM_MOUNTAIN_VILLAGE_WINTER && gMiscFlags.erSpring)
    {
        variable = SWITCH_SPRING;
        x = -1200.f;
        y = 30.f;
        z = 600.f;
    }

    if ((comboConfig(CFG_ER_MAJOR_DUNGEONS) || gComboData.preCompleted & (1 << DUNGEONID_TEMPLE_WOODFALL)) && play->sceneId == SCE_MM_SOUTHERN_SWAMP && !MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF) && gMiscFlags.erSwampClear)
    {
        variable = SWITCH_SWAMP_CLEAR;
        x = -910.f;
        y = 50.f;
        z = -550.f;
    }

    if ((comboConfig(CFG_ER_MAJOR_DUNGEONS) || gComboData.preCompleted & (1 << DUNGEONID_TEMPLE_GREAT_BAY)) && play->sceneId == SCE_MM_GREAT_BAY_COAST && !MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB) && gMiscFlags.erCoastClear)
    {
        variable = SWITCH_COAST_CLEAR;
        x = -3020.f;
        y = 240.f;
        z = 3921.f;
    }
#endif

    if (variable == -1)
        return;

    SpawnActor(
        (char*)play + 0x1ca0,
        play,
        AC_CUSTOM_WARP,
        x, y, z,
        0, 0, 0,
        variable
    );
}
