#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x144) = (h); } while (0)

void EnSi_WaitForPlayerToCloseMessage(Actor* this, GameState_Play* play)
{
    if (Message_IsClosed(this, play))
    {
        UnfreezePlayer(play);
        ActorDestroy(this);
    }
    else
    {
        FreezePlayer(play);
    }
}

static void EnSi_ItemQuery(ComboItemQuery* q, Actor* this, GameState_Play* play)
{
    bzero(q, sizeof(*q));
    q->ovType = OV_CHEST;
    q->sceneId = play->sceneId;
    q->id = (this->variable & 0xfc) >> 2;
    q->gi = play->sceneId == SCE_MM_SPIDER_HOUSE_OCEAN ? GI_MM_GS_TOKEN_OCEAN : GI_MM_GS_TOKEN_SWAMP;
}

static void EnSi_ItemOverride(ComboItemOverride* o, Actor* this, GameState_Play* play)
{
    ComboItemQuery q;

    EnSi_ItemQuery(&q, this, play);
    comboItemOverride(o, &q);
}

void EnSi_AddItem(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;

    EnSi_ItemQuery(&q, this, play);
    SetChestFlag(play, (this->variable & 0xfc) >> 2);
    PlayerDisplayTextBox(play, 0x52, NULL);
    comboAddItemEx(play, &q);
    PlaySoundSpecial(0x39);
    FreezePlayer(play);
    SET_HANDLER(this, EnSi_WaitForPlayerToCloseMessage);
    this->draw = NULL;
}

PATCH_FUNC(0x8098cad0, EnSi_AddItem);

void EnSi_Draw(Actor* this, GameState_Play* play)
{
    ComboItemOverride o;

    EnSi_ItemOverride(&o, this, play);
    comboDrawGI(play, this, o.gi, 0);
}

PATCH_FUNC(0x8098cd0c, EnSi_Draw);
