#include <combo.h>
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

#define PLAYER_MAGIC_SPELL_MIN  0
#define PLAYER_MAGIC_SPELL_WIND 0
#define PLAYER_MAGIC_SPELL_LOVE 1
#define PLAYER_MAGIC_SPELL_FIRE 2
#define PLAYER_MAGIC_SPELL_MAX  2

static s32 sCustomItemActions[] =
{
    PLAYER_CUSTOM_IA_SPELL_WIND,
    PLAYER_CUSTOM_IA_SPELL_LOVE,
    PLAYER_CUSTOM_IA_SPELL_FIRE,
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

            gSaveContext.respawnFlag = 3;
            play->transitionTrigger = TRANS_TRIGGER_NORMAL;
            play->nextEntrance = entrance;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
            gSaveContext.respawn[RESPAWN_MODE_TOP].pos = gSaveContext.save.fw.pos;
            gSaveContext.respawn[RESPAWN_MODE_TOP].yaw = gSaveContext.save.fw.yaw;
            gSaveContext.respawn[RESPAWN_MODE_TOP].playerParams = gSaveContext.save.fw.playerParams;
            gSaveContext.respawn[RESPAWN_MODE_TOP].entrance = entrance;
            gSaveContext.respawn[RESPAWN_MODE_TOP].roomIndex = gSaveContext.save.fw.roomIndex;
            gSaveContext.respawn[RESPAWN_MODE_TOP].tempSwitchFlags = gSaveContext.save.fw.tempSwchFlags;
            gSaveContext.respawn[RESPAWN_MODE_TOP].tempCollectFlags = gSaveContext.save.fw.tempCollectFlags;

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

s32 Player_CustomUseItem(Actor_Player* this, s32 itemAction)
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
    if (initMode == 6 && gSaveContext.respawnFlag == 3)
    {
        Player_InitFaroresWindRespawn(play, player);
    }
}

PATCH_CALL(0x808424d8, Player_AfterInit);
