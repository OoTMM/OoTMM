#include <combo.h>
#include <combo/config.h>
#include <combo/global.h>
#include <combo/text.h>
#include <combo/environment.h>
#include <combo/mm/regs.h>
#include <combo/mm/message.h>
#include <combo/common/ocarina.h>
#include <combo/player.h>

#include "../../build/include/combo/generated_config.h"

void Interface_DrawClock(PlayState* play);
void Message_DrawMain(PlayState* play, Gfx** gfxP);
void Font_LoadMessageBoxEndIcon(Font* font, u16 icon);

#define DT_SNAP_MINUTES 30u
#define DT_MINUTES_PER_DAY 1440u
#define DT_HALF_TICKS 0x8000u
#define DT_HALF_COUNT 6u
#define DT_MAX_TICKS 0x2c000u
#define DT_BASE_TEXT_ID 0x1b91
#define DT_FINAL_NIGHT_FAIL_TEXT_ID 0x1b94
#define DT_CLOCK_DRAW_TEXT_ID 0x0100
#define DT_FAST_FORWARD_SPEED 400
#define DT_SELECTOR_BOX_Y 90

typedef enum {
    DT_STATE_NONE,
    DT_STATE_SELECT,
    DT_STATE_CONFIRM,
    DT_STATE_FAST_FORWARD,
} DoubleTimeState;

static DoubleTimeState sDtState;
static u8 sDtClockTextureDay;
static u8 sDtSceneFlagsBackup[sizeof(gSave.info.permanentSceneFlags)];
static s16 sDtHoldFrames, sDtOldTimeSpeed, sDtStickRepeatTimer;
static s8 sDtStickRepeatDir;
static s32 sDtDisplayedHalf = -1;
static u32 sDtStartTicks, sDtTargetTicks;

u8 gDoubleTimeTargetPending;
u8 gDoubleTimeTargetDayChanged;

static int DtIsNight(u16 time) { return time < CLOCK_TIME(6, 0) || time >= CLOCK_TIME(18, 0); }
static u32 DtTicksToMinutes(u32 ticks) { return (ticks * DT_MINUTES_PER_DAY) >> 16; }
static u32 DtMinutesToTicks(u32 minutes) { return ((minutes << 16) + DT_MINUTES_PER_DAY - 1) / DT_MINUTES_PER_DAY; }

static u32 DtCurrentTicks(void)
{
    u32 day = gSave.day < 1 ? 1 : gSave.day;
    return ((day - 1) << 16) + (u16)(gSave.time - CLOCK_TIME(6, 0));
}

static void DtTicksToDayTime(u32 ticks, u32* day, u16* time)
{
    *day = (ticks >> 16) + 1;
    *time = (u16)((ticks & 0xffff) + CLOCK_TIME(6, 0));
}

static u32 DtNextSnapTicks(u32 ticks)
{
    u32 m = DtTicksToMinutes(ticks);
    u32 snap = ((m + DT_SNAP_MINUTES - 1) / DT_SNAP_MINUTES) * DT_SNAP_MINUTES;
    u32 result = DtMinutesToTicks(snap);
    if (result <= ticks)
        result = DtMinutesToTicks(snap + DT_SNAP_MINUTES);
    return result > DT_MAX_TICKS ? DT_MAX_TICKS : result;
}

static u32 DtPrevSnapTicks(u32 ticks)
{
    u32 m = DtTicksToMinutes(ticks);
    u32 snap = (m / DT_SNAP_MINUTES) * DT_SNAP_MINUTES;
    u32 result = DtMinutesToTicks(snap);
    if (result < ticks)
        return result;
    return snap < DT_SNAP_MINUTES ? 0 : DtMinutesToTicks(snap - DT_SNAP_MINUTES);
}

static u32 DtGetLandingTicks(u32 ticks)
{
    if (ticks && (!(ticks % DT_HALF_TICKS) || ticks == DT_MAX_TICKS))
        return DtMinutesToTicks(DtTicksToMinutes(ticks) - 1);
    return ticks;
}

static void DtLoadClockDayTexture(PlayState* play, u32 day)
{
    if (day < 1) day = 1;
    else if (day > 3) day = 3;
    if (sDtClockTextureDay == day)
        return;

    memcpy(sDtSceneFlagsBackup, gSave.info.permanentSceneFlags, sizeof(sDtSceneFlagsBackup));
    Interface_NewDay(play, day);
    memcpy(gSave.info.permanentSceneFlags, sDtSceneFlagsBackup, sizeof(sDtSceneFlagsBackup));
    sDtClockTextureDay = day;
}

static void DtAppendDayName(char** b, u32 day, u16 time)
{
    static const char* names[2][3] = {
        { "First Day", "Second Day", "Final Day" },
        { "Night of the First Day", "Night of the Second Day", "Night of the Final Day" },
    };
    if (day < 1) day = 1;
    else if (day > 3) day = 3;
    comboTextAppendStr(b, names[DtIsNight(time)][day - 1]);
}

static void DtAppendTime(char** b, u16 time)
{
    char tmp[9];
    char* p = tmp;
    u32 m = ((u32)time * DT_MINUTES_PER_DAY) >> 16;
    u32 h24 = m / 60, h = h24 % 12, min = m % 60;

    if (!h) h = 12;
    if (h >= 10) *p++ = '0' + h / 10;
    *p++ = '0' + h % 10;
    *p++ = ':';
    *p++ = '0' + min / 10;
    *p++ = '0' + min % 10;
    *p++ = ' ';
    *p++ = h24 >= 12 ? 'P' : 'A';
    *p++ = 'M';
    *p = '\0';
    comboTextAppendStr(b, tmp);
}

static void DtShowSelectorText(PlayState* play)
{
    char* b;
    u32 day;
    u16 time;

    Message_ContinueTextbox(play, DT_BASE_TEXT_ID);
    DtTicksToDayTime(sDtTargetTicks, &day, &time);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, TEXT_FAST "Select a time." TEXT_NL TEXT_COLOR_YELLOW);
    DtAppendDayName(&b, day, time);
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, TEXT_NL "Left/Right: Change" TEXT_NL "A: Confirm  B: Cancel" TEXT_FAST_END TEXT_NOCLOSE);
    play->msgCtx.ocarinaMode = OCARINA_MODE_PROCESS_DOUBLE_TIME;
    sDtDisplayedHalf = sDtTargetTicks / DT_HALF_TICKS;
}

static void DtShowConfirmation(PlayState* play)
{
    char* b;
    u32 day;
    u16 time;

    DtTicksToDayTime(sDtTargetTicks, &day, &time);
    play->msgCtx.ocarinaMode = OCARINA_MODE_14;
    Message_ContinueTextbox(play, DT_BASE_TEXT_ID);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, TEXT_FAST "Travel to" TEXT_NL TEXT_COLOR_YELLOW);
    DtAppendDayName(&b, day, time);
    comboTextAppendStr(&b, " ");
    DtAppendTime(&b, time);
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_FAST_END TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN "Yes" TEXT_NL "No" TEXT_END);
    Font_LoadMessageBoxEndIcon(&play->msgCtx.font, 2);
    play->msgCtx.choiceIndex = 0;
}

static int DtReadHorizontalInput(Input* input)
{
    s8 dir = 0;

    if (input->cur.button & L_JPAD) dir = -1;
    else if (input->cur.button & R_JPAD) dir = 1;
    else if (input->rel.stick_x < -30) dir = -1;
    else if (input->rel.stick_x > 30) dir = 1;

    if (!dir) {
        sDtStickRepeatDir = sDtStickRepeatTimer = sDtHoldFrames = 0;
        return 0;
    }

    if (dir != sDtStickRepeatDir) {
        sDtStickRepeatDir = dir;
        sDtStickRepeatTimer = 6;
        sDtHoldFrames = 0;
        return dir;
    }

    if (sDtHoldFrames < 0x7fff)
        sDtHoldFrames++;

    if (sDtStickRepeatTimer > 0) {
        sDtStickRepeatTimer--;
        return 0;
    }

    sDtStickRepeatTimer = sDtHoldFrames < 6 ? 4 : sDtHoldFrames < 12 ? 2 : sDtHoldFrames < 20 ? 1 : 0;
    return dir;
}

static void DtMoveTarget(PlayState* play, int dir)
{
    u32 old = sDtTargetTicks, next, half, target;
    s32 i;
    s32 oldHalf = old / DT_HALF_TICKS;

    if (dir > 0) {
        next = DtNextSnapTicks(old);
        half = next / DT_HALF_TICKS;

        if (half >= DT_HALF_COUNT || !(gSharedCustomSave.mm.halfDays & (1u << half))) {
            next = old;
            for (i = half + 1; i < DT_HALF_COUNT; i++) {
                if (!(gSharedCustomSave.mm.halfDays & (1u << i)))
                    continue;
                target = (u32)i * DT_HALF_TICKS;
                if (target <= DT_MAX_TICKS)
                    next = target;
                break;
            }
        }

        sDtTargetTicks = next;
    } else if (dir < 0) {
        next = old <= sDtStartTicks ? sDtStartTicks : DtPrevSnapTicks(old);

        if (next <= sDtStartTicks) {
            sDtTargetTicks = sDtStartTicks;
        } else {
            half = next / DT_HALF_TICKS;

            if (half < DT_HALF_COUNT && (gSharedCustomSave.mm.halfDays & (1u << half))) {
                sDtTargetTicks = next;
            } else {
                sDtTargetTicks = sDtStartTicks;
                for (i = (s32)half - 1; i >= 0; i--) {
                    if (!(gSharedCustomSave.mm.halfDays & (1u << i)))
                        continue;
                    target = DtMinutesToTicks(DtTicksToMinutes(((u32)i + 1) * DT_HALF_TICKS) - DT_SNAP_MINUTES);
                    sDtTargetTicks = target > sDtStartTicks ? target : sDtStartTicks;
                    break;
                }
            }
        }
    }

    if (sDtTargetTicks < sDtStartTicks) sDtTargetTicks = sDtStartTicks;
    if (sDtTargetTicks > DT_MAX_TICKS) sDtTargetTicks = DT_MAX_TICKS;
    if (sDtTargetTicks != old) Audio_PlaySfx(NA_SE_SY_CURSOR);

    DtLoadClockDayTexture(play, (sDtTargetTicks >> 16) + 1);
    if ((s32)(sDtTargetTicks / DT_HALF_TICKS) != oldHalf ||
        (s32)(sDtTargetTicks / DT_HALF_TICKS) != sDtDisplayedHalf)
        DtShowSelectorText(play);
}

static int DtShouldBegin(PlayState* play)
{
    MessageContext* msg = &play->msgCtx;
    if (DtCurrentTicks() >= DT_MAX_TICKS)
        return 0;
    return msg->ocarinaMode == OCARINA_MODE_PROCESS_DOUBLE_TIME ||
        (msg->ocarinaMode == OCARINA_MODE_END &&
         msg->songPlayed == OCARINA_SONG_DOUBLE_TIME &&
         msg->currentTextId == DT_FINAL_NIGHT_FAIL_TEXT_ID);
}

static void DtBegin(PlayState* play)
{
    sDtStartTicks = sDtTargetTicks = DtCurrentTicks();
    sDtStickRepeatDir = sDtStickRepeatTimer = sDtHoldFrames = 0;
    sDtDisplayedHalf = -1;
    sDtState = DT_STATE_SELECT;
    DtShowSelectorText(play);
}

static void DtCancel(PlayState* play)
{
    u32 day = gSave.day;

    if (day < 1) day = 1;
    else if (day > 3) day = 3;
    if (sDtClockTextureDay && sDtClockTextureDay != day)
        DtLoadClockDayTexture(play, day);

    sDtClockTextureDay = 0;
    Message_Close(play);
    play->msgCtx.ocarinaMode = OCARINA_MODE_END;
    sDtStickRepeatDir = sDtStickRepeatTimer = sDtHoldFrames = 0;
    sDtDisplayedHalf = -1;
    sDtState = DT_STATE_NONE;
}

static void DtConfigureFastForwardSpeed(void)
{
    u32 now = DtCurrentTicks(), remaining;
    s32 speed;

    if (now >= sDtTargetTicks)
        return;

    remaining = sDtTargetTicks - now;
    if (remaining > (u32)(DT_FAST_FORWARD_SPEED + gSave.daySpeed)) {
        R_TIME_SPEED = DT_FAST_FORWARD_SPEED;
        return;
    }

    speed = (s32)remaining - gSave.daySpeed;
    R_TIME_SPEED = speed <= 0 ? 1 : speed > DT_FAST_FORWARD_SPEED ? DT_FAST_FORWARD_SPEED : speed;
}

static void DtFinishFastForward(void)
{
    u32 day;
    u16 time;

    DtTicksToDayTime(sDtTargetTicks, &day, &time);
    gSave.day = day;
    gSave.time = time;
    gSave.isNight = DtIsNight(time);
    R_TIME_SPEED = sDtOldTimeSpeed;
    sDtState = DT_STATE_NONE;
}

static void DtStartFastForward(PlayState* play)
{
    sDtTargetTicks = DtGetLandingTicks(sDtTargetTicks);
    sDtClockTextureDay = 0;
    sDtOldTimeSpeed = R_TIME_SPEED;
    Message_Close(play);
    play->msgCtx.ocarinaMode = OCARINA_MODE_END;
    Environment_StartTime();
    R_TIME_SPEED = DT_FAST_FORWARD_SPEED;
    DtConfigureFastForwardSpeed();
    sDtState = DT_STATE_FAST_FORWARD;
}

static void DtUpdateFastForward(PlayState* play)
{
    u32 now = DtCurrentTicks(), remaining;
    s32 next;

    if (now >= sDtTargetTicks || !play->envCtx.sceneTimeSpeed) {
        DtFinishFastForward();
        return;
    }

    remaining = sDtTargetTicks - now;
    next = R_TIME_SPEED + gSave.daySpeed;

    if (next <= 0 || remaining <= (u32)next ||
        remaining <= (u32)(DT_FAST_FORWARD_SPEED + gSave.daySpeed))
        DtConfigureFastForwardSpeed();
    else
        R_TIME_SPEED = DT_FAST_FORWARD_SPEED;
}

static void DtStartNormalDoubleTime(PlayState* play)
{
    u32 oldDay = gSave.day, day;
    u16 time;

    DtTicksToDayTime(DtGetLandingTicks(sDtTargetTicks), &day, &time);
    while (gSave.day < day)
        Sram_IncrementDay();

    gSave.time = time;
    gSave.isNight = DtIsNight(time);
    sDtClockTextureDay = 0;
    gDoubleTimeTargetDayChanged = oldDay != gSave.day;
    gDoubleTimeTargetPending = 1;
    gSaveContext.timerStates[TIMER_ID_MOON_CRASH] = 0;
    Message_Close(play);
    play->msgCtx.ocarinaMode = OCARINA_MODE_APPLY_DOUBLE_SOT;
    sDtState = DT_STATE_NONE;
}

static void DtAcceptTarget(PlayState* play)
{
    if (sDtTargetTicks <= sDtStartTicks) {
        DtCancel(play);
        return;
    }

    if (sDtStartTicks / DT_HALF_TICKS == sDtTargetTicks / DT_HALF_TICKS && play->envCtx.sceneTimeSpeed)
        DtStartFastForward(play);
    else
        DtStartNormalDoubleTime(play);
}

static void DtUpdateSelector(PlayState* play)
{
    Input* input = &play->state.input[0];
    int dir = DtReadHorizontalInput(input);

    if (input->press.button & B_BUTTON) {
        DtCancel(play);
        return;
    }

    if (dir) {
        s16 steps = sDtHoldFrames < 20 ? 1 : sDtHoldFrames < 30 ? 2 : sDtHoldFrames < 40 ? 4 : sDtHoldFrames < 50 ? 8 : 12;
        for (s16 i = 0; i < steps; i++) {
            u32 old = sDtTargetTicks;
            DtMoveTarget(play, dir);
            if (old == sDtTargetTicks)
                break;
        }
    }

    if (input->press.button & A_BUTTON) {
        DtShowConfirmation(play);
        sDtState = DT_STATE_CONFIRM;
    }
}

static void DtUpdateConfirmation(PlayState* play)
{
    Input* input = &play->state.input[0];

    if (input->press.button & B_BUTTON) {
        Audio_PlaySfx_MessageCancel();
        DtShowSelectorText(play);
        sDtState = DT_STATE_SELECT;
        return;
    }

    if (Message_GetState(&play->msgCtx) != TEXT_STATE_CHOICE || !(input->press.button & A_BUTTON))
        return;

    if (!play->msgCtx.choiceIndex) {
        Audio_PlaySfx_MessageDecide();
        DtAcceptTarget(play);
    } else {
        Audio_PlaySfx_MessageCancel();
        DtShowSelectorText(play);
        sDtState = DT_STATE_SELECT;
    }
}

void DoubleTimeSelector_Update(PlayState* play)
{
    if (!Config_Flag(CFG_MM_SONG_OF_DOUBLE_TIME_TIME_SELECTOR))
        return;

    switch (sDtState) {
    case DT_STATE_NONE: if (DtShouldBegin(play)) DtBegin(play); break;
    case DT_STATE_SELECT: DtUpdateSelector(play); break;
    case DT_STATE_CONFIRM: DtUpdateConfirmation(play); break;
    case DT_STATE_FAST_FORWARD: DtUpdateFastForward(play); break;
    }
}

void DoubleTimeSelector_Draw(PlayState* play)
{
    InterfaceContext* iface = &play->interfaceCtx;
    u32 targetDay, oldDay;
    u16 targetTime, oldTime, oldTextId;
    s32 oldNight;
    s16 oldSpeed, oldAlpha;
    u8 stopped;

    if (!Config_Flag(CFG_MM_SONG_OF_DOUBLE_TIME_TIME_SELECTOR) || sDtState != DT_STATE_SELECT)
        return;

    DtTicksToDayTime(sDtTargetTicks, &targetDay, &targetTime);

    oldDay = gSave.day;
    oldTime = gSave.time;
    oldNight = gSave.isNight;
    oldTextId = play->msgCtx.currentTextId;
    oldSpeed = R_TIME_SPEED;
    oldAlpha = iface->bAlpha;
    stopped = Environment_IsTimeStopped();

    gSave.day = targetDay;
    gSave.time = targetTime;
    gSave.isNight = DtIsNight(targetTime);

    if (!R_TIME_SPEED)
        R_TIME_SPEED = 1;

    play->msgCtx.currentTextId = DT_CLOCK_DRAW_TEXT_ID;
    iface->bAlpha = 255;

    if (stopped)
        Environment_StartTime();

    Interface_DrawClock(play);

    if (stopped)
        Environment_StopTime();

    iface->bAlpha = oldAlpha;
    play->msgCtx.currentTextId = oldTextId;
    gSave.day = oldDay;
    gSave.time = oldTime;
    gSave.isNight = oldNight;
    R_TIME_SPEED = oldSpeed;
}

static void DtMessageDraw(PlayState* play)
{
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    MessageContext* msg = &play->msgCtx;
    Gfx* gfx;
    Gfx* head;
    s16 oldBoxY, oldBoxTarget, oldTextY, oldTextTarget, offset;
    int move = Config_Flag(CFG_MM_SONG_OF_DOUBLE_TIME_TIME_SELECTOR) && sDtState == DT_STATE_SELECT;

    if (move) {
        oldBoxY = msg->unk_1206a;
        oldBoxTarget = msg->unk_12006;
        oldTextY = msg->unk_11ff6;
        oldTextTarget = msg->unk_11ffa;
        offset = DT_SELECTOR_BOX_Y - msg->unk_12006;
        msg->unk_12006 += offset;
        msg->unk_1206a += offset;
        msg->unk_11ff6 += offset;
        msg->unk_11ffa += offset;
    }

    OPEN_DISPS(gfxCtx);

    head = POLY_OPA_DISP;
    gfx = Gfx_Open(head);
    gSPDisplayList(OVERLAY_DISP++, gfx);

    if (msg->currentTextId != 0x5e6 || !Player_InCsMode(play))
        Message_DrawMain(play, &gfx);

    gSPEndDisplayList(gfx++);
    Gfx_Close(head, gfx);
    POLY_OPA_DISP = gfx;

    CLOSE_DISPS();

    if (move) {
        msg->unk_1206a = oldBoxY;
        msg->unk_12006 = oldBoxTarget;
        msg->unk_11ff6 = oldTextY;
        msg->unk_11ffa = oldTextTarget;
    }

    DoubleTimeSelector_Draw(play);
}

PATCH_FUNC(0x80156758, DtMessageDraw);