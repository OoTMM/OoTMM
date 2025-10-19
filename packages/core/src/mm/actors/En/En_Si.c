#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/draw.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x144) = (h); } while (0)

void EnSi_WaitForPlayerToCloseMessage(Actor* this, PlayState* play)
{
    if (Message_IsClosed(this, play))
    {
        Player_Unfreeze(play);
        Actor_Kill(this);
    }
    else
    {
        Player_Freeze(play);
    }
}

static void EnSi_ItemQuery(ComboItemQuery* q, Actor* this, PlayState* play)
{
    bzero(q, sizeof(*q));
    q->ovType = OV_CHEST;
    q->sceneId = comboSceneKey(play->sceneId);
    q->id = (this->params & 0xfc) >> 2;
    q->gi = play->sceneId == SCE_MM_SPIDER_HOUSE_OCEAN ? GI_MM_GS_TOKEN_OCEAN : GI_MM_GS_TOKEN_SWAMP;
}

void EnSi_ItemOverride(ComboItemOverride* o, Actor* this, PlayState* play)
{
    ComboItemQuery q;

    EnSi_ItemQuery(&q, this, play);
    comboItemOverride(o, &q);
}

void EnSi_AddItem(Actor* this, PlayState* play)
{
    ComboItemOverride o;
    ComboItemQuery q;

    EnSi_ItemQuery(&q, this, play);
    comboItemOverride(&o, &q);
    SetChestFlag(play, (this->params & 0xfc) >> 2);
    PlayerDisplayTextBox(play, 0x52, NULL);
    comboAddItemEx(play, &q, 1);
    comboPlayItemFanfare(o.gi, 1);
    Player_Freeze(play);
    SET_HANDLER(this, EnSi_WaitForPlayerToCloseMessage);
    this->draw = NULL;
}

PATCH_FUNC(0x8098cad0, EnSi_AddItem);

void EnSi_Draw(Actor* this, PlayState* play)
{
    ComboItemOverride o;
    s16 gi;

    EnSi_ItemOverride(&o, this, play);
    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        Matrix_RotateYS(-this->shape.rot.y * 2, MTXMODE_APPLY);
    }
    Draw_Gi(play, this, gi, 0);
}

PATCH_FUNC(0x8098cd0c, EnSi_Draw);
