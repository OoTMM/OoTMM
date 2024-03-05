#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

#if defined(GAME_OOT)
# define EN_ELF_INIT_VROM        0x808862f4
# define EN_ELF_UPDATE_VROM      0x8088957c
# define EN_ELF_DEFAULT_GI       GI_OOT_FAIRY
# define EN_ELF_BIG_GI           GI_OOT_FAIRY_BIG
# define EN_ELF_SFX_HEAL         0x20a8
# define EN_ELF_SFX_ICE_TRAP     0x31f1
#else
# define EN_ELF_INIT_VROM        0x8088cdac
# define EN_ELF_UPDATE_VROM      0x80890438
# define EN_ELF_DEFAULT_GI       GI_MM_FAIRY
# define EN_ELF_BIG_GI           GI_MM_FAIRY_BIG
# define EN_ELF_SFX_HEAL         0x20a8
# define EN_ELF_SFX_ICE_TRAP     0x31a4
#endif

void EnElf_Aliases(Actor_EnElf* this, GameState_Play* play)
{
    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

#if defined(GAME_OOT)
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
#endif
}

void EnElf_ItemQuery(ComboItemQuery* q, Actor_EnElf* this)
{
    comboXflagItemQuery(q, &this->xflag, this->extendedGi);
    q->giRenew = this->extendedGi;
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
    comboDrawGI(play, &this->base, this->extendedGiDraw, 0);
}

#if defined(GAME_OOT)
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
#endif

void EnElf_UpdateWrapper(Actor_EnElf* this, GameState_Play* play)
{
    ActorCallback update = actorAddr(AC_EN_ELF, EN_ELF_UPDATE_VROM);
    update(&this->base, play);

    if (Message_IsClosed(&this->base, play))
    {
        UnfreezePlayer(play);
        this->base.update = update;
    }
    else
    {
#if defined(GAME_OOT)
        this->unk_2B4 = 0.0f;
#else
        this->unk_250 = 0.0f;
#endif
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

    if (o.gi == this->extendedGi)
    {
        Health_ChangeBy(play, 0x80);
#if defined(GAME_MM)
        gSaveContext.save.jinxTimer = 0;
#else
        if (this->extendedGi == EN_ELF_BIG_GI) {
            Magic_Refill(play);
        }
#endif
        return;
    }

    this->itemGiven = 1;

    major = !isItemFastBuy(o.gi);
    if (major)
    {
#if defined(GAME_OOT)
        PlayerDisplayTextBox(play, 0xb4, NULL);
#else
        PlayerDisplayTextBox(play, 0x52, NULL);
#endif
        FreezePlayer(play);

        this->base.update = EnElf_UpdateWrapper;
    }

    comboAddItemEx(play, &q, major);
    comboXflagsSet(&this->xflag);

    /* Play the sound */
    comboPlayItemFanfare(o.gi, 1);
}

void EnElf_InitWrapper(Actor_EnElf* this, GameState_Play* play)
{
    ActorCallback init;

    /* Fairy Group Spawner */
    if (this->base.variable == 4) {
        EnElf_Aliases(this, play);
    }

    init = actorAddr(AC_EN_ELF, EN_ELF_INIT_VROM);
    init(&this->base, play);

    if (this->fairyFlags & 0x200)
    {
        this->extendedGi = EN_ELF_BIG_GI;
    }
    else
    {
        this->extendedGi = EN_ELF_DEFAULT_GI;
    }
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

    if (o.gi != fairy->extendedGi)
    {
        fairy->itemGiven = 0;
        fairy->extendedGiDraw = o.gi;
        fairy->base.draw = EnElf_Draw;
    }
}

void EnElf_PlayItemSfx(Actor_EnElf* this, GameState_Play* play)
{
    Actor_PlaySfx(&this->base, EN_ELF_SFX_HEAL);
}

#if defined(GAME_MM)
void Fairy_SetHealthAccumulator(Actor_EnElf* this, GameState_Play* play)
{
    /* Displaced code: */
    this->unk_246++;
    /* End displaced code */

    if (this->extendedGiDraw == 0 || this->extendedGiDraw == this->extendedGi)
    {
        gSaveContext.healthDelta = 0xA0;
    }
}
#endif
