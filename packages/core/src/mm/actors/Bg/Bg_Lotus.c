#include <combo.h>
#include <combo/player.h>

s32 BgLotus_ShouldSink(Actor* this, GameState_Play* play)
{
    Actor_Player* player = GET_PLAYER(play);
    return gSaveContext.save.playerForm != MM_PLAYER_FORM_DEKU && GET_PLAYER_CUSTOM_BOOTS(player) != PLAYER_BOOTS_HOVER;
}
