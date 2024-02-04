#include <combo.h>
#include <combo/sr.h>
#include <combo/dungeon.h>

void ArrowCycle_Handle(Actor_Player* link, GameState_Play* play);

void comboPlayerUseItem(GameState_Play* play, Actor_Player* link, s16 itemId)
{
    void (*Player_UseItem)(GameState_Play* play, Actor_Player* link, s16 itemId);

    switch (itemId)
    {
    case ITEM_OOT_WEIRD_EGG:
        gComboTriggersData.events.weirdEgg = 1;
        break;
    case ITEM_OOT_POCKET_EGG:
        gComboTriggersData.events.pocketEgg = 1;
        break;
    case ITEM_OOT_MASK_BLAST:
        link->mask = 9;
        break;
    default:
        Player_UseItem = OverlayAddr(0x80834000);
        Player_UseItem(play, link, itemId);
        break;
    }
}

PATCH_CALL(0x8083212c, comboPlayerUseItem);

static void DrawExtendedMaskBlast(GameState_Play* play, Actor_Player* link)
{
    void* obj;

    obj = comboGetObject(0x01dd | MASK_FOREIGN_OBJECT);
    if (!obj)
        return;
    OPEN_DISPS(play->gs.gfx);
    gSPMatrix(POLY_OPA_DISP++, 0x0d0001c0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 0x09, kDListEmpty);
    gSPSegment(POLY_OPA_DISP++, 0x0a, obj);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0xff);
    gSPDisplayList(POLY_OPA_DISP++, 0x0a0005c0);
    CLOSE_DISPS();
}

static void DrawExtendedMask(GameState_Play* play, Actor_Player* link)
{
    DrawExtendedMaskBlast(play, link);
}

static void DrawLinkWrapper(GameState_Play* play, void** skeleton, Vec3s* jointTable, s32 dListCount, s32 lod, s32 tunic, s32 boots, s32 face, void* overrideLimbDraw, void* postLimbDraw, void* data)
{
    Actor_Player* link;

    link = GET_LINK(play);
    DrawLink(play, skeleton, jointTable, dListCount, lod, tunic, boots, face, overrideLimbDraw, postLimbDraw, data);
    //if (data == (void*)0x8007aa94 && link->mask >= 9)
    //if (link->mask >= 9)
        DrawExtendedMask(play, link);
}

PATCH_CALL(0x8084829c, DrawLinkWrapper);

void Player_UpdateWrapper(Actor_Player* this, GameState_Play* play)
{
    ArrowCycle_Handle(this, play);
    Player_Update(this, play);
    comboDpadUpdate(play);
    comboDpadUse(play, DPF_EQUIP);

    if (!(this->state & (PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_CLIMB2)))
    {
        if (g.delayedSwitchFlag != 0xff)
        {
            SetSwitchFlag(play, g.delayedSwitchFlag);
            g.delayedSwitchFlag = 0xff;
        }
    }

    comboSrUpdate(play);
    comboMultiProcessMessages(play);
}

int Player_DpadHook(Actor_Player* this, GameState_Play* play)
{
    if (Player_UsingItem(this))
        return 1;
    return comboDpadUse(play, DPF_ITEMS);
}

void EnGs_TalkedTo(Actor*, GameState_Play*);
void EnGm_TalkedTo(Actor*, GameState_Play*);
void EnMs_TalkedTo(Actor*, GameState_Play*);
void EnSsh_TalkedTo(Actor*, GameState_Play*);

void DemoEffect_TextRutoSapphire(GameState_Play*);

void Player_TalkDisplayTextBox(GameState_Play* play, s16 textId, Actor* actor)
{
    PlayerDisplayTextBox(play, textId, actor);
    if (actor)
    {
        switch (actor->id)
        {
        case AC_EN_GS:
            EnGs_TalkedTo(actor, play);
            break;
        case AC_EN_GM:
            EnGm_TalkedTo(actor, play);
            break;
        case AC_EN_MS:
            EnMs_TalkedTo(actor, play);
            break;
        case AC_EN_SSH:
            EnSsh_TalkedTo(actor, play);
            break;
        }
    }

    /* Ruto's Sapphire */
    if (textId == 0x4050)
        DemoEffect_TextRutoSapphire(play);
}

PATCH_CALL(0x80838464, Player_TalkDisplayTextBox);
PATCH_CALL(0x80055d50, Player_TalkDisplayTextBox);
