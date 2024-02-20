#include <combo.h>
#include <combo/custom.h>
#include "../actors.h"

void ArrowCycle_Handle(Actor_Player* link, GameState_Play* play);

void Player_UpdateWrapper(Actor_Player* this, GameState_Play* play)
{
    ArrowCycle_Handle(this, play);
    Player_Update(this, play);
    comboDpadUpdate(play);
    Ocarina_HandleWarp(this, play);
    comboMultiProcessMessages(play);
}

int Player_DpadHook(Actor_Player* this, GameState_Play* play)
{
    if (Player_UsingItem(this))
        return 1;
    return comboDpadUse(play, DPF_ITEMS);
}

void Player_DrawHookshotReticleCustom(GameState_Play* play, Actor_Player* player)
{
    Player_DrawHookshotReticle(play, player, player->heldItemId == 0xF ? 77600.0f : 38600.0f);
}

PATCH_CALL(0x8082fe58, Player_DpadHook);

void Player_TalkDisplayTextBox(GameState_Play* play, s16 textId, Actor* actor)
{
    PlayerDisplayTextBox(play, textId, actor);
}

PATCH_CALL(0x80837bb0, Player_TalkDisplayTextBox);

static s32 sCustomItemActions[] =
{
    PLAYER_CUSTOM_IA_SPELL_WIND,
    PLAYER_CUSTOM_IA_SPELL_LOVE,
    PLAYER_CUSTOM_IA_SPELL_FIRE,
    PLAYER_CUSTOM_IA_BOOTS_IRON,
    PLAYER_CUSTOM_IA_BOOTS_HOVER,
    PLAYER_CUSTOM_IA_TUNIC_GORON,
    PLAYER_CUSTOM_IA_TUNIC_ZORA,
};

static u8 sMagicSpellCosts[] =
{
    12,
    24,
    12,
};

static s16 sMagicSpellActorIds[] =
{
    AC_CUSTOM_SPELL_WIND,
    AC_CUSTOM_SPELL_LOVE,
    AC_CUSTOM_SPELL_FIRE,
};

typedef enum {
    /* 1 */ ANIMSFX_TYPE_1 = 1,
    /* 2 */ ANIMSFX_TYPE_2,
    /* 3 */ ANIMSFX_TYPE_3,
    /* 4 */ ANIMSFX_TYPE_4,
    /* 5 */ ANIMSFX_TYPE_5,
    /* 6 */ ANIMSFX_TYPE_6,
    /* 7 */ ANIMSFX_TYPE_7,
    /* 8 */ ANIMSFX_TYPE_8,
    /* 9 */ ANIMSFX_TYPE_9
} AnimSfxType;

typedef struct AnimSfxEntry {
    /* 0x0 */ u16 sfxId;
    /* 0x2 */ s16 flags; // negative marks the end
} AnimSfxEntry;

#define ANIMSFX_SHIFT_TYPE(type) ((type) << 11)

#define ANIMSFX_DATA(type, frame) ((ANIMSFX_SHIFT_TYPE(type) | ((frame)&0x7FF)))

static AnimSfxEntry sMagicSpellStartSfx[] =
{
    { 0x83D, ANIMSFX_DATA(ANIMSFX_TYPE_1, 20) }, // NA_SE_PL_SKIP
    { 0x6800, ANIMSFX_DATA(ANIMSFX_TYPE_4, 20) }, // NA_SE_VO_LI_SWORD_N
    { 0, -ANIMSFX_DATA(ANIMSFX_TYPE_6, 26) },
};

static AnimSfxEntry sMagicSpellSfx[][2] =
{
    {
        { 0, ANIMSFX_DATA(ANIMSFX_TYPE_8, 20) },
        { 0x6817, -ANIMSFX_DATA(ANIMSFX_TYPE_4, 30) }, // NA_SE_VO_LI_MAGIC_FROL
    },
    {
        { 0, ANIMSFX_DATA(ANIMSFX_TYPE_8, 20) },
        { 0x6815, -ANIMSFX_DATA(ANIMSFX_TYPE_4, 44) }, // NA_SE_VO_LI_MAGIC_NALE
    },
    {
        { 0x681C, ANIMSFX_DATA(ANIMSFX_TYPE_4, 20) }, // NA_SE_VO_LI_MAGIC_ATTACK
        { 0x1818, -ANIMSFX_DATA(ANIMSFX_TYPE_1, 20) }, // NA_SE_IT_SWORD_SWING_HARD
    },
};

static PlayerAnimationHeader* sMagicSpellFirstAnimation[] =
{
    (PlayerAnimationHeader*)0x0400da28, // &gPlayerAnim_link_magic_kaze1,
    (PlayerAnimationHeader*)0x0400da10, // &gPlayerAnim_link_magic_honoo1,
    (PlayerAnimationHeader*)0x0400da40, // &gPlayerAnim_link_magic_tamashii1,
};

static PlayerAnimationHeader* sMagicSpellSecondAnimation[] =
{
    (PlayerAnimationHeader*)0x0400da30, // &gPlayerAnim_link_magic_kaze2,
    (PlayerAnimationHeader*)0x0400da18, // &gPlayerAnim_link_magic_honoo2,
    (PlayerAnimationHeader*)0x0400da48, // &gPlayerAnim_link_magic_tamashii2,
};

static PlayerAnimationHeader* sMagicSpellThirdAnimation[] =
{
    (PlayerAnimationHeader*)0x0400da38, // &gPlayerAnim_link_magic_kaze3,
    (PlayerAnimationHeader*)0x0400da20, // &gPlayerAnim_link_magic_honoo3,
    (PlayerAnimationHeader*)0x0400da50, // &gPlayerAnim_link_magic_tamashii3,
};

static u8 sMagicSpellLoopTimer[] = { 70, 10, 10 };

s32 Player_CustomItemToItemAction(Actor_Player* this, s32 item, s32 itemAction)
{
    s32 customItem = item - ITEM_MM_CUSTOM_MIN;
    if (customItem < 0)
    {
        return itemAction;
    }
    if (customItem >= ARRAY_SIZE(sCustomItemActions))
    {
        return 0;
    }
    return sCustomItemActions[customItem];
}

static s32 Player_ActionToMagicSpell(Actor_Player* this, s32 itemAction)
{
    s32 magicSpell = itemAction - PLAYER_CUSTOM_IA_SPELL_MIN;

    if ((magicSpell >= PLAYER_MAGIC_SPELL_MIN) && (magicSpell <= PLAYER_MAGIC_SPELL_MAX))
    {
        return magicSpell;
    }
    else
    {
        return -1;
    }
}

static s32 Player_ActionToBoots(Actor_Player* this, s32 itemAction)
{
    s32 boots = itemAction - PLAYER_CUSTOM_IA_BOOTS_MIN;

    if ((boots >= PLAYER_BOOTS_MIN) && (boots <= PLAYER_BOOTS_MAX))
    {
        return boots;
    }
    else
    {
        return -1;
    }
}

static s32 Player_ActionToTunic(Actor_Player* this, s32 itemAction)
{
    s32 tunic = itemAction - PLAYER_CUSTOM_IA_TUNIC_MIN + PLAYER_TUNIC_MIN;

    if ((tunic >= PLAYER_TUNIC_MIN) && (tunic <= PLAYER_TUNIC_MAX))
    {
        return tunic;
    }
    else
    {
        return -1;
    }
}

Actor* Player_SpawnMagicSpellActor(GameState_Play* play, Actor_Player* this, s8 magicSpell)
{
    return SpawnActor(&play->actorCtx, play, sMagicSpellActorIds[magicSpell], this->base.world.pos.x, this->base.world.pos.y, this->base.world.pos.z, 0, 0, 0, 0);
}      // size = 0x4

typedef void (*Player_PlayAnimSfx)(Actor_Player*, AnimSfxEntry*);
typedef void (*Player_func_80832F24)(Actor_Player*);
typedef void (*Player_func_8083249C)(Actor_Player*);
typedef void (*Player_StopCutscene)(Actor_Player*);

extern Vec3f gFwPointerPos;

void Player_Action_CastingSpell(Actor_Player* this, GameState_Play* play)
{
    PlayerActionFunc func_808369F4;
    Player_PlayAnimSfx Player_PlayAnimSfx;
    Player_func_80832F24 Player_func_80832F24;
    Player_func_8083249C Player_func_8083249C;
    Player_StopCutscene Player_StopCutscene;

    if (PlayerAnimation_Update(play, &this->skelAnime))
    {
        if (this->av1.actionVar1 < 0)
        {
            if ((this->itemAction == PLAYER_CUSTOM_IA_SPELL_LOVE) || (gSaveContext.magicState == MAGIC_STATE_IDLE))
            {
                func_808369F4 = OverlayAddr(0x808369F4);
                func_808369F4(this, play);

                Player_StopCutscene = OverlayAddr(0x80838760);
                Player_StopCutscene(this);
            }
        }
        else
        {
            if (this->av2.actionVar2 == 0)
            {
                PlayerAnimation_PlayOnceSetSpeed(play, &this->skelAnime, sMagicSpellFirstAnimation[this->av1.actionVar1], 0.83f);

                if (Player_SpawnMagicSpellActor(play, this, this->av1.actionVar1) != NULL)
                {
                    this->state |= 0x30000000; // PLAYER_STATE1_20000000 | PLAYER_STATE1_10000000
                    if ((this->av1.actionVar1 != 0) || (gSaveContext.respawn[RESPAWN_MODE_HUMAN].data <= 0))
                    {
                        gSaveContext.magicState = MAGIC_STATE_CONSUME_SETUP;
                    }
                }
                else
                {
                    Magic_Reset(play);
                }
            }
            else
            {
                PlayerAnimation_PlayLoopSetSpeed(play, &this->skelAnime, sMagicSpellSecondAnimation[this->av1.actionVar1], 0.83f);

                if (this->av1.actionVar1 == 0)
                {
                    this->av2.actionVar2 = -10;
                }
            }

            this->av2.actionVar2++;
        }
    }
    else
    {
        if (this->av2.actionVar2 < 0)
        {
            this->av2.actionVar2++;

            if (this->av2.actionVar2 == 0)
            {
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 1;
                Play_SetupRespawnPoint(play, RESPAWN_MODE_HUMAN, 0x6ff);
                gSave.fw = gSaveContext.respawn[RESPAWN_MODE_DOWN];
                gSave.fw.playerParams = 0x6ff;
                gSave.fw.data = 40;

                // Copy Game Over / Soar to Entrance respawn data.
                gSave.fwRespawnTop = gSaveContext.respawn[RESPAWN_MODE_TOP];

                this->av2.actionVar2 = 2;
            }
        }
        else if (this->av1.actionVar1 >= 0)
        {
            Player_PlayAnimSfx = OverlayAddr(0x8082E224);
            if (this->av2.actionVar2 == 0)
            {
                Player_PlayAnimSfx(this, sMagicSpellStartSfx);
            }
            else if (this->av2.actionVar2 == 1)
            {
                Player_PlayAnimSfx(this, sMagicSpellSfx[this->av1.actionVar1]);
                if ((this->av1.actionVar1 == 2) && PlayerAnimation_OnFrame(&this->skelAnime, 30.0f))
                {
                    this->state &= ~0x30000000; // PLAYER_STATE1_20000000 | PLAYER_STATE1_10000000
                }
            }
            else if (sMagicSpellLoopTimer[this->av1.actionVar1] < this->av2.actionVar2++)
            {
                PlayerAnimation_PlayOnceSetSpeed(play, &this->skelAnime, sMagicSpellThirdAnimation[this->av1.actionVar1], 0.83f);
                this->currentYaw = this->base.rot2.y;
                this->av1.actionVar1 = -1;
            }
        }
    }

    Player_func_80832F24 = OverlayAddr(0x80832F24);
    Player_func_80832F24(this);

    Player_func_8083249C = OverlayAddr(0x8083249c);
    Player_func_8083249C(this);
}

typedef void (*Player_SetAction_PreserveItemAction)(GameState_Play*, Actor_Player*, PlayerActionFunc, s32);

void Player_CastSpell(GameState_Play* play, Actor_Player* this, s32 magicSpell)
{
    Player_SetAction_PreserveItemAction Player_SetAction_PreserveItemAction;
    Player_func_8083249C Player_func_8083249C;

    if (Magic_RequestChange(play, sMagicSpellCosts[magicSpell], MAGIC_CONSUME_WAIT_PREVIEW))
    {
        Player_SetAction_PreserveItemAction = OverlayAddr(0x80831760);
        Player_SetAction_PreserveItemAction(play, this, Player_Action_CastingSpell, 0);

        this->av1.actionVar1 = magicSpell; // - 3?

        PlayerAnimation_PlayOnceSetSpeed(play, &this->skelAnime, (PlayerAnimationHeader*)0x0400da58, 0.83f); // gPlayerAnim_link_magic_tame

        this->csId = 0x7B; // CS_ID_GLOBAL_TALK
        Player_func_8083249C = OverlayAddr(0x8083249c);
        Player_func_8083249C(this);

        this->state3 |= 0x80; // PLAYER_STATE3_80
    }
}

typedef void (*Player_UpdateUpperBody)(Actor_Player*, GameState_Play*);
typedef void (*Player_func_8085B384)(Actor_Player*, GameState_Play*);

void Player_Action_FaroresWindText(Actor_Player* this, GameState_Play* play)
{
    Player_UpdateUpperBody Player_UpdateUpperBody = OverlayAddr(0x8083216c);
    Player_StopCutscene Player_StopCutscene;
    Player_func_8085B384 Player_func_8085B384;
    char* b;

    this->state3 |= 0x80; // PLAYER_STATE3_80

    PlayerAnimation_Update(play, &this->skelAnime);
    Player_UpdateUpperBody(this, play);

    if (this->av2.actionVar2 == 0)
    {
        // Message Id is the same as OoT, but here it's the Great Fairy's Sword get-item message. Doesn't matter though.
        PlayerDisplayTextBox(play, 0x3B, &this->base);
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; // Use No icon.
        b += 11;
        comboTextAppendStr(&b, TEXT_FAST "You cast Farore's Wind!" TEXT_NL);
        comboTextAppendStr(&b, TEXT_COLOR_GREEN TEXT_CHOICE3 "     Return to the Warp Point" TEXT_NL "     Dispel the Warp Point" TEXT_NL "     Exit" TEXT_END);
        this->av2.actionVar2 = 1;
        return;
    }

    // MSGMODE_TEXT_DONE
    if (play->msgCtx.msgMode == 0x42 && Message_ShouldAdvance(play))
    {
        Audio_PlaySfx_MessageDecide();

        Message_Close(play);

        if (play->msgCtx.choiceIndex == 0)
        {
            gSaveContext.respawnFlag = 8;
            play->transitionTrigger = TRANS_TRIGGER_NORMAL;
            play->nextEntrance = gSaveContext.respawn[RESPAWN_MODE_HUMAN].entrance;
            play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;

            // Restore Game Over / Soar to Entrance respawn data.
            gSaveContext.respawn[RESPAWN_MODE_TOP] = gSaveContext.save.fwRespawnTop;

            // TODO cancel timers?

            return;
        }

        if (play->msgCtx.choiceIndex == 1)
        {
            gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = -gSaveContext.respawn[RESPAWN_MODE_HUMAN].data;
            gSave.fw.data = 0;
            Audio_PlaySfx_AtPos(&gSaveContext.save.fw.pos, 0x8C8); // NA_SE_PL_MAGIC_WIND_VANISH
        }

        Player_func_8085B384 = OverlayAddr(0x8085B384);
        Player_func_8085B384(this, play);

        Player_StopCutscene = OverlayAddr(0x80838760);
        Player_StopCutscene(this);
    }
}

typedef void (*Player_func_8082DAD4)(Actor_Player*);
typedef PlayerAnimationHeader* (*Player_GetWaitAnimation)(Actor_Player*);
typedef void (*Player_SetAction)(GameState_Play*, Actor_Player*, PlayerActionFunc, s32);

s32 Player_CustomCsItem(Actor_Player* this, GameState_Play* play)
{
    Player_func_8082DAD4 func_8082DAD4;
    Player_func_8083249C Player_func_8083249C;
    Player_GetWaitAnimation Player_GetWaitAnimation;
    Player_SetAction Player_SetAction;
    s32 magicSpell = Player_ActionToMagicSpell(this, this->itemAction);
    if (magicSpell >= PLAYER_MAGIC_SPELL_MIN)
    {
        if ((magicSpell != PLAYER_MAGIC_SPELL_WIND) || (gSaveContext.respawn[RESPAWN_MODE_HUMAN].data <= 0))
        {
            Player_CastSpell(play, this, magicSpell);
        }
        else
        {
            Player_SetAction = OverlayAddr(0x80831494);
            Player_SetAction(play, this, Player_Action_FaroresWindText, 1);
            this->state |= 0x30000000; // PLAYER_STATE1_20000000 | PLAYER_STATE1_10000000
            Player_GetWaitAnimation = OverlayAddr(0x8082ED20);
            PlayerAnimation_PlayOnce(play, &this->skelAnime, Player_GetWaitAnimation(this));
            this->csId = 0x7B; // CS_ID_GLOBAL_TALK
            Player_func_8083249C = OverlayAddr(0x8083249c);
            Player_func_8083249C(this);
        }
        func_8082DAD4 = OverlayAddr(0x8082DAD4);
        func_8082DAD4(this);
        return 1;
    }
    else if (this->itemAction >= 0x3a && this->itemAction <= 0x51) // PLAYER_IA_MASK_MIN // PLAYER_IA_MASK_MAX
    {
        return -1;
    }
    return 0;
}

s16 sBootData[3][18] = {
    { 200, 1000, 300, 700, 550, 270, 1000, 0, 120, 800, 300, -160, 600, 590, 750, 125, 200, 130 },
    { 200, 1000, 300, 700, 550, 270, 600, 600, 120, 800, 550, -100, 600, 540, 270, 25, 0, 130 },
    { 80, 800, 150, 700, 480, 270, 600, 50, 120, 800, 550, -40, 400, 540, 270, 25, 0, 80 },
};

void Player_CheckCustomBoots(GameState_Play* play)
{
    Actor_Player* player = GET_LINK(play);
    if (player->transformation == MM_PLAYER_FORM_HUMAN)
    {
        player->base.flags &= ~(1 << 17); // ~ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCH
        player->base.flags |= (1 << 26); // ACTOR_FLAG_CAN_PRESS_SWITCH

        if (gSaveContext.save.itemEquips.boots > 0)
        {
            s32 currentBoots = gSaveContext.save.itemEquips.boots - 1;
            s16* bootRegs;

            REG(27) = 2000;
            REG(48) = 370;

            switch(currentBoots)
            {
            case PLAYER_BOOTS_IRON:
                player->currentBoots = 6; // PLAYER_BOOTS_GORON
                player->base.flags |= (1 << 17); // ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCH
                if (player->state & PLAYER_ACTOR_STATE_WATER)
                {
                    currentBoots = 2; // Iron Underwater
                }
                REG(27) = 500;
                REG(48) = 100;
                break;
            case PLAYER_BOOTS_HOVER:
                player->currentBoots = 0; // PLAYER_BOOTS_FIERCE_DEITY
                player->base.flags &= ~(1 << 26); // ~ACTOR_FLAG_CAN_PRESS_SWITCH
                break;
            }

            bootRegs = sBootData[currentBoots];
            REG(19) = bootRegs[0];
            REG(30) = bootRegs[1];
            REG(32) = bootRegs[2];
            REG(34) = bootRegs[3];
            REG(35) = bootRegs[4];
            REG(36) = bootRegs[5];
            REG(37) = bootRegs[6];
            REG(38) = bootRegs[7];
            REG(39) = bootRegs[8];
            REG(43) = bootRegs[9];
            R_RUN_SPEED_LIMIT = bootRegs[10];
            REG(68) = bootRegs[11];
            REG(69) = bootRegs[12];
            IREG(66) = bootRegs[13];
            IREG(67) = bootRegs[14];
            IREG(68) = bootRegs[15];
            IREG(69) = bootRegs[16];
            MREG(95) = bootRegs[17];

            if (play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_2)
            {
                R_RUN_SPEED_LIMIT = 500;
            }
        }
    }
    else
    {
        gSaveContext.save.itemEquips.boots = 0;
        gSaveContext.save.itemEquips.tunic = 0;
    }
}

s32 Player_CustomUseItem(Actor_Player* this, GameState_Play* play, s32 itemAction)
{
    s32 magicSpell = Player_ActionToMagicSpell(this, itemAction);
    if (magicSpell >= 0)
    {
        if (((magicSpell == PLAYER_MAGIC_SPELL_WIND) && (gSaveContext.respawn[RESPAWN_MODE_HUMAN].data > 0)) ||
            ((gSaveContext.magicCapacity != 0) && (gSaveContext.magicState == MAGIC_STATE_IDLE) &&
             (gSaveContext.save.playerData.magicAmount >= sMagicSpellCosts[magicSpell])))
        {
            this->unk_AA5 = 5;
            this->itemAction = itemAction;
        }
        else
        {
            PlaySound(0x4806); // NA_SE_SY_ERROR
        }
        // Handled
        return 1;
    }

    s32 boots = Player_ActionToBoots(this, itemAction);
    if (boots >= 0)
    {
        if (this->transformation == MM_PLAYER_FORM_HUMAN)
        {
            u16 newBoots = boots + 1;
            if (gSaveContext.save.itemEquips.boots == newBoots)
            {
                newBoots = 0;
            }
            gSaveContext.save.itemEquips.boots = newBoots;

            if (!newBoots)
            {
                this->currentBoots = 1; // PLAYER_BOOTS_HYLIAN
            }

            Player_SetBootData(play, this);
            Player_PlaySfx(this, 0x835); // NA_SE_PL_CHANGE_ARMS
        }

        // Handled
        return 1;
    }

    s32 tunic = Player_ActionToTunic(this, itemAction);
    if (tunic >= 0)
    {
        if (this->transformation == MM_PLAYER_FORM_HUMAN)
        {
            u16 newTunic = tunic;
            if (gSaveContext.save.itemEquips.tunic == newTunic)
            {
                newTunic = 0;
            }
            gSaveContext.save.itemEquips.tunic = newTunic;

            Player_PlaySfx(this, 0x835); // NA_SE_PL_CHANGE_ARMS
        }

        // Handled
        return 1;
    }

    // PLAYER_IA_MASK_MIN = 0x3A,
    // PLAYER_IA_MASK_MAX = 0x51,
    // PLAYER_IA_MASK_GIANT = 0x4D,
    if (itemAction >= 0x3A && itemAction <= 0x51 && (this->transformation != MM_PLAYER_FORM_HUMAN || itemAction >= 0x4D))
    {
        // Vanilla code doesn't need to check more conditions.
        return -1;
    }

    return 0;
}

typedef void (*Player_func_80833AA0)(Actor_Player*, GameState_Play*);
typedef void (*Player_Draw)(Actor_Player*, GameState_Play*);

void Player_Action_FaroresWindSpawning(Actor_Player* this, GameState_Play* play)
{
    Player_func_80833AA0 Player_func_80833AA0;
    Player_Draw Player_Draw;
    if (this->av2.actionVar2 > 20)
    {
        Player_Draw = OverlayAddr(0x80846528);
        this->base.draw = Player_Draw;
        this->base.world.pos.y += 60.0f;
        Player_func_80833AA0 = OverlayAddr(0x80833AA0);
        Player_func_80833AA0(this, play);
        return;
    }

    if (this->av2.actionVar2++ == 20)
    {
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].data++;
        Audio_PlaySfx_AtPos(&gSaveContext.save.fw.pos, 0x87B); // NA_SE_PL_MAGIC_WIND_WARP
    }
}

void Player_InitFaroresWindRespawn(GameState_Play* play, Actor_Player* this)
{
    Player_SetAction Player_SetAction = OverlayAddr(0x80831494);
    this->base.draw = NULL;
    Player_SetAction(play, this, Player_Action_FaroresWindSpawning, 0);
    this->state |= 0x20000000; // PLAYER_STATE1_20000000
}

void Player_AfterInit(GameState_Play* play)
{
    // Displaced code:
    Map_SetAreaEntrypoint(play);

    Actor_Player* player = GET_LINK(play);

    if (gSaveContext.nayrusLoveTimer != 0)
    {
        gSaveContext.magicState = MAGIC_STATE_METER_FLASH_1;
        Player_SpawnMagicSpellActor(play, player, PLAYER_MAGIC_SPELL_LOVE);
    }

    Actor_InitFaroresWind(play);

    if (gSaveContext.respawnFlag == 8)
    {
        Player_InitFaroresWindRespawn(play, player);
    }
}

PATCH_CALL(0x808424d8, Player_AfterInit);

const u32 gLinkAdultIronBootTex = 0x08000000 | CUSTOM_KEEP_BOOTS_IRON_TEXTURE;
const u32 gLinkAdultIronBootTLUT = 0x08000000 | CUSTOM_KEEP_BOOTS_IRON_TLUT;

const Vtx sIronBootsVtx[87] = {
#include "sIronBootsVtx.vtx.inc"
};

const Gfx gLinkAdultLeftIronBootDL[] = {
    gsSPMatrix(0x0D000180, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultIronBootTex, G_IM_FMT_CI, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadTLUT_pal256(gLinkAdultIronBootTLUT),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsSPDisplayList(0x0C000000),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sIronBootsVtx[0], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 1, 3, 0),
    gsSP2Triangles(2, 3, 4, 0, 5, 6, 7, 0),
    gsSP2Triangles(5, 7, 8, 0, 4, 9, 10, 0),
    gsSP2Triangles(4, 10, 11, 0, 12, 13, 14, 0),
    gsSP2Triangles(12, 14, 15, 0, 12, 15, 5, 0),
    gsSP2Triangles(12, 5, 8, 0, 16, 17, 18, 0),
    gsSP2Triangles(19, 20, 21, 0, 18, 17, 22, 0),
    gsSP2Triangles(0, 23, 24, 0, 11, 25, 21, 0),
    gsSP2Triangles(23, 22, 26, 0, 26, 22, 17, 0),
    gsSP2Triangles(23, 26, 24, 0, 27, 28, 29, 0),
    gsSP2Triangles(29, 30, 31, 0, 29, 31, 27, 0),
    gsSP1Triangle(25, 19, 21, 0),
    gsSPVertex(&sIronBootsVtx[32], 11, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 3, 7, 0, 0),
    gsSP1Triangle(8, 9, 10, 0),
    gsSPEndDisplayList(),
};

const Gfx gLinkAdultRightIronBootDL[] = {
    gsSPMatrix(0x0D0000C0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultIronBootTex, G_IM_FMT_CI, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadTLUT_pal256(gLinkAdultIronBootTLUT),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsSPDisplayList(0x0C000000),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sIronBootsVtx[43], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
    gsSP2Triangles(4, 3, 0, 0, 5, 6, 7, 0),
    gsSP2Triangles(5, 7, 8, 0, 9, 3, 4, 0),
    gsSP2Triangles(10, 9, 4, 0, 5, 8, 11, 0),
    gsSP2Triangles(5, 11, 12, 0, 5, 12, 13, 0),
    gsSP2Triangles(5, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 15, 17, 0, 19, 16, 15, 0),
    gsSP2Triangles(20, 21, 2, 0, 18, 22, 10, 0),
    gsSP2Triangles(23, 24, 21, 0, 16, 19, 25, 0),
    gsSP2Triangles(20, 23, 21, 0, 26, 27, 28, 0),
    gsSP2Triangles(28, 29, 30, 0, 28, 30, 26, 0),
    gsSP1Triangle(18, 17, 22, 0),
    gsSPVertex(&sIronBootsVtx[74], 13, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP1Triangle(10, 11, 12, 0),
    gsSPEndDisplayList(),
};

const u32 gLinkAdultHoverBootsHeelTex = 0x08000000 | CUSTOM_KEEP_BOOTS_HOVER_HEEL_TEXTURE;
const u32 gLinkAdultHoverBootsJetTex = 0x08000000 | CUSTOM_KEEP_BOOTS_HOVER_JET_TEXTURE;
const u32 gLinkAdultHoverBootsFeatherTex = 0x08000000 | CUSTOM_KEEP_BOOTS_HOVER_FEATHER_TEXTURE;

const Vtx sHoverBootsVtx[154] = {
#include "sHoverBootsVtx.vtx.inc"
};

const Gfx gLinkAdultLeftHoverBootDL[] = {
    gsSPMatrix(0x0D000180, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultHoverBootsHeelTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 8, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 4, 3, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsSPDisplayList(0x0C000000),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sHoverBootsVtx[0], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 8, 13, 9, 0),
    gsSP2Triangles(11, 14, 12, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(21, 23, 24, 0, 25, 26, 5, 0),
    gsSP2Triangles(25, 5, 4, 0, 27, 28, 29, 0),
    gsSPVertex(&sHoverBootsVtx[30], 12, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 6, 8, 9, 0),
    gsSP2Triangles(9, 10, 11, 0, 9, 11, 6, 0),
    gsDPPipeSync(),
    gsDPLoadTextureBlock(gLinkAdultHoverBootsJetTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&sHoverBootsVtx[42], 27, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 3, 6, 0),
    gsSP2Triangles(10, 11, 12, 0, 11, 13, 14, 0),
    gsSP2Triangles(15, 16, 17, 0, 18, 19, 20, 0),
    gsSP2Triangles(21, 14, 22, 0, 22, 14, 13, 0),
    gsSP2Triangles(9, 4, 3, 0, 13, 10, 23, 0),
    gsSP2Triangles(20, 19, 24, 0, 0, 20, 24, 0),
    gsSP2Triangles(5, 6, 3, 0, 10, 13, 11, 0),
    gsSP2Triangles(5, 7, 6, 0, 23, 22, 13, 0),
    gsSP2Triangles(24, 1, 0, 0, 8, 25, 9, 0),
    gsSP2Triangles(6, 8, 9, 0, 2, 26, 0, 0),
    gsSP2Triangles(0, 26, 20, 0, 26, 18, 20, 0),
    gsDPPipeSync(),
    gsDPLoadTextureBlock(gLinkAdultHoverBootsFeatherTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 16, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPVertex(&sHoverBootsVtx[69], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSPEndDisplayList(),
};

const Gfx gLinkAdultRightHoverBootDL[] = {
    gsSPMatrix(0x0D0000C0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultHoverBootsHeelTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 8, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 4, 3, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsSPDisplayList(0x0C000000),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sHoverBootsVtx[77], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 7, 13, 8, 0),
    gsSP2Triangles(10, 14, 11, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 21, 23, 0, 5, 25, 26, 0),
    gsSP2Triangles(6, 5, 26, 0, 27, 28, 29, 0),
    gsSPVertex(&sHoverBootsVtx[107], 12, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 6, 8, 0),
    gsSP2Triangles(10, 11, 9, 0, 8, 10, 9, 0),
    gsDPPipeSync(),
    gsDPLoadTextureBlock(gLinkAdultHoverBootsJetTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&sHoverBootsVtx[119], 27, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 8, 5, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 11, 0),
    gsSP2Triangles(15, 16, 17, 0, 18, 19, 20, 0),
    gsSP2Triangles(21, 13, 22, 0, 14, 13, 21, 0),
    gsSP2Triangles(5, 4, 9, 0, 23, 12, 14, 0),
    gsSP2Triangles(24, 19, 18, 0, 24, 18, 2, 0),
    gsSP2Triangles(5, 8, 3, 0, 11, 14, 12, 0),
    gsSP2Triangles(8, 7, 3, 0, 14, 21, 23, 0),
    gsSP2Triangles(2, 1, 24, 0, 9, 25, 6, 0),
    gsSP2Triangles(9, 6, 8, 0, 2, 26, 0, 0),
    gsSP2Triangles(18, 26, 2, 0, 18, 20, 26, 0),
    gsDPPipeSync(),
    gsDPLoadTextureBlock(gLinkAdultHoverBootsFeatherTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 16, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPVertex(&sHoverBootsVtx[146], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSPEndDisplayList(),
};

const Vtx sHoverBootsCircleVtx[] = {
#include "sHoverBootsCircleVtx.vtx.inc"
};

const u32 gEffUnknown13Tex = 0x08000000 | CUSTOM_KEEP_BOOTS_HOVER_CIRCLE_TEXTURE;
const u32 gEffUnknown14Tex = 0x04000000 | 0x1aab0;

const Gfx gHoverBootsCircleDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffUnknown13Tex, G_IM_FMT_I, G_IM_SIZ_8b, 16, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR
                         | G_TX_CLAMP, 4, 5, G_TX_NOLOD, 1),
    gsDPLoadMultiBlock(gEffUnknown14Tex, 0x0100, 1, G_IM_FMT_I, G_IM_SIZ_8b, 16, 32, 0, G_TX_MIRROR | G_TX_WRAP,
                       G_TX_NOMIRROR | G_TX_WRAP, 4, 5, 15, 14),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, ENV_ALPHA, TEXEL0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, ENVIRONMENT,
                       COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPDisplayList(0x09000000),
    gsSPVertex(sHoverBootsCircleVtx, 13, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 2, 0),
    gsSP2Triangles(5, 6, 2, 0, 7, 8, 2, 0),
    gsSP2Triangles(9, 10, 2, 0, 11, 12, 2, 0),
    gsSPEndDisplayList(),
};

const Vtx sGoronBraceletVtx[] = {
#include "sGoronBraceletVtx.vtx.inc"
};

const u32 gLinkChildGoronBraceletTex = 0x08000000 | CUSTOM_KEEP_GORON_BRACELET_TEXTURE;
const u32 gLinkChildGoronSymbolTex = 0x08000000 | CUSTOM_KEEP_GORON_SYMBOL_TEXTURE;

Gfx gLinkChildGoronBraceletDL[] = {
    gsSPMatrix(0x0D0002C0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x012C, 0x0190, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkChildGoronBraceletTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, 0, G_TX_MIRROR | G_TX_WRAP,
                         G_TX_MIRROR | G_TX_WRAP, 3, 3, G_TX_NOLOD, 15),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGoronBraceletVtx[0], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&sGoronBraceletVtx[3], 14, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 0, 2, 0, 6, 2, 7, 0),
    gsSP2Triangles(8, 9, 1, 0, 10, 6, 7, 0),
    gsSP2Triangles(10, 7, 3, 0, 11, 12, 8, 0),
    gsSP2Triangles(5, 12, 11, 0, 11, 10, 3, 0),
    gsSP2Triangles(11, 3, 5, 0, 13, 11, 1, 0),
    gsSP1Triangle(1, 11, 8, 0),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkChildGoronSymbolTex, G_IM_FMT_IA, G_IM_SIZ_16b, 16, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_DECAL2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGoronBraceletVtx[17], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 4, 6, 0),
    gsSPEndDisplayList(),
};

const Vtx sGauntletVtx[] = {
#include "sGauntletVtx.vtx.inc"
};

const u32 gLinkAdultGauntletPlate1Tex = 0x08000000 | CUSTOM_KEEP_GAUNTLET1_TEXTURE;

Gfx gLinkAdultLeftGauntletPlate1DL[] = {
    gsSPMatrix(0x0D0002C0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultGauntletPlate1Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 32, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGauntletVtx[0], 24, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 5, 4, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 12, 13, 0),
    gsSP2Triangles(11, 13, 14, 0, 8, 15, 9, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 20, 21, 0),
    gsSP2Triangles(18, 22, 16, 0, 19, 21, 23, 0),
    gsSPEndDisplayList(),
};

const u32 gLinkAdultGauntletPlate2Tex = 0x08000000 | CUSTOM_KEEP_GAUNTLET2_TEXTURE;

const u32 gHilite2Tex = 0x04000000 | 0xca30;

// gLinkAdultLeftGauntletPlate2DL is omitted because we always render the "closed hand" variant so it doesn't clip into the link's hand

Gfx gLinkAdultLeftGauntletPlate3DL[] = {
    gsSPMatrix(0x0D000300, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultGauntletPlate2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGauntletVtx[24], 26, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 6, 5, 13, 0),
    gsSP2Triangles(2, 1, 14, 0, 11, 15, 12, 0),
    gsSP2Triangles(15, 11, 16, 0, 8, 7, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 20, 21, 18, 0),
    gsSP2Triangles(22, 23, 24, 0, 24, 25, 22, 0),
    gsDPPipeSync(),
    gsSPTexture(0x2710, 0x2710, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gHilite2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR |
                         G_TX_WRAP, 4, 4, 1, 15),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPSetGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPDisplayList(0x0C000000),
    gsDPSetPrimColor(0, 0, 254, 15, 15, 255),
    gsSPVertex(&sGauntletVtx[50], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&sGauntletVtx[53], 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSPEndDisplayList(),
};

Gfx gLinkAdultRightGauntletPlate1DL[] = {
    gsSPMatrix(0x0D000380, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultGauntletPlate1Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 32, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGauntletVtx[62], 24, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 6, 5, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 12, 13, 0),
    gsSP2Triangles(14, 11, 13, 0, 9, 15, 10, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 20, 21, 0),
    gsSP2Triangles(18, 22, 16, 0, 23, 19, 21, 0),
    gsSPEndDisplayList(),
};

// gLinkAdultRightGauntletPlate2DL is omitted because we always render the "closed hand" variant so it doesn't clip into the link's hand

Gfx gLinkAdultRightGauntletPlate3DL[] = {
    gsSPMatrix(0x0D0003C0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultGauntletPlate2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGauntletVtx[86], 26, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 5, 4, 0),
    gsSP2Triangles(14, 1, 0, 0, 10, 15, 11, 0),
    gsSP2Triangles(16, 11, 15, 0, 17, 9, 8, 0),
    gsSP2Triangles(18, 19, 20, 0, 20, 21, 18, 0),
    gsSP2Triangles(22, 23, 24, 0, 24, 25, 22, 0),
    gsDPPipeSync(),
    gsSPTexture(0x2710, 0x2710, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gHilite2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR |
                         G_TX_WRAP, 4, 4, 1, 15),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPSetGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPDisplayList(0x0C000000),
    gsDPSetPrimColor(0, 0, 254, 15, 15, 255),
    gsSPVertex(&sGauntletVtx[112], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&sGauntletVtx[115], 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSPEndDisplayList(),
};

static Color_RGB8 sGauntletColors[] = {
    { 255, 255, 255 },
    { 254, 207, 15 },
};

static Vec3s sHoverBootsRot = { 0, 0, 0 };
static s32 sHoverBootCircleAlpha = 255;

static Color_RGB8 sTunicColors[4] = {
    { 30, 105, 27 }, // PLAYER_TUNIC_NONE (default in MM)
    { 30, 105, 27 }, // PLAYER_TUNIC_KOKIRI (will not be used)
    { 100, 20, 0 },  // PLAYER_TUNIC_GORON
    { 0, 60, 100 },  // PLAYER_TUNIC_ZORA
};

COSMETIC(MM_COLOR_TUNIC_KOKIRI, sTunicColors[0]);
COSMETIC(MM_COLOR_TUNIC_GORON, sTunicColors[2]);
COSMETIC(MM_COLOR_TUNIC_ZORA, sTunicColors[3]);

void Player_SkelAnime_DrawFlexLod(GameState_Play* play, void** skeleton, Vec3s* jointTable, s32 dListCount,
                                  OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw, Actor_Player* player, s32 lod)
{
    OPEN_DISPS(play->gs.gfx);

    if (player->transformation == MM_PLAYER_FORM_HUMAN)
    {
        Color_RGB8* tunicColor;
        u16 tunic;

        tunic = CLAMP(gSaveContext.save.itemEquips.tunic, 0, 3);
        tunicColor = &sTunicColors[tunic];
        gDPSetEnvColor(POLY_OPA_DISP++, tunicColor->r, tunicColor->g, tunicColor->b, 0xFF);
    }

    SkelAnime_DrawFlexLod(play, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, &player->base, lod);

    gSPSegment(POLY_OPA_DISP++, 0x08, gCustomKeep);
    gSPSegment(POLY_XLU_DISP++, 0x08, gCustomKeep);

    if (overrideLimbDraw != Player_OverrideLimbDrawGameplayFirstPerson && gSaveContext.gameMode != 3) // GAMEMODE_END_CREDITS
    {
        switch (GET_PLAYER_CUSTOM_BOOTS(player))
        {
        case PLAYER_BOOTS_IRON:
            gSPDisplayList(POLY_OPA_DISP++, gLinkAdultLeftIronBootDL);
            gSPDisplayList(POLY_OPA_DISP++, gLinkAdultRightIronBootDL);
            break;
        case PLAYER_BOOTS_HOVER:
            gSPDisplayList(POLY_OPA_DISP++, gLinkAdultLeftHoverBootDL);
            gSPDisplayList(POLY_OPA_DISP++, gLinkAdultRightHoverBootDL);
            break;
        }

        if (player->transformation == MM_PLAYER_FORM_HUMAN && comboConfig(CFG_MM_STRENGTH))
        {
            Color_RGB8* gauntletColor;
            s32 strength = gSaveContext.save.inventory.upgrades.strength;
            switch (strength)
            {
                case 1:
                    gSPDisplayList(POLY_OPA_DISP++, gLinkChildGoronBraceletDL);
                    break;
                case 2:
                case 3:
                    gauntletColor = &sGauntletColors[strength - 2];
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetEnvColor(POLY_OPA_DISP++, gauntletColor->r, gauntletColor->g, gauntletColor->b, 0);

                    gSPDisplayList(POLY_OPA_DISP++, gLinkAdultLeftGauntletPlate1DL);
                    gSPDisplayList(POLY_OPA_DISP++, gLinkAdultRightGauntletPlate1DL);
                    gSPDisplayList(POLY_OPA_DISP++, gLinkAdultLeftGauntletPlate3DL);
                    gSPDisplayList(POLY_OPA_DISP++, gLinkAdultRightGauntletPlate3DL);
                    break;
            }
        }
    }

    if (GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_HOVER && !(player->base.bgCheckFlags & BGCHECKFLAG_GROUND)
        && !(player->state & (1 << 23)) && player->hoverBootsTimer != 0)
    {
        s32 sp5C;
        s32 hoverBootsTimer = player->hoverBootsTimer;

        if (player->hoverBootsTimer < 19) {
            if (hoverBootsTimer >= 15) {
                sHoverBootCircleAlpha = (19 - hoverBootsTimer) * 51.0f;
            } else if (hoverBootsTimer < 19) {
                sp5C = hoverBootsTimer;

                if (sp5C > 9) {
                    sp5C = 9;
                }

                sHoverBootCircleAlpha = (-sp5C * 4) + 36;
                sHoverBootCircleAlpha = SQ(sHoverBootCircleAlpha);
                sHoverBootCircleAlpha = (s32)((Math_CosS(sHoverBootCircleAlpha) * 100.0f) + 100.0f) + 55.0f;
                sHoverBootCircleAlpha *= sp5C * (1.0f / 9.0f);
            }

            Matrix_SetTranslateRotateYXZ(player->base.world.pos.x, player->base.world.pos.y + 2.0f,
                                         player->base.world.pos.z, &sHoverBootsRot);
            ModelViewScale(4.0f, 4.0f, 4.0f, MAT_MUL);

            gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(POLY_XLU_DISP++, 0x09,
                       DisplaceTexture(play->gs.gfx, G_TX_RENDERTILE, 0, 0, 16, 32, 1, 0,
                                        (play->gameplayFrames * -15) % 128, 16, 32));
            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 255, sHoverBootCircleAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 120, 90, 30, 128);
            gSPDisplayList(POLY_XLU_DISP++, gHoverBootsCircleDL);
        }
    }

    CLOSE_DISPS();
}

void Player_ColorAfterMask(GfxContext* gfxCtx, s32 maskIDMinusOne, PlayerMaskDList* maskDList, Actor_Player* player) {
    u32 dl = maskDList->maskDListEntry[maskIDMinusOne];
    gSPDisplayList(gfxCtx->polyOpa.append++, dl);

    if (player->transformation == MM_PLAYER_FORM_HUMAN)
    {
        Color_RGB8 tunicColor;
        u16 tunic;

        tunic = CLAMP(gSaveContext.save.itemEquips.tunic, 0, 3);
        tunicColor = sTunicColors[tunic];
        gDPSetEnvColor(gfxCtx->polyOpa.append++, tunicColor.r, tunicColor.g, tunicColor.b, 0xFF);
    }
}

PATCH_CALL(0x80124858, Player_SkelAnime_DrawFlexLod);

u16 Player_GetFloorSfxByAge(Actor_Player* this, u16 sfxId) {
    if (GET_PLAYER_CUSTOM_BOOTS(this) == PLAYER_BOOTS_IRON)
    {
        switch (sfxId)
        {
        case 0x800: // NA_SE_PL_WALK_GROUND
        case 0x810: // NA_SE_PL_JUMP_GROUND
        case 0x820: // NA_SE_PL_LAND_GROUND
            return 0x28DC; // NA_SE_EV_IRON_DOOR_CLOSE
        }
    }

    return sfxId + this->floorSfxOffset + this->ageProperties->surfaceSfxIdOffset;
}

PATCH_FUNC(0x8082e0cc, Player_GetFloorSfxByAge);

s32 Player_ShouldCheckItemUsabilityWhileSwimming(Actor_Player* player, u8 itemAction)
{
    switch (itemAction)
    {
    case 0x50: // PLAYER_IA_MASK_ZORA
    case PLAYER_CUSTOM_IA_BOOTS_IRON:
    case PLAYER_CUSTOM_IA_BOOTS_HOVER:
    case PLAYER_CUSTOM_IA_TUNIC_ZORA:
    case PLAYER_CUSTOM_IA_TUNIC_GORON:
        return 0;
    default:
        return 1;
    }
}

f32 Player_GetEnvironmentWindSpeed(GameState_Play* play)
{
    Actor_Player* player = GET_LINK(play);
    if (GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_IRON)
    {
        return 0.0f;
    }

    // Displaced code:
    return play->envCtx.windSpeed;
}

s32 Player_IsBeingPushed(Actor_Player* this, GameState_Play* play)
{
    if (GET_PLAYER_CUSTOM_BOOTS(this) == PLAYER_BOOTS_IRON)
    {
        return this->pushedSpeed != 0.0f;
    }

    // Displaced code:
    return this->pushedSpeed != 0.0f || this->windSpeed != 0.0f || play->envCtx.windSpeed >= 50.0f;
}

s32 Player_PlayHoverSound(Actor_Player* this, f32* arg1Ptr)
{
    // Displaced code:
    f32 arg1 = *arg1Ptr;
    f32 updateScale = R_UPDATE_RATE / 2.0f;

    arg1 *= updateScale;
    if (arg1 < -7.25f) {
        arg1 = -7.25f;
    } else if (arg1 > 7.25f) {
        arg1 = 7.25f;
    }
    *arg1Ptr = arg1;
    // End displaced code

    if (GET_PLAYER_CUSTOM_BOOTS(this) == PLAYER_BOOTS_HOVER
        && !(this->base.bgCheckFlags & BGCHECKFLAG_GROUND)
        && this->hoverBootsTimer != 0)
    {
        Actor_PlaySfx_FlaggedCentered1(&this->base, 0xC9); // NA_SE_PL_HOBBERBOOTS_LV - SFX_FLAG
        return 1;
    }

    return 0;
}

typedef s32 (*Player_func_808340AC)(s32);
typedef s32 (*Player_func_808340D4)(s32);
typedef s32 (*Player_func_8083784C)(Actor_Player*);

static void Player_IncrementFloorTypeTimer(Actor_Player* this, s32* sPlayerFloorType)
{
    if (this->prevFloorType == *sPlayerFloorType)
    {
        this->floorTypeTimer++;
    }
    else
    {
        this->prevFloorType = *sPlayerFloorType;
        this->floorTypeTimer = 0;
    }
}

s32 Player_UpdateHoverBoots(Actor_Player* this)
{
    s32* sPlayerFloorType = OverlayAddr(0x80862B08);
    s16* sPlayerFloorPitchShape = OverlayAddr(0x80862B28);
    Player_func_808340AC Player_func_808340AC = OverlayAddr(0x808340AC);
    Player_func_808340D4 Player_func_808340D4 = OverlayAddr(0x808340D4);
    Player_func_8083784C Player_func_8083784C = OverlayAddr(0x8083784C);
    s32 canHoverOnGround;

    if (this->transformation == MM_PLAYER_FORM_DEKU && this->base.bgCheckFlags & BGCHECKFLAG_GROUND && Player_func_8083784C(this))
    {
        this->base.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    }

    if ((GET_PLAYER_CUSTOM_BOOTS(this) == PLAYER_BOOTS_HOVER) && (this->hoverBootsTimer != 0))
    {
        this->hoverBootsTimer--;
    }
    else
    {
        this->hoverBootsTimer = 0;
    }

    canHoverOnGround =
        (GET_PLAYER_CUSTOM_BOOTS(this) == PLAYER_BOOTS_HOVER) &&
        ((this->base.yDistToWater >= 0.0f) || (Player_func_808340AC(*sPlayerFloorType) >= 0) || Player_func_808340D4(*sPlayerFloorType));

    if (canHoverOnGround && (this->base.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->hoverBootsTimer != 0))
    {
        this->base.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    }

    if (this->base.bgCheckFlags & BGCHECKFLAG_GROUND)
    {
        if (!canHoverOnGround) {
            this->hoverBootsTimer = 19;
        }

        Player_IncrementFloorTypeTimer(this, sPlayerFloorType);
        return 0;
    }

    if (!(this->state & 0x8000000)) // PLAYER_STATE1_8000000 // Swimming
    {
        *sPlayerFloorType = 0; // FLOOR_TYPE_0;
    }
    this->floorPitch = this->floorPitchAlt = *sPlayerFloorPitchShape = 0;

    Player_IncrementFloorTypeTimer(this, sPlayerFloorType);

    return 1;
}

PATCH_FUNC(0x808430E0, Player_UpdateHoverBoots);

s32 Player_CheckHoverBoots(Actor_Player* player)
{
    if (player->hoverBootsTimer != 0)
    {
        s32* sPlayerPrevFloorProperty = OverlayAddr(0x80862B1C);
        player->base.velocity.y = 1.0f;
        *sPlayerPrevFloorProperty = 9; // FLOOR_PROPERTY_9
        return 1;
    }
    return 0;
}

s32 Player_IsFloorSlippery(Actor_Player* player, s32 floorType)
{
    if (GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_HOVER && !(player->state & ((1 < 27) | (1 << 29))))
    {
        return 1;
    }

    // Displaced code:
    return (player->base.bgCheckFlags & BGCHECKFLAG_GROUND) && floorType == 5 && player->currentBoots < 5; // PLAYER_BOOTS_ZORA_UNDERWATER
    // End displaced code
}

s32 Player_IsGoronOrGoronTunic(Actor_Player* player)
{
    return player->transformation == MM_PLAYER_FORM_GORON || gSaveContext.save.itemEquips.tunic == PLAYER_TUNIC_GORON;
}

static u8 sFloorDamageDelay[] = { 120, 60 };
s32 Player_ShouldTakeFloorDamage(Actor_Player* player, s32 isWallDamaging, s32 floorDamageType, s32 isFloorDamaging)
{
    // (floorDamageType < 0 && !isWallDamaging) never enters this code

    if (isFloorDamaging || isWallDamaging || player->base.yDistToWater > 0.0f)
    {
        player->floorTypeTimer = 0;
        player->base.colChkInfo.damage = 4;
        return 1;
    }

    // probably redundant, but better to be safe
    if (floorDamageType < 0 || floorDamageType > 1)
    {
        return 0;
    }

    if (gSaveContext.save.itemEquips.tunic != PLAYER_TUNIC_GORON || player->floorTypeTimer >= sFloorDamageDelay[floorDamageType])
    {
        player->floorTypeTimer = 0;
        player->base.colChkInfo.damage = 4;
        return 1;
    }

    return 0;
}

// This skips the environment hazard text for hot rooms, but there are no hot rooms in MM.
s32 Player_GetEnvironmentalHazardCustom(GameState_Play* play) {
    Actor_Player* player = GET_LINK(play);

    if (play->roomCtx.curRoom.behaviorType2 == ROOM_BEHAVIOR_TYPE2_HOT)
    {
        return PLAYER_ENV_HAZARD_HOTROOM;
    }
    else if (player->transformation != MM_PLAYER_FORM_ZORA && player->underwaterTimer > 80
        && (!comboConfig(CFG_MM_SCALES) || GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_IRON || player->underwaterTimer >= 300))
    {
        if (GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_IRON && (player->base.bgCheckFlags & BGCHECKFLAG_GROUND))
        {
            return PLAYER_ENV_HAZARD_UNDERWATER_FLOOR;
        }
        return PLAYER_ENV_HAZARD_UNDERWATER_FREE;
    }
    else if (player->state & 0x08000000) // PLAYER_STATE1_SWIM
    {
        if ((player->transformation == MM_PLAYER_FORM_ZORA)
            && (player->currentBoots >= 5) // PLAYER_BOOTS_ZORA_UNDERWATER
            && (player->base.bgCheckFlags & BGCHECKFLAG_GROUND))
        {
            return PLAYER_ENV_HAZARD_UNDERWATER_FLOOR;
        }
        else
        {
            return PLAYER_ENV_HAZARD_SWIMMING;
        }
    }
    else
    {
        return PLAYER_ENV_HAZARD_NONE;
    }
}

PATCH_FUNC(0x801242dc, Player_GetEnvironmentalHazardCustom);

s32 Player_CanSurface(Actor_Player* player)
{
    return player->state & 0x08000000 // PLAYER_STATE1_SWIM
        && player->transformation == MM_PLAYER_FORM_ZORA;
}

s32 Player_GetItemCamMode(Actor_Player* this)
{
    if (this->transformation == MM_PLAYER_FORM_HUMAN)
    {
        if (this->heldItemAction == 0xD && (this->state & 0x8000000)) // PLAYER_IA_HOOKSHOT // PLAYER_STATE1_8000000 // Swimming
        {
            return CAM_MODE_BOWARROW;
        }
        else
        {
            return CAM_MODE_SLINGSHOT;
        }
    }
    else if (this->transformation == MM_PLAYER_FORM_DEKU)
    {
        return CAM_MODE_DEKUSHOOT;
    }
    else
    {
        return CAM_MODE_BOWARROW;
    }
}

static f32 sDiveDepths[] = { 120.0f, 240.0f, 360.0f };

static u8 sDiveDoActions[] = { DO_ACTION_1, DO_ACTION_2, DO_ACTION_3, DO_ACTION_4,
                               DO_ACTION_5, DO_ACTION_6, DO_ACTION_7, DO_ACTION_8 };

u8 Player_GetActionAWhileDiving(Actor_Player* this)
{
    s32 diveIndex = (sDiveDepths[gSaveContext.save.inventory.upgrades.scale] - this->base.yDistToWater) / 40.0f;
    diveIndex = CLAMP(diveIndex, 0, ARRAY_SIZE(sDiveDoActions) - 1);
    return sDiveDoActions[diveIndex];
}

f32 Player_GetMaxDiveDepth()
{
    return sDiveDepths[gSaveContext.save.inventory.upgrades.scale];
}

u8 Player_GetStrengthCustom(u8 formStrength)
{
    if (comboConfig(CFG_MM_STRENGTH) && gSaveContext.save.playerForm == MM_PLAYER_FORM_HUMAN) // || gSaveContext.save.playerForm == MM_PLAYER_FORM_FIERCE_DEITY)
    {
        return gSaveContext.save.inventory.upgrades.strength;
    }
    return formStrength;
}

s32 Player_HasStrength(u8 requiredStrength)
{
    return Player_GetStrength() >= requiredStrength;
}
