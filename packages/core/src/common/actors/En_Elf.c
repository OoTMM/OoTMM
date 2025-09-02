#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/magic.h>
#include <combo/global.h>
#include <combo/draw.h>
#include <combo/actor.h>

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

void EnElf_Aliases(Xflag* xf)
{
#if defined(GAME_OOT)
    u8 fairyFountainIndex;

    switch (xf->sceneId)
    {
    case SCE_OOT_FAIRY_FOUNTAIN:
        switch (gLastScene)
        {
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
        default:
            UNREACHABLE();
        }
        xf->roomId = 0x20 | fairyFountainIndex;
        break;
    case SCE_OOT_DESERT_COLOSSUS:
        xf->id = 1;
        xf->setupId = 0;
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

static void EnElf_ItemOverride(ComboItemOverride* o, Actor_EnElf* this)
{
    ComboItemQuery q;

    EnElf_ItemQuery(&q, this);
    comboItemOverride(o, &q);
}

static s16 EnElf_ItemGi(Actor_EnElf* this)
{
    ComboItemOverride o;
    EnElf_ItemOverride(&o, this);
    return o.gi;
}

void EnElf_Draw(Actor_EnElf* this, PlayState* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
    s16 cloakGi;

    if (!this->itemGiven) {
        EnElf_ItemQuery(&q, this);
        comboItemOverride(&o, &q);

        this->extendedGiDraw = o.gi;
        cloakGi = o.cloakGi;
    }
    else
        cloakGi = GI_NONE;

    static const float scale = 25.0f;

    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Draw_GiCloaked(play, &this->base, this->extendedGiDraw, cloakGi, 0);
}

#if defined(GAME_OOT)
s32 EnElf_CantGiveItem(PlayState* play) {
    Player* link;
    link = GET_PLAYER(play);

    if (link->stateFlags1 & (PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_EPONA))
        return 1;

    /* Check for textbox */
    if (Message_GetState(&play->msgCtx) != 0)
        return 1;

    return Player_InCsMode(play);
}
#endif

void EnElf_UpdateWrapper(Actor_EnElf* this, PlayState* play)
{
    ActorCallback update = actorAddr(ACTOR_EN_ELF, EN_ELF_UPDATE_VROM);
    update(&this->base, play);

    if (Message_IsClosed(&this->base, play))
    {
        Player_Unfreeze(play);
        this->base.update = update;
    }
    else
    {
#if defined(GAME_OOT)
        this->unk_2B4 = 0.0f;
#else
        this->unk_250 = 0.0f;
#endif
        Player_Freeze(play);
    }
}

void EnElf_GiveItem(Actor_EnElf* this, PlayState* play)
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
        Player_Freeze(play);

        this->base.update = EnElf_UpdateWrapper;
    }

    comboAddItemEx(play, &q, major);
    comboXflagsSet(&this->xflag);

    /* Play the sound */
    comboPlayItemFanfare(o.gi, 1);
}

static int EnElf_IsShuffled(Actor_EnElf* this, PlayState* play)
{
    ComboItemQuery q;
    ComboItemOverride o;

    comboXflagItemQuery(&q, &this->xflag, 0);
    if (comboXflagsGet(&this->xflag))
        q.ovFlags |= OVF_RENEW;
    comboItemOverride(&o, &q);
    return o.gi != GI_NONE;
}

void EnElf_InitWrapper(Actor_EnElf* this, PlayState* play)
{
    int type;
    ActorCallback init;

    type = this->base.params & 0xf;
    memset(&this->xflag, 0, sizeof(Xflag));
    if (type < 2)
    {
        Xflag_Clear(&this->xflag);
    }
    else
    {
        if (comboXflagInit(&this->xflag, &this->base, play))
            EnElf_Aliases(&this->xflag);
    }

    init = actorAddr(ACTOR_EN_ELF, EN_ELF_INIT_VROM);
    init(&this->base, play);

    if (this->fairyFlags & 0x200)
    {
        this->extendedGi = EN_ELF_BIG_GI;
    }
    else
    {
        this->extendedGi = EN_ELF_DEFAULT_GI;
    }

    /* Fairy Group Spawner */
    if (type >= 2 && type != 4)
    {
        if (EnElf_IsShuffled(this, play))
        {
            this->itemGiven = 0;
            this->extendedGiDraw = EnElf_ItemGi(this);
            this->base.draw = EnElf_Draw;
        }
    }
}

void EnElf_SpawnFairyGroupMember(Actor_EnElf* spawner, PlayState* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, u8 count)
{
    memcpy(&g.xflag, &spawner->xflag, sizeof(Xflag));
    g.xflag.sliceId = count;
    g.xflagOverride = TRUE;
    Actor_Spawn(&play->actorCtx, play, actorId, x, y, z, rx, ry, rz, variable);
    g.xflagOverride = FALSE;
}

void EnElf_PlayItemSfx(Actor_EnElf* this, PlayState* play)
{
    Actor_PlaySfx(&this->base, EN_ELF_SFX_HEAL);
}

#if defined(GAME_MM)
void Fairy_SetHealthAccumulator(Actor_EnElf* this, PlayState* play)
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
