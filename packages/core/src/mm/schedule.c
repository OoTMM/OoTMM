#include <combo.h>

u8 gScheduleXitemFlags;

int Schedule_CheckMiscSWrapper(GameState_Play* play, void* unk)
{
    gScheduleXitemFlags = 0;
    if (gMmExtraTrade.trade2 & (1 << XITEM_MM_TRADE2_ROOM_KEY))
        gScheduleXitemFlags |= 1;
    if (gMmExtraTrade.trade3 & (1 << XITEM_MM_TRADE3_LETTER_TO_KAFEI))
        gScheduleXitemFlags |= 2;

    return Schedule_CheckMiscS(play, unk);
}
