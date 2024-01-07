#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

void EnElf_Aliases(Actor_EnElf* this, GameState_Play* play)
{
    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->actor.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    switch (this->xflag.sceneId)
    {
    case SCE_OOT_FAIRY_FOUNTAIN:
        u8 fairyFountainIndex = 0xFF;
        switch (gLastScene) {
        case SCE_OOT_HYRULE_FIELD:
            fairyFountainIndex = 0;
            break;
        case SCE_OOT_ZORA_RIVER:
            fairyFountainIndex = 1;
            break;
        case SCE_OOT_SACRED_FOREST_MEADOW:
            fairyFountainIndex = 2;
            break;
        case SCE_OOT_ZORA_DOMAIN:
            fairyFountainIndex = 3;
            break;
        case SCE_OOT_GERUDO_FORTRESS:
            fairyFountainIndex = 4;
            break;
        }
        this->xflag.roomId = 0x20 | fairyFountainIndex;
        break;
    case SCE_OOT_TOMB_FAIRY:
        break;
    case SCE_OOT_DESERT_COLOSSUS:
        this->xflag.id = 1;
        this->xflag.setupId = 0;
        break;
    case SCE_OOT_GANON_TOWER:
        break;
    }
}

static void EnElf_ItemQuery(ComboItemQuery* q, Actor_EnElf* this)
{
    comboXflagItemQuery(q, &this->xflag, GI_OOT_FAIRY);
    q->giRenew = GI_OOT_FAIRY;
    if (comboXflagsGet(&this->xflag)) {
        q->ovFlags = OVF_RENEW;
    }
}

void EnElf_Draw(Actor_EnElf* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;

    if (!this->itemGiven) {
        EnElf_ItemQuery(&q, this);
        comboItemOverride(&o, &q);

        this->extendedGiDraw = o.gi;
    }

    static const float scale = 25.0f;

    ModelViewScale(scale, scale, scale, MAT_MUL);
    comboDrawGI(play, &this->actor, this->extendedGiDraw, 0);
}

s32 EnElf_CantGiveItem(GameState_Play* play) {
    Actor_Player* link;
    link = GET_LINK(play);

    if (link->state & (PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_EPONA))
        return 1;

    /* Check for textbox */
    if (Message_GetState(&play->msgCtx) != 0)
        return 1;

    return Player_InCsMode(play);
}

void EnElf_UpdateWrapper(Actor_EnElf* this, GameState_Play* play)
{
    ActorCallback update = actorAddr(AC_EN_ELF, 0x8088957c);
    update(&this->actor, play);

    if (Message_IsClosed(&this->actor, play))
    {
        UnfreezePlayer(play);
        this->actor.update = update;
    }
    else
    {
        this->unk_2B4 = 0.0f;
        FreezePlayer(play);
    }
}

void EnElf_GiveItem(Actor_EnElf* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
    int major;

    /* Query the item */
    EnElf_ItemQuery(&q, this);
    comboItemOverride(&o, &q);

    if (o.gi == GI_OOT_FAIRY)
    {
        Health_ChangeBy(play, 0x80);
        return;
    }

    this->itemGiven = 1;

    major = !isItemFastBuy(o.gi);
    if (major)
    {
        PlayerDisplayTextBox(play, 0xb4, NULL);
        FreezePlayer(play);

        this->actor.update = EnElf_UpdateWrapper;
    }

    comboAddItemEx(play, &q, major);
    comboXflagsSet(&this->xflag);

    /* Play the sound */
    PlaySound(0x4824);
}

void EnElf_InitWrapper(Actor_EnElf* this, GameState_Play* play)
{
    ActorCallback init;

    // Fairy Group Spawner
    if (this->actor.variable == 4) {
        EnElf_Aliases(this, play);
    }

    init = actorAddr(AC_EN_ELF, 0x808862f4);
    init(&this->actor, play);
}

void EnElf_SpawnFairyGroupMember(Actor_EnElf* spawner, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, u8 count)
{
    ComboItemQuery q;
    ComboItemOverride o;
    Actor_EnElf* fairy = (Actor_EnElf*)SpawnActor(&play->actorCtx, play, actorId, x, y, z, rx, ry, rz, variable);

    if (!fairy)
    {
        return;
    }

    /* Copy the extended flag */
    memcpy(&fairy->xflag, &spawner->xflag, sizeof(Xflag));
    fairy->xflag.sliceId = count;

    /* Query the item */
    EnElf_ItemQuery(&q, fairy);
    comboItemOverride(&o, &q);

    if (o.gi != GI_OOT_FAIRY)
    {
        fairy->itemGiven = 0;
        fairy->extendedGiDraw = o.gi;
        fairy->actor.draw = EnElf_Draw;
    }
}
