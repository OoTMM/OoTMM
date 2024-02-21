#include <combo.h>
#include <combo/sr.h>
#include <combo/dungeon.h>
#include <combo/custom.h>

void ArrowCycle_Handle(Actor_Player* link, GameState_Play* play);
void Ocarina_HandleCustomSongs(Actor_Player* link, GameState_Play* play);

void* Player_AllocObjectBuffer(u32 size)
{
    static void* sBuffer;

    if (!sBuffer)
        sBuffer = malloc(size);
    return sBuffer;
}

static void maskToggle(GameState_Play* play, Actor_Player* player, u8 maskId)
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
    u8 prevMask;

    prevMask = link->mask;
    switch (itemId)
    {
    case ITEM_OOT_WEIRD_EGG:
        gComboTriggersData.events.weirdEgg = 1;
        break;
    case ITEM_OOT_POCKET_EGG:
        gComboTriggersData.events.pocketEgg = 1;
        break;
    case ITEM_OOT_MASK_BLAST:
        maskToggle(play, link, PLAYER_MASK_BLAST);
        break;
    case ITEM_OOT_MASK_STONE:
        maskToggle(play, link, PLAYER_MASK_STONE);
        break;
    default:
        Player_UseItem = OverlayAddr(0x80834000);
        Player_UseItem(play, link, itemId);
        break;
    }

    if (prevMask != link->mask)
        Interface_LoadItemIconImpl(play, 0);
}

PATCH_CALL(0x8083212c, comboPlayerUseItem);

static int prepareMask(GameState_Play* play, u16 objectId, int needsMatrix)
{
    void* obj;

    obj = comboGetObject(objectId);
    if (!obj)
        return 0;

    OPEN_DISPS(play->gs.gfx);
    if (needsMatrix)
        gSPMatrix(POLY_OPA_DISP++, 0x0d0001c0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 0x0a, obj);
    CLOSE_DISPS();

    return 1;
}

static void DrawExtendedMaskKeaton(GameState_Play* play, Actor_Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_KEATON, 1))
        return;
    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_KEATON_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskSkull(GameState_Play* play, Actor_Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_SKULL, 1))
        return;
    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_SKULL_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskSpooky(GameState_Play* play, Actor_Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_SPOOKY, 1))
        return;
    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_SPOOKY_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskBunny(GameState_Play* play, Actor_Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_BUNNY, 0))
        return;

    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_BUNNY_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskGoron(GameState_Play* play, Actor_Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_GORON, 1))
        return;
    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_GORON_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskZora(GameState_Play* play, Actor_Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_ZORA, 1))
        return;
    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_ZORA_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskGerudo(GameState_Play* play, Actor_Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_GERUDO, 1))
        return;
    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_GERUDO_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskTruth(GameState_Play* play, Actor_Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_TRUTH, 1))
        return;
    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_TRUTH_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskBlast(GameState_Play* play, Actor_Player* link)
{
    u8 opacity;

    if (!prepareMask(play, 0x01dd | MASK_FOREIGN_OBJECT, 1))
        return;
    if (gBlastMaskDelayAcc > 0x11)
        opacity = 0;
    else
        opacity = 0xff - (gBlastMaskDelayAcc * 0x0f);

    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x09, kDListEmpty);

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

static void DrawExtendedMaskStone(GameState_Play* play, Actor_Player* link)
{
    if (!prepareMask(play, 0x24e | MASK_FOREIGN_OBJECT, 1))
        return;

    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, 0x0a000820);
    CLOSE_DISPS();
}

typedef void (*MaskCallback)(GameState_Play*, Actor_Player*);

static const MaskCallback kMaskCallbacks[] = {
    DrawExtendedMaskKeaton,
    DrawExtendedMaskSkull,
    DrawExtendedMaskSpooky,
    DrawExtendedMaskBunny,
    DrawExtendedMaskGoron,
    DrawExtendedMaskZora,
    DrawExtendedMaskGerudo,
    DrawExtendedMaskTruth,
    DrawExtendedMaskBlast,
    DrawExtendedMaskStone,
};

void comboDrawExtendedMask(void)
{
    GameState_Play* play;
    Actor_Player* link;
    MaskCallback cb;
    int index;

    play = gPlay;
    link = GET_LINK(play);

    if (link->mask == 0)
        return;
    index = link->mask - 1;
    if (index >= ARRAY_SIZE(kMaskCallbacks))
        return;
    cb = kMaskCallbacks[index];
    if (!cb)
        return;
    cb(play, link);
}

void Player_UpdateWrapper(Actor_Player* this, GameState_Play* play)
{
    if (gBlastMaskDelayAcc)
    {
        gBlastMaskDelayAcc--;
        if (!gBlastMaskDelayAcc)
            Interface_LoadItemIconImpl(play, 0);
    }

    ArrowCycle_Handle(this, play);
    Player_Update(this, play);
    Ocarina_HandleCustomSongs(this, play);
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
    bomb = SpawnActor(&play->actorCtx, play, AC_EN_BOM, link->base.focus.pos.x, link->base.focus.pos.y, link->base.focus.pos.z, 0, 0, 0, 0);
    if (!bomb)
        return;
    bombTimer = (void*)((char*)bomb + 0x1e8);
    *bombTimer = 2;
    gBlastMaskDelayAcc = blastMaskDelay();
    Interface_LoadItemIconImpl(play, 0);
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

void Player_DrawDekuStick(void)
{
    GameState_Play* play;
    void* obj;

    play = gPlay;
    obj = comboGetObject(CUSTOM_OBJECT_ID_EQ_DEKU_STICK);
    if (!obj)
        return;

    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x0a, obj);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_EQ_DEKU_STICK_0);
    CLOSE_DISPS();
}

void Player_AfterSetEquipmentData(GameState_Play* play)
{
    Actor_Player* player = GET_LINK(play);
    if (player->rightHandType != 0xd && // PLAYER_MODELTYPE_RH_OCARINA
        player->rightHandType != 0xe && // PLAYER_MODELTYPE_RH_OOT
        (player->heldItemAction == 0x1c || // PLAYER_IA_OCARINA_FAIRY
        player->heldItemAction == 0x1d)) // PLAYER_IA_OCARINA_OF_TIME
    {
        Player_SetModels(player, Player_ActionToModelGroup(player, player->heldItemAction));
    }
}
