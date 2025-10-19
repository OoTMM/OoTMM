#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/global.h>
#include <combo/draw.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x13c) = (h); } while (0)

static u16 EnGSwitch_LocalID(Actor* this)
{
    return *(u16*)((char*)this + 0x148);
}

static u16 EnGSwitch_ID(Actor* this, PlayState* play)
{
    u16 base;
    u16 offset;

    switch (play->sceneId)
    {
    case SCE_OOT_DODONGO_CAVERN:
        base = 0x00;
        break;
    case SCE_OOT_BOTTOM_OF_THE_WELL:
        base = 0x05;
        break;
    case SCE_OOT_TEMPLE_SPIRIT:
        switch (this->room)
        {
        case 0x00:
        case 0x02:
            base = 0x0a;
            break;
        case 0x08:
        case 0x17:
            base = 0x0f;
            break;
        case 0x0d:
            base = 0x14;
            break;
        default:
            UNREACHABLE();
        }
        break;
    case SCE_OOT_TEMPLE_SHADOW:
        switch (this->room)
        {
        case 0x06:
            base = 0x19;
            break;
        case 0x10:
            base = 0x1e;
            break;
        case 0x09:
            base = 0x28;
            break;
        case 0x0b:
            base = 0x2d;
            break;
        default:
            UNREACHABLE();
        }
        break;
    case SCE_OOT_ICE_CAVERN:
        switch (this->room)
        {
        case 0x03:
            base = 0x37;
            break;
        case 0x05:
            base = 0x3c;
            break;
        default:
            UNREACHABLE();
        }
        break;
    case SCE_OOT_GERUDO_TRAINING_GROUND:
        switch (this->room)
        {
        case 0x02:
            base = 0x41;
            break;
        case 0x06:
            base = 0x46;
            break;
        case 0x09:
            base = 0x4c;
            break;
        default:
            UNREACHABLE();
        }
        break;
    case SCE_OOT_INSIDE_GANON_CASTLE:
        switch (this->room)
        {
        case 0x0c:
        case 0x11:
            base = 0x51;
            break;
        case 0x03:
        case 0x08:
            base = 0x56;
            break;
        case 0x0e:
            base = 0x5b;
            break;
        case 0x06:
            base = 0x60;
            break;
        default:
            UNREACHABLE();
        }
        break;
    default:
        UNREACHABLE();
    }
    offset = EnGSwitch_LocalID(this);

    return base + offset;
}

static void EnGSwitch_ItemQuery(ComboItemQuery* q, Actor* this, PlayState* play)
{
    bzero(q, sizeof(*q));
    q->ovType = OV_SR;
    q->id = EnGSwitch_ID(this, play);
}

static void EnGSwitch_ItemOverride(ComboItemOverride* o, Actor* this, PlayState* play)
{
    ComboItemQuery q;

    EnGSwitch_ItemQuery(&q, this, play);
    comboItemOverride(o, &q);
}

int EnGSwitch_AlreadyTaken(PlayState* play, Actor* this)
{
    u16 id;

    /* Compute and store the ID */
    id = g.silverRupee++;
    *(u16*)((char*)this + 0x148) = id;

    /* Check for flag */
    return BITMAP8_GET(gCustomSave.sr, EnGSwitch_ID(this, play));
}

void EnGSwitch_DrawSilverRupee(Actor* this, PlayState* play)
{
    static const float scale = 12.5f;
    ComboItemOverride o;
    s16 gi;

    EnGSwitch_ItemOverride(&o, this, play);
    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        Matrix_RotateYS(-this->shape.rot.y * 2, MTXMODE_APPLY);
    }
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Draw_Gi(play, this, gi, 0);
}

static const Gfx kPotDrawListNormalTop[] = {
    gsDPLoadTextureBlock(0x06001000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 4, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kPotDrawListNormalSide[] = {
    gsDPLoadTextureBlock(0x06000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

void EnGSwitch_DrawArcheryPot(Actor* this, PlayState* play)
{
    if ((*(u16*)((char*)this + 0x14a)) != 0)
        return;

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x0a, kPotDrawListNormalSide);
    gSPSegment(POLY_OPA_DISP++, 0x0b, kPotDrawListNormalTop);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, 0x060017c0);
    CLOSE_DISPS();
}

PATCH_FUNC(0x80a71680, EnGSwitch_DrawArcheryPot);

void EnGSwitch_HandlerAfterCollected(Actor* this, PlayState* play)
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

void EnGSwitch_GiveItemSilverRupee(Actor* this)
{
    ComboItemOverride o;
    ComboItemQuery q;

    EnGSwitch_ItemQuery(&q, this, gPlay);
    comboItemOverride(&o, &q);
    PlayerDisplayTextBox(gPlay, 0xb4, NULL);
    comboAddItemEx(gPlay, &q, 1);
    comboPlayItemFanfare(o.gi, 1);
    BITMAP8_SET(gCustomSave.sr, EnGSwitch_ID(this, gPlay));
    SET_HANDLER(this, EnGSwitch_HandlerAfterCollected);
}

