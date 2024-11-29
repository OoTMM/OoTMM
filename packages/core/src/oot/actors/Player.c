#include <combo.h>
#include <combo/sr.h>
#include <combo/dungeon.h>
#include <combo/custom.h>
#include <combo/player.h>
#include <combo/mask.h>
#include <combo/config.h>
#include <combo/global.h>
#include <combo/dpad.h>
#include <combo/multi.h>
#include <combo/actor.h>
#include <combo/hint.h>
#include <combo/entrance.h>
#include <actors/En_Kanban/En_Kanban.h>

void ArrowCycle_Handle(Player* link, PlayState* play);
void Ocarina_HandleCustomSongs(Player* link, PlayState* play);

void* Player_AllocObjectBuffer(u32 size)
{
    static void* sBuffer;

    if (!sBuffer)
        sBuffer = malloc(size);
    return sBuffer;
}

static void maskToggle(PlayState* play, Player* player, u8 maskId)
{
    /* Set the mask */
    if (player->mask)
        player->mask = 0;
    else
        player->mask = maskId;

    /* Play a sfx */
    PlaySound(0x835);
}

static void Player_UseBoots(PlayState* play, Player* this, int bootsId)
{
    if (gSave.info.equips.equipment.boots == bootsId)
        gSave.info.equips.equipment.boots = 1;
    else
        gSave.info.equips.equipment.boots = bootsId;
    UpdateEquipment(play, GET_PLAYER(play));
    PlaySound(0x835);
}

void Player_UseItem(PlayState* play, Player* link, s16 itemId)
{
    void (*Player_UseItemImpl)(PlayState* play, Player* link, s16 itemId);
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
        maskToggle(play, link, MASK_BLAST);
        break;
    case ITEM_OOT_MASK_STONE:
        maskToggle(play, link, MASK_STONE);
        break;
    case ITEM_OOT_BOOTS_IRON:
        Player_UseBoots(play, link, 2);
        break;
    case ITEM_OOT_BOOTS_HOVER:
        Player_UseBoots(play, link, 3);
        break;
    default:
        Player_UseItemImpl = OverlayAddr(0x80834000);
        Player_UseItemImpl(play, link, itemId);
        break;
    }

    if (prevMask != link->mask)
        Interface_LoadItemIconImpl(play, 0);
}

PATCH_CALL(0x8083212c, Player_UseItem);

static int prepareMask(PlayState* play, u16 objectId, int needsMatrix)
{
    void* obj;

    obj = comboGetObject(objectId);
    if (!obj)
        return 0;

    OPEN_DISPS(play->state.gfxCtx);
    if (needsMatrix)
        gSPMatrix(POLY_OPA_DISP++, 0x0d0001c0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 0x0a, obj);
    CLOSE_DISPS();

    return 1;
}

static void DrawExtendedMaskKeaton(PlayState* play, Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_KEATON, 1))
        return;
    OPEN_DISPS(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_KEATON_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskSkull(PlayState* play, Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_SKULL, 1))
        return;
    OPEN_DISPS(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_SKULL_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskSpooky(PlayState* play, Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_SPOOKY, 1))
        return;
    OPEN_DISPS(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_SPOOKY_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskBunny(PlayState* play, Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_BUNNY, 0))
        return;

    OPEN_DISPS(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_BUNNY_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskGoron(PlayState* play, Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_GORON, 1))
        return;
    OPEN_DISPS(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_GORON_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskZora(PlayState* play, Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_ZORA, 1))
        return;
    OPEN_DISPS(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_ZORA_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskGerudo(PlayState* play, Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_GERUDO, 1))
        return;
    OPEN_DISPS(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_GERUDO_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskTruth(PlayState* play, Player* link)
{
    if (!prepareMask(play, CUSTOM_OBJECT_ID_MASK_OOT_TRUTH, 1))
        return;
    OPEN_DISPS(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_MASK_OOT_TRUTH_0);
    CLOSE_DISPS();
}

static void DrawExtendedMaskBlast(PlayState* play, Player* link)
{
    u8 opacity;

    if (!prepareMask(play, 0x01dd | MASK_FOREIGN_OBJECT, 1))
        return;
    if (gBlastMaskDelayAcc > 0x11)
        opacity = 0;
    else
        opacity = 0xff - (gBlastMaskDelayAcc * 0x0f);

    OPEN_DISPS(play->state.gfxCtx);
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

static void DrawExtendedMaskStone(PlayState* play, Player* link)
{
    if (!prepareMask(play, 0x24e | MASK_FOREIGN_OBJECT, 1))
        return;

    OPEN_DISPS(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, 0x0a000820);
    CLOSE_DISPS();
}

typedef void (*MaskCallback)(PlayState*, Player*);

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
    PlayState* play;
    Player* link;
    MaskCallback cb;
    int index;

    play = gPlay;
    link = GET_PLAYER(play);

    if (link->mask == 0)
        return;
    index = link->mask - 1;
    if (index >= ARRAY_COUNT(kMaskCallbacks))
        return;
    cb = kMaskCallbacks[index];
    if (!cb)
        return;
    cb(play, link);
}

static void updateKokiriSwordLength(void)
{
    float* dst;

    dst = (float*)0x800f7bb8;

    switch (gSharedCustomSave.extraSwordsOot)
    {
    case 0: *dst = 3000.f; break;
    case 1: *dst = 3000.f; break;
    case 2: *dst = 4000.f; break;
    }
}

void Player_UpdateWrapper(Player* this, PlayState* play)
{
    updateKokiriSwordLength();

    if (gBlastMaskDelayAcc)
    {
        gBlastMaskDelayAcc--;
        if (!gBlastMaskDelayAcc)
            Interface_LoadItemIconImpl(play, 0);
    }

    ArrowCycle_Handle(this, play);
    Player_Update(this, play);
    Ocarina_HandleCustomSongs(this, play);
    Dpad_Update(play);
    Dpad_Use(play, DPF_EQUIP);

    if (!(this->stateFlags1 & (PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_CLIMB2)) || Message_GetState(&play->msgCtx) == TEXT_STATE_NONE)
    {
        if (g.delayedSwitchFlag != 0xff)
        {
            Flags_SetSwitch(play, g.delayedSwitchFlag);
            g.delayedSwitchFlag = 0xff;
        }
    }

    comboSrUpdate(play);
}

int Player_DpadHook(Player* this, PlayState* play)
{
    if (Player_UsingItem(this))
        return 1;
    return Dpad_Use(play, DPF_ITEMS);
}

void EnGs_TalkedTo(Actor*, PlayState*);
void EnGm_TalkedTo(Actor*, PlayState*);
void EnMs_TalkedTo(Actor*, PlayState*);
void EnSsh_TalkedTo(Actor*, PlayState*);

void DemoEffect_TextRutoSapphire(PlayState*);

void (*gEnKanban_TalkedTo)(Actor*, PlayState*);

void Player_TalkDisplayTextBox(PlayState* play, s16 textId, Actor* actor)
{
    PlayerDisplayTextBox(play, textId, actor);
    if (actor)
    {
        switch (actor->id)
        {
        case ACTOR_EN_GS:
            EnGs_TalkedTo(actor, play);
            break;
        case ACTOR_EN_GM:
            EnGm_TalkedTo(actor, play);
            break;
        case ACTOR_EN_MS:
            EnMs_TalkedTo(actor, play);
            break;
        case ACTOR_EN_SSH:
            EnSsh_TalkedTo(actor, play);
            break;
        case ACTOR_EN_KANBAN:
            gEnKanban_TalkedTo(actor, play);
            break;
        }
    }

    /* Ruto's Sapphire */
    if (textId == 0x4050)
        DemoEffect_TextRutoSapphire(play);

    /* Navi */
    if (textId == 0x0141)
        Hint_DisplayJunkRandom(play);
}

PATCH_CALL(0x80838464, Player_TalkDisplayTextBox);
PATCH_CALL(0x80055d50, Player_TalkDisplayTextBox);

u16 gBlastMaskDelayAcc;

static u16 blastMaskDelay(void)
{
    if (Config_Flag(CFG_BLAST_MASK_DELAY_INSTANT)) return 0x001;
    if (Config_Flag(CFG_BLAST_MASK_DELAY_VERYSHORT)) return 0x020;
    if (Config_Flag(CFG_BLAST_MASK_DELAY_SHORT)) return 0x80;
    if (Config_Flag(CFG_BLAST_MASK_DELAY_LONG)) return 0x200;
    if (Config_Flag(CFG_BLAST_MASK_DELAY_VERYLONG)) return 0x400;
    return 0x136;
}

static void Player_BlastMask(PlayState* play, Player* link)
{
    Actor* bomb;
    s16* bombTimer;

    if (gBlastMaskDelayAcc)
        return;
    bomb = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, link->actor.focus.pos.x, link->actor.focus.pos.y, link->actor.focus.pos.z, 0, 0, 0, 0);
    if (!bomb)
        return;
    bombTimer = (void*)((char*)bomb + 0x1e8);
    *bombTimer = 2;
    gBlastMaskDelayAcc = blastMaskDelay();
    Interface_LoadItemIconImpl(play, 0);
}

void Player_ProcessItemButtonsWrapper(Player* link, PlayState* play)
{
    void (*Player_ProcessItemButtons)(Player* link, PlayState* play);
    Input* input;
    int bPress;

    input = *(Input**)(OverlayAddr(0x80856734));
    Player_ProcessItemButtons = OverlayAddr(0x80831e64);
    bPress = !!(input->press.button & B_BUTTON);

    /* Handle masks that have B actions */
    if (bPress && !(link->stateFlags1 & (PLAYER_ACTOR_STATE_HOLD_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN)) && !Player_UsingItem(link))
    {
        switch (link->mask)
        {
        case 9:
            Player_BlastMask(play, link);
            input->press.button &= ~B_BUTTON;
            break;
        }
    }

    Player_ProcessItemButtons(link, play);
}

void Player_DrawDekuStick(void)
{
    PlayState* play;
    void* obj;

    play = gPlay;
    obj = comboGetObject(CUSTOM_OBJECT_ID_EQ_DEKU_STICK);
    if (!obj)
        return;

    OPEN_DISPS(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x0a, obj);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_EQ_DEKU_STICK_0);
    CLOSE_DISPS();
}

void Player_AfterSetEquipmentData(PlayState* play)
{
    Player* player = GET_PLAYER(play);
    if (player->rightHandType != 0xd && /* PLAYER_MODELTYPE_RH_OCARINA */
        player->rightHandType != 0xe && /* PLAYER_MODELTYPE_RH_OOT */
        (player->heldItemAction == 0x1c || /* PLAYER_IA_OCARINA_FAIRY */
        player->heldItemAction == 0x1d)) /* PLAYER_IA_OCARINA_OF_TIME */
    {
        Player_SetModels(player, Player_ActionToModelGroup(player, player->heldItemAction));
    }
}

#define DLIST_INDIRECT(x)           (*(u32*)((x)))
#define DLIST_CHILD_LHAND_CLOSED    DLIST_INDIRECT(0x800f78ec)
#define DLIST_CHILD_RHAND_CLOSED    DLIST_INDIRECT(0x800f77c4)
#define DLIST_ADULT_LHAND_CLOSED    DLIST_INDIRECT(0x800f78e8)
#define DLIST_ADULT_RHAND_CLOSED    DLIST_INDIRECT(0x800f77b8)
#define DLIST_ADULT_RHAND_OPEN      DLIST_INDIRECT(0x800f7918)
#define DLIST_CHILD_STRING          DLIST_INDIRECT(0x800f7c14)
#define DLIST_ADULT_STRING          DLIST_INDIRECT(0x800f7c04)

static void* Player_CustomHandEq(u32 handDlist, void* eqData, u32 eqDlist)
{
    Gfx* dlist;
    Gfx* d;

    if (!eqData) return (void*)kDListEmpty;

    d = dlist = GRAPH_ALLOC(gPlay->state.gfxCtx, sizeof(Gfx) * 3);
    gSPDisplayList(d++, handDlist);
    gSPSegment(d++, 0x0a, eqData);
    gSPBranchList(d++, eqDlist);

    return dlist;
}

const Gfx* gDlistHookshotEnd;
const Gfx* gDlistHookshotChain;
const Gfx* gDlistHookshotReticle;
const Gfx* gDlistHookshotBodyFP;
const Gfx* gDlistHookshotBodyTP;

static void* Player_CustomEq(void* eqData, u32 eqDlist)
{
    Gfx* dlist;
    Gfx* d;

    if (!eqData) return (void*)kDListEmpty;

    d = dlist = GRAPH_ALLOC(gPlay->state.gfxCtx, sizeof(Gfx) * 2);
    gSPSegment(d++, 0x0a, eqData);
    gSPBranchList(d++, eqDlist);

    return dlist;
}

static void* dlistOrNothing(void* dlist)
{
    if (!dlist)
        return (void*)kDListEmpty;
    return dlist;
}

static void* Player_CustomPair(void* a, void* b)
{
    Gfx* dlist;
    Gfx* d;

    d = dlist = GRAPH_ALLOC(gPlay->state.gfxCtx, sizeof(Gfx) * 2);
    gSPDisplayList(d++, dlistOrNothing(a));
    gSPBranchList(d++, dlistOrNothing(b));

    return dlist;
}

static void Player_OverrideCustomSheath(PlayState* play, Player* this, Gfx** dlist, int isPause)
{
    void*   shield;
    void*   sword;
    u8      swordInUse;
    u8      shieldOnBack;

    shield = NULL;
    sword = NULL;
    swordInUse = !!isPause;

    switch (this->leftHandType)
    {
    case PLAYER_MODELTYPE_LH_SWORD:
    case PLAYER_MODELTYPE_LH_SWORD_2:
    case PLAYER_MODELTYPE_LH_BGS:
        swordInUse = 1;
        break;
    }

    if (isPause)
    {
        shieldOnBack = 0;
        if (gSave.age == AGE_CHILD && gSave.info.equips.equipment.shields > 1)
            shieldOnBack = 1;
    }
    else
        shieldOnBack = !!(this->rightHandType != PLAYER_MODELTYPE_RH_SHIELD);
    if (shieldOnBack)
    {
        /* Shield on back */
        switch (gSave.info.equips.equipment.shields)
        {
        case 1:
            shield = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHIELD_DEKU), CUSTOM_OBJECT_EQ_SHIELD_DEKU_1);
            break;
        case 2:
            if (gSave.age == AGE_CHILD)
                shield = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHEATH_SHIELD_HYLIAN_CHILD), CUSTOM_OBJECT_EQ_SHEATH_SHIELD_HYLIAN_CHILD_0);
            else
                shield = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHEATH_SHIELD_HYLIAN_ADULT), CUSTOM_OBJECT_EQ_SHEATH_SHIELD_HYLIAN_ADULT_0);
            break;
        case 3:
            shield = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHEATH_SHIELD_MIRROR), CUSTOM_OBJECT_EQ_SHEATH_SHIELD_MIRROR_0);
            break;
        }
    }

    switch (gSave.info.equips.equipment.swords)
    {
    case 1:
        switch (gSharedCustomSave.extraSwordsOot)
        {
        case 0:
            if (swordInUse)
                sword = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHEATH_SWORD_OOT_CHILD_EMPTY), CUSTOM_OBJECT_EQ_SHEATH_SWORD_OOT_CHILD_EMPTY_0);
            else
                sword = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHEATH_SWORD_OOT_CHILD_FULL), CUSTOM_OBJECT_EQ_SHEATH_SWORD_OOT_CHILD_FULL_0);
            break;
        case 1:
            sword = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHEATH_SWORD_RAZOR), swordInUse ? CUSTOM_OBJECT_EQ_SHEATH_SWORD_RAZOR_0 : CUSTOM_OBJECT_EQ_SHEATH_SWORD_RAZOR_1);
            break;
        case 2:
            sword = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHEATH_SWORD_GILDED), swordInUse ? CUSTOM_OBJECT_EQ_SHEATH_SWORD_GILDED_0 : CUSTOM_OBJECT_EQ_SHEATH_SWORD_GILDED_1);
            break;
        }
        break;
    case 2:
    case 3:
        if (swordInUse)
            sword = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHEATH_SWORD_OOT_ADULT_EMPTY), CUSTOM_OBJECT_EQ_SHEATH_SWORD_OOT_ADULT_EMPTY_0);
        else
            sword = Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SHEATH_SWORD_OOT_ADULT_FULL), CUSTOM_OBJECT_EQ_SHEATH_SWORD_OOT_ADULT_FULL_0);
        break;
    }

    *dlist = Player_CustomPair(shield, sword);
}

static void fixTunicColoLimb(PlayState* play, int limb)
{
    static const u8* kColors = (const u8*)0x800f7ad8;
    int index;
    u8 cr;
    u8 cg;
    u8 cb;

    if (limb != PLAYER_LIMB_R_SHOULDER && limb != PLAYER_LIMB_TORSO)
        return;

    index = gSave.info.equips.equipment.tunics - 1;
    cr = kColors[index * 3 + 0];
    cg = kColors[index * 3 + 1];
    cb = kColors[index * 3 + 2];
    OPEN_DISPS(play->state.gfxCtx);
    gDPSetEnvColor(POLY_OPA_DISP++, cr, cg, cb, 0xff);
    CLOSE_DISPS();
}

static void Player_OverrideAdult(PlayState* play, Player* this, int limb, Gfx** dlist, int isPause)
{
    fixTunicColoLimb(play, limb);

    if (limb == PLAYER_LIMB_L_HAND)
    {
        if ((this->leftHandType == PLAYER_MODELTYPE_LH_SWORD || isPause) && gSave.info.equips.equipment.swords == 1)
        {
            if (gSharedCustomSave.extraSwordsOot == 0)
                *dlist = Player_CustomHandEq(DLIST_ADULT_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_KOKIRI_SWORD), CUSTOM_OBJECT_EQ_KOKIRI_SWORD_0);
            else if (gSharedCustomSave.extraSwordsOot == 1)
                *dlist = Player_CustomHandEq(DLIST_ADULT_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_RAZOR_SWORD), CUSTOM_OBJECT_EQ_RAZOR_SWORD_0);
            else if (gSharedCustomSave.extraSwordsOot == 2)
                *dlist = Player_CustomHandEq(DLIST_ADULT_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_GILDED_SWORD), CUSTOM_OBJECT_EQ_GILDED_SWORD_0);
        }

        if (this->leftHandType == PLAYER_MODELTYPE_LH_BOOMERANG)
            *dlist = Player_CustomHandEq(DLIST_ADULT_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_BOOMERANG), CUSTOM_OBJECT_EQ_BOOMERANG_0);
    }

    if (limb == PLAYER_LIMB_R_HAND)
    {
        if ((this->rightHandType == PLAYER_MODELTYPE_RH_SHIELD) && gSave.info.equips.equipment.shields == 1)
            *dlist = Player_CustomHandEq(DLIST_ADULT_RHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_SHIELD_DEKU), CUSTOM_OBJECT_EQ_SHIELD_DEKU_0);

        if (this->rightHandType == PLAYER_MODELTYPE_RH_OCARINA)
            *dlist = Player_CustomHandEq(DLIST_ADULT_RHAND_OPEN, comboGetObject(CUSTOM_OBJECT_ID_EQ_OCARINA_FAIRY), CUSTOM_OBJECT_EQ_OCARINA_FAIRY_0);

        if (this->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT || this->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT_2)
        {
            if (this->heldItemAction == 15)
                *dlist = Player_CustomHandEq(DLIST_ADULT_RHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_SLINGSHOT), CUSTOM_OBJECT_EQ_SLINGSHOT_2);
        }
    }

    if (limb == PLAYER_LIMB_SHEATH)
    {
        if (gSave.info.equips.equipment.shields == 1 || gSave.info.equips.equipment.swords <= 1)
            Player_OverrideCustomSheath(play, this, dlist, isPause);
    }
}

static void Player_OverrideChild(PlayState* play, Player* this, int limb, Gfx** dlist, int isPause)
{
    fixTunicColoLimb(play, limb);

    if (limb == PLAYER_LIMB_L_HAND)
    {
        if (this->leftHandType == PLAYER_MODELTYPE_LH_SWORD || this->leftHandType == PLAYER_MODELTYPE_LH_SWORD_2 || isPause)
        {
            if (gSave.info.equips.equipment.swords == 1)
            {
                if (gSharedCustomSave.extraSwordsOot == 1)
                    *dlist = Player_CustomHandEq(DLIST_CHILD_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_RAZOR_SWORD), CUSTOM_OBJECT_EQ_RAZOR_SWORD_0);
                else if (gSharedCustomSave.extraSwordsOot == 2)
                    *dlist = Player_CustomHandEq(DLIST_CHILD_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_GILDED_SWORD), CUSTOM_OBJECT_EQ_GILDED_SWORD_0);
            }
            else if (gSave.info.equips.equipment.swords == 2)
            {
                *dlist = Player_CustomHandEq(DLIST_CHILD_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_MASTER_SWORD), CUSTOM_OBJECT_EQ_MASTER_SWORD_0);
            }
        }
        else if ((this->leftHandType == PLAYER_MODELTYPE_LH_BGS || isPause) && gSave.info.equips.equipment.swords == 3)
        {
            if (gSave.info.playerData.swordHealth)
                *dlist = Player_CustomHandEq(DLIST_CHILD_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_BIGGORON_SWORD), CUSTOM_OBJECT_EQ_BIGGORON_SWORD_0);
            else
                *dlist = Player_CustomHandEq(DLIST_CHILD_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_BIGGORON_SWORD_BROKEN), CUSTOM_OBJECT_EQ_BIGGORON_SWORD_BROKEN_0);
        }
        else if (this->leftHandType == PLAYER_MODELTYPE_LH_HAMMER)
            *dlist = Player_CustomHandEq(DLIST_CHILD_LHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_HAMMER), CUSTOM_OBJECT_EQ_HAMMER_0);
    }

    if (limb == PLAYER_LIMB_R_HAND)
    {
        if (this->rightHandType == PLAYER_MODELTYPE_RH_HOOKSHOT)
            *dlist = (Gfx*)gDlistHookshotBodyTP;

        if ((this->rightHandType == PLAYER_MODELTYPE_RH_SHIELD) && gSave.info.equips.equipment.shields == 3)
            *dlist = Player_CustomHandEq(DLIST_CHILD_RHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_SHIELD_MIRROR), CUSTOM_OBJECT_EQ_SHIELD_MIRROR_0);

        if (this->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT || this->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT_2)
        {
            if (this->heldItemAction != 15)
                *dlist = Player_CustomHandEq(DLIST_CHILD_RHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_BOW), CUSTOM_OBJECT_EQ_BOW_2);
        }
    }

    if (limb == PLAYER_LIMB_SHEATH)
    {
        if (gSave.info.equips.equipment.shields == 3 || gSave.info.equips.equipment.swords != 1 || gSharedCustomSave.extraSwordsOot)
            Player_OverrideCustomSheath(play, this, dlist, isPause);
    }
}

static void Player_OverrideCustom(PlayState* play, Player* this, int limb, Gfx** dlist, int isPause)
{
    if (gSave.age == AGE_CHILD)
        Player_OverrideChild(play, this, limb, dlist, isPause);
    else
        Player_OverrideAdult(play, this, limb, dlist, isPause);
}

int Player_OverrideLimbDrawGameplayDefaultWrapper(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Player* player)
{
    /* Forward */
    if (Player_OverrideLimbDrawGameplayDefault(play, limbIndex, dList, pos, rot, player))
        return 0;

    Player_OverrideCustom(play, player, limbIndex, dList, 0);
    return 0;
}

int Player_OverrideLimbDrawPauseWrapper(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Player* player)
{
    /* Forward */
    if (Player_OverrideLimbDrawPause(play, limbIndex, dList, pos, rot, player))
        return 0;

    Player_OverrideCustom(play, player, limbIndex, dList, 1);
    return 0;
}

int Player_OverrideLimbDrawGameplayFirstPersonWrapper(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Player* player)
{
    if (!(gSave.age == AGE_CHILD && limbIndex == PLAYER_LIMB_R_FOREARM))
    {
        /* Forward */
        if (Player_OverrideLimbDrawGameplayFirstPerson(play, limbIndex, dList, pos, rot, player))
            return 0;
    }

    /* Handle adult slingshot */
    if (gSave.age == AGE_ADULT && limbIndex == PLAYER_LIMB_R_HAND && (player->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT || player->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT_2) && player->heldItemAction == 15)
        *dList = Player_CustomHandEq(DLIST_ADULT_RHAND_OPEN, comboGetObject(CUSTOM_OBJECT_ID_EQ_SLINGSHOT), CUSTOM_OBJECT_EQ_SLINGSHOT_0);

    /* Handle child bow */
    if (gSave.age == AGE_CHILD && limbIndex == PLAYER_LIMB_R_HAND && (player->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT || player->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT_2) && player->heldItemAction != 15 && play->sceneId != SCE_OOT_SHOOTING_GALLERY)
        *dList = Player_CustomHandEq(DLIST_CHILD_RHAND_CLOSED, comboGetObject(CUSTOM_OBJECT_ID_EQ_BOW), CUSTOM_OBJECT_EQ_BOW_0);

    return 0;
}

static Color_RGB8 sGauntletColors[] = {
    { 255, 255, 255 },
    { 254, 207, 15 },
};

void DrawChildGauntlets(PlayState* play);

void Player_DrawFlexLod(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, void* overrideLimbDraw, void* postLimbDraw, void* arg, s32 lod)
{
    Color_RGB8* c;
    void* hookshotObj;
    void* bootsData;
    u32 bootsList;
    u32 bootsList2;

    /* Load hookshot data if child */
    if (gSave.age == AGE_CHILD)
    {
        hookshotObj = comboGetObject(CUSTOM_OBJECT_ID_EQ_HOOKSHOT);
        if (hookshotObj)
        {
            gDlistHookshotEnd = Player_CustomEq(hookshotObj, CUSTOM_OBJECT_EQ_HOOKSHOT_0);
            gDlistHookshotChain = Player_CustomEq(hookshotObj, CUSTOM_OBJECT_EQ_HOOKSHOT_1);
            gDlistHookshotReticle = Player_CustomEq(hookshotObj, CUSTOM_OBJECT_EQ_HOOKSHOT_2);
            gDlistHookshotBodyFP = Player_CustomHandEq(DLIST_CHILD_RHAND_CLOSED, hookshotObj, CUSTOM_OBJECT_EQ_HOOKSHOT_3);
            gDlistHookshotBodyTP = Player_CustomHandEq(DLIST_CHILD_RHAND_CLOSED, hookshotObj, CUSTOM_OBJECT_EQ_HOOKSHOT_4);
        }
        else
        {
            gDlistHookshotEnd = kDListEmpty;
            gDlistHookshotChain = kDListEmpty;
            gDlistHookshotReticle = kDListEmpty;
            gDlistHookshotBodyFP = kDListEmpty;
            gDlistHookshotBodyTP = kDListEmpty;
        }
    }

    /* Forward */
    SkelAnime_DrawFlexLod(play, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, arg, lod);

    /* Boots */
    if (gSave.age == AGE_CHILD)
    {
        bootsData = NULL;
        if (gSave.info.equips.equipment.boots == 2)
        {
            bootsData = comboGetObject(CUSTOM_OBJECT_ID_BOOTS_IRON);
            bootsList = CUSTOM_OBJECT_BOOTS_IRON_0;
            bootsList2 = CUSTOM_OBJECT_BOOTS_IRON_1;
        }
        else if (gSave.info.equips.equipment.boots == 3)
        {
            bootsData = comboGetObject(CUSTOM_OBJECT_ID_BOOTS_HOVER);
            bootsList = CUSTOM_OBJECT_BOOTS_HOVER_0;
            bootsList2 = CUSTOM_OBJECT_BOOTS_HOVER_1;
        }

        if (bootsData)
        {
            OPEN_DISPS(play->state.gfxCtx);
            gSPSegment(POLY_OPA_DISP++, 0x0a, bootsData);
            gSPDisplayList(POLY_OPA_DISP++, bootsList);
            gSPDisplayList(POLY_OPA_DISP++, bootsList2);
            CLOSE_DISPS();
        }

        if (Config_Flag(CFG_OOT_AGELESS_STRENGTH) && gSave.info.inventory.upgrades.strength > 1)
        {
            c = &sGauntletColors[gSave.info.inventory.upgrades.strength > 2 ? 1 : 0];
            OPEN_DISPS(play->state.gfxCtx);
            gDPPipeSync(POLY_OPA_DISP++);
            gDPSetEnvColor(POLY_OPA_DISP++, c->r, c->g, c->b, 0);
            CLOSE_DISPS();
            DrawChildGauntlets(play);
        }
    }
}

PATCH_CALL(0x8007a0d0, Player_DrawFlexLod);

static void Player_AddMasterSwordGanonFight(PlayState* play)
{
    if (gSharedCustomSave.foundMasterSword)
        AddItem(play, ITEM_OOT_SWORD_MASTER);
}

PATCH_CALL(0x80850758, Player_AddMasterSwordGanonFight);

int Player_GetStrength(void)
{
    int level;

    level = gSave.info.inventory.upgrades.strength;
    if (level > 3)
        level = 3;
    if (gSave.age == AGE_CHILD && level > 1 && !Config_Flag(CFG_OOT_AGELESS_STRENGTH))
        level = 1;
    return level;
}

PATCH_FUNC(0x80079af8, Player_GetStrength);

static int Player_IsStrengthGoronBracelet(void)
{
    return Player_GetStrength() == 1;
}

PATCH_CALL(0x8007a2b4, Player_IsStrengthGoronBracelet);

static int Player_ItemAndArrowType(PlayState* play, Player* this, int* outItem, int* outArrow)
{
    if (this->heldItemAction == 15 || (gSave.age == AGE_CHILD && play->sceneId == SCE_OOT_SHOOTING_GALLERY))
    {
        /* Slingshot */
        *outItem = ITEM_OOT_SLINGSHOT;
        *outArrow = 9;
    }
    else
    {
        /* Bow, maybe with magical arrow */
        *outItem = ITEM_OOT_BOW;
        if (this->stateFlags1 & (1 << 23))
            *outArrow = 1;
        else
            *outArrow = this->heldItemAction - 6;
    }

    if (gSaveContext.minigameState == 1)
        return play->interfaceCtx.hbaAmmo;
    else if (play->shootingGalleryStatus != 0)
        return play->shootingGalleryStatus;
    return gSave.info.inventory.ammo[*outItem];
}

PATCH_FUNC(0x808323dc, Player_ItemAndArrowType);

typedef struct
{
    u32 dlist;
    float x;
    float y;
    float z;
}
BowSlingshotString;

const BowSlingshotString* Player_GetBowSlingshotStringData(void)
{
    static BowSlingshotString bss;
    Player* link;

    link = GET_PLAYER(gPlay);
    if (link->heldItemAction == 15 || (gSave.age == AGE_CHILD && gPlay->sceneId == SCE_OOT_SHOOTING_GALLERY))
    {
        /* Slingshot */
        bss.x = 606.f;
        bss.y = 236.f;
        bss.z = 0.f;
        bss.dlist = (u32)kDListEmpty;

        if (gSave.age == AGE_CHILD)
        {
            bss.dlist = DLIST_CHILD_STRING;
        }
        else
        {
            bss.dlist = (u32)Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_SLINGSHOT), CUSTOM_OBJECT_EQ_SLINGSHOT_1);
        }
    }
    else
    {
        /* Bow */
        bss.x = 0.f;
        bss.y = -360.4f;
        bss.z = 0.f;

        if (gSave.age == AGE_ADULT)
        {
            bss.dlist = DLIST_ADULT_STRING;
        }
        else
        {
            bss.dlist = (u32)Player_CustomEq(comboGetObject(CUSTOM_OBJECT_ID_EQ_BOW), CUSTOM_OBJECT_EQ_BOW_1);
        }
    }

    return &bss;
}

void Player_RumbleAgony(void)
{
    Interface_AgonyIconTick();
    Rumble_Request(120, 20, 10, 0);
}

void Player_ChargeSword(Player* this)
{
    float max;

    if (!Config_Flag(CFG_OOT_SPIN_UPGRADE) || gOotExtraFlags.spinUpgrade)
        max = 1.f;
    else
        max = 0.5f;

    Math_StepToF(&this->unk_858, max, 0.02f);
}

PATCH_FUNC(0x80842ef8, Player_ChargeSword);

void Player_SetFaroresWindRespawnPoint(PlayState* play, s32 respawnMode, s32 playerParams)
{
    /* Displaced code: */
    Play_SetupRespawnPoint(play, respawnMode, playerParams);
    /* End displaced code */

    memcpy(&gCustomSave.fwRespawnDungeonEntrance[gSave.age], &gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE], sizeof(OotRespawnData));
}

PATCH_CALL(0x8084e7b0, Player_SetFaroresWindRespawnPoint)

void Player_AfterBeginFaroresWindTransition(PlayState* play)
{
    /* Displaced code: */
    Interface_SetSubTimerToFinalSecond(play);
    /* End displaced code */

    memcpy(&gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE], &gCustomSave.fwRespawnDungeonEntrance[gSave.age], sizeof(OotRespawnData));
}

PATCH_CALL(0x8084e540, Player_AfterBeginFaroresWindTransition);
