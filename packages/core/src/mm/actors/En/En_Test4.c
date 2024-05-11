#include <combo.h>
#include <combo/time.h>
#include <combo/player.h>
#include <combo/interface.h>
#include <combo/environment.h>
#include <combo/mm/actors/En_Test4.h>

static int EnTest4_TimeSkip(GameState_Play* play, u8* day, u16* time)
{
    int currentHalfDay;
    int nextHalfDay;

    /* Sanity checks */
    if (gNoTimeFlow)
        return 0;

    switch (play->sceneId)
    {
    case SCE_MM_MOON:
    case SCE_MM_MOON_DEKU:
    case SCE_MM_MOON_GORON:
    case SCE_MM_MOON_ZORA:
    case SCE_MM_MOON_LINK:
    case SCE_MM_LAIR_MAJORA:
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

static void EnTest4_CheckSpawn(GameState_Play* play)
{
    int firstHalfDay;

    /* Check if we need to skip forward in time */
    if (!(gSave.day == 0 || (gSave.day == 1 && gSave.time == 0x4000)))
        return;

    firstHalfDay = 6;
    for (int i = 0; i < 6; ++i)
    {
        if (gSharedCustomSave.mm.halfDays & (1 << i))
        {
            firstHalfDay = i;
            break;
        }
    }

    if (firstHalfDay == 0)
    {
        /* Work around a vanilla bug */
        if (gSave.entranceIndex != ENTR_MM_CLOCK_TOWN)
            gSave.time = 0x4040;
        return;
    }

    /* We start on a later clock */
    if (firstHalfDay & 1)
    {
        gSave.day = (firstHalfDay / 2) + 1;
        gSave.time = 0xc000;
        Interface_NewDay(play, gSave.day);
        Environment_NewDay(&play->envCtx);
    }
    else
    {
        gSave.day = (firstHalfDay / 2);
        gSave.time = 0x3fff;
    }
}

static void EnTest4_CheckTimeSkip(Actor_EnTest4* this, GameState_Play* play)
{
    u8 day;
    u16 time;

    day = gSave.day;
    time = gSave.time;
    if (this->prevTime < 0x4000 && time >= 0x4000)
        day++;

    if (day < 4 && EnTest4_TimeSkip(play, &day, &time))
    {
        gSave.day = day;
        gSave.time = time;
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
        this->prevTime = time - 1;
        this->daytimeIndex = (time == 0xc000);
    }
}

static void EnTest4_UpdateWrapper(Actor_EnTest4* this, GameState_Play* play)
{
    void (*EnTest4_Update)(Actor_EnTest4*, GameState_Play*);

    EnTest4_CheckTimeSkip(this, play);

    EnTest4_Update = actorAddr(AC_EN_TEST4, 0x80a43274);
    EnTest4_Update(this, play);
}

void EnTest4_InitWrapper(Actor_EnTest4* this, GameState_Play* play)
{
    void (*EnTest4_Init)(Actor_EnTest4*, GameState_Play*);

    EnTest4_CheckSpawn(play);
    this->base.update = EnTest4_UpdateWrapper;

    EnTest4_Init = actorAddr(AC_EN_TEST4, 0x80a427e8);
    EnTest4_Init(this, play);
}
