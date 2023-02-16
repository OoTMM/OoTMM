#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x144) = (h); } while (0)

// maybe worth moving somewhere common
static void FreezePlayer(GameState_Play* play) {
    Actor_Player* link;
    link = GET_LINK(play);
    link->base.freezeTimer = 100;
    link->state |= PLAYER_ACTOR_STATE_FROZEN;
}

// maybe worth moving somewhere common
static void UnfreezePlayer(GameState_Play* play) {
    Actor_Player* link;
    link = GET_LINK(play);
    link->base.freezeTimer = 0;
    link->state &= ~PLAYER_ACTOR_STATE_FROZEN;
}

void EnSi_WaitForPlayerToCloseMessage(Actor* this, GameState_Play* play) {
    if (Message_IsClosed(this, play)) {
        UnfreezePlayer(play);
        ActorDestroy(this);
    } else {
        FreezePlayer(play);
    }
}

static s16 EnSi_GetOverride(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (play->sceneId == SCE_MM_SPIDER_HOUSE_OCEAN)
        gi = GI_MM_GS_TOKEN_OCEAN;
    else
        gi = GI_MM_GS_TOKEN_SWAMP;
    return comboOverride(OV_CHEST, play->sceneId, (this->variable & 0xfc) >> 2, gi);
}

void EnSi_AddItem(Actor* this, GameState_Play* play)
{
    s16 gi;

    gi = EnSi_GetOverride(this, play);
    SetChestFlag(play, (this->variable & 0xfc) >> 2);
    PlayerDisplayTextBox(play, 0x52, NULL);
    comboAddItem(play, gi);
    PlaySoundSpecial(0x39);
    FreezePlayer(play);
    SET_HANDLER(this, EnSi_WaitForPlayerToCloseMessage);
    this->draw = NULL;
}

PATCH_FUNC(0x8098cad0, EnSi_AddItem);

void EnSi_Draw(Actor* this, GameState_Play* play)
{
    s16 gi;

    gi = EnSi_GetOverride(this, play);
    comboDrawGI(play, this, gi, 0);
}

PATCH_FUNC(0x8098cd0c, EnSi_Draw);
