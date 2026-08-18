#include <combo.h>
#include <combo/player.h>
#include <combo/custom.h>
#include <combo/mm/adultmask.h>
#include <combo/mm/save.h>
#include <combo/mm/actors/En_Horse.h>
#include <combo/common/scene.h>
#include <combo/config.h>
#include "combo/actors.h"

#define ADULT_MASK_SKIP_BUTTONS (BTN_A | BTN_B | BTN_CUP | BTN_CLEFT | BTN_CDOWN | BTN_CRIGHT)
#define ADULT_MASK_ITEM_ICON_SIZE 0x1000

#define Player_GetIdleAnim ((Player_GetIdleAnimFunc)OverlayAddr(0x8082ED20))
#define Player_SetAction_PreserveItemAction ((Player_SetAction_PreserveItemActionFunc)OverlayAddr(0x80831760))
#define Player_BeforeCsAction ((Player_BeforeCsActionFunc)OverlayAddr(0x8082DE50))
#define Player_ClearAttentionAndCameraMode ((Player_ClearAttentionAndCameraModeFunc)OverlayAddr(0x8082DAD4))
#define Player_UpdateUpperBodyOrHeldItem ((Player_UpdateUpperBodyOrHeldItemFunc)OverlayAddr(0x8083249C))
#define Player_DecelerateToZero ((Player_DecelerateToZeroFunc)OverlayAddr(0x80832F24))
#define Player_ReturnToDefaultAction ((Player_ReturnToDefaultActionFunc)OverlayAddr(0x808369F4))
#define Player_StopCutscene ((Player_StopCutsceneFunc)OverlayAddr(0x80838760))
#define Player_Anim_PlayOnceAdjusted_Ovl ((Player_Anim_PlayOnceAdjustedFunc)OverlayAddr(0x8082DB90))
#define Player_Anim_PlayOnceMorphAdjusted_Ovl ((Player_Anim_PlayOnceMorphAdjustedFunc)OverlayAddr(0x8082E4A4))
#define Player_StartMaskTransformCs ((Player_StartMaskTransformCsFunc)OverlayAddr(0x808323C0))
#define Player_UpdateMaskTransformEffects ((Player_UpdateMaskTransformEffectsFunc)OverlayAddr(0x80855218))
#define Player_DrawMaskTransformEffects ((Player_DrawMaskTransformEffectsFunc)OverlayAddr(0x808550D0))
#define D_8085D910 ((struct_8085D910*)OverlayAddr(0x8085D910))

_Static_assert(sizeof(PlayerAgeProperties) == 0xDC, "Unexpected PlayerAgeProperties size");
#define PLAYER_AGE_PROPERTIES_HUMAN ((PlayerAgeProperties*)OverlayAddr(0x8085BA38 + sizeof(PlayerAgeProperties) * MM_PLAYER_FORM_HUMAN))

extern PlayerAnimationHeader gPlayerAnim_cl_setmask;
extern PlayerAnimationHeader gPlayerAnim_cl_setmaskend;
extern PlayerAnimationHeader gPlayerAnim_pz_maskoffstart;
extern PlayerAnimationHeader gPlayerAnim_cl_maskoff;
extern void Play_EnableMotionBlurPriority(s32 priority);
extern void Play_DisableMotionBlurPriority(void);
extern void ComboPlayer_ApplyAgeModelTables(void);
extern s32 Camera_ChangeMode(Camera* camera, s16 mode);
extern void func_800E0348(Camera* camera);
extern void func_80123140(PlayState* play, Player* player);
extern f32 Player_GetHeight(Player* player);
extern void PlayerCall_Update(Actor* thisx, PlayState* play);
extern void PlayerCall_Draw(Actor* thisx, PlayState* play);
extern void AudioSfx_StopById(u16 sfxId);
extern AnimatedMaterial gameplay_keep_Matanimheader_054F18[];
extern Gfx gameplay_keep_DL_054C90[];

void AdultMask_Action(Player* player, PlayState* play);

typedef enum {
    ADULT_MASK_CS_NONE,
    ADULT_MASK_CS_PUT_ON,
    ADULT_MASK_CS_TAKE_OFF_TRANSFORM,
    ADULT_MASK_CS_ENDING_ANIM,
} AdultMaskCutsceneMode;

typedef enum {
    ADULT_MASK_WHITE_FILL_NONE,
    ADULT_MASK_WHITE_FILL_IN,
    ADULT_MASK_WHITE_FILL_HOLD,
    ADULT_MASK_WHITE_FILL_OUT,
} AdultMaskWhiteFillMode;

static AdultMaskCutsceneMode sAdultMaskCutsceneMode;
static AdultMaskWhiteFillMode sAdultMaskWhiteFillMode;
static AdjLightSettings sAdultMaskSavedAdjLightSettings;
static s32 sAdultMaskSavedAdjLightSettingsValid;
static s32 sAdultMaskTargetAdult;
static s32 sAdultMaskCommitted;
static s32 sAdultMaskTimer;
static s32 sAdultMaskPlayedFlashSfx;
static s32 sAdultMaskDrawTransformFace;
static s32 sAdultMaskPlayerReloadStarted;
static s32 sAdultMaskPendingReloadAfterWhiteIn;
static s32 sAdultMaskInterrupted;
static s32 sAdultMaskReloadBusy;

static void AdultMask_ClearState(void);
static void AdultMask_StartEndingAnim(Player* player, PlayState* play);
static void AdultMask_PlayerReloadUpdateWrapper(Actor* thisx, PlayState* play);

static PlayerAnimationHeader sAdultMask_ClMaskOffHeader = {
    { 10 },
    { (void*)0x0702C1A0 },
};

static void AdultMask_DrawObject(PlayState* play, void* object, u32 dl)
{
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x0a, object);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, (Gfx*)dl);
    CLOSE_DISPS();
}

void AdultMask_DrawMaskInHand(PlayState* play, Player* player)
{
    void* obj = comboGetObject(CUSTOM_OBJECT_ID_ADULT_MASK_EQUIPMENT | MASK_CUSTOM_OBJECT);
    if (obj == NULL)
        return;

    Matrix_Push();
    Matrix_Translate(-323.67f, 412.15f, -969.96f, MTXMODE_APPLY);
    Matrix_RotateZYX(-0x32BE, -0x50DE, -0x7717, MTXMODE_APPLY);
    AdultMask_DrawObject(play, obj, CUSTOM_OBJECT_ADULT_MASK_EQUIPMENT_0);
    Matrix_Pop();
    (void)player;
}

void AdultMask_DrawMaskOnFaceNativeLike(PlayState* play, Player* player)
{
    void* obj = comboGetObject(CUSTOM_OBJECT_ID_ADULT_MASK_EQUIPMENT | MASK_CUSTOM_OBJECT);
    if (obj == NULL)
        return;

    Matrix_Push();
    Matrix_Scale(1.0f, 1.0f - player->unk_B10[3], 1.0f - player->unk_B10[2], MTXMODE_APPLY);
    AdultMask_DrawObject(play, obj, CUSTOM_OBJECT_ADULT_MASK_EQUIPMENT_0);
    Matrix_Pop();
}

void AdultMask_DrawTransformationMaskOnFace(PlayState* play, Player* player)
{
    void* obj = comboGetObject(CUSTOM_OBJECT_ID_MASK_ADULT_TRANSFORM_PLAYER);
    if (obj == NULL)
        return;

    Matrix_Push();
    AdultMask_DrawObject(play, obj, CUSTOM_OBJECT_MASK_ADULT_TRANSFORM_PLAYER_0);
    Matrix_Pop();
    (void)player;
}

s32 AdultMask_IsActive(void) { return sAdultMaskCutsceneMode != ADULT_MASK_CS_NONE; }
s32 AdultMask_IsPuttingOn(void) { return sAdultMaskCutsceneMode == ADULT_MASK_CS_PUT_ON; }
s32 AdultMask_IsTakeOffTransform(void) { return sAdultMaskCutsceneMode == ADULT_MASK_CS_TAKE_OFF_TRANSFORM; }
s32 AdultMask_IsTakeOffChild(void) { return sAdultMaskCutsceneMode == ADULT_MASK_CS_ENDING_ANIM; }
s32 AdultMask_IsTakingOff(void) { return AdultMask_IsTakeOffTransform() || AdultMask_IsTakeOffChild(); }
s32 AdultMask_IsCsItem(Player* player) { return player->itemAction == PLAYER_CUSTOM_IA_MASK_ADULT; }
s32 AdultMask_ShouldDrawAdultModel(void) { return comboIsLinkAdult(); }
s32 AdultMask_GetTimer(void) { return sAdultMaskTimer; }
s32 AdultMask_ShouldDrawTransformFace(void) { return sAdultMaskDrawTransformFace; }

static void AdultMask_SetB10(Player* player, f32 value)
{
    player->unk_B10[2] = value;
    player->unk_B10[3] = value;
    player->unk_B10[4] = value;
    player->unk_B10[5] = value;
}

static void AdultMask_StopTransformLoopSfx(void)
{
    AudioSfx_StopById(NA_SE_PL_TRANSFORM_VOICE);
    AudioSfx_StopById(NA_SE_IT_TRANSFORM_MASK_BROKEN);
    AudioSfx_StopById(NA_SE_PL_FACE_CHANGE);
    Play_DisableMotionBlurPriority();
}

static void AdultMask_SaveEnvVisualState(PlayState* play)
{
    if (sAdultMaskSavedAdjLightSettingsValid)
        return;
    sAdultMaskSavedAdjLightSettings = play->envCtx.adjLightSettings;
    sAdultMaskSavedAdjLightSettingsValid = 1;
}

static void AdultMask_RestoreEnvVisualState(PlayState* play)
{
    if (sAdultMaskSavedAdjLightSettingsValid)
        play->envCtx.adjLightSettings = sAdultMaskSavedAdjLightSettings;
}

static void AdultMask_ApplyRuntimeAgeProperties(Player* player, PlayState* play)
{
    f32 height;
    player->ageProperties = PLAYER_AGE_PROPERTIES_HUMAN;
    func_80123140(play, player);
    height = Player_GetHeight(player);

    player->actor.focus.pos.y = player->actor.world.pos.y + height;
    player->cylinder.dim.height = height;
    player->cylinder.dim.yShift = 0;
    player->cylinder.dim.radius = player->ageProperties->wallCheckRadius;
    player->actor.cullingVolumeDistance = 4000.0f;
    player->actor.cullingVolumeScale = height;
    player->actor.cullingVolumeDownward = height;
}

static MmHumanAgeLoadout* AdultMask_GetHumanAgeLoadout(u8 age)
{
    if (age >= MM_LINK_AGE_COUNT)
        age = MM_LINK_AGE_CHILD;
    return &gCustomSave.humanAgeLoadouts[age];
}

static void AdultMask_CaptureHumanAgeLoadout(MmHumanAgeLoadout* loadout)
{
    MmItemEquips* equips = &gMmSave.info.itemEquips;
    s32 button;

    for (button = EQUIP_SLOT_C_LEFT; button <= EQUIP_SLOT_C_RIGHT; button++) {
        loadout->buttonItems[button] = equips->buttonItems[0][button];
        loadout->cButtonSlots[button] = equips->cButtonSlots[0][button];
    }

    loadout->boots = equips->boots;
    loadout->tunic = equips->tunic;
    loadout->valid = 1;
}

static s32 AdultMask_IsBottleSlot(u8 slot)
{
    return slot >= ITS_MM_BOTTLE && slot <= ITS_MM_BOTTLE6;
}

static void AdultMask_RestoreHumanAgeLoadout(const MmHumanAgeLoadout* loadout)
{
    MmItemEquips* equips = &gMmSave.info.itemEquips;
    s32 button;

    for (button = EQUIP_SLOT_C_LEFT; button <= EQUIP_SLOT_C_RIGHT; button++) {
        u8 slot = loadout->cButtonSlots[button];
        u8 item = loadout->buttonItems[button];

        equips->cButtonSlots[0][button] = slot;

        if (slot == 0xff)
            equips->buttonItems[0][button] = ITEM_NONE;
        else if (AdultMask_IsBottleSlot(slot))
            equips->buttonItems[0][button] = gSave.info.inventory.items[slot];
        else
            equips->buttonItems[0][button] = item;
    }

    equips->boots = loadout->boots;
    equips->tunic = loadout->tunic;
}

static void AdultMask_RefreshButtonIcons(PlayState* play)
{
    s32 button;

    for (button = EQUIP_SLOT_C_LEFT; button <= EQUIP_SLOT_C_RIGHT; button++) {
        u8 item = BUTTON_ITEM_EQUIP(0, button);

        if (item != ITEM_NONE && item != 0xff) {
            Interface_LoadItemIconImpl(play, button);
        } else {
            u8* iconBuffer = (u8*)play->interfaceCtx.iconItemSegment + button * ADULT_MASK_ITEM_ICON_SIZE;
            bzero(iconBuffer, ADULT_MASK_ITEM_ICON_SIZE);
        }
    }
}

static void AdultMask_InitEmptyHumanAgeLoadout(MmHumanAgeLoadout* loadout)
{
    s32 button;

    bzero(loadout, sizeof(*loadout));
    for (button = EQUIP_SLOT_C_LEFT; button <= EQUIP_SLOT_C_RIGHT; button++) {
        loadout->buttonItems[button] = ITEM_NONE;
        loadout->cButtonSlots[button] = 0xff;
    }

    loadout->valid = 1;
}

void AdultMask_ResetDrawStateForNewPlay(void)
{
    sAdultMaskReloadBusy = 0;
    AdultMask_ClearState();
    EponaAge_ClearReload();
}

static s32 AdultMask_CanUse(Player* player, PlayState* play)
{
    return !sAdultMaskReloadBusy &&
           player->transformation == MM_PLAYER_FORM_HUMAN &&
           player->actor.draw != NULL &&
           !Player_InCsMode(play) &&
           !(player->stateFlags1 & 0x207c7080) &&
           !(player->stateFlags3 & 0x1000);
}

s32 AdultMask_TryUse(Player* player, PlayState* play, s32 itemAction)
{
    s32 fastMasks;
    if (itemAction != PLAYER_CUSTOM_IA_MASK_ADULT)
        return 0;
    fastMasks = Config_Flag(CFG_MM_FAST_MASKS);
    if (fastMasks && sAdultMaskReloadBusy)
        return 1;

    if (!AdultMask_CanUse(player, play)) {
        PlaySound(0x4806);
        return 1;
    }
    if (fastMasks) {
        AdultMask_StartCsItem(player, play);
        return 1;
    }

    player->unk_AA5 = 5;
    player->itemAction = itemAction;
    gCustomSave.customMask = PLAYER_CUSTOM_MASK_NONE;
    gSaveContext.save.equippedMask = 0;
    player->currentMask = 0;
    return 1;
}

static void AdultMask_SetWhiteFillAlpha(PlayState* play, s32 alpha)
{
    if (alpha < 0)
        alpha = 0;
    else if (alpha > 255)
        alpha = 255;
    if (alpha > 0)
        Play_FillScreen(play, 45, 220, 220, 220, alpha);
    else {
        R_PLAY_FILL_SCREEN_ON = 0;
        R_PLAY_FILL_SCREEN_ALPHA = 0;
    }
}

static void AdultMask_StartWhiteFillIn(void)
{
    if (sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_IN || sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_HOLD)
        return;

    sAdultMaskWhiteFillMode = ADULT_MASK_WHITE_FILL_IN;
    R_PLAY_FILL_SCREEN_ON = 45;
    R_PLAY_FILL_SCREEN_R = R_PLAY_FILL_SCREEN_G = R_PLAY_FILL_SCREEN_B = 220;
    R_PLAY_FILL_SCREEN_ALPHA = 0;
}

static void AdultMask_StartWhiteFillOut(void)
{
    sAdultMaskWhiteFillMode = ADULT_MASK_WHITE_FILL_OUT;
    R_PLAY_FILL_SCREEN_ON = 45;
    R_PLAY_FILL_SCREEN_R = R_PLAY_FILL_SCREEN_G = R_PLAY_FILL_SCREEN_B = 220;
    R_PLAY_FILL_SCREEN_ALPHA = 255;
}

static s32 AdultMask_UpdateWhiteFill(PlayState* play)
{
    s32 alpha;

    if (sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_IN) {
        alpha = R_PLAY_FILL_SCREEN_ALPHA + 45;
        if (alpha >= 255) {
            sAdultMaskWhiteFillMode = ADULT_MASK_WHITE_FILL_HOLD;
            AdultMask_SetWhiteFillAlpha(play, 255);
            return 1;
        }
        AdultMask_SetWhiteFillAlpha(play, alpha);
    } else if (sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_OUT) {
        alpha = R_PLAY_FILL_SCREEN_ALPHA - 45;
        if (alpha <= 0) {
            sAdultMaskWhiteFillMode = ADULT_MASK_WHITE_FILL_NONE;
            AdultMask_SetWhiteFillAlpha(play, 0);
            return 1;
        }
        AdultMask_SetWhiteFillAlpha(play, alpha);
    }

    return 0;
}

static void AdultMask_ClearFill(void)
{
    sAdultMaskWhiteFillMode = ADULT_MASK_WHITE_FILL_NONE;
    R_PLAY_FILL_SCREEN_ON = 0;
    R_PLAY_FILL_SCREEN_ALPHA = 0;
}

static void AdultMask_ResetRunState(void)
{
    sAdultMaskTimer = 0;
    sAdultMaskPlayedFlashSfx = 0;
    sAdultMaskDrawTransformFace = 0;
    sAdultMaskPlayerReloadStarted = 0;
    sAdultMaskPendingReloadAfterWhiteIn = 0;
    sAdultMaskInterrupted = 0;
    AdultMask_ClearFill();
}

static void AdultMask_ResetPlayerVisualState(Player* player)
{
    player->av1.actionVar1 = 0;
    player->av2.actionVar2 = 0;
    AdultMask_SetB10(player, 0.0f);
}

static void AdultMask_ClearState(void)
{
    sAdultMaskCutsceneMode = ADULT_MASK_CS_NONE;
    sAdultMaskTargetAdult = 0;
    sAdultMaskCommitted = 0;
    sAdultMaskSavedAdjLightSettingsValid = 0;
    AdultMask_ResetRunState();
}

static void AdultMask_CommitAge(Player* player)
{
    MmHumanAgeLoadout* outgoingLoadout;
    MmHumanAgeLoadout* incomingLoadout;
    u8 outgoingAge;
    u8 incomingAge;

    if (sAdultMaskCommitted)
        return;

    sAdultMaskCommitted = 1;
    outgoingAge = gMmSave.linkAge;
    incomingAge = sAdultMaskTargetAdult ? MM_LINK_AGE_ADULT : MM_LINK_AGE_CHILD;
    if (outgoingAge >= MM_LINK_AGE_COUNT)
        outgoingAge = sAdultMaskTargetAdult ? MM_LINK_AGE_CHILD : MM_LINK_AGE_ADULT;
    outgoingLoadout = AdultMask_GetHumanAgeLoadout(outgoingAge);
    incomingLoadout = AdultMask_GetHumanAgeLoadout(incomingAge);
    AdultMask_CaptureHumanAgeLoadout(outgoingLoadout);
    if (!incomingLoadout->valid)
        AdultMask_InitEmptyHumanAgeLoadout(incomingLoadout);

    gMmSave.linkAge = incomingAge;
    gSaveContext.respawn[RESPAWN_MODE_HUMAN] = gCustomSave.fw[comboMmFwAge()];
    AdultMask_RestoreHumanAgeLoadout(incomingLoadout);

    gSaveContext.save.playerForm = MM_PLAYER_FORM_HUMAN;
    player->transformation = MM_PLAYER_FORM_HUMAN;
    player->currentMask = 0;
    gSaveContext.save.equippedMask = 0;
    gCustomSave.customMask = PLAYER_CUSTOM_MASK_NONE;
}

static void AdultMask_EndCutsceneCamera(Player* player)
{
    Player_StopCutscene(player);
    Player_ClearAttentionAndCameraMode(player);
    player->csMode = 0;
    player->prevCsAction = 0;
    player->csId = CS_ID_NONE;
    player->subCamId = SUB_CAM_ID_DONE;
}

static void AdultMask_ClearHeldMaskAndUnlock(Player* player)
{
    player->itemAction = PLAYER_IA_NONE;
    player->heldItemAction = PLAYER_IA_NONE;
    player->stateFlags1 &= ~0x30000000;
    player->stateFlags3 &= ~0x80;
}

static void AdultMask_Finish(Player* player, PlayState* play)
{
    Play_DisableMotionBlurPriority();
    AdultMask_EndCutsceneCamera(player);
    AdultMask_ClearHeldMaskAndUnlock(player);
    Player_ReturnToDefaultAction(player, play);
    AdultMask_RestoreEnvVisualState(play);
    AdultMask_ClearState();
}

static s32 AdultMask_HasHorseActor(PlayState* play)
{
    s32 category;
    Actor* actor;

    for (category = 0; category < ACTORCAT_MAX; category++) {
        for (actor = play->actorCtx.actors[category].first; actor != NULL; actor = actor->next) {
            if (actor->id == ACTOR_EN_HORSE)
                return 1;
        }
    }

    return 0;
}

static void AdultMask_CommitAgeApplyTablesAndReloadPlayer(Player* player, PlayState* play)
{
    s32 fastMasks;
    if (sAdultMaskPlayerReloadStarted)
        return;
    fastMasks = Config_Flag(CFG_MM_FAST_MASKS);
    sAdultMaskReloadBusy = 1;
    sAdultMaskPlayerReloadStarted = 1;
    player->actor.draw = NULL;
    AdultMask_StopTransformLoopSfx();
    AdultMask_RestoreEnvVisualState(play);
    AdultMask_CommitAge(player);
    Scene_ApplyHumanAgeGameplayKeep(&play->objectCtx, sAdultMaskTargetAdult);
    if (!fastMasks || AdultMask_HasHorseActor(play))
        EponaAge_Request(play, sAdultMaskTargetAdult);
    else
        EponaAge_ClearReload();
    ComboPlayer_ApplyAgeModelTables();
    PlayerAge_ApplyOverlayData(sAdultMaskTargetAdult);
    if (sAdultMaskCutsceneMode != ADULT_MASK_CS_NONE)
        AdultMask_EndCutsceneCamera(player);
    AdultMask_ClearHeldMaskAndUnlock(player);
    AdultMask_ResetPlayerVisualState(player);
    player->actor.update = AdultMask_PlayerReloadUpdateWrapper;
    player->actor.draw = NULL;

    if (sAdultMaskCutsceneMode != ADULT_MASK_CS_TAKE_OFF_TRANSFORM && sAdultMaskCutsceneMode != ADULT_MASK_CS_PUT_ON)
        sAdultMaskCutsceneMode = ADULT_MASK_CS_NONE;
}

static void AdultMask_PlayerPostReloadUpdateWrapper(Actor* thisx, PlayState* play)
{
    Player* player = (Player*)thisx;
    MmHumanAgeLoadout* incomingLoadout;
    s32 fastMasks = Config_Flag(CFG_MM_FAST_MASKS);
    u8 sharedBItem = gMmSave.info.itemEquips.buttonItems[0][EQUIP_SLOT_B];
    u8 sharedBSlot = gMmSave.info.itemEquips.cButtonSlots[0][EQUIP_SLOT_B];
    u8 sharedSword = gMmSave.info.itemEquips.sword;
    u8 sharedShield = gMmSave.info.itemEquips.shield;

    player->actor.draw = NULL;
    AdultMask_ApplyRuntimeAgeProperties(player, play);
    PlayerCall_Update(thisx, play);
    AdultMask_ApplyRuntimeAgeProperties(player, play);
    player->actor.draw = NULL;

    if (EponaAge_IsBusy() && (!fastMasks || AdultMask_HasHorseActor(play))) {
        if (fastMasks) {
            AdultMask_ClearFill();
        } else {
            AdultMask_SetWhiteFillAlpha(play, 255);
            sAdultMaskWhiteFillMode = ADULT_MASK_WHITE_FILL_HOLD;
        }
        return;
    }

    if (fastMasks)
        AdultMask_ClearFill();
    gMmSave.info.itemEquips.sword = sharedSword;
    gMmSave.info.itemEquips.shield = sharedShield;
    gMmSave.info.itemEquips.buttonItems[0][EQUIP_SLOT_B] = sharedBItem;
    gMmSave.info.itemEquips.cButtonSlots[0][EQUIP_SLOT_B] = sharedBSlot;

    incomingLoadout = AdultMask_GetHumanAgeLoadout(gMmSave.linkAge);
    AdultMask_RestoreHumanAgeLoadout(incomingLoadout);

    Player_SetBootData(play, player);
    Player_CheckCustomBoots(play);
    UpdateEquipment(play, player);
    AdultMask_RefreshButtonIcons(play);

    player->actor.update = (ActorFunc)Player_UpdateWrapper;
    player->actor.draw = PlayerCall_Draw;
    sAdultMaskReloadBusy = 0;

    if (sAdultMaskCutsceneMode == ADULT_MASK_CS_TAKE_OFF_TRANSFORM || sAdultMaskCutsceneMode == ADULT_MASK_CS_PUT_ON)
        AdultMask_StartEndingAnim(player, play);
    else
        AdultMask_ClearState();
}

static void AdultMask_PlayerReloadUpdateWrapper(Actor* thisx, PlayState* play)
{
    Player* player = (Player*)thisx;

    func_8012301C(thisx, play);
    if (player->actor.update == AdultMask_PlayerReloadUpdateWrapper)
        return;
    player->actor.update = AdultMask_PlayerPostReloadUpdateWrapper;
    player->actor.draw = NULL;
    AdultMask_ApplyRuntimeAgeProperties(player, play);
}

static u8 AdultMask_GetFierceDeityRingAlpha(void)
{
    s32 ringFrame;

    if (sAdultMaskCutsceneMode == ADULT_MASK_CS_PUT_ON) {
        if (sAdultMaskTimer < 54)
            return 0;
        ringFrame = sAdultMaskTimer - 54;
        if (ringFrame < 12)
            return 255;
        return ringFrame < 36 ? (u8)((36 - ringFrame) * (255.0f / 24.0f)) : 0;
    }

    if (sAdultMaskCutsceneMode == ADULT_MASK_CS_TAKE_OFF_TRANSFORM) {
        if (sAdultMaskTimer < 0x10)
            return 0;
        ringFrame = sAdultMaskTimer - 0x10;
        if (ringFrame < 0x28)
            return 255;
        ringFrame -= 0x28;
        return ringFrame < 0x0c ? (u8)((0x0c - ringFrame) * (255.0f / 0x0c)) : 0;
    }

    return 0;
}

void AdultMask_DrawTransformRing(PlayState* play, Player* player)
{
    u8 alpha = AdultMask_GetFierceDeityRingAlpha();
    if (alpha == 0)
        return;

    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Push();
    AnimatedMat_DrawXlu(play, Lib_SegmentedToVirtual(gameplay_keep_Matanimheader_054F18));
    Matrix_Translate(-230.0f, -520.0f, 0.0f, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, alpha);
    gSPDisplayList(POLY_XLU_DISP++, gameplay_keep_DL_054C90);
    Matrix_Pop();
    CLOSE_DISPS();
    (void)player;
}

static void AdultMask_UpdateEffectSteps(Player* player, struct_8085D910* cfg, s32 playLightningSfx)
{
    s32 frame = player->av1.actionVar1;
    if (frame >= cfg->unk_0) {
        if (frame < cfg->unk_2) {
            Math_StepToF(&player->unk_B10[4], 1.0f, cfg->unk_1 / 100.0f);
        } else if (frame < cfg->unk_3) {
            if (playLightningSfx && frame == cfg->unk_2)
                PlaySound(NA_SE_EV_LIGHTNING_HARD);
            Math_StepToF(&player->unk_B10[4], 2.0f, 0.5f);
        } else {
            Math_StepToF(&player->unk_B10[4], 3.0f, 0.2f);
        }
    }

    if (frame >= 0x10) {
        if (frame < 0x37)
            Math_StepToF(&player->unk_B10[5], 1.0f, 0.2f);
        else if (frame < 0x40)
            Math_StepToF(&player->unk_B10[5], 2.0f, 1.0f);
        else
            Math_StepToF(&player->unk_B10[5], 3.0f, 0.55f);
    }
}

static void AdultMask_UpdateTransformCameraAndEffects(Player* player, PlayState* play, s32 form, s32 camMode, s32 drawArg, s32 playLightningSfx)
{
    struct_8085D910* cfg = &D_8085D910[0];
    Camera* cam;
    s32 prevPlayerForm;
    s32 prevSaveForm;
    Player_StartMaskTransformCs(player, player->csId);
    cam = Play_GetCamera(play, play->activeCamId);
    Camera_ChangeMode(cam, camMode);
    player->stateFlags2 |= PLAYER_STATE2_MM_40;
    player->actor.shape.rot.y = Camera_GetCamDirYaw(cam) + 0x8000;
    prevPlayerForm = player->transformation;
    prevSaveForm = gSaveContext.save.playerForm;
    player->transformation = form;
    gSaveContext.save.playerForm = form;
    Player_UpdateMaskTransformEffects(play, player, &cfg);
    player->transformation = prevPlayerForm;
    gSaveContext.save.playerForm = prevSaveForm;
    if (!sAdultMaskInterrupted && player->av1.actionVar1 == 0x14)
        Play_EnableMotionBlurPriority(100);
    sAdultMaskTimer = ++player->av1.actionVar1;
    AdultMask_UpdateEffectSteps(player, cfg, playLightningSfx);
    Player_DrawMaskTransformEffects(play, player, player->unk_B10[4], player->unk_B10[5], drawArg);
}

static s32 AdultMask_ShouldSkipTransformLoop(PlayState* play)
{
    return CHECK_BTN_ANY(play->state.input[0].press.button, ADULT_MASK_SKIP_BUTTONS);
}

static void AdultMask_SkipTransformLoopToFlash(Player* player, PlayState* play, s32 flashFrame)
{
    s16 currentCsId;
    s16 subCamId;
    if (player->av1.actionVar1 >= flashFrame)
        return;
    sAdultMaskInterrupted = 1;
    AdultMask_StopTransformLoopSfx();
    currentCsId = CutsceneManager_GetCurrentCsId();
    if (currentCsId == player->csId) {
        subCamId = CutsceneManager_GetCurrentSubCamId(player->csId);
        if (subCamId != SUB_CAM_ID_DONE && subCamId != CS_ID_NONE)
            func_800E0348(Play_GetCamera(play, subCamId));
    }
    AdultMask_RestoreEnvVisualState(play);
    AdultMask_SetB10(player, 0.0f);
    player->av1.actionVar1 = flashFrame;
    sAdultMaskTimer = flashFrame;
}

static void AdultMask_StartReloadFlash(Player* player)
{
    if (!sAdultMaskPlayedFlashSfx) {
        sAdultMaskPlayedFlashSfx = 1;
        AdultMask_StopTransformLoopSfx();
        Player_PlaySfx(player, NA_SE_SY_TRANSFORM_MASK_FLASH);
    }

    AdultMask_StartWhiteFillIn();
    sAdultMaskPendingReloadAfterWhiteIn = 1;
}

static void AdultMask_UpdatePutOn(Player* player, PlayState* play)
{
    if (AdultMask_ShouldSkipTransformLoop(play))
        AdultMask_SkipTransformLoopToFlash(player, play, 0x54);

    if (player->skelAnime.animation == &gPlayerAnim_cl_setmask && player->skelAnime.curFrame >= 51)
        sAdultMaskDrawTransformFace = 1;

    if (sAdultMaskTimer < 0x54 && sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_NONE)
        AdultMask_UpdateTransformCameraAndEffects(player, play, MM_PLAYER_FORM_FIERCE_DEITY, CAM_MODE_NORMAL, 0, 1);

    if (sAdultMaskTimer >= 0x54)
        AdultMask_StartReloadFlash(player);
}

static s32 AdultMask_UpdateTakeOffTransform(Player* player, PlayState* play)
{
    if (AdultMask_ShouldSkipTransformLoop(play))
        AdultMask_SkipTransformLoopToFlash(player, play, 0x45);
    if (sAdultMaskTimer < 0x45 && sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_NONE)
        AdultMask_UpdateTransformCameraAndEffects(player, play, MM_PLAYER_FORM_ZORA, CAM_MODE_JUMP, 1, 0);
    if (sAdultMaskTimer < 0x45)
        return 0;
    AdultMask_StartReloadFlash(player);
    return 1;
}

static void AdultMask_UpdateEndingAnim(Player* player, PlayState* play)
{
    sAdultMaskTimer++;

    if (sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_HOLD)
        AdultMask_StartWhiteFillOut();
    if (sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_OUT)
        AdultMask_UpdateWhiteFill(play);
    if (!PlayerAnimation_Update(play, &player->skelAnime))
        return;
    PlayerAnimation_Change(play, &player->skelAnime, Player_GetIdleAnim(player), 1, 0.0f, 20.0f, ANIMMODE_ONCE, 20.0f);
    AdultMask_Finish(player, play);
}

static void AdultMask_StartEndingAnim(Player* player, PlayState* play)
{
    PlayerAnimationHeader* endingAnim = sAdultMaskTargetAdult ? &gPlayerAnim_cl_setmaskend : &sAdultMask_ClMaskOffHeader;
    sAdultMaskCutsceneMode = ADULT_MASK_CS_ENDING_ANIM;
    sAdultMaskTimer = 0;
    sAdultMaskPlayerReloadStarted = 0;
    sAdultMaskWhiteFillMode = ADULT_MASK_WHITE_FILL_HOLD;
    AdultMask_SetWhiteFillAlpha(play, 255);
    AdultMask_ResetPlayerVisualState(player);
    player->itemAction = PLAYER_CUSTOM_IA_MASK_ADULT;
    player->heldItemAction = PLAYER_CUSTOM_IA_MASK_ADULT;
    Player_BeforeCsAction(play, player);
    Player_SetAction_PreserveItemAction(play, player, AdultMask_Action, 0);
    Player_Anim_PlayOnceAdjusted_Ovl(play, player, endingAnim);
    player->stateFlags1 |= 0x30000000;
    player->stateFlags3 |= 0x80;
}

void AdultMask_Action(Player* player, PlayState* play)
{
    player->actor.gravity = 0.0f;
    player->actor.velocity.y = 0.0f;
    player->stateFlags1 |= 0x30000000;
    player->stateFlags3 |= 0x80;
    gSaveContext.save.playerForm = MM_PLAYER_FORM_HUMAN;
    player->transformation = MM_PLAYER_FORM_HUMAN;
    player->itemAction = PLAYER_CUSTOM_IA_MASK_ADULT;

    if (sAdultMaskPlayerReloadStarted)
        return;

    if (sAdultMaskWhiteFillMode != ADULT_MASK_WHITE_FILL_NONE) {
        Play_DisableMotionBlurPriority();
        AdultMask_SetB10(player, 0.0f);
    }

    if ((sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_IN || sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_OUT) &&
        AdultMask_UpdateWhiteFill(play) &&
        sAdultMaskPendingReloadAfterWhiteIn &&
        sAdultMaskWhiteFillMode == ADULT_MASK_WHITE_FILL_HOLD) {
        sAdultMaskPendingReloadAfterWhiteIn = 0;
        AdultMask_CommitAgeApplyTablesAndReloadPlayer(player, play);
        return;
    }

    if (sAdultMaskCutsceneMode == ADULT_MASK_CS_PUT_ON) {
        AdultMask_UpdatePutOn(player, play);
        if (sAdultMaskPendingReloadAfterWhiteIn)
            return;
    } else if (sAdultMaskCutsceneMode == ADULT_MASK_CS_TAKE_OFF_TRANSFORM) {
        if (AdultMask_UpdateTakeOffTransform(player, play) || sAdultMaskPlayerReloadStarted)
            return;
    } else if (sAdultMaskCutsceneMode == ADULT_MASK_CS_ENDING_ANIM) {
        AdultMask_UpdateEndingAnim(player, play);
    } else {
        AdultMask_Finish(player, play);
        return;
    }

    Player_DecelerateToZero(player);
    if (sAdultMaskCutsceneMode != ADULT_MASK_CS_ENDING_ANIM)
        Player_UpdateUpperBodyOrHeldItem(player);
}

static void AdultMask_SetStartCamera(Player* player, PlayState* play, s32 camMode)
{
    Camera* cam;

    player->csId = play->playerActorCsIds[5];
    Player_StartMaskTransformCs(player, player->csId);
    cam = Play_GetCamera(play, play->activeCamId);
    Camera_ChangeMode(cam, camMode);
    player->actor.shape.rot.y = Camera_GetCamDirYaw(cam) + 0x8000;
}

static void AdultMask_FastStartUpdateWrapper(Actor* thisx, PlayState* play)
{
    Player* player = (Player*)thisx;
    player->actor.draw = NULL;
    AdultMask_CommitAgeApplyTablesAndReloadPlayer(player, play);
}

void AdultMask_StartCsItem(Player* player, PlayState* play)
{
    s32 isAdult = comboIsLinkAdult();
    s32 fastMasks = Config_Flag(CFG_MM_FAST_MASKS);
    sAdultMaskTargetAdult = !isAdult;
    sAdultMaskCommitted = 0;
    AdultMask_ResetRunState();
    AdultMask_ResetPlayerVisualState(player);
    if (fastMasks) {
        sAdultMaskCutsceneMode = ADULT_MASK_CS_NONE;
        sAdultMaskSavedAdjLightSettingsValid = 0;
        sAdultMaskReloadBusy = 1;
        player->actor.gravity = 0.0f;
        player->actor.velocity.y = 0.0f;
        player->actor.draw = NULL;
        player->actor.update = AdultMask_FastStartUpdateWrapper;
        return;
    }

    sAdultMaskCutsceneMode = isAdult ? ADULT_MASK_CS_TAKE_OFF_TRANSFORM : ADULT_MASK_CS_PUT_ON;
    AdultMask_SaveEnvVisualState(play);
    player->actor.gravity = 0.0f;
    player->actor.velocity.y = 0.0f;
    Player_BeforeCsAction(play, player);
    Player_SetAction_PreserveItemAction(play, player, AdultMask_Action, 0);
    player->itemAction = PLAYER_CUSTOM_IA_MASK_ADULT;

    if (sAdultMaskCutsceneMode == ADULT_MASK_CS_PUT_ON) {
        AdultMask_SetStartCamera(player, play, CAM_MODE_NORMAL);
        Player_Anim_PlayOnceMorphAdjusted_Ovl(play, player, &gPlayerAnim_cl_setmask);
    } else {
        AdultMask_SetStartCamera(player, play, CAM_MODE_JUMP);
        Player_Anim_PlayOnceMorphAdjusted_Ovl(play, player, &gPlayerAnim_pz_maskoffstart);
    }

    Player_UpdateUpperBodyOrHeldItem(player);
    player->stateFlags1 |= 0x30000000;
    player->stateFlags3 |= 0x80;
}

void AdultMask_AfterStart(Player* player)
{
    (void)player;
}