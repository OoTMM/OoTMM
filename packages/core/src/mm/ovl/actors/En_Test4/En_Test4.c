#include <combo.h>
#include <combo/mm/actors/En_Horse.h>
#include <combo/entrance.h>
#include <combo/environment.h>
#include <combo/interface.h>
#include <combo/config.h>
#include "En_Test4.h"

#define FLAGS (ACTOR_FLAG_MM_10 | ACTOR_FLAG_MM_20 | ACTOR_FLAG_MM_100000)

void EnTest4_Init(Actor_EnTest4* this, PlayState* play);
void EnTest4_Destroy(Actor_EnTest4* this, PlayState* play);
void EnTest4_Update(Actor_EnTest4* this, PlayState* play);

void EnTest4_HandleEvents(Actor_EnTest4* this, PlayState* play);
void EnTest4_HandleCutscene(Actor_EnTest4* this, PlayState* play);

static s32 sIsLoaded = 0;

static s16 sCsIdList[THREEDAY_DAYTIME_MAX];
static s16 sCurCsId;

void EnTest4_HandleDayNightSwapFromInit(Actor_EnTest4* this, PlayState* play)
{
    static s16 sNightOfTextIds[] = { 0x1bb4, 0x1bb5, 0x1bb6 };
    static s16 sDawnOfTextIds[] = { 0x1bb2, 0x1bb2, 0x1bb3 };

    if (this->daytimeIndex != THREEDAY_DAYTIME_NIGHT)
        Message_DisplaySceneTitleCard(play, sNightOfTextIds[gMmSave.day - 1]);
    else if ((sCsIdList[this->daytimeIndex] <= -1) || (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON))
    {
        if (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)
        {
            Sram_IncrementDay();
            gMmSave.time = CLOCK_TIME(6, 0);
            Message_DisplaySceneTitleCard(play, sDawnOfTextIds[gMmSave.day - 1]);
        }
        else
        {
            this->daytimeIndex = THREEDAY_DAYTIME_NIGHT;
            gMmSave.time += CLOCK_TIME(0, 1);
            this->prevTime = CURRENT_TIME;
        }

        Interface_NewDay(play, gMmSave.day);
        gSceneSeqState = SCENESEQ_MORNING;
        Environment_PlaySceneSequence(play);
        Environment_NewDay(&play->envCtx);
        this->actionFunc = EnTest4_HandleEvents;
    }

    if (gSaveContext.cutsceneTrigger == 0)
    {
        if ((sCsIdList[this->daytimeIndex] > -1) && !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON))
        {
            this->actionFunc = EnTest4_HandleCutscene;
            sCurCsId = sCsIdList[this->daytimeIndex];
            this->transitionCsTimer = 0;
            MM_SET_EVENT_INF(0x17);
        }
        else if (this->daytimeIndex == THREEDAY_DAYTIME_NIGHT)
            Audio_PlaySfx(NA_SE_EV_CHICKEN_CRY_M);
        else
            Audio_PlaySfx_2(NA_SE_EV_DOG_CRY_EVENING);
    }
    else
    {
        this->actionFunc = EnTest4_HandleEvents;
        if (this->daytimeIndex == THREEDAY_DAYTIME_NIGHT)
            this->daytimeIndex = THREEDAY_DAYTIME_DAY;
        else
            this->daytimeIndex = THREEDAY_DAYTIME_NIGHT;

        gMmSave.time += CLOCK_TIME(0, 1);
        this->prevTime = CURRENT_TIME;
    }
}

static void EnTest4_Reload(PlayState* play)
{
    Player* link;

    link = GET_PLAYER(play);
    Play_SetRespawnData(gPlay, 1, gSave.entrance, gPlay->roomCtx.curRoom.num, 0xdff, &link->actor.world.pos, link->actor.shape.rot.y);
    gSaveContext.respawnFlag = 2;
    gSaveContext.nextCutscene = 0;
    comboTransition(gPlay, gSave.entrance);
}

void EnTest4_HandleDayNightSwap(Actor_EnTest4* this, PlayState* play)
{
    static s16 sNightOfTextIds[] = { 0x1BB4, 0x1BB5, 0x1BB6 };
    static s16 sDawnOfTextIds[] = { 0x1BB2, 0x1BB2, 0x1BB3 };

    if (this->daytimeIndex != THREEDAY_DAYTIME_NIGHT)
        Message_DisplaySceneTitleCard(play, sNightOfTextIds[gMmSave.day - 1]);
    else if ((sCsIdList[this->daytimeIndex] <= -1) || (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON))
    {
        Sram_IncrementDay();
        gMmSave.time = CLOCK_TIME(6, 0);
        Interface_NewDay(play, gMmSave.day);
        Message_DisplaySceneTitleCard(play, sDawnOfTextIds[gMmSave.day - 1]);
        gSceneSeqState = SCENESEQ_MORNING;
        Environment_PlaySceneSequence(play);
        Environment_NewDay(&play->envCtx);
        this->actionFunc = EnTest4_HandleEvents;
    }

    if (gSaveContext.cutsceneTrigger == 0)
    {
        if ((sCsIdList[this->daytimeIndex] > -1) && !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON))
        {
            this->actionFunc = EnTest4_HandleCutscene;
            sCurCsId = sCsIdList[this->daytimeIndex];
            this->transitionCsTimer = 0;
            MM_SET_EVENT_INF(EVENTINF_17);
        }
        else if (this->daytimeIndex == THREEDAY_DAYTIME_NIGHT)
            Audio_PlaySfx(NA_SE_EV_CHICKEN_CRY_M);
        else
            Audio_PlaySfx_2(NA_SE_EV_DOG_CRY_EVENING);
    }
    else
    {
        this->actionFunc = EnTest4_HandleEvents;
        if (this->daytimeIndex == THREEDAY_DAYTIME_NIGHT)
            this->daytimeIndex = THREEDAY_DAYTIME_DAY;
        else
            this->daytimeIndex = THREEDAY_DAYTIME_NIGHT;

        gMmSave.time += CLOCK_TIME(0, 1);
        this->prevTime = CURRENT_TIME;
    }
}

/**
 * Get the next bell time on Day 3
 */
void EnTest4_GetBellTimeOnDay3(Actor_EnTest4* this)
{
    if ((CURRENT_TIME >= CLOCK_TIME(6, 0)) && (CURRENT_TIME <= CLOCK_TIME(18, 0)))
    {
        if (CURRENT_TIME < CLOCK_TIME(17, 30))
            this->nextBellTime = CLOCK_TIME(17, 30);
        else if (CURRENT_TIME < CLOCK_TIME(17, 36))
            this->nextBellTime = CLOCK_TIME(17, 36);
        else if (CURRENT_TIME < CLOCK_TIME(17, 42))
            this->nextBellTime = CLOCK_TIME(17, 42);
        else if (CURRENT_TIME < CLOCK_TIME(17, 48))
            this->nextBellTime = CLOCK_TIME(17, 48);
        else if (CURRENT_TIME < CLOCK_TIME(17, 54))
            this->nextBellTime = CLOCK_TIME(17, 54);
        else
            this->nextBellTime = CLOCK_TIME(0, 0);
        }
    else if (CURRENT_TIME > CLOCK_TIME(6, 0))
        this->nextBellTime = CLOCK_TIME(0, 0);
    else if (CURRENT_TIME < CLOCK_TIME(0, 10))
        this->nextBellTime = CLOCK_TIME(0, 10);
    else if (CURRENT_TIME < CLOCK_TIME(0, 20))
        this->nextBellTime = CLOCK_TIME(0, 20);
    else if (CURRENT_TIME < CLOCK_TIME(0, 30))
        this->nextBellTime = CLOCK_TIME(0, 30);
    else if (CURRENT_TIME < CLOCK_TIME(0, 40))
        this->nextBellTime = CLOCK_TIME(0, 40);
    else if (CURRENT_TIME < CLOCK_TIME(0, 50))
        this->nextBellTime = CLOCK_TIME(0, 50);
    else if (CURRENT_TIME < CLOCK_TIME(1, 0))
        this->nextBellTime = CLOCK_TIME(1, 0);
    else if (CURRENT_TIME < CLOCK_TIME(1, 10))
        this->nextBellTime = CLOCK_TIME(1, 10);
    else if (CURRENT_TIME < CLOCK_TIME(1, 20))
        this->nextBellTime = CLOCK_TIME(1, 20);
    else if (CURRENT_TIME < CLOCK_TIME(1, 30) - 1)
        this->nextBellTime = CLOCK_TIME(1, 30) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(1, 40) - 1)
        this->nextBellTime = CLOCK_TIME(1, 40) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(1, 50) - 1)
        this->nextBellTime = CLOCK_TIME(1, 50) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(2, 0))
        this->nextBellTime = CLOCK_TIME(2, 0);
    else if (CURRENT_TIME < CLOCK_TIME(2, 10))
        this->nextBellTime = CLOCK_TIME(2, 10);
    else if (CURRENT_TIME < CLOCK_TIME(2, 20))
        this->nextBellTime = CLOCK_TIME(2, 20);
    else if (CURRENT_TIME < CLOCK_TIME(2, 30))
        this->nextBellTime = CLOCK_TIME(2, 30);
    else if (CURRENT_TIME < CLOCK_TIME(2, 40))
        this->nextBellTime = CLOCK_TIME(2, 40);
    else if (CURRENT_TIME < CLOCK_TIME(2, 50))
        this->nextBellTime = CLOCK_TIME(2, 50);
    else if (CURRENT_TIME < CLOCK_TIME(3, 0))
        this->nextBellTime = CLOCK_TIME(3, 0);
    else if (CURRENT_TIME < CLOCK_TIME(3, 10))
        this->nextBellTime = CLOCK_TIME(3, 10);
    else if (CURRENT_TIME < CLOCK_TIME(3, 20))
        this->nextBellTime = CLOCK_TIME(3, 20);
    else if (CURRENT_TIME < CLOCK_TIME(3, 30))
        this->nextBellTime = CLOCK_TIME(3, 30);
    else if (CURRENT_TIME < CLOCK_TIME(3, 40))
        this->nextBellTime = CLOCK_TIME(3, 40);
    else if (CURRENT_TIME < CLOCK_TIME(3, 50))
        this->nextBellTime = CLOCK_TIME(3, 50);
    else if (CURRENT_TIME < CLOCK_TIME(4, 0))
        this->nextBellTime = CLOCK_TIME(4, 0);
    else if (CURRENT_TIME < CLOCK_TIME(4, 10))
        this->nextBellTime = CLOCK_TIME(4, 10);
    else if (CURRENT_TIME < CLOCK_TIME(4, 20))
        this->nextBellTime = CLOCK_TIME(4, 20);
    else if (CURRENT_TIME < CLOCK_TIME(4, 30) - 1)
        this->nextBellTime = CLOCK_TIME(4, 30) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(4, 40) - 1)
        this->nextBellTime = CLOCK_TIME(4, 40) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(4, 50) - 1)
        this->nextBellTime = CLOCK_TIME(4, 50) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(5, 0))
        this->nextBellTime = CLOCK_TIME(5, 0);
    else if (CURRENT_TIME < CLOCK_TIME(5, 5))
        this->nextBellTime = CLOCK_TIME(5, 5);
    else if (CURRENT_TIME < CLOCK_TIME(5, 10))
        this->nextBellTime = CLOCK_TIME(5, 10);
    else if (CURRENT_TIME < CLOCK_TIME(5, 15) - 1)
        this->nextBellTime = CLOCK_TIME(5, 15) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(5, 20))
        this->nextBellTime = CLOCK_TIME(5, 20);
    else if (CURRENT_TIME < CLOCK_TIME(5, 25) - 1)
        this->nextBellTime = CLOCK_TIME(5, 25) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(5, 30))
        this->nextBellTime = CLOCK_TIME(5, 30);
    else if (CURRENT_TIME < CLOCK_TIME(5, 33) - 1)
        this->nextBellTime = CLOCK_TIME(5, 33) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(5, 36))
        this->nextBellTime = CLOCK_TIME(5, 36);
    else if (CURRENT_TIME < CLOCK_TIME(5, 39) - 1)
        this->nextBellTime = CLOCK_TIME(5, 39) - 1;
    else if (CURRENT_TIME < CLOCK_TIME(5, 42))
        this->nextBellTime = CLOCK_TIME(5, 42);
    else if (CURRENT_TIME < CLOCK_TIME(5, 45))
        this->nextBellTime = CLOCK_TIME(5, 45);
    else if (CURRENT_TIME < CLOCK_TIME(5, 48))
        this->nextBellTime = CLOCK_TIME(5, 48);
    else if (CURRENT_TIME < CLOCK_TIME(5, 51))
        this->nextBellTime = CLOCK_TIME(5, 51);
    else if (CURRENT_TIME < CLOCK_TIME(5, 54))
        this->nextBellTime = CLOCK_TIME(5, 54);
    else if (CURRENT_TIME < CLOCK_TIME(5, 57))
        this->nextBellTime = CLOCK_TIME(5, 57);
    else if (CURRENT_TIME < CLOCK_TIME(6, 0))
        this->nextBellTime = CLOCK_TIME(6, 0);
}

void EnTest4_GetBellTimeAndShrinkScreenBeforeDay3(Actor_EnTest4* this, PlayState* play)
{
    if ((CURRENT_TIME >= CLOCK_TIME(6, 0)) && (CURRENT_TIME < CLOCK_TIME(18, 0)))
    {
        if (CURRENT_TIME < CLOCK_TIME(17, 30))
            this->nextBellTime = CLOCK_TIME(17, 30);
        else if (CURRENT_TIME < CLOCK_TIME(17, 36))
            this->nextBellTime = CLOCK_TIME(17, 36);
        else if (CURRENT_TIME < CLOCK_TIME(17, 42))
            this->nextBellTime = CLOCK_TIME(17, 42);
        else if (CURRENT_TIME < CLOCK_TIME(17, 48))
            this->nextBellTime = CLOCK_TIME(17, 48);
        else if (CURRENT_TIME < CLOCK_TIME(17, 54))
            this->nextBellTime = CLOCK_TIME(17, 54);
        else
            this->nextBellTime = CLOCK_TIME(5, 30);
    }
    else
    {
        if (CURRENT_TIME < CLOCK_TIME(5, 30))
            this->nextBellTime = CLOCK_TIME(5, 30);
        else if (CURRENT_TIME < CLOCK_TIME(5, 36))
            this->nextBellTime = CLOCK_TIME(5, 36);
        else if (CURRENT_TIME < CLOCK_TIME(5, 42))
            this->nextBellTime = CLOCK_TIME(5, 42);
        else if (CURRENT_TIME < CLOCK_TIME(5, 48))
            this->nextBellTime = CLOCK_TIME(5, 48);
        else if (CURRENT_TIME < CLOCK_TIME(5, 54))
            this->nextBellTime = CLOCK_TIME(5, 54);
        else if (CURRENT_TIME < CLOCK_TIME(6, 0))
            this->nextBellTime = CLOCK_TIME(17, 30);
        else
            this->nextBellTime = CLOCK_TIME(17, 30);
    }
}

void EnTest4_Init(Actor_EnTest4* this, PlayState* play)
{
    u32 eventDayCount;
    Player* player = GET_PLAYER(play);
    s8 csId = this->actor.csId;

    sCsIdList[THREEDAY_DAYTIME_NIGHT] = csId;
    if (csId > -1)
    {
        CutsceneEntry* csEntry = CutsceneManager_GetCutsceneEntry(sCsIdList[THREEDAY_DAYTIME_NIGHT]);
        MM_SET_EVENT_INF(EVENTINF_HAS_DAYTIME_TRANSITION_CS);
        sCsIdList[THREEDAY_DAYTIME_DAY] = csEntry->additionalCsId;
    }
    else
    {
        MM_CLEAR_EVENT_INF(EVENTINF_HAS_DAYTIME_TRANSITION_CS);
        sCsIdList[THREEDAY_DAYTIME_DAY] = sCsIdList[THREEDAY_DAYTIME_NIGHT];
    }

    if (sIsLoaded || MM_CHECK_EVENT_INF(EVENTINF_TRIGGER_DAYTELOP))
        Actor_Kill(&this->actor);
    else
    {
        sIsLoaded = 1;
        this->actor.room = -1;
        gSaveContext.screenScaleFlag = 0;
        gSaveContext.screenScale = 1000.0f;

        if (gMmSave.day == 0)
        {
            if (CURRENT_TIME < CLOCK_TIME(6, 1))
            {
                gMmSave.time = CLOCK_TIME(6, 0);
                gSaveContext.gameMode = GAMEMODE_NORMAL;
                STOP_GAMESTATE(&play->state);
                SET_NEXT_GAMESTATE(&play->state, DayTelop_Init, sizeof(DayTelopState));
                this->daytimeIndex = THREEDAY_DAYTIME_DAY;
                gMmSave.time = CLOCK_TIME(6, 0);
                Actor_Kill(&this->actor);
            }
            else
            {
                gMmSave.day = 1;
                eventDayCount = gMmSave.day;
                gMmSave.daysElapsed = eventDayCount;
                this->daytimeIndex = THREEDAY_DAYTIME_DAY;
                this->prevTime = CURRENT_TIME;
                this->actionFunc = EnTest4_HandleEvents;
            }
        }
        else if (CURRENT_TIME == CLOCK_TIME(6, 0))
        {
            this->daytimeIndex = THREEDAY_DAYTIME_NIGHT;
            EnTest4_HandleDayNightSwapFromInit(this, play);
            if ((gSaveContext.cutsceneTrigger == 0) && (sCsIdList[this->daytimeIndex] > -1) &&
                !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON))
                    player->stateFlags1 |= (1 << 9);
        }
        else
        {
            if ((CURRENT_TIME > CLOCK_TIME(18, 0)) || (CURRENT_TIME < CLOCK_TIME(6, 0)))
                this->daytimeIndex = THREEDAY_DAYTIME_NIGHT;
            else
                this->daytimeIndex = THREEDAY_DAYTIME_DAY;
            this->prevTime = CURRENT_TIME;
            this->actionFunc = EnTest4_HandleEvents;
        }
    }

    if (gMmSave.day == 3)
        EnTest4_GetBellTimeOnDay3(this);
    else
        EnTest4_GetBellTimeAndShrinkScreenBeforeDay3(this, play);

    this->prevBellTime = CURRENT_TIME;

    if ((sCsIdList[this->daytimeIndex] <= -1) || (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON))
    {
        gSaveContext.screenScaleFlag = 0;
        gSaveContext.screenScale = 1000.0f;
    }
}

void EnTest4_Destroy(Actor_EnTest4* this, PlayState* play) {}

void EnTest4_HandleEvents(Actor_EnTest4* this, PlayState* play)
{
    static u16 sDayNightTransitionTimes[THREEDAY_DAYTIME_MAX] =
    { CLOCK_TIME(6, 0), CLOCK_TIME(18, 0) };
    Player* player = GET_PLAYER(play);

    if ((play->transitionMode == 0) && !Play_InCsMode(play) && (play->numSetupActors <= 0) &&
        (play->roomCtx.status == 0) && !Play_IsDebugCamEnabled())
        {
        u16 transitionTime = sDayNightTransitionTimes[this->daytimeIndex];
        s16 curTimeUntilTransition;
        s16 prevTimeUntilTransition;
        s16 prevTimeUntilBell;
        s16 curTimeUntilBell;

        curTimeUntilTransition = CURRENT_TIME - transitionTime;
        prevTimeUntilTransition = this->prevTime - transitionTime;

        prevTimeUntilBell = this->prevBellTime - this->nextBellTime;
        curTimeUntilBell = CURRENT_TIME - this->nextBellTime;

        if ((curTimeUntilTransition * prevTimeUntilTransition) <= 0)
        {
            gSaveContext.unk_3CA7 = 1;
            if (play->actorCtx.flags & ACTORCTX_FLAG_PICTO_BOX_ON)
                play->actorCtx.flags &= ~ACTORCTX_FLAG_PICTO_BOX_ON;

            if (transitionTime != CLOCK_TIME(6, 0))
                EnTest4_HandleDayNightSwap(this, play);
            else if (transitionTime == CLOCK_TIME(6, 0))
            {
                if (gMmSave.day == 3)
                {
                    Interface_SkipMoonCrash(play);
                    if(!Config_Flag(CFG_MM_MOON_CRASH_CYCLE))
                    {
                        Actor_Kill(&this->actor);
                        MM_SET_EVENT_INF(EVENTINF_17);
                    }

                }
                else if (((sCsIdList[this->daytimeIndex] <= -1) ||
                            (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) &&
                           (gMmSave.day != 3))
                    EnTest4_HandleDayNightSwap(this, play);
                else
                {
                    gSaveContext.screenScale = 0.0f;
                    Play_SetRespawnData(play, RESPAWN_MODE_DOWN, Entrance_CreateFromSpawn(0), player->unk_3CE,
                                        PLAYER_PARAMS(0xFF, PLAYER_INITMODE_B), &player->unk_3C0, player->unk_3CC);
                    func_80169EFC(play);
                    if (player->stateFlags1 & (1 << 23))
                    {
                        EnHorse* rideActor = (EnHorse*)player->rideActor;
                        if ((rideActor->type == HORSE_TYPE_EPONA) || (rideActor->type == HORSE_TYPE_2))
                        {
                            if (gMmSave.day < 3)
                                gHorseIsMounted = 1;
                            else
                                gHorseIsMounted = 0;
                        }
                    }

                    gSaveContext.respawnFlag = -4;
                    MM_SET_EVENT_INF(EVENTINF_TRIGGER_DAYTELOP);
                    Actor_Kill(&this->actor);
                }
            }

            if ((sCsIdList[this->daytimeIndex] > -1) && !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON))
            {
                player->stateFlags1 |= (1 << 9);
                this->prevTime = CURRENT_TIME;
            }
            else
            {
                if (this->daytimeIndex == THREEDAY_DAYTIME_NIGHT)
                    this->daytimeIndex = THREEDAY_DAYTIME_DAY;
                else
                    this->daytimeIndex = THREEDAY_DAYTIME_NIGHT;

                gMmSave.time += CLOCK_TIME(0, 1);
                this->prevTime = CURRENT_TIME;
            }
            return;
        }
        if ((curTimeUntilBell * prevTimeUntilBell) <= 0)
        {
            Audio_PlaySfx_BigBells(&this->actor.projectedPos, THREEDAY_GET_BIG_BELLS_SFX_VOLUME_INDEX(&this->actor));
            this->prevBellTime = CURRENT_TIME;

            if (gMmSave.day == 3)
            {
                if ((this->nextBellTime == CLOCK_TIME(0, 0)) && (play->sceneId == SCE_MM_CLOCK_TOWN_SOUTH))
                    EnTest4_Reload(play);
                EnTest4_GetBellTimeOnDay3(this);
            }
            else
                EnTest4_GetBellTimeAndShrinkScreenBeforeDay3(this, play);
        }
    }
}

void EnTest4_HandleCutscene(Actor_EnTest4* this, PlayState* play)
{
    if (!this->transitionCsTimer)
    {
        if (sCurCsId > -1)
        {
            if (!CutsceneManager_IsNext(sCurCsId))
                CutsceneManager_Queue(sCurCsId);
            else
            {
                CutsceneManager_Start(sCurCsId, &this->actor);
                this->transitionCsTimer = 1;
            }
        }
        else
            this->transitionCsTimer = 1;
    }
    else if (this->transitionCsTimer < 60)
    {
        this->transitionCsTimer++;
        if (this->transitionCsTimer == 10)
        {
            if (this->daytimeIndex == THREEDAY_DAYTIME_NIGHT)
                Audio_PlaySfx(NA_SE_EV_CHICKEN_CRY_M);
            else
                Audio_PlaySfx_2(NA_SE_EV_DOG_CRY_EVENING);
        }
        if (this->transitionCsTimer == 60)
        {
            Player* player = GET_PLAYER(play);

            gMmSave.time += CLOCK_TIME(0, 1);
            this->prevTime = CURRENT_TIME;
            play->numSetupActors = -play->numSetupActors;
            player->stateFlags1 &= ~(1 << 9);
        }
    }
    else
    {
        this->actionFunc = EnTest4_HandleEvents;
        if (this->daytimeIndex == THREEDAY_DAYTIME_NIGHT)
            this->daytimeIndex = THREEDAY_DAYTIME_DAY;
        else
            this->daytimeIndex = THREEDAY_DAYTIME_NIGHT;

        if (sCurCsId > -1)
            CutsceneManager_Stop(sCurCsId);

        gSaveContext.hudVisibility = HUD_VISIBILITY_IDLE;
        MM_CLEAR_EVENT_INF(EVENTINF_17);
        Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
    }
}

void EnTest4_UpdateWeatherClear(Actor_EnTest4* this, PlayState* play)
{
    if ((gMmSave.day == 2) && (CURRENT_TIME >= CLOCK_TIME(7, 0)) && (CURRENT_TIME < CLOCK_TIME(17, 30)) &&
        (play->envCtx.precipitation[PRECIP_SNOW_CUR] == 0))
        {
        gWeatherMode = WEATHER_MODE_RAIN;
        Environment_PlayStormNatureAmbience(play);
        play->envCtx.lightningState = LIGHTNING_ON;
        play->envCtx.precipitation[PRECIP_RAIN_MAX] = 60;
    }
    else if ((play->envCtx.precipitation[PRECIP_RAIN_MAX] != 0) && ((play->gameplayFrames % 4) == 0))
    {
        play->envCtx.precipitation[PRECIP_RAIN_MAX]--;
        if (play->envCtx.precipitation[PRECIP_RAIN_MAX] == 8)
            Environment_StopStormNatureAmbience(play);
    }

    if (gWeatherMode == WEATHER_MODE_RAIN)
        this->weather = THREEDAY_WEATHER_RAIN;
}

void EnTest4_UpdateWeatherRainy(Actor_EnTest4* this, PlayState* play)
{
    if (((CURRENT_TIME >= CLOCK_TIME(17, 30)) && (CURRENT_TIME < CLOCK_TIME(23, 0)) &&
         (play->envCtx.precipitation[PRECIP_RAIN_MAX] != 0)) ||
        (play->envCtx.precipitation[PRECIP_SNOW_CUR] != 0))
        {
        gWeatherMode = WEATHER_MODE_CLEAR;
        play->envCtx.lightningState = LIGHTNING_LAST;
    }

    if (gWeatherMode == WEATHER_MODE_CLEAR)
        this->weather = THREEDAY_WEATHER_CLEAR;
}

void EnTest4_SetSkyboxNumStars(Actor_EnTest4* this, PlayState* play)
{
    s32 numStars = THREEDAY_GET_SKYBOX_NUM_STARS(&this->actor);

    if (numStars > 0)
        gSkyboxNumStars = numStars;
}

static int EnTest4_TimeSkip(PlayState* play, u8* day, u16* time)
{
    int currentHalfDay;
    int nextHalfDay;

    if (play->envCtx.sceneTimeSpeed == 0) {
        return 0;
    }

    if (Player_InCsMode(play))
        return 0;

    if (*day >= 4)
        return 0;
    if (*day < 1)
        currentHalfDay = 0;
    else
    {
        currentHalfDay = (*day - 1) * 2;
        if (*time < 0x4000 || *time >= 0xc000)
            currentHalfDay++;
    }

    /* Check for a clock for the current half-day */
    if (gSharedCustomSave.mm.halfDays & (1 << currentHalfDay))
        return 0;

    /* We don't have the correct clock, check for the next one */
    nextHalfDay = 6;
    for (int i = currentHalfDay + 1; i < 6; ++i)
    {
        if (gSharedCustomSave.mm.halfDays & (1 << i))
        {
            nextHalfDay = i;
            break;
        }
    }

    /* Store the new time */
    *day = (nextHalfDay / 2) + 1;
    if (nextHalfDay & 1)
        *time = 0xc000;
    else
        *time = 0x4000;
    return 1;
}

static int isNight(u16 time)
{
    return !!((time < 0x4000) || (time >= 0xc000));
}

static void EnTest4_CheckTimeSkip(Actor_EnTest4* this, PlayState* play)
{
    u8 day;
    u16 time;

    day = gSave.day;
    time = gSave.time + CLOCK_TIME(0, 1);
    if (isNight(this->prevTime) && !isNight(time))
        day++;

    if (day < 4 && EnTest4_TimeSkip(play, &day, &time))
    {
        gSave.day = day;
        gSave.time = time;

        /* Some scenes should be reloaded */
        switch (play->sceneId)
        {
        case SCE_MM_HONEY_DARLING:
            EnTest4_Reload(play);
            return;
        }

        if (time == 0x4000)
        {
            this->daytimeIndex = 0;
            gSave.day--;
        }
        else
        {
            this->daytimeIndex = 1;
            /* By default night doesn't reload the day counter/skybox, but we need to as we might have crossed a day */
            Interface_NewDay(play, gSave.day);
            Environment_NewDay(&play->envCtx);
        }
        this->prevTime = time - CLOCK_TIME(0, 1);
    }
}

void EnTest4_Update(Actor_EnTest4* this, PlayState* play)
{
    if (!BITMAP8_GET(gSave.eventInf, 0x0f))
        EnTest4_CheckTimeSkip(this, play);

    Player* player = GET_PLAYER(play);

    if (player->stateFlags1 & PLAYER_STATE1_MM_2) return;

    this->actionFunc(this, play);

    if (Environment_GetStormState(play) != STORM_STATE_OFF) {
        switch (this->weather) {
            case THREEDAY_WEATHER_CLEAR:
                EnTest4_UpdateWeatherClear(this, play);
                break;

            case THREEDAY_WEATHER_RAIN:
                EnTest4_UpdateWeatherRainy(this, play);
                break;

            default:
                break;
        }
    }

    EnTest4_SetSkyboxNumStars(this, play);
}

ActorProfile En_Test4_InitVars =
{
    ACTOR_EN_TEST4,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Actor_EnTest4),
    (ActorFunc)EnTest4_Init,
    (ActorFunc)EnTest4_Destroy,
    (ActorFunc)EnTest4_Update,
    NULL,
};

OVL_INFO_ACTOR(ACTOR_EN_TEST4, En_Test4_InitVars);
