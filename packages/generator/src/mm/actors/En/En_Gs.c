#include <combo.h>
#include <combo/item.h>
#include <combo/hint.h>

struct EnGs;

typedef void (*EnGsFunc)(struct EnGs*);
typedef void (*EnGsActionFunc)(struct EnGs*, PlayState*);

#define ENGS_GET_1F(thisx) ((thisx)->params & 0x1F)
#define ENGS_GET_SWITCH_FLAG(thisx) (((thisx)->params >> 5) & 0x7F)
#define ENGS_GET_F000(thisx) (((thisx)->params >> 0xC) & 0xF)

#define FAIRY_PARAMS(type, boolParam, collectibleFlag) (((type) /* & 0xF */) | (((boolParam) & 0x1) << 8) | ((((collectibleFlag) & 0x7F) << 9) & 0xFE00))

typedef enum {
    /* 0x0 */ FAIRY_TYPE_0,
    /* 0x1 */ FAIRY_TYPE_1,
    /* 0x2 */ FAIRY_TYPE_2,
    /* 0x3 */ FAIRY_TYPE_3,
    /* 0x4 */ FAIRY_TYPE_4,
    /* 0x5 */ FAIRY_TYPE_5,
    /* 0x6 */ FAIRY_TYPE_6,
    /* 0x7 */ FAIRY_TYPE_7,
    /* 0x8 */ FAIRY_TYPE_8,
    /* 0x9 */ FAIRY_TYPE_9,
    /* 0xA */ FAIRY_TYPE_10
} FairyType;

typedef enum {
    /* 0 */ ENGS_0,
    /* 1 */ ENGS_1,
    /* 2 */ ENGS_2,
    /* 3 */ ENGS_3
} EnGsParam;

typedef struct EnGs {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ EnGsActionFunc actionFunc;
    /* 0x194 */ s8 unk_194;
    /* 0x195 */ u8 unk_195;
    /* 0x196 */ u8 switchFlag;
    /* 0x197 */ s8 unk_197;
    /* 0x198 */ s16 unk_198;
    /* 0x19A */ s16 unk_19A;
    /* 0x19C */ s8 unk_19C;
    /* 0x19D */ u8 unk_19D;
    /* 0x19E */ Vec3s unk_19E[3];
    /* 0x1B0 */ Vec3f unk_1B0[2];
    /* 0x1C8 */ s8 unk1C8[0xC];
    /* 0x1D4 */ s16 unk_1D4;
    /* 0x1D6 */ s8 unk1D6[0x6];
    /* 0x1DC */ f32 unk_1DC;
    /* 0x1E0 */ f32 unk_1E0;
    /* 0x1E4 */ f32 unk_1E4;
    /* 0x1E8 */ f32 unk_1E8;
    /* 0x1EC */ f32 unk_1EC;
    /* 0x1F0 */ f32 unk_1F0;
    /* 0x1F4 */ Color_RGB8 unk_1F4;
    /* 0x1F7 */ Color_RGB8 unk_1F7;
    /* 0x1FA */ Color_RGB8 unk_1FA;
    /* 0x200 */ f32 unk_200;
    /* 0x204 */ s32 unk_204;
    /* 0x208 */ s32 unk_208;
    /* 0x20C */ s32 getItemId;
    /* 0x210 */ u16 unk_210;
    /* 0x212 */ s16 csIdList[2];
    /* 0x216 */ s16 unk_216;
    /* 0x218 */ s16 unk_218;
    /* 0x21A */ s16 unk_21A;
    /* 0x21C */ s16 quakeY;
    /* 0x21E */ s16 quakeDuration;
} EnGs; // size = 0x220

void EnGs_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
        npc = NPC_MM_GOSSIP_HEART_PIECE;
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x809989cc, EnGs_GiveItem);
PATCH_CALL(0x80998a30, EnGs_GiveItem);

void EnGs_MessageBox(PlayState* play, Actor* this)
{
    u16 tmp;
    u8 key;

    if (this->params == 1 || this->params == 2)
    {
        tmp = *(u16*)((char*)this + 0x198);
        key = (tmp & 0x1f) | 0x20;
    }
    else
    {
        key = (*(u8*)((char*)this + 0x195) & 0x1f);
        switch (play->sceneId)
        {
        case SCE_MM_MOON_DEKU:
        case SCE_MM_MOON_GORON:
        case SCE_MM_MOON_ZORA:
        case SCE_MM_MOON_LINK:
            key |= 0x40;
            break;
        }
    }
    PlayerDisplayTextBox(play, 0x20d0, this);
    Hint_Display(play, key);
}

PATCH_CALL(0x80997e98, EnGs_MessageBox);

static void EnGs_AfterDungeonSong(EnGs* this, PlayState* play)
{
    EnGsFunc func_80999AC0 = actorAddr(ACTOR_EN_GS, 0x80999ac0);
    func_80999AC0(this);
    EnGsActionFunc func_809984F4 = actorAddr(ACTOR_EN_GS, 0x809984f4);
    func_809984F4(this, play);
}

void EnGs_WaitForSong(EnGs* this, PlayState* play) {
    switch (play->msgCtx.ocarinaMode) {
        case OCARINA_MODE_EVENT:
            switch (play->msgCtx.ocarinaSong) {
                case OCARINA_SONG_HEALING:
                case OCARINA_SONG_EPONAS:
                case OCARINA_SONG_ZELDAS_LULLABY:
                case OCARINA_SONG_SARIAS:
                    if (!Flags_GetSwitch(play, this->switchFlag)) {
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, this->actor.world.pos.x,
                                    this->actor.world.pos.y + 40.0f, this->actor.world.pos.z, 0, 0, 0,
                                    FAIRY_PARAMS(FAIRY_TYPE_2, false, 0));
                        Actor_PlaySfx(&this->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                        Flags_SetSwitch(play, this->switchFlag);
                    }
                    break;

                case OCARINA_SONG_STORMS:
                    if (!Flags_GetSwitch(play, this->switchFlag)) {
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, this->actor.world.pos.x,
                                    this->actor.world.pos.y + 40.0f, this->actor.world.pos.z, 0, 0, 0,
                                    FAIRY_PARAMS(FAIRY_TYPE_7, false, 0));
                        Actor_PlaySfx(&this->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                        Flags_SetSwitch(play, this->switchFlag);
                    }
                    break;

                case OCARINA_SONG_SONATA:
                    if ((this->actor.params == ENGS_1) && (gSaveContext.save.playerForm == MM_PLAYER_FORM_DEKU)) {
                        this->unk_194 = 1;
                        this->unk_19C = 5;
                        this->unk_19A |= 1;
                        EnGs_AfterDungeonSong(this, play);
                    }
                    break;

                case OCARINA_SONG_NEW_WAVE:
                    if ((this->actor.params == ENGS_1) && (gSaveContext.save.playerForm == MM_PLAYER_FORM_ZORA)) {
                        this->unk_194 = 3;
                        this->unk_19C = 5;
                        this->unk_19A |= 1;
                        EnGs_AfterDungeonSong(this, play);
                    }
                    break;

                case OCARINA_SONG_GORON_LULLABY:
                    if ((this->actor.params == ENGS_1) && (gSaveContext.save.playerForm == MM_PLAYER_FORM_GORON)) {
                        this->unk_194 = 2;
                        this->unk_19C = 5;
                        this->unk_19A |= 1;
                        EnGs_AfterDungeonSong(this, play);
                    }
                    break;

                default:
                    break;
            }
            break;

        case OCARINA_MODE_NONE:
        case OCARINA_MODE_END:
            EnGsActionFunc func_80998300 = actorAddr(ACTOR_EN_GS, 0x80998300);
            func_80998300(this, play);
        /* Fallthrough */
        case OCARINA_MODE_APPLY_DOUBLE_SOT:
            EnGsActionFunc func_80997D14 = actorAddr(ACTOR_EN_GS, 0x80997d14);
            func_80997D14(this, play);
            break;

        default:
            break;
    }
}

PATCH_FUNC(0x8099807c, EnGs_WaitForSong)
