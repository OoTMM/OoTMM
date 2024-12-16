#include <combo.h>
#include <combo/custom.h>
#include <combo/entrance.h>
#include <combo/player.h>
#include <combo/mask.h>
#include <combo/magic.h>
#include <combo/math.h>
#include <combo/config.h>
#include <combo/dpad.h>
#include <combo/multi.h>
#include <combo/global.h>
#include <combo/effect.h>
#include "../actors.h"

void ArrowCycle_Handle(Player* link, PlayState* play);

void Player_UpdateWrapper(Player* this, PlayState* play)
{
    ArrowCycle_Handle(this, play);
    Player_Update(this, play);
    Dpad_Update(play);
    Ocarina_HandleWarp(this, play);
}

int Player_DpadHook(Player* this, PlayState* play)
{
    if (Player_UsingItem(this))
        return 1;
    return Dpad_Use(play, DPF_ITEMS);
}

void Player_DrawHookshotReticleCustom(PlayState* play, Player* player)
{
    Player_DrawHookshotReticle(play, player, player->heldItemId == 0xF ? 77600.0f : 38600.0f);
}

PATCH_CALL(0x8082fe58, Player_DpadHook);

void (*gEnKanban_TalkedTo)(Actor*, PlayState*);

void Player_TalkDisplayTextBox(PlayState* play, s16 textId, Actor* actor)
{
    PlayerDisplayTextBox(play, textId, actor);
    if (actor)
    {
        switch (actor->id)
        {
        case ACTOR_EN_KANBAN:
            gEnKanban_TalkedTo(actor, play);
            break;
        }
    }
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
    PLAYER_CUSTOM_IA_HAMMER,
};

static u8 sMagicSpellCosts[] =
{
    12,
    24,
    12,
};

static s16 sMagicSpellActorIds[] =
{
    ACTOR_CUSTOM_SPELL_WIND,
    ACTOR_CUSTOM_SPELL_LOVE,
    ACTOR_CUSTOM_SPELL_FIRE,
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
    /* 0x2 */ s16 flags; /* negative marks the end */
} AnimSfxEntry;

#define ANIMSFX_SHIFT_TYPE(type) ((type) << 11)

#define ANIMSFX_DATA(type, frame) ((ANIMSFX_SHIFT_TYPE(type) | ((frame)&0x7FF)))

static AnimSfxEntry sMagicSpellStartSfx[] =
{
    { 0x83D, ANIMSFX_DATA(ANIMSFX_TYPE_1, 20) }, /* NA_SE_PL_SKIP */
    { 0x6800, ANIMSFX_DATA(ANIMSFX_TYPE_4, 20) }, /* NA_SE_VO_LI_SWORD_N */
    { 0, -ANIMSFX_DATA(ANIMSFX_TYPE_6, 26) },
};

static AnimSfxEntry sMagicSpellSfx[][2] =
{
    {
        { 0, ANIMSFX_DATA(ANIMSFX_TYPE_8, 20) },
        { 0x6817, -ANIMSFX_DATA(ANIMSFX_TYPE_4, 30) }, /* NA_SE_VO_LI_MAGIC_FROL */
    },
    {
        { 0, ANIMSFX_DATA(ANIMSFX_TYPE_8, 20) },
        { 0x6815, -ANIMSFX_DATA(ANIMSFX_TYPE_4, 44) }, /* NA_SE_VO_LI_MAGIC_NALE */
    },
    {
        { 0x681C, ANIMSFX_DATA(ANIMSFX_TYPE_4, 20) }, /* NA_SE_VO_LI_MAGIC_ATTACK */
        { 0x1818, -ANIMSFX_DATA(ANIMSFX_TYPE_1, 20) }, /* NA_SE_IT_SWORD_SWING_HARD */
    },
    {
        { 0x6801, ANIMSFX_DATA(ANIMSFX_TYPE_4, 20) }, /* NA_SE_VO_LI_SWORD_L */
        { 0x1818, -ANIMSFX_DATA(ANIMSFX_TYPE_1, 20) }, /* NA_SE_IT_SWORD_SWING_HARD */
    },
};

static PlayerAnimationHeader* sMagicSpellFirstAnimation[] =
{
    (PlayerAnimationHeader*)0x0400da28, /* &gPlayerAnim_link_magic_kaze1, */
    (PlayerAnimationHeader*)0x0400da10, /* &gPlayerAnim_link_magic_honoo1, */
    (PlayerAnimationHeader*)0x0400da40, /* &gPlayerAnim_link_magic_tamashii1, */
};

static PlayerAnimationHeader* sMagicSpellSecondAnimation[] =
{
    (PlayerAnimationHeader*)0x0400da30, /* &gPlayerAnim_link_magic_kaze2, */
    (PlayerAnimationHeader*)0x0400da18, /* &gPlayerAnim_link_magic_honoo2, */
    (PlayerAnimationHeader*)0x0400da48, /* &gPlayerAnim_link_magic_tamashii2, */
};

static PlayerAnimationHeader* sMagicSpellThirdAnimation[] =
{
    (PlayerAnimationHeader*)0x0400da38, /* &gPlayerAnim_link_magic_kaze3, */
    (PlayerAnimationHeader*)0x0400da20, /* &gPlayerAnim_link_magic_honoo3, */
    (PlayerAnimationHeader*)0x0400da50, /* &gPlayerAnim_link_magic_tamashii3, */
};

static u8 sMagicSpellLoopTimer[] = { 70, 10, 10 };

s32 Player_CustomItemToItemAction(Player* this, s32 item, s32 itemAction)
{
    s32 customItem = item - ITEM_MM_CUSTOM_MIN;
    if (customItem < 0)
    {
        return itemAction;
    }
    if (customItem >= ARRAY_COUNT(sCustomItemActions))
    {
        return 0;
    }
    return sCustomItemActions[customItem];
}

static s32 Player_ActionToMagicSpell(Player* this, s32 itemAction)
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

static s32 Player_ActionToBoots(Player* this, s32 itemAction)
{
    s32 boots = itemAction - PLAYER_CUSTOM_IA_BOOTS_MIN + 1;

    if ((boots >= PLAYER_BOOTS_IRON) && (boots <= PLAYER_BOOTS_HOVER))
    {
        return boots;
    }
    else
    {
        return -1;
    }
}

static s32 Player_ActionToTunic(Player* this, s32 itemAction)
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

Actor* Player_SpawnMagicSpellActor(PlayState* play, Player* this, s8 magicSpell)
{
    return Actor_Spawn(&play->actorCtx, play, sMagicSpellActorIds[magicSpell], this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0);
}      /* size = 0x4 */

typedef void (*Player_PlayAnimSfx)(Player*, AnimSfxEntry*);
typedef void (*Player_func_80832F24)(Player*);
typedef void (*Player_func_8083249C)(Player*);
typedef void (*Player_StopCutscene)(Player*);

extern Vec3f gFwPointerPos;

void Player_Action_CastingSpell(Player* this, PlayState* play)
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
                    this->stateFlags1 |= 0x30000000; /* PLAYER_STATE1_20000000 | PLAYER_STATE1_10000000 */
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
                RespawnData* fw = &gCustomSave.fw[gOotSave.age];
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 1;
                Play_SetupRespawnPoint(play, RESPAWN_MODE_HUMAN, 0x6ff);
                *fw = gSaveContext.respawn[RESPAWN_MODE_DOWN];
                fw->playerParams = 0x6ff;
                fw->data = 40;

                /* Copy Game Over / Soar to Entrance respawn data. */
                memcpy(&gCustomSave.fwRespawnTop[gOotSave.age], &gSaveContext.respawn[RESPAWN_MODE_TOP], sizeof(RespawnData));
                memcpy(&gCustomSave.fwRespawnDungeonEntrance[gOotSave.age], &gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE], sizeof(RespawnData));

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
                s8 spellSfxIndex = this->av1.actionVar1;
                if (spellSfxIndex == 2 && comboIsLinkAdult())
                {
                    spellSfxIndex = 3;
                }
                Player_PlayAnimSfx(this, sMagicSpellSfx[spellSfxIndex]);
                if ((this->av1.actionVar1 == 2) && PlayerAnimation_OnFrame(&this->skelAnime, 30.0f))
                {
                    this->stateFlags1 &= ~0x30000000; /* PLAYER_STATE1_20000000 | PLAYER_STATE1_10000000 */
                }
            }
            else if (sMagicSpellLoopTimer[this->av1.actionVar1] < this->av2.actionVar2++)
            {
                PlayerAnimation_PlayOnceSetSpeed(play, &this->skelAnime, sMagicSpellThirdAnimation[this->av1.actionVar1], 0.83f);
                this->currentYaw = this->actor.shape.rot.y;
                this->av1.actionVar1 = -1;
            }
        }
    }

    Player_func_80832F24 = OverlayAddr(0x80832F24);
    Player_func_80832F24(this);

    Player_func_8083249C = OverlayAddr(0x8083249c);
    Player_func_8083249C(this);
}

typedef void (*Player_SetAction_PreserveItemAction)(PlayState*, Player*, PlayerActionFunc, s32);

void Player_CastSpell(PlayState* play, Player* this, s32 magicSpell)
{
    Player_SetAction_PreserveItemAction Player_SetAction_PreserveItemAction;
    Player_func_8083249C Player_func_8083249C;

    if (Magic_RequestChange(play, sMagicSpellCosts[magicSpell], MAGIC_CONSUME_WAIT_PREVIEW))
    {
        Player_SetAction_PreserveItemAction = OverlayAddr(0x80831760);
        Player_SetAction_PreserveItemAction(play, this, Player_Action_CastingSpell, 0);

        this->av1.actionVar1 = magicSpell; /* - 3? */

        PlayerAnimation_PlayOnceSetSpeed(play, &this->skelAnime, (PlayerAnimationHeader*)0x0400da58, 0.83f); /* gPlayerAnim_link_magic_tame */

        this->csId = 0x7B; /* CS_ID_GLOBAL_TALK */
        Player_func_8083249C = OverlayAddr(0x8083249c);
        Player_func_8083249C(this);

        this->stateFlags3 |= 0x80; /* PLAYER_STATE3_80 */
    }
}

typedef void (*Player_UpdateUpperBody)(Player*, PlayState*);
typedef void (*Player_func_8085B384)(Player*, PlayState*);

void Player_Action_FaroresWindText(Player* this, PlayState* play)
{
    Player_UpdateUpperBody Player_UpdateUpperBody = OverlayAddr(0x8083216c);
    Player_StopCutscene Player_StopCutscene;
    Player_func_8085B384 Player_func_8085B384;
    char* b;

    this->stateFlags3 |= 0x80; /* PLAYER_STATE3_80 */

    PlayerAnimation_Update(play, &this->skelAnime);
    Player_UpdateUpperBody(this, play);

    if (this->av2.actionVar2 == 0)
    {
        /* Message Id is the same as OoT, but here it's the Great Fairy's Sword get-item message. Doesn't matter though. */
        PlayerDisplayTextBox(play, 0x3B, &this->actor);
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No icon. */
        b += 11;
        comboTextAppendStr(&b, TEXT_FAST "You cast Farore's Wind!" TEXT_NL);
        comboTextAppendStr(&b, TEXT_COLOR_GREEN TEXT_CHOICE3 "     Return to the Warp Point" TEXT_NL "     Dispel the Warp Point" TEXT_NL "     Exit" TEXT_END);
        this->av2.actionVar2 = 1;
        return;
    }

    /* MSGMODE_TEXT_DONE */
    if (play->msgCtx.msgMode == 0x42 && Message_ShouldAdvance(play))
    {
        Audio_PlaySfx_MessageDecide();

        Message_Close(play);

        if (play->msgCtx.choiceIndex == 0)
        {
            gSaveContext.respawnFlag = 8;
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->nextEntrance = gSaveContext.respawn[RESPAWN_MODE_HUMAN].entrance;
            play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;
            gNoTimeFlow = 0;

            /* Restore Game Over / Soar to Entrance respawn data. */
            memcpy(&gSaveContext.respawn[RESPAWN_MODE_TOP], &gCustomSave.fwRespawnTop[gOotSave.age], sizeof(RespawnData));
            memcpy(&gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE], &gCustomSave.fwRespawnDungeonEntrance[gOotSave.age], sizeof(RespawnData));

            /* TODO cancel timers? */

            return;
        }

        if (play->msgCtx.choiceIndex == 1)
        {
            gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = -gSaveContext.respawn[RESPAWN_MODE_HUMAN].data;
            gCustomSave.fw[gOotSave.age].data = 0;
            Audio_PlaySfx_AtPos(&gCustomSave.fw[gOotSave.age].pos, 0x8C8); /* NA_SE_PL_MAGIC_WIND_VANISH */
        }

        Player_func_8085B384 = OverlayAddr(0x8085B384);
        Player_func_8085B384(this, play);

        Player_StopCutscene = OverlayAddr(0x80838760);
        Player_StopCutscene(this);
    }
}

typedef void (*Player_func_8082DAD4)(Player*);
typedef PlayerAnimationHeader* (*Player_GetWaitAnimation)(Player*);
typedef void (*Player_SetAction)(PlayState*, Player*, PlayerActionFunc, s32);

s32 Player_CustomCsItem(Player* this, PlayState* play)
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
            this->stateFlags1 |= 0x30000000; /* PLAYER_STATE1_20000000 | PLAYER_STATE1_10000000 */
            Player_GetWaitAnimation = OverlayAddr(0x8082ED20);
            PlayerAnimation_PlayOnce(play, &this->skelAnime, Player_GetWaitAnimation(this));
            this->csId = 0x7B; /* CS_ID_GLOBAL_TALK */
            Player_func_8083249C = OverlayAddr(0x8083249c);
            Player_func_8083249C(this);
        }
        func_8082DAD4 = OverlayAddr(0x8082DAD4);
        func_8082DAD4(this);
        return 1;
    }
    else if (this->itemAction >= 0x3a && this->itemAction <= 0x51) /* PLAYER_IA_MASK_MIN // PLAYER_IA_MASK_MAX */
    {
        return -1;
    }
    return 0;
}

/* Written as a function because there are several uses for this */
u8 Player_IsHumanChild(Player* player) {
    return (player->transformation == MM_PLAYER_FORM_HUMAN && !comboIsLinkAdult());
}

s16 sBootData[7][18] = {
    /* Child Link - Iron Boots */
    { 200, 1000, 300, 700, 550, 270, 1000, 0, 120, 800, 300, -160, 600, 590, 750, 125, 200, 130 },

    /* Child Link - Hover Boots */
    { 200, 1000, 300, 700, 550, 270, 600, 600, 120, 800, 550, -100, 600, 540, 270, 25, 0, 130 },

    /* Child Link - Iron Boots (Underwater) */
    { 80, 800, 150, 700, 480, 270, 600, 50, 120, 800, 550, -40, 400, 540, 270, 25, 0, 80 },

    /* Adult Link - Kokiri Boots */
    { 200, 1000, 300, 700, 550, 270, 700, 300, 120, 800, 600, -100, 600, 590, 750, 125, 200, 130 },

    /* Adult Link - Iron Boots */
    { 200, 1000, 300, 700, 550, 270, 1000, 0, 120, 800, 300, -160, 600, 590, 750, 125, 200, 130 },

    /* Adult Link - Hover Boots */
    { 200, 1000, 300, 700, 550, 270, 600, 600, 120, 800, 550, -100, 600, 540, 270, 25, 0, 130 },

    /* Adult Link - Iron Boots (Underwater) */
    { 80, 800, 150, 700, 480, 270, 600, 50, 120, 800, 550, -40, 400, 540, 270, 25, 0, 80 },
};

void Player_CheckCustomBoots(PlayState* play)
{
    Player* player = GET_PLAYER(play);
    if (player->transformation == MM_PLAYER_FORM_HUMAN)
    {
        s32 isAdult = comboIsLinkAdult();
        player->actor.flags &= ~ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCHES;
        player->actor.flags |= ACTOR_FLAG_CAN_PRESS_SWITCHES;

        if (gSaveContext.save.info.itemEquips.boots > 0 || isAdult)
        {
            s32 currentBoots = gSaveContext.save.info.itemEquips.boots;
            s16* bootRegs;

            REG(27) = 2000;
            REG(48) = 370;

            switch(currentBoots)
            {
            case PLAYER_BOOTS_IRON:
                player->currentBoots = 6; /* PLAYER_BOOTS_GORON */
                player->actor.flags |= ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCHES;
                if (player->stateFlags1 & PLAYER_ACTOR_STATE_WATER)
                {
                    currentBoots = 3; /* Iron Underwater */
                }
                REG(27) = 500;
                REG(48) = 100;
                break;
            case PLAYER_BOOTS_HOVER:
                player->currentBoots = 0; /* PLAYER_BOOTS_FIERCE_DEITY */
                player->actor.flags &= ~ACTOR_FLAG_CAN_PRESS_SWITCHES;
                break;
            }

            if (isAdult)
            {
                currentBoots += 4;
            }

            bootRegs = sBootData[currentBoots - 1];
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
        gSaveContext.save.info.itemEquips.boots = 0;
        gSaveContext.save.info.itemEquips.tunic = 0;
    }
}

s32 Player_CustomUseItem(Player* this, PlayState* play, s32 itemAction)
{
    s32 magicSpell = Player_ActionToMagicSpell(this, itemAction);
    if (magicSpell >= 0)
    {
        if (((magicSpell == PLAYER_MAGIC_SPELL_WIND) && (gSaveContext.respawn[RESPAWN_MODE_HUMAN].data > 0)) ||
            ((gSaveContext.magicCapacity != 0) && (gSaveContext.magicState == MAGIC_STATE_IDLE) &&
             (gSaveContext.save.info.playerData.magic >= sMagicSpellCosts[magicSpell])))
        {
            this->unk_AA5 = 5;
            this->itemAction = itemAction;
        }
        else
        {
            PlaySound(0x4806); /* NA_SE_SY_ERROR */
        }
        /* Handled */
        return 1;
    }

    s32 boots = Player_ActionToBoots(this, itemAction);
    if (boots > 0)
    {
        if (this->transformation == MM_PLAYER_FORM_HUMAN)
        {
            if (gSaveContext.save.info.itemEquips.boots == boots)
            {
                boots = 0;
            }
            gSaveContext.save.info.itemEquips.boots = boots;

            if (!boots)
            {
                this->currentBoots = 1; /* PLAYER_BOOTS_HYLIAN */
            }

            Player_SetBootData(play, this);
            Player_PlaySfx(this, 0x835); /* NA_SE_PL_CHANGE_ARMS */
        }

        /* Handled */
        return 1;
    }

    s32 tunic = Player_ActionToTunic(this, itemAction);
    if (tunic >= 0)
    {
        if (this->transformation == MM_PLAYER_FORM_HUMAN)
        {
            u16 newTunic = tunic;
            if (gSaveContext.save.info.itemEquips.tunic == newTunic)
            {
                newTunic = 0;
            }
            gSaveContext.save.info.itemEquips.tunic = newTunic;

            Player_PlaySfx(this, 0x835); /* NA_SE_PL_CHANGE_ARMS */
        }

        /* Handled */
        return 1;
    }

    /* PLAYER_IA_MASK_MIN = 0x3A, */
    /* PLAYER_IA_MASK_MAX = 0x51, */
    /* PLAYER_IA_MASK_GIANT = 0x4D, */
    if (itemAction >= 0x3A && itemAction <= 0x51 && (this->transformation != MM_PLAYER_FORM_HUMAN || itemAction >= 0x4D))
    {
        /* Vanilla code doesn't need to check more conditions. */
        return -1;
    }

    return 0;
}

typedef void (*Player_func_80833AA0)(Player*, PlayState*);
typedef void (*Player_Draw)(Player*, PlayState*);

void Player_Action_FaroresWindSpawning(Player* this, PlayState* play)
{
    Player_func_80833AA0 Player_func_80833AA0;
    Player_Draw Player_Draw;
    if (this->av2.actionVar2 > 20)
    {
        Player_Draw = OverlayAddr(0x80846528);
        this->actor.draw = Player_Draw;
        this->actor.world.pos.y += 60.0f;
        Player_func_80833AA0 = OverlayAddr(0x80833AA0);
        Player_func_80833AA0(this, play);
        return;
    }

    if (this->av2.actionVar2++ == 20)
    {
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].data++;
        Audio_PlaySfx_AtPos(&gCustomSave.fw[gOotSave.age].pos, 0x87B); /* NA_SE_PL_MAGIC_WIND_WARP */
    }
}

void Player_InitFaroresWindRespawn(PlayState* play, Player* this)
{
    Player_SetAction Player_SetAction = OverlayAddr(0x80831494);
    this->actor.draw = NULL;
    Player_SetAction(play, this, Player_Action_FaroresWindSpawning, 0);
    this->stateFlags1 |= 0x20000000; /* PLAYER_STATE1_20000000 */
}

void Player_AfterInit(PlayState* play)
{
    /* Displaced code: */
    Map_SetAreaEntrypoint(play);

    Player* player = GET_PLAYER(play);

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

static int prepareObject(PlayState* play, u16 objectId)
{
    void* obj;

    obj = comboGetObject(objectId);
    if (!obj)
        return 0;

    OPEN_DISPS(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x0a, obj);
    CLOSE_DISPS();

    return 1;
}

static void DrawHammer(PlayState* play)
{
    if (!prepareObject(play, CUSTOM_OBJECT_ID_EQ_HAMMER))
        return;

    OPEN_DISPS(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, 0x0D000300, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_EQ_HAMMER_0);
    CLOSE_DISPS();
}

static void DrawBootsIron(PlayState* play, Player* link)
{
    OPEN_DISPS(play->state.gfxCtx);
    if (comboIsLinkAdult())
    {
        if (!prepareObject(play, CUSTOM_OBJECT_ID_BOOTS_IRON))
            return;
        gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_BOOTS_IRON_0);
        gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_BOOTS_IRON_1);
    }
    else
    {
        gSPSegment(POLY_OPA_DISP++, 0x08, g.customKeep);
        gSPDisplayList(POLY_OPA_DISP++, gLinkAdultLeftIronBootDL);
        gSPDisplayList(POLY_OPA_DISP++, gLinkAdultRightIronBootDL);
    }
    CLOSE_DISPS();
}

static void DrawBootsHover(PlayState* play, Player* link)
{
    OPEN_DISPS(play->state.gfxCtx);
    if (comboIsLinkAdult())
    {
        if (!prepareObject(play, CUSTOM_OBJECT_ID_BOOTS_HOVER))
            return;
        gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_BOOTS_HOVER_0);
        gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_BOOTS_HOVER_1);
    }
    else
    {
        gSPSegment(POLY_OPA_DISP++, 0x08, g.customKeep);
        gSPDisplayList(POLY_OPA_DISP++, gLinkAdultLeftHoverBootDL);
        gSPDisplayList(POLY_OPA_DISP++, gLinkAdultRightHoverBootDL);
    }
    CLOSE_DISPS();
}

void DrawChildGauntlets(PlayState* play);

static void DrawGauntlets(PlayState* play, Player* link)
{
    OPEN_DISPS(play->state.gfxCtx);
    if (comboIsLinkAdult())
    {
        if (!prepareObject(play, CUSTOM_OBJECT_ID_GAUNTLETS))
            return;
        s32 playerLeftHandType = *(s32*)0x801f59f4;
        s32 playerRightHandType = *(s32*)0x801f59f8;
        gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_GAUNTLETS_0);
        gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_GAUNTLETS_3);
        gSPDisplayList(POLY_OPA_DISP++, (playerLeftHandType == 0) /* PLAYER_MODELTYPE_LH_OPEN */
                                            ? CUSTOM_OBJECT_GAUNTLETS_1
                                            : CUSTOM_OBJECT_GAUNTLETS_2);
        gSPDisplayList(POLY_OPA_DISP++, (playerRightHandType == 6) /* PLAYER_MODELTYPE_RH_OPEN */
                                            ? CUSTOM_OBJECT_GAUNTLETS_4
                                            : CUSTOM_OBJECT_GAUNTLETS_5);
    }
    else
    {
        DrawChildGauntlets(play);
    }
    CLOSE_DISPS();
}

static Color_RGB8 sGauntletColors[] = {
    { 255, 255, 255 },
    { 254, 207, 15 },
};

static Vec3s sHoverBootsRot = { 0, 0, 0 };
static s32 sHoverBootCircleAlpha = 255;

static Color_RGB8 sTunicColors[4] = {
    { 30, 105, 27 }, /* PLAYER_TUNIC_NONE (default in MM) */
    { 30, 105, 27 }, /* PLAYER_TUNIC_KOKIRI (will not be used) */
    { 100, 20, 0 },  /* PLAYER_TUNIC_GORON */
    { 0, 60, 100 },  /* PLAYER_TUNIC_ZORA */
};

EXPORT_SYMBOL(MM_COLOR_TUNIC_KOKIRI, sTunicColors[0]);
EXPORT_SYMBOL(MM_COLOR_TUNIC_GORON, sTunicColors[2]);
EXPORT_SYMBOL(MM_COLOR_TUNIC_ZORA, sTunicColors[3]);

void Player_SkelAnime_DrawFlexLod(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw, Player* player, s32 lod)
{
    OPEN_DISPS(play->state.gfxCtx);

    if (player->transformation == MM_PLAYER_FORM_HUMAN)
    {
        Color_RGB8* tunicColor;
        u16 tunic;

        tunic = CLAMP(gSaveContext.save.info.itemEquips.tunic, 0, 3);
        tunicColor = &sTunicColors[tunic];
        gDPSetEnvColor(POLY_OPA_DISP++, tunicColor->r, tunicColor->g, tunicColor->b, 0xFF);
    }

    SkelAnime_DrawFlexLod(play, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, &player->actor, lod);


    if (overrideLimbDraw != Player_OverrideLimbDrawGameplayFirstPerson && gSaveContext.gameMode != 3) /* GAMEMODE_END_CREDITS */
    {
        switch (GET_PLAYER_CUSTOM_BOOTS(player))
        {
        case PLAYER_BOOTS_IRON:
            DrawBootsIron(play, player);
            break;
        case PLAYER_BOOTS_HOVER:
            DrawBootsHover(play, player);
            break;
        }

        if (player->transformation == MM_PLAYER_FORM_HUMAN && player->itemAction == PLAYER_CUSTOM_IA_HAMMER) {
            DrawHammer(play);
        }

        if (player->transformation == MM_PLAYER_FORM_HUMAN && Config_Flag(CFG_MM_STRENGTH))
        {
            Color_RGB8* gauntletColor;
            s32 strength = gSaveContext.save.info.inventory.upgrades.strength;
            switch (strength)
            {
                case 1:
                    gSPSegment(POLY_OPA_DISP++, 0x08, g.customKeep);
                    gSPDisplayList(POLY_OPA_DISP++, gLinkChildGoronBraceletDL);
                    break;
                case 2:
                case 3:
                    gauntletColor = &sGauntletColors[strength - 2];
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetEnvColor(POLY_OPA_DISP++, gauntletColor->r, gauntletColor->g, gauntletColor->b, 0);

                    DrawGauntlets(play, player);
                    break;
            }
        }
    }

    gSPSegment(POLY_XLU_DISP++, 0x08, g.customKeep);

    if (GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_HOVER && !(player->actor.bgCheckFlags & BGCHECKFLAG_GROUND)
        && !(player->stateFlags1 & (1 << 23)) && player->hoverBootsTimer != 0)
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

            Matrix_SetTranslateRotateYXZ(player->actor.world.pos.x, player->actor.world.pos.y + 2.0f,
                                         player->actor.world.pos.z, &sHoverBootsRot);
            Matrix_Scale(4.0f, 4.0f, 4.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(POLY_XLU_DISP++, 0x09,
                       DisplaceTexture(play->state.gfxCtx, G_TX_RENDERTILE, 0, 0, 16, 32, 1, 0,
                                        (play->gameplayFrames * -15) % 128, 16, 32));
            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 255, sHoverBootCircleAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 120, 90, 30, 128);
            gSPDisplayList(POLY_XLU_DISP++, gHoverBootsCircleDL);
        }
    }

    CLOSE_DISPS();
}

void Player_ColorAfterMask(GraphicsContext* gfxCtx, s32 maskIDMinusOne, PlayerMaskDList* maskDList, Player* player) {
    u32 dl = maskDList->maskDListEntry[maskIDMinusOne];
    gSPDisplayList(gfxCtx->polyOpa.append++, dl);

    if (player->transformation == MM_PLAYER_FORM_HUMAN)
    {
        Color_RGB8 tunicColor;
        u16 tunic;

        tunic = CLAMP(gSaveContext.save.info.itemEquips.tunic, 0, 3);
        tunicColor = sTunicColors[tunic];
        gDPSetEnvColor(gfxCtx->polyOpa.append++, tunicColor.r, tunicColor.g, tunicColor.b, 0xFF);
    }
}

PATCH_CALL(0x80124858, Player_SkelAnime_DrawFlexLod);

u16 Player_GetFloorSfxByAge(Player* this, u16 sfxId) {
    if (GET_PLAYER_CUSTOM_BOOTS(this) == PLAYER_BOOTS_IRON)
    {
        switch (sfxId)
        {
        case 0x800: /* NA_SE_PL_WALK_GROUND */
        case 0x810: /* NA_SE_PL_JUMP_GROUND */
        case 0x820: /* NA_SE_PL_LAND_GROUND */
            return 0x28DC; /* NA_SE_EV_IRON_DOOR_CLOSE */
        }
    }

    return sfxId + this->floorSfxOffset + this->ageProperties->surfaceSfxIdOffset;
}

PATCH_FUNC(0x8082e0cc, Player_GetFloorSfxByAge);

s32 Player_ShouldCheckItemUsabilityWhileSwimming(Player* player, u8 itemAction)
{
    switch (itemAction)
    {
    case 0x50: /* PLAYER_IA_MASK_ZORA */
    case PLAYER_CUSTOM_IA_BOOTS_IRON:
    case PLAYER_CUSTOM_IA_BOOTS_HOVER:
    case PLAYER_CUSTOM_IA_TUNIC_ZORA:
    case PLAYER_CUSTOM_IA_TUNIC_GORON:
        return 0;
    default:
        return 1;
    }
}

f32 Player_GetEnvironmentWindSpeed(PlayState* play)
{
    Player* player = GET_PLAYER(play);
    if (GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_IRON)
    {
        return 0.0f;
    }

    /* Displaced code: */
    return play->envCtx.windSpeed;
}

s32 Player_IsBeingPushed(Player* this, PlayState* play)
{
    if (GET_PLAYER_CUSTOM_BOOTS(this) == PLAYER_BOOTS_IRON)
    {
        return this->pushedSpeed != 0.0f;
    }

    /* Displaced code: */
    return this->pushedSpeed != 0.0f || this->windSpeed != 0.0f || play->envCtx.windSpeed >= 50.0f;
}

s32 Player_PlayHoverSound(Player* this, f32* arg1Ptr)
{
    /* Displaced code: */
    f32 arg1 = *arg1Ptr;
    f32 updateScale = R_UPDATE_RATE / 2.0f;

    arg1 *= updateScale;
    if (arg1 < -7.25f) {
        arg1 = -7.25f;
    } else if (arg1 > 7.25f) {
        arg1 = 7.25f;
    }
    *arg1Ptr = arg1;
    /* End displaced code */

    if (GET_PLAYER_CUSTOM_BOOTS(this) == PLAYER_BOOTS_HOVER
        && !(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)
        && this->hoverBootsTimer != 0)
    {
        Actor_PlaySfx_FlaggedCentered1(&this->actor, 0xC9); /* NA_SE_PL_HOBBERBOOTS_LV - SFX_FLAG */
        return 1;
    }

    return 0;
}

typedef s32 (*Player_func_808340AC)(s32);
typedef s32 (*Player_func_808340D4)(s32);
typedef s32 (*Player_func_8083784C)(Player*);

static void Player_IncrementFloorTypeTimer(Player* this, s32* sPlayerFloorType)
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

s32 Player_UpdateHoverBoots(Player* this)
{
    s32* sPlayerFloorType = OverlayAddr(0x80862B08);
    s16* sPlayerFloorPitchShape = OverlayAddr(0x80862B28);
    Player_func_808340AC Player_func_808340AC = OverlayAddr(0x808340AC);
    Player_func_808340D4 Player_func_808340D4 = OverlayAddr(0x808340D4);
    Player_func_8083784C Player_func_8083784C = OverlayAddr(0x8083784C);
    s32 canHoverOnGround;

    if (this->transformation == MM_PLAYER_FORM_DEKU && this->actor.bgCheckFlags & BGCHECKFLAG_GROUND && Player_func_8083784C(this))
    {
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
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
        ((this->actor.depthInWater >= 0.0f) || (Player_func_808340AC(*sPlayerFloorType) >= 0) || Player_func_808340D4(*sPlayerFloorType));

    if (canHoverOnGround && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->hoverBootsTimer != 0))
    {
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    }

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)
    {
        if (!canHoverOnGround) {
            this->hoverBootsTimer = 19;
        }

        Player_IncrementFloorTypeTimer(this, sPlayerFloorType);
        return 0;
    }

    if (!(this->stateFlags1 & 0x8000000)) /* PLAYER_STATE1_8000000 // Swimming */
    {
        *sPlayerFloorType = 0; /* FLOOR_TYPE_0; */
    }
    this->floorPitch = this->floorPitchAlt = *sPlayerFloorPitchShape = 0;

    Player_IncrementFloorTypeTimer(this, sPlayerFloorType);

    return 1;
}

PATCH_FUNC(0x808430E0, Player_UpdateHoverBoots);

s32 Player_CheckHoverBoots(Player* player)
{
    if (player->hoverBootsTimer != 0)
    {
        s32* sPlayerPrevFloorProperty = OverlayAddr(0x80862B1C);
        player->actor.velocity.y = 1.0f;
        *sPlayerPrevFloorProperty = 9; /* FLOOR_PROPERTY_9 */
        return 1;
    }
    return 0;
}

s32 Player_IsFloorSlippery(Player* player, s32 floorType)
{
    if (GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_HOVER && !(player->stateFlags1 & ((1 < 27) | (1 << 29))))
    {
        return 1;
    }

    /* Displaced code: */
    return (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && floorType == 5 && player->currentBoots < 5; /* PLAYER_BOOTS_ZORA_UNDERWATER */
    /* End displaced code */
}

s32 Player_IsGoronOrGoronTunic(Player* player)
{
    return player->transformation == MM_PLAYER_FORM_GORON || gSaveContext.save.info.itemEquips.tunic == PLAYER_TUNIC_GORON;
}

static u8 sFloorDamageDelay[] = { 120, 60 };
s32 Player_ShouldTakeFloorDamage(Player* player, s32 isWallDamaging, s32 floorDamageType, s32 isFloorDamaging)
{
    /* (floorDamageType < 0 && !isWallDamaging) never enters this code */

    if (isFloorDamaging || isWallDamaging || player->actor.depthInWater > 0.0f)
    {
        player->floorTypeTimer = 0;
        player->actor.colChkInfo.damage = 4;
        return 1;
    }

    /* probably redundant, but better to be safe */
    if (floorDamageType < 0 || floorDamageType > 1)
    {
        return 0;
    }

    if (gSaveContext.save.info.itemEquips.tunic != PLAYER_TUNIC_GORON || player->floorTypeTimer >= sFloorDamageDelay[floorDamageType])
    {
        player->floorTypeTimer = 0;
        player->actor.colChkInfo.damage = 4;
        return 1;
    }

    return 0;
}

/* This skips the environment hazard text for hot rooms, but there are no hot rooms in MM. */
s32 Player_GetEnvironmentalHazardCustom(PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (play->roomCtx.curRoom.behaviorType2 == ROOM_BEHAVIOR_TYPE2_HOT)
    {
        return PLAYER_ENV_HAZARD_HOTROOM;
    }
    else if (player->transformation != MM_PLAYER_FORM_ZORA && player->underwaterTimer > 80
        && (!Config_Flag(CFG_MM_SCALES) || GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_IRON || player->underwaterTimer >= 300))
    {
        if (GET_PLAYER_CUSTOM_BOOTS(player) == PLAYER_BOOTS_IRON && (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND))
        {
            return PLAYER_ENV_HAZARD_UNDERWATER_FLOOR;
        }
        return PLAYER_ENV_HAZARD_UNDERWATER_FREE;
    }
    else if (player->stateFlags1 & 0x08000000) /* PLAYER_STATE1_SWIM */
    {
        if ((player->transformation == MM_PLAYER_FORM_ZORA)
            && (player->currentBoots >= 5) /* PLAYER_BOOTS_ZORA_UNDERWATER */
            && (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND))
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

s32 Player_CanSurface(Player* player)
{
    return player->stateFlags1 & 0x08000000 /* PLAYER_STATE1_SWIM */
        && player->transformation == MM_PLAYER_FORM_ZORA;
}

s32 Player_GetItemCamMode(Player* this)
{
    if (this->transformation == MM_PLAYER_FORM_HUMAN)
    {
        if ((this->heldItemAction == PLAYER_IA_HOOKSHOT && (this->stateFlags1 & 0x8000000)) || (comboIsLinkAdult())) /* PLAYER_IA_HOOKSHOT // PLAYER_STATE1_8000000 // Swimming */
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

u8 Player_GetActionAWhileDiving(Player* this)
{
    s32 diveIndex = (sDiveDepths[gSaveContext.save.info.inventory.upgrades.scale] - this->actor.depthInWater) / 40.0f;
    diveIndex = CLAMP(diveIndex, 0, ARRAY_COUNT(sDiveDoActions) - 1);
    return sDiveDoActions[diveIndex];
}

f32 Player_GetMaxDiveDepth()
{
    return sDiveDepths[gSaveContext.save.info.inventory.upgrades.scale];
}

u8 Player_GetStrengthCustom(u8 formStrength)
{
    if (Config_Flag(CFG_MM_STRENGTH) && gSaveContext.save.playerForm == MM_PLAYER_FORM_HUMAN) /* || gSaveContext.save.playerForm == MM_PLAYER_FORM_FIERCE_DEITY) */
    {
        return gSaveContext.save.info.inventory.upgrades.strength;
    }
    return formStrength;
}

s32 Player_HasStrength(u8 requiredStrength)
{
    return Player_GetStrength() >= requiredStrength;
}

void Player_PlaySfx_GiantsMask(u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* volume, s8* reverbAdd)
{
    if (comboIsLinkAdult())
    {
        sfxId -= 0x20;
    }
    AudioSfx_PlaySfx(sfxId, pos, token, freqScale, volume, reverbAdd);
}

PATCH_CALL(0x8019f874, Player_PlaySfx_GiantsMask);

u8 Player_AfterMaskLoaded(Player* player)
{
    player->maskObjectLoadState = 0;

    if (player->actor.id == ACTOR_PLAYER && comboIsLinkAdult())
    {
        u32* maskObjectSegment;
        switch (player->currentMask)
        {
        case MASK_KEATON:
            maskObjectSegment = (u32*) player->maskObjectSegment;
            maskObjectSegment[0x12a] = 0xde000000;
            maskObjectSegment[0x12b] = 0x0405a2e0;
            break;
        case MASK_BUNNY:
            maskObjectSegment = (u32*) player->maskObjectSegment;
            maskObjectSegment[0x188] = 0xde000000;
            maskObjectSegment[0x189] = 0x0405a2e0;
            maskObjectSegment[0x1e0] = 0xde000000;
            maskObjectSegment[0x1e1] = 0x0405a2e8;
            maskObjectSegment[0x1ec] = 0xde000000;
            maskObjectSegment[0x1ed] = 0x0405a2e0;
            maskObjectSegment[0x224] = 0xde000000;
            maskObjectSegment[0x225] = 0x0405a2e8;
            maskObjectSegment[0x230] = 0xde000000;
            maskObjectSegment[0x231] = 0x0405a2e0;
            break;
        case MASK_SCENTS:
            maskObjectSegment = (u32*) player->maskObjectSegment;
            maskObjectSegment[0x1c6] = 0xde000000;
            maskObjectSegment[0x1c7] = 0x0405a2e0;
            break;
        }
    }

    return player->currentMask;
}

s32 Player_OverrideLimbDrawGameplayDefault_Custom(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Player* player)
{
    if (!Player_OverrideLimbDrawGameplayDefault(play, limbIndex, dList, pos, rot, player))
    {
        switch (limbIndex)
        {
        case PLAYER_LIMB_ROOT:
            if (player->stateFlags3 & 0x20000000 && comboIsLinkAdult()) /* PLAYER_STATE3_20000000 */
            {
                pos->y += 1280.0f;
            }
            break;
        }
    }

    return 0;
}

static void Player_FormChangeDeleteEffects(void)
{
    EffectSs* eff;

    for (u32 i = 0; i < gEffectSsTable.size; ++i)
    {
        eff = &gEffectSsTable.data[i];
        if (eff->type == 0xf && ((u32)eff->gfx) == 0x06008860)
        {
            EffectSs_Delete(eff);
        }
    }
}

static Vec3f sTransformPos;

static void Player_FormChangeResetState(Player* this)
{
    this->actor.speed = 0.f;
    this->actor.velocity.x = 0.f;
    this->actor.velocity.y = 0.f;
    this->actor.velocity.z = 0.f;
    this->actor.world.pos.x = sTransformPos.x;
    this->actor.world.pos.y = sTransformPos.y;
    this->actor.world.pos.z = sTransformPos.z;

    /*
     * KLUDGE: This is a workaround for various glitches related to fast-masking
     * quickly on a shore. Should be fixed properly someday.
     */
    this->stateFlags1 ^= 0x08000000;
    this->stateFlags3 &= ~0x8000;

    Player_FormChangeDeleteEffects();
}

static void Player_ToggleForm(PlayState* play, Player* link, int form)
{
    /* Sanity checks */
    if (link->actor.draw == NULL)
        return;
    if (Player_InCsMode(play))
        return;
    if (link->stateFlags1 & 0x207c7080)
        return;
    if (link->stateFlags3 & 0x1000)
        return;

    /* Toggle form */
    if (gSave.playerForm == form)
        gSave.playerForm = MM_PLAYER_FORM_HUMAN;
    else
        gSave.playerForm = (u8)form;

    /* Start the process */
    *((u8*)link + 0xae7) = 0;
    link->actor.update = Player_UpdateForm;
    link->actor.draw = NULL;
    sTransformPos.x = link->actor.world.pos.x;
    sTransformPos.y = link->actor.world.pos.y;
    sTransformPos.z = link->actor.world.pos.z;
    Player_FormChangeResetState(link);
}

static s8 sNextForm = -1;

void Player_TryUpdateForm(Player* this, PlayState* play)
{
    if (!Config_Flag(CFG_MM_FAST_MASKS))
        return;

    if (sNextForm != -1)
    {
        Player_ToggleForm(play, this, sNextForm);
        sNextForm = -1;
    }
    else if (this->actor.update == Player_UpdateForm)
    {
        Player_FormChangeResetState(this);
        Player_UpdateForm(this, play);
        if (this->actor.update != Player_UpdateForm)
            this->stateFlags1 &= ~PLAYER_ACTOR_STATE_FROZEN;
    }
}

static void Player_ToggleFormDelayed(int form)
{
    sNextForm = (s8)form;
    Player_FormChangeDeleteEffects();
}

void Player_UseItem(PlayState* play, Player* this, s16 itemId)
{
    void (*Player_UseItemImpl)(PlayState* play, Player* this, s16 itemId);
    u8 useDefault;

    useDefault = 1;

    if (Config_Flag(CFG_MM_FAST_MASKS))
    {
        switch (itemId)
        {
        case ITEM_MM_MASK_DEKU:
            Player_ToggleFormDelayed(MM_PLAYER_FORM_DEKU);
            useDefault = 0;
            break;
        case ITEM_MM_MASK_GORON:
            Player_ToggleFormDelayed(MM_PLAYER_FORM_GORON);
            useDefault = 0;
            break;
        case ITEM_MM_MASK_ZORA:
            Player_ToggleFormDelayed(MM_PLAYER_FORM_ZORA);
            useDefault = 0;
            break;
        case ITEM_MM_MASK_FIERCE_DEITY:
            Player_ToggleFormDelayed(MM_PLAYER_FORM_FIERCE_DEITY);
            useDefault = 0;
            break;
        }
    }

    if (useDefault)
    {
        Player_UseItemImpl = OverlayAddr(0x80831990);
        Player_UseItemImpl(play, this, itemId);
    }
}

/* Hammer Stuff */

s32 Player_CustomActionToModelGroup(Player* player, s32 itemAction) {
    if (itemAction == PLAYER_CUSTOM_IA_HAMMER) return 10; /* uses deku stick model group but does not draw deku stick because of the way the original draw code for it works */

    u8* sActionModelGroups = (u8*)0x801BFF3C; /* using original table also means original glitches, if that matters */
    s32 modelGroup = sActionModelGroups[itemAction];
    /* if ((modelGroup == PLAYER_MODELGROUP_ONE_HAND_SWORD) && Player_IsGoronOrDeku(player)) { */
    if ((modelGroup == 2) && (player->transformation == MM_PLAYER_FORM_GORON || player->transformation == MM_PLAYER_FORM_DEKU)) {
        /* return PLAYER_MODELGROUP_1;  */
        return 1;
    }
    return modelGroup;
}

PATCH_FUNC(0x80123960, Player_CustomActionToModelGroup)

/*
    first two arguments match Player_SetUpperAction which allows it to replace relevant calls in which the third argument would always be this->heldItemAction
*/
void Player_SetCustomItemActionUpperFunc(PlayState* play, Player* player) {
    PlayerUpperActionFunc* sPlayerUpperActionUpdateFuncs = (PlayerUpperActionFunc*)OverlayAddr(0x8085c9f0);
    void (*Player_SetUpperAction)(PlayState* play, Player* this, PlayerUpperActionFunc upperActionFunc) = OverlayAddr(0x8082f43c);
    s8 upperItemAction = player->heldItemAction;

    if (upperItemAction == PLAYER_CUSTOM_IA_HAMMER) {
        upperItemAction = PLAYER_IA_SWORD_TWO_HANDED;
    }

    /* If more custom items were to be added that go to this extent I would suggest a sPlayerCustomUpperActionUpdateFuncs array */
    Player_SetUpperAction(play, player, sPlayerUpperActionUpdateFuncs[upperItemAction]);
}

void Player_RunCustomItemActionInitFunc(PlayState* play, Player* player, s32 itemAction) {
    PlayerInitItemActionFunc* sPlayerItemActionInitFuncs = (PlayerInitItemActionFunc*)OverlayAddr(0x8085cb3c);

    if (itemAction == PLAYER_CUSTOM_IA_HAMMER) {
        itemAction = PLAYER_IA_SWORD_TWO_HANDED;
    }

    /* If more custom items were to be added that go to this extent I would suggest a sPlayerItemActionInitFuncs array */
    sPlayerItemActionInitFuncs[itemAction](play, player);
}

PATCH_CALL(0x80830A14, Player_SetCustomItemActionUpperFunc);
PATCH_CALL(0x808488B4, Player_SetCustomItemActionUpperFunc);
PATCH_CALL(0x80848B10, Player_SetCustomItemActionUpperFunc);
PATCH_CALL(0x8082F914, Player_RunCustomItemActionInitFunc); /* this was not a function but it's ok */

PlayerMeleeWeapon Player_CustomMeleeWeaponFromIA(PlayerItemAction itemAction) {
    PlayerMeleeWeapon weapon = GET_MELEE_WEAPON_FROM_IA(itemAction);

    if (((weapon > PLAYER_MELEEWEAPON_NONE) && (weapon < PLAYER_MELEEWEAPON_MAX)) || (weapon == PLAYER_MELEEWEAPON_HAMMER)) {
        return weapon;
    }

    return PLAYER_MELEEWEAPON_NONE;
}

PATCH_FUNC(0x80124168, Player_CustomMeleeWeaponFromIA);

void Player_GetCustomSwordLength(PlayState* play, Player* player) {
    Vec3f* D_801C0994 = (Vec3f*)0x801C0994;
    f32* sMeleeWeaponLengths = (f32*)0x801C0D78;
    PlayerMeleeWeapon (*Player_GetMeleeWeaponHeld)(Player* this) = (void*)0x80124190;

    if (player->itemAction == PLAYER_CUSTOM_IA_HAMMER) {
        D_801C0994->x = 2500.0f;
    } else if (player->itemAction == PLAYER_IA_DEKU_STICK) {
        D_801C0994->x = player->unk_B0C * 5000.0f;
    } else {
        D_801C0994->x = sMeleeWeaponLengths[Player_GetMeleeWeaponHeld(player)];
    }
}

static MeleeWeaponDamageInfo megatonHammerDmgInfo = { DMG_GORON_PUNCH, 2, 4, 2, 4 };

void Player_SetMeleeWeaponInfo(Player* this, PlayerMeleeWeaponAnimation meleeWeaponAnim) {
    MeleeWeaponDamageInfo* D_8085D09C = OverlayAddr(0x8085D09C);
    MeleeWeaponDamageInfo* dmgInfo = &D_8085D09C[0];
    PlayerMeleeWeapon (*Player_GetMeleeWeaponHeld)(Player* this) = (void*)0x80124190;
    void (*func_80833728)(Player* this, s32 index, u32 dmgFlags, s32 damage) = OverlayAddr(0x80833728);

    u32 dmgFlags;
    s32 damage;

    if (this->actor.id == ACTOR_EN_TEST3) {
        /* Was Kafei originally intended to be able to punch? (Part of the original function)*/
        meleeWeaponAnim = PLAYER_MWA_GORON_PUNCH_LEFT;
        this->meleeWeaponAnimation = -1;
        dmgFlags = dmgInfo->dmgFlags;
    } else if (this->heldItemAction == PLAYER_CUSTOM_IA_HAMMER) {
        dmgInfo = &megatonHammerDmgInfo;
        dmgFlags = dmgInfo->dmgFlags;

    } else {
        dmgInfo = &D_8085D09C[(this->transformation == MM_PLAYER_FORM_GORON) ? PLAYER_MELEEWEAPON_NONE : Player_GetMeleeWeaponHeld(this)];
        dmgFlags = dmgInfo->dmgFlags;
    }

    damage =
        ((meleeWeaponAnim >= PLAYER_MWA_FLIPSLASH_START) && (meleeWeaponAnim <= PLAYER_MWA_ZORA_JUMPKICK_FINISH))
            ? ((this->transformation == MM_PLAYER_FORM_HUMAN) ? dmgInfo->dmgHumanStrong : dmgInfo->dmgTransformedStrong)
            : ((this->transformation == MM_PLAYER_FORM_HUMAN) ? dmgInfo->dmgHumanNormal : dmgInfo->dmgTransformedNormal);

    func_80833728(this, 0, dmgFlags, damage);
    func_80833728(this, 1, dmgFlags, damage);
}

PATCH_FUNC(0x8083375C, Player_SetMeleeWeaponInfo)

static AttackAnimInfo sHammerAttackAnimInfo[] = {
    /* PLAYER_MWA_HAMMER_FORWARD */
    //{ &gPlayerAnim_link_hammer_hit, &gPlayerAnim_link_hammer_hit_end, &gPlayerAnim_link_hammer_hit_endR, 3, 10 },
    { (PlayerAnimationHeader*)0x0400E4E8, (PlayerAnimationHeader*)0x0400E4F0, (PlayerAnimationHeader*)0x0400E4E0, 3, 10 },
    /* PLAYER_MWA_HAMMER_SIDE */
    //{ &gPlayerAnim_link_hammer_side_hit, &gPlayerAnim_link_hammer_side_hit_end, &gPlayerAnim_link_hammer_side_hit_endR, 2, 11 },
    { (PlayerAnimationHeader*)0x0400E4D8, (PlayerAnimationHeader*)0x0400E4D0, (PlayerAnimationHeader*)0x0400E4C8, 2, 11 },
};

AttackAnimInfo* Player_GetMeleeAttackAnimInfo(void* a0, Player* player, PlayerMeleeWeaponAnimation meleeWeaponAnim) {
    AttackAnimInfo* sMeleeAttackAnimInfo = OverlayAddr(0x8085cd30);
    AttackAnimInfo* ret = &sMeleeAttackAnimInfo[meleeWeaponAnim];

    if (player->heldItemAction == PLAYER_CUSTOM_IA_HAMMER && meleeWeaponAnim < PLAYER_MWA_FLIPSLASH_START) {
        if (meleeWeaponAnim == PLAYER_MWA_RIGHT_SLASH_2H) {
            ret = &sHammerAttackAnimInfo[1];
        } else {
            ret = &sHammerAttackAnimInfo[0];
        }
    }

    return ret;
}

s32 Player_IsHoldingTwoHandedWeapon(Player* player) {
    if (((player->heldItemAction >= PLAYER_IA_SWORD_TWO_HANDED) && (player->heldItemAction <= PLAYER_IA_DEKU_STICK)) || (player->heldItemAction == PLAYER_CUSTOM_IA_HAMMER)) {
        return 1;
    }

    return 0;
}

PATCH_FUNC(0x801241b4, Player_IsHoldingTwoHandedWeapon)

static s8 sHammerAttackAnims[] = {
    PLAYER_MWA_FORWARD_SLASH_1H,
    PLAYER_MWA_RIGHT_SLASH_1H,
    PLAYER_MWA_FORWARD_SLASH_1H,
    PLAYER_MWA_RIGHT_SLASH_1H,
};

void Player_SpecialMeleeWeaponAnim(Player* this, void* a1, PlayerMeleeWeaponAnimation* animation) {
    s32 temp_a1 = this->unk_AE3[this->unk_ADE];

    if (this->heldItemAction == PLAYER_CUSTOM_IA_HAMMER) {
        *animation = sHammerAttackAnims[temp_a1];
        /* no combo and no lunge */
        this->unk_ADD = 0;
        this->stateFlags2 &= ~(1 << 30);
    } else if (this->heldItemAction == PLAYER_IA_DEKU_STICK) {
        *animation = PLAYER_MWA_FORWARD_SLASH_1H;
    }
}

s32 Player_CanQuickspin(Player* this) {
    s8 sp3C[4];
    s8* iter;
    s8* iter2;
    s8 temp1;
    s8 temp2;
    s32 i;

    if (this->heldItemAction == PLAYER_IA_DEKU_STICK || this->heldItemAction == PLAYER_CUSTOM_IA_HAMMER) {
        return 0;
    }

    iter = &this->unk_ADF[0];
    iter2 = &sp3C[0];
    for (i = 0; i < 4; i++, iter++, iter2++) {
        if ((*iter2 = *iter) < 0) {
            return 0;
        }
        *iter2 *= 2;
    }

    temp1 = sp3C[0] - sp3C[1];
    if (ABS_ALT(temp1) < 10) {
        return 0;
    }

    iter2 = &sp3C[1];
    for (i = 1; i < 3; i++, iter2++) {
        temp2 = *iter2 - *(iter2 + 1);
        if ((ABS_ALT(temp2) < 10) || (temp2 * temp1 < 0)) {
            return 0;
        }
    }

    return 1;
}

PATCH_FUNC(0x808333CC, Player_CanQuickspin)

Actor* Player_FindGrottoNearPos(PlayState* play, Vec3f* refPos, f32 distanceXZ, f32 distanceY) {
    Actor* actor = play->actorCtx.actors[ACTORCAT_ITEMACTION].first;

    while (actor != NULL) {
        if (ACTOR_DOOR_ANA != actor->id) {
            actor = actor->next;
            continue;
        }

        if (VectDist(refPos, &actor->world.pos) <= distanceXZ && fabsf(refPos->y - actor->world.pos.y) <= distanceY) {
            return actor;
        }

        actor = actor->next;
    }

    return NULL;
}

/* This function hits all of the active colliders for all of the HakuginPost actors */
/* This seems bad, but because these actors have a somewhat deranged check to make sure you're in front and facing it, I think this is okay. */
void Player_HammerBonkHakuginPost(BgHakuginPost* hakuginPost) {
    BgHakuginPostUnkStruct* hakuginPostCrazyStruct = actorAddr(ACTOR_BG_HAKUGIN_POST, 0x80a9e028);
    ColliderCylinder* collider;
    s32 i;

    for (i = 0; i < hakuginPostCrazyStruct->count; i++) {
        collider = hakuginPostCrazyStruct->unk_0000[i].collider;

        /* Force hit one of the Hakugin_Post colliders belonging to the one you just smacked with a hammer */
        if (collider != NULL) {
            collider->base.acFlags |= AC_HIT;
        }
    }
}

s32 Player_CollideMeleeWithWall(PlayState* play, Player* this) {
    void (*func_808400CC)(PlayState* play, Player* this) = OverlayAddr(0x808400CC);
    void (*func_80840094)(PlayState* play, Player* this) = OverlayAddr(0x80840094);
    void (*Player_RequestRumble)(PlayState* play, Player* this, s32 sourceIntensity, s32 decayTimer, s32 decayStep, s32 distSq) = OverlayAddr(0x8082df48);
    u8 (*func_8083FF30)(PlayState* play, Player* this) = OverlayAddr(0x8083FF30);
    u8 (*func_8083FFEC)(PlayState* play, Player* this) = OverlayAddr(0x8083FFEC);
    void (*func_80833B18)(PlayState* play, Player* this, s32 arg2, f32 speed, f32 velocityY, s16 arg5, s32 invincibilityTimer) = OverlayAddr(0x80833B18);
    void (*func_8082DF2C)(PlayState* play) = OverlayAddr(0x8082DF2C);
    void (*Player_RequestQuakeAndRumble)(PlayState* play, Player* this, u16 sfxId) = OverlayAddr(0x8083fe90);

    if (this->meleeWeaponState >= 1) {
        s32 temp_v0_3;

        if (this->meleeWeaponAnimation < PLAYER_MWA_SPIN_ATTACK_1H) {
            if (!(this->meleeWeaponQuads[0].base.atFlags & AT_BOUNCED) &&
                !(this->meleeWeaponQuads[1].base.atFlags & AT_BOUNCED)) {
                if (this->skelAnime.curFrame >= 2.0f) {
                    CollisionPoly* poly;
                    s32 bgId;
                    Vec3f spC8;
                    Vec3f pos;
                    Vec3f spB0;
                    Vec3f* var_a1;
                    Vec3f* temp_a0 = &this->meleeWeaponInfo[0].tip;
                    f32 var_fv1;

                    if (this->linearVelocity >= 0.0f) {
                        var_a1 = &this->meleeWeaponInfo[0].base;
                        if ((this->transformation == MM_PLAYER_FORM_GORON) || (this->actor.id == ACTOR_EN_TEST3)) {
                            var_a1 = &this->unk_AF0[1];
                        }

                        var_fv1 = Math_Vec3f_DistXYZAndStoreDiff(temp_a0, var_a1, &spB0);
                        if (var_fv1 != 0.0f) {
                            var_fv1 = (var_fv1 + 10.0f) / var_fv1;
                        }

                        spC8.x = temp_a0->x + (spB0.x * var_fv1);
                        spC8.y = temp_a0->y + (spB0.y * var_fv1);
                        spC8.z = temp_a0->z + (spB0.z * var_fv1);
                        if (BgCheck_EntityLineTest2(&play->colCtx, &spC8, temp_a0, &pos, &poly, 1, 0, 0, 1, &bgId,
                                                    &this->actor)) {
                            if (!SurfaceType_IsIgnoredByEntities(&play->colCtx, poly, bgId) &&
                                (SurfaceType_GetFloorType(&play->colCtx, poly, bgId) != FLOOR_TYPE_6) &&
                                !func_800B90AC(play, &this->actor, poly, bgId, &pos)) {
                                if (this->transformation == MM_PLAYER_FORM_GORON || this->heldItemAction == PLAYER_CUSTOM_IA_HAMMER) { // for hammer
                                    MtxF sp64;
                                    Vec3s actorRot;
                                    DynaPolyActor* temp_v0;

                                    func_8082DF2C(play);
                                    Player_RequestQuakeAndRumble(play, this, NA_SE_IT_HAMMER_HIT);
                                    if (this->transformation == MM_PLAYER_FORM_GORON) {
                                        Actor_SetPlayerImpact(play, PLAYER_IMPACT_GORON_GROUND_POUND, 2, 100.0f,
                                                              &this->actor.world.pos);
                                        func_800C0094(poly, pos.x, pos.y, pos.z, &sp64);
                                        Matrix_MtxFToYXZRot(&sp64, &actorRot, 1);
                                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TEST, pos.x, pos.y, pos.z,
                                                    actorRot.x, actorRot.y, actorRot.z, 500);
                                    }

                                    if (this->heldItemAction == PLAYER_CUSTOM_IA_HAMMER) {
                                        Actor_SetPlayerImpact(play, PLAYER_IMPACT_GORON_GROUND_POUND, 2, 100.0f, &this->actor.world.pos);
                                    }

                                    /*
                                        This statement despite normally only being accessible checks for the goron form a couple more times.
                                        Probably intended for before the Hammer was removed. This piece right here does not check the form and
                                        may also have been intended for the Hammer, but this is not in OoT so I blocked it off anyway.
                                        When enabled it just makes the hammer hit dynapoly objects multiple times.
                                    */
                                    if (bgId != BGCHECK_SCENE) {
                                        temp_v0 = DynaPoly_GetActor(&play->colCtx, bgId);

                                        if (this->heldItemAction != PLAYER_CUSTOM_IA_HAMMER) {
                                            if (((this->meleeWeaponQuads[0].base.atFlags & AT_HIT) &&
                                                (&temp_v0->actor == this->meleeWeaponQuads[0].base.at)) ||
                                                ((this->meleeWeaponQuads[1].base.atFlags & AT_HIT) &&
                                                (&temp_v0->actor == this->meleeWeaponQuads[1].base.at))) {
                                                return 0;
                                            }
                                        } else if (temp_v0 != NULL) { /* hammer versus dynapoly object */
                                            if (temp_v0->actor.id == ACTOR_BG_HAKUGIN_POST) { /* inactive snowhead temple ice pillar */
                                                Player_HammerBonkHakuginPost((BgHakuginPost*)temp_v0);
                                            }
                                        }
                                    }

                                    func_808400CC(play, this);
                                    if (this->transformation != MM_PLAYER_FORM_GORON) {
                                        return 1;
                                    }
                                    return 0;
                                }

                                if (this->linearVelocity >= 0.0f) {
                                    SurfaceMaterial surfaceMaterial =
                                        SurfaceType_GetMaterial(&play->colCtx, poly, bgId);

                                    if (surfaceMaterial == SURFACE_MATERIAL_WOOD) {
                                        CollisionCheck_SpawnShieldParticlesWood(play, &pos, &this->actor.projectedPos);
                                    } else {
                                        pos.x += 8.0f * COLPOLY_GET_NORMAL(poly->normal.x);
                                        pos.y += 8.0f * COLPOLY_GET_NORMAL(poly->normal.y);
                                        pos.x += 8.0f * COLPOLY_GET_NORMAL(poly->normal.z);
                                        CollisionCheck_SpawnShieldParticles(play, &pos);

                                        if (surfaceMaterial == SURFACE_MATERIAL_DIRT_SOFT) {
                                            Player_PlaySfx(this, NA_SE_IT_WALL_HIT_SOFT);
                                        } else {
                                            Player_PlaySfx(this, NA_SE_IT_WALL_HIT_HARD);
                                        }
                                    }

                                    func_80840094(play, this);
                                    Player_RequestRumble(play, this, 180, 20, 100, SQ(0));
                                    this->linearVelocity = -14.0f;
                                }
                            }
                        }
                    }
                }
            } else {
                func_808400CC(play, this);
                func_8082DF2C(play);
                return 1;
            }
        }

        temp_v0_3 = (this->meleeWeaponQuads[0].base.atFlags & AT_HIT) != 0;
        if (temp_v0_3 || (this->meleeWeaponQuads[1].base.atFlags & AT_HIT)) {
            if ((this->meleeWeaponAnimation < PLAYER_MWA_SPIN_ATTACK_1H) &&
                (this->transformation != MM_PLAYER_FORM_GORON)) {
                Actor* temp_v1 = this->meleeWeaponQuads[temp_v0_3 ? 0 : 1].base.at;

                if ((temp_v1 != NULL) && (temp_v1->id != ACTOR_EN_KANBAN)) {
                    func_8082DF2C(play);
                }
            }

            if (!func_8083FF30(play, this)) {
                func_8083FFEC(play, this);
                if (this->actor.colChkInfo.atHitEffect == 1) {
                    this->actor.colChkInfo.damage = 8;
                    func_80833B18(play, this, 4, 0.0f, 0.0f, this->actor.shape.rot.y, 20);
                    return 1;
                }
            }
        }
    }

    return 0;
}

void Player_HammerShockwaveCheck(PlayState* play, Player* this) {
    /* Can be moved */
    void (*Player_RequestQuakeAndRumble)(PlayState* play, Player* this, u16 sfxId) = OverlayAddr(0x8083fe90);
    f32 (*func_80835CD8)(PlayState* play, Player* this, Vec3f* arg2, Vec3f* pos, CollisionPoly** outPoly, s32* outBgId) = OverlayAddr(0x80835CD8);
    s32 (*func_80832754)(Player* this, s32 arg1) = OverlayAddr(0x80832754);

    if (!PlayerAnimation_OnFrame(&this->skelAnime, this->skelAnime.endFrame) && this->heldItemAction == PLAYER_CUSTOM_IA_HAMMER) { // OoT floor bonk function, originally somewhere else completely
        if ((this->meleeWeaponAnimation == PLAYER_MWA_FORWARD_SLASH_2H) ||
            (this->meleeWeaponAnimation == PLAYER_MWA_JUMPSLASH_FINISH)) {
            static Vec3f sShockwaveOffset = { 0.0f, 40.0f, 45.0f };
            static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
            Actor* nearbyGrotto = NULL;
            Vec3f shockwavePos;
            f32 sp2C;
            CollisionPoly* poly;
            s32 bgId;

            shockwavePos.y = func_80835CD8(play, this, &sShockwaveOffset, &shockwavePos, &poly, &bgId); // OoT uses a wrapper for this function that discards the poly and bgId. MM has this too at 0x80835D2C, but I need those for the goron switch check.
            sp2C = this->actor.world.pos.y - shockwavePos.y;

            Math_ScaledStepToS(&this->actor.focus.rot.x, Math_Atan2S_XY(45.0f, sp2C), 800);
            func_80832754(this, 1);

            if ((((this->meleeWeaponAnimation == PLAYER_MWA_FORWARD_SLASH_2H) &&
                PlayerAnimation_OnFrame(&this->skelAnime, 7.0f)) ||
                ((this->meleeWeaponAnimation == PLAYER_MWA_JUMPSLASH_FINISH) &&
                PlayerAnimation_OnFrame(&this->skelAnime, 2.0f))) &&
                (sp2C > -40.0f) && (sp2C < 40.0f)) {
                //func_80842A28(play, this);
                Player_RequestQuakeAndRumble(play, this, NA_SE_IT_HAMMER_HIT);
                EffectSsBlast_SpawnWhiteShockwave(play, &shockwavePos, &zeroVec, &zeroVec);
                Actor_SetPlayerImpact(play, PLAYER_IMPACT_GORON_GROUND_POUND, 2, 100.0f, &this->actor.world.pos);
                play->actorCtx.unk2 = 4; /* flips some things over like tektites and snappers */

                /* Somewhat unusual solution to flipping the goron switch without the pound damage type. */
                if (bgId != BGCHECK_SCENE) {
                    DynaPolyActor* dynaActor = DynaPoly_GetActor(&play->colCtx, bgId);

                    if (dynaActor != NULL && dynaActor->actor.id == ACTOR_BG_HAKUGIN_SWITCH) {
                        /* the cylinder collider for Bg_Hakugin_Switch is at Actor + 0x15C.
                            the purpose of this is to be able to press it with the hammer without there being a damage type for it. */
                        ColliderCylinder* hakuginSwitchCyl = (ColliderCylinder*)((void*)dynaActor + 0x15C);
                        hakuginSwitchCyl->base.acFlags |= AC_HIT;
                    }
                }

                /* Somewhat unusual solution to opening bomb grottos with the Hammer. Alternative option is to make the grotto bomb collider detect goron punch, but ignore it if you are not a human */
                if ((nearbyGrotto = Player_FindGrottoNearPos(play, &shockwavePos, 50.0f, 10.0f)) != NULL) {
                    u8 isBombGrotto = ((nearbyGrotto->params & 0x300) == 0x200);
                    if (isBombGrotto) {
                        ColliderCylinder* bombCollider = ((void*)nearbyGrotto + 0x144); /* collider is only checked if grotto is closed */
                        bombCollider->base.acFlags |= AC_HIT;
                    }

                }
            }
        }
    }
}

PATCH_FUNC(0x808401F4, Player_CollideMeleeWithWall)

void Player_RumbleAgony(void)
{
    if (Config_Flag(CFG_MM_STONE_OF_AGONY) && !gMmExtraFlags3.stoneAgony)
        return;

    Interface_AgonyIconTick();
    Rumble_Request(120, 20, 10, 0);
}
