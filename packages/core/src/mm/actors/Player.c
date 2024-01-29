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

#define PLAYER_CUSTOM_IA_SPELL_MIN  0x37
#define PLAYER_CUSTOM_IA_SPELL_WIND 0x37
#define PLAYER_CUSTOM_IA_SPELL_LOVE 0x38
#define PLAYER_CUSTOM_IA_SPELL_FIRE 0x39

#define PLAYER_CUSTOM_IA_BOOTS_MIN   0x34
#define PLAYER_CUSTOM_IA_BOOTS_IRON  0x34
#define PLAYER_CUSTOM_IA_BOOTS_HOVER 0x35

#define PLAYER_MAGIC_SPELL_MIN  0
#define PLAYER_MAGIC_SPELL_WIND 0
#define PLAYER_MAGIC_SPELL_LOVE 1
#define PLAYER_MAGIC_SPELL_FIRE 2
#define PLAYER_MAGIC_SPELL_MAX  2

#define PLAYER_BOOTS_MIN   0
#define PLAYER_BOOTS_IRON  0
#define PLAYER_BOOTS_HOVER 1
#define PLAYER_BOOTS_MAX   1

static s32 sCustomItemActions[] =
{
    PLAYER_CUSTOM_IA_SPELL_WIND,
    PLAYER_CUSTOM_IA_SPELL_LOVE,
    PLAYER_CUSTOM_IA_SPELL_FIRE,
    PLAYER_CUSTOM_IA_BOOTS_IRON,
    PLAYER_CUSTOM_IA_BOOTS_HOVER,
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

Actor* Player_SpawnMagicSpellActor(GameState_Play* play, Actor_Player* this, s8 magicSpell)
{
    return SpawnActor(&play->actorCtx, play, sMagicSpellActorIds[magicSpell], this->base.position.x, this->base.position.y, this->base.position.z, 0, 0, 0, 0);
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
                    if ((this->av1.actionVar1 != 0) || (gSaveContext.save.fw.set <= 0))
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
                gFwPointerPos = this->base.position;
                gSaveContext.save.fw.set = 1;
                gSaveContext.save.fw.pos.x = gSaveContext.respawn[RESPAWN_MODE_DOWN].pos.x;
                gSaveContext.save.fw.pos.y = gSaveContext.respawn[RESPAWN_MODE_DOWN].pos.y;
                gSaveContext.save.fw.pos.z = gSaveContext.respawn[RESPAWN_MODE_DOWN].pos.z;
                gSaveContext.save.fw.yaw = gSaveContext.respawn[RESPAWN_MODE_DOWN].yaw;
                gSaveContext.save.fw.playerParams = 0x6FF;
                gSaveContext.save.fw.entranceIndex = gSaveContext.respawn[RESPAWN_MODE_DOWN].entrance;
                gSaveContext.save.fw.roomIndex = gSaveContext.respawn[RESPAWN_MODE_DOWN].roomIndex;
                gSaveContext.save.fw.tempSwchFlags = gSaveContext.respawn[RESPAWN_MODE_DOWN].tempSwitchFlags;
                gSaveContext.save.fw.tempCollectFlags = gSaveContext.respawn[RESPAWN_MODE_DOWN].tempCollectFlags;
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

        s32 respawnData = gSaveContext.save.fw.set;

        if (play->msgCtx.choiceIndex == 0)
        {
            s32 entrance = gSaveContext.save.fw.entranceIndex & 0xFF00;
            Vec3f* pos = &gSaveContext.save.fw.pos;
            if (entrance == 0x8a00 || entrance == 0x9400) // Goron Village (Spring) or Goron Village (Winter)
            {
                if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH) && pos->x > 1100.0f) // from Lens Cave
                {
                    // spawn near the owl area instead
                    pos->x = 1189.0f;
                    pos->z = -911.0f;
                }
            }
            else if (entrance == 0x8600) // Woodfall
            {
                if (!MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE) && ABS(pos->z) < 500.0f)
                {
                    if (pos->z > 0) // from front of temple
                    {
                        // spawn at owl statue instead
                        pos->x = 1.0f;
                        pos->y = 200.0f;
                        pos->z = 1094.0f;
                    }
                    else // from back of temple
                    {
                        // spawn near exit to southern swamp instead
                        pos->x = -41.0f;
                        pos->y = 12.0f;
                        pos->z = -1353.0f;
                    }
                }
            }

            gSaveContext.respawnFlag = -8;
            play->transitionTrigger = TRANS_TRIGGER_NORMAL;
            play->nextEntrance = entrance;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
            gSaveContext.respawn[RESPAWN_MODE_DOWN].pos = gSaveContext.save.fw.pos;
            gSaveContext.respawn[RESPAWN_MODE_DOWN].yaw = gSaveContext.save.fw.yaw;
            gSaveContext.respawn[RESPAWN_MODE_DOWN].playerParams = gSaveContext.save.fw.playerParams;
            gSaveContext.respawn[RESPAWN_MODE_DOWN].entrance = entrance;
            gSaveContext.respawn[RESPAWN_MODE_DOWN].roomIndex = gSaveContext.save.fw.roomIndex;
            gSaveContext.respawn[RESPAWN_MODE_DOWN].tempSwitchFlags = gSaveContext.save.fw.tempSwchFlags;
            gSaveContext.respawn[RESPAWN_MODE_DOWN].tempCollectFlags = gSaveContext.save.fw.tempCollectFlags;

            // TODO cancel timers?

            return;
        }

        if (play->msgCtx.choiceIndex == 1)
        {
            gSaveContext.save.fw.set = -respawnData;
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
        if ((magicSpell != PLAYER_MAGIC_SPELL_WIND) || (gSaveContext.save.fw.set <= 0))
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

s16 sBootData[3][17] = {
    { 200, 1000, 300, 700, 550, 270, 1000, 0, 800, 300, -160, 600, 590, 750, 125, 200, 130 },
    { 200, 1000, 300, 700, 550, 270, 600, 600, 800, 550, -100, 600, 540, 270, 25, 0, 130 },
    { 80, 800, 150, 700, 480, 270, 600, 50, 800, 550, -40, 400, 540, 270, 25, 0, 80 },
};

void Player_CheckCustomBoots(GameState_Play* play)
{
    Actor_Player* player = GET_LINK(play);
    if (player->transformation == MM_PLAYER_FORM_HUMAN)
    {
        if (gSaveContext.save.itemEquips.boots > 0)
        {
            s32 currentBoots = gSaveContext.save.itemEquips.boots - 1;
            s16* bootRegs;

            REG(27) = 2000;
            REG(48) = 370;

            if (currentBoots == PLAYER_BOOTS_IRON)
            {
                if (player->state & PLAYER_ACTOR_STATE_WATER)
                {
                    currentBoots = 2; // Iron Underwater
                }
                REG(27) = 500;
                REG(48) = 100;
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
            REG(43) = bootRegs[8];
            R_RUN_SPEED_LIMIT = bootRegs[9];
            REG(68) = bootRegs[10];
            REG(69) = bootRegs[11];
            IREG(66) = bootRegs[12];
            IREG(67) = bootRegs[13];
            IREG(68) = bootRegs[14];
            IREG(69) = bootRegs[15];
            MREG(95) = bootRegs[16];

            if (play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_2)
            {
                R_RUN_SPEED_LIMIT = 500;
            }
        }

        player->base.flags &= ~(1 << 17); // ~ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCH

        switch(gSaveContext.save.itemEquips.boots)
        {
        case 0:
            player->currentBoots = 1; // PLAYER_BOOTS_HYLIAN
            break;
        case PLAYER_BOOTS_IRON + 1:
            player->currentBoots = 6; // PLAYER_BOOTS_GORON
            player->base.flags |= (1 << 17); // ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCH
            break;
        case PLAYER_BOOTS_HOVER + 1:
            break;
        }
    }
    else
    {
        gSaveContext.save.itemEquips.boots = 0;
        player->base.flags &= ~(1 << 17); // ~ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCH
    }
}

s32 Player_CustomUseItem(Actor_Player* this, GameState_Play* play, s32 itemAction)
{
    s32 magicSpell = Player_ActionToMagicSpell(this, itemAction);
    if (magicSpell >= 0)
    {
        if (((magicSpell == PLAYER_MAGIC_SPELL_WIND) && (gSaveContext.save.fw.set > 0)) ||
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
            if (newBoots > 0)
            {
                Player_CheckCustomBoots(play);
            }
            else
            {
                Player_SetBootData(play, this);
            }
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
        this->base.position.y += 60.0f;
        Player_func_80833AA0 = OverlayAddr(0x80833AA0);
        Player_func_80833AA0(this, play);
        return;
    }

    if (this->av2.actionVar2++ == 20)
    {
        gSaveContext.save.fw.set++;
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

    u16 initMode = (player->base.variable & 0xF00) >> 8;
    if (initMode == 6 && gSaveContext.respawnFlag == -8)
    {
        Player_InitFaroresWindRespawn(play, player);
    }
}

PATCH_CALL(0x808424d8, Player_AfterInit);

const u32 gLinkAdultIronBootTex = 0x08000000 | CUSTOM_KEEP_BOOTS_IRON_TEXTURE;
const u32 gLinkAdultIronBootTLUT = 0x08000000 | CUSTOM_KEEP_BOOTS_IRON_TLUT;

static Vtx sIronBootsVtx[87] = {
#include "sIronBootsVtx.vtx.inc"
};

Gfx gLinkAdultLeftIronBootDL[] = {
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

Gfx gLinkAdultRightIronBootDL[] = {
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

void Player_SkelAnime_DrawFlexLod(GameState_Play* play, void** skeleton, Vec3s* jointTable, s32 dListCount,
                                  OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw, Actor_Player* player, s32 lod)
{
    SkelAnime_DrawFlexLod(play, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, &player->base, lod);

    OPEN_DISPS(play->gs.gfx);

    gSPSegment(POLY_OPA_DISP++, 0x08, gCustomKeep);

    if (overrideLimbDraw != Player_OverrideLimbDrawGameplayFirstPerson && gSaveContext.gameMode != 3) // GAMEMODE_END_CREDITS
    {
        if (player->transformation == MM_PLAYER_FORM_HUMAN)
        {
            if (player->currentBoots == 6) // PLAYER_BOOTS_GORON
            {
                gSPDisplayList(POLY_OPA_DISP++, gLinkAdultLeftIronBootDL);
                gSPDisplayList(POLY_OPA_DISP++, gLinkAdultRightIronBootDL);
            }
        }
    }

    CLOSE_DISPS();
}

PATCH_CALL(0x80124858, Player_SkelAnime_DrawFlexLod);

u16 Player_GetFloorSfxByAge(Actor_Player* this, u16 sfxId) {
    if (this->transformation == MM_PLAYER_FORM_HUMAN && this->currentBoots == 6)
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
    // Displaced code:
    if (itemAction == 0x50) // PLAYER_IA_MASK_ZORA
    {
        return 0;
    }
    // End displaced code

    if (itemAction == PLAYER_CUSTOM_IA_BOOTS_IRON)
    {
        return 0;
    }
    return 1;
}

f32 Player_GetEnvironmentWindSpeed(GameState_Play* play)
{
    Actor_Player* player = GET_LINK(play);
    if (player->transformation == MM_PLAYER_FORM_HUMAN && player->currentBoots == 6) // PLAYER_BOOTS_GORON
    {
        return 0.0f;
    }

    // Displaced code:
    return play->envCtx.windSpeed;
}

s32 Player_IsBeingPushed(Actor_Player* this, GameState_Play* play)
{
    if (this->transformation == MM_PLAYER_FORM_HUMAN && this->currentBoots == 6) // PLAYER_BOOTS_GORON
    {
        return 0;
    }

    // Displaced code:
    return this->pushedSpeed != 0.0f || this->windSpeed != 0.0f || play->envCtx.windSpeed >= 50.0f;
}
