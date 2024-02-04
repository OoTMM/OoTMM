#include <combo.h>
#include <combo/sr.h>
#include <combo/dungeon.h>

void ArrowCycle_Handle(Actor_Player* link, GameState_Play* play);

static void maskToggle(Actor_Player* player, u8 maskId)
{
    /* Set the mask */
    if (player->mask)
        player->mask = 0;
    else
        player->mask = maskId;

    /* Play a sfx */
    PlaySound(0x835);
}

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
        maskToggle(link, 9);
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
    u8 opacity;

    obj = comboGetObject(0x01dd | MASK_FOREIGN_OBJECT);
    if (!obj)
        return;
    if (gBlastMaskDelayAcc > 0x11)
        opacity = 0;
    else
        opacity = 0xff - (gBlastMaskDelayAcc * 0x0f);

    OPEN_DISPS(play->gs.gfx);
    gSPMatrix(POLY_OPA_DISP++, 0x0d0001c0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 0x09, kDListEmpty);
    gSPSegment(POLY_OPA_DISP++, 0x0a, obj);

    if (opacity)
    {
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, opacity);
        gSPDisplayList(POLY_OPA_DISP++, 0x0a0005c0);
    }
    else
    {
        gSPSegment(POLY_OPA_DISP++, 0x08, kDListEmpty);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0xff);
        gSPDisplayList(POLY_OPA_DISP++, 0x0a000440);
    }
    CLOSE_DISPS();
}

static void DrawExtendedMask(GameState_Play* play, Actor_Player* link)
{
    switch (link->mask)
    {
    case 9:
        DrawExtendedMaskBlast(play, link);
        break;
    }
}

static void DrawLinkWrapper(GameState_Play* play, void** skeleton, Vec3s* jointTable, s32 dListCount, s32 lod, s32 tunic, s32 boots, s32 face, void* overrideLimbDraw, void* postLimbDraw, void* data)
{
    Actor_Player* link;

    link = GET_LINK(play);
    DrawLink(play, skeleton, jointTable, dListCount, lod, tunic, boots, face, overrideLimbDraw, postLimbDraw, data);
    //if (data == (void*)0x8007aa94)
    if (link->mask >= 9)
        DrawExtendedMask(play, link);
}

PATCH_CALL(0x8084829c, DrawLinkWrapper);

void Player_UpdateWrapper(Actor_Player* this, GameState_Play* play)
{
    if (gBlastMaskDelayAcc)
        gBlastMaskDelayAcc--;

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

u16 gBlastMaskDelayAcc;

static u16 blastMaskDelay(void)
{
    if (comboConfig(CFG_BLAST_MASK_DELAY_INSTANT)) return 0x001;
    if (comboConfig(CFG_BLAST_MASK_DELAY_VERYSHORT)) return 0x020;
    if (comboConfig(CFG_BLAST_MASK_DELAY_SHORT)) return 0x80;
    if (comboConfig(CFG_BLAST_MASK_DELAY_LONG)) return 0x200;
    if (comboConfig(CFG_BLAST_MASK_DELAY_VERYLONG)) return 0x400;
    return 0x136;
}

static void Player_BlastMask(GameState_Play* play, Actor_Player* link)
{
    Actor* bomb;
    s16* bombTimer;

    if (gBlastMaskDelayAcc)
        return;
    bomb = SpawnActor(&play->actorCtx, play, AC_EN_BOM, link->base.world.pos.x, link->base.world.pos.y, link->base.world.pos.z, 0, 0, 0, 0);
    if (!bomb)
        return;
    bombTimer = (void*)((char*)bomb + 0x1e8);
    *bombTimer = 2;
    gBlastMaskDelayAcc = blastMaskDelay();
}

void Player_ProcessItemButtonsWrapper(Actor_Player* link, GameState_Play* play)
{
    void (*Player_ProcessItemButtons)(Actor_Player* link, GameState_Play* play);
    ControllerInput* input;
    int bPress;

    input = *(ControllerInput**)(OverlayAddr(0x80856734));
    Player_ProcessItemButtons = OverlayAddr(0x80831e64);
    bPress = !!(input->pressed.buttons & B_BUTTON);

    /* Handle masks that have B actions */
    if (bPress && !(link->state & (PLAYER_ACTOR_STATE_HOLD_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN)) && !Player_UsingItem(link))
    {
        switch (link->mask)
        {
        case 9:
            Player_BlastMask(play, link);
            input->pressed.buttons &= ~B_BUTTON;
            break;
        }
    }

    Player_ProcessItemButtons(link, play);
}
