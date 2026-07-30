#include <combo.h>

#include <combo/config.h>
#include <combo/dungeon.h>

#include <combo/oot/actors/Door_Shutter.h>
#include <combo/oot/actors/Obj_Switch.h>
#include <combo/oot/actors/Shot_Sun.h>
#include <combo/oot/actors/En_Box.h>
#include <combo/oot/actors/Bg_Jya_Bombchuiwa.h>
#include <combo/oot/actors/Bg_Hidan_Kousi.h>
#include <combo/oot/actors/Bg_Bdan_Switch.h>
#include <combo/oot/actors/Obj_Lightswitch.h>
#include <combo/oot/actors/En_Go2.h>

static Actor *csActor = NULL;

inline void OnePointCutscene_Sfx(s32 *shouldCsPlay, s32 playSfx)
{
    *shouldCsPlay = false;
    if (playSfx)
    {
        Sfx_PlaySuccessChime();
    }
}

static s32 OnePointCutscene_ShouldCutscenePlay(PlayState *play, Actor *actor)
{
    s32 shouldCsPlay = true;
    if (Config_Flag(CFG_OOT_SKIP_CUTSCENES) && actor && actor->id != ACTOR_PLAYER)
    {
        D_8011D3AC = actor->category;
        if (actor->category == ACTORCAT_BG)
        {
            switch (actor->id)
            {
            case ACTOR_BG_HEAVY_BLOCK:
                break;
            case ACTOR_BG_DDAN_KD:
                Flags_SetSwitch(play, actor->params);
                // FALLTHROUGH
            default:
                shouldCsPlay = false;
                OnePointCutscene_Sfx(&shouldCsPlay, true);
            }
        }

        switch (actor->id)
        {
        case ACTOR_OBJ_SWITCH:
        {
            if (Config_Flag(CFG_OOT_CUTSCENE_GLITCH_AID))
            {
                if (play->sceneId == SCE_OOT_DODONGO_CAVERN && actor->params == 0x2020)
                    break;
                if (play->sceneId == SCE_OOT_TEMPLE_WATER && actor->params == 0x1b43)
                    break;
            }
            ObjSwitch *objSwitch = (ObjSwitch *)actor;
            objSwitch->cooldownTimer = 0;
            OnePointCutscene_Sfx(&shouldCsPlay, true);
            break;
        }
        case ACTOR_BG_BDAN_SWITCH:
        {
            // Blue switches in Jabu, left in for glitch purposes
            if (Config_Flag(CFG_OOT_CUTSCENE_GLITCH_AID) && PARAMS_GET_U(actor->params, 0, 8) == 0)
            {
                break;
            }
            BgBdanSwitch *bdanSwitch = (BgBdanSwitch *)actor;
            bdanSwitch->unk_1D8 = bdanSwitch->unk_1DA = 0;
            OnePointCutscene_Sfx(&shouldCsPlay, true);
            break;
        }
        case ACTOR_BG_JYA_BOMBCHUIWA:
        {
            BgJyaBombchuiwa *bombchuiwa = (BgJyaBombchuiwa *)actor;
            if (bombchuiwa->timer >= 0 && bombchuiwa->timer <= 8 && !(bombchuiwa->drawFlags & 0x4))
            {
                bombchuiwa->timer = 9;
            }
            OnePointCutscene_Sfx(&shouldCsPlay, true);
            break;
        }
        case ACTOR_BG_HIDAN_KOUSI:
        {
            BgHidanKousi *hidanKousi = (BgHidanKousi *)actor;
            hidanKousi->actionFunc = actorAddr(ACTOR_BG_HIDAN_KOUSI, 0x8091d1f0);
            OnePointCutscene_Sfx(&shouldCsPlay, true);
            break;
        }
        case ACTOR_EN_GO2:
        {
            EnGo2 *goron = (EnGo2 *)actor;
            goron->isAwake = true;
            OnePointCutscene_Sfx(&shouldCsPlay, false);
            break;
        }
        case ACTOR_EN_SIOFUKI:
        {
            // Allows Actor Glitch in MQ Spirit
            if (Config_Flag(CFG_OOT_CUTSCENE_GLITCH_AID) && Config_IsMq(MQ_TEMPLE_SPIRIT) && play->sceneId == SCE_OOT_TEMPLE_SPIRIT)
            {
                shouldCsPlay = true;
                break;
            }
            OnePointCutscene_Sfx(&shouldCsPlay, false);
            break;
        }
        case ACTOR_EN_BOX:
        {
            // Allows Actor Glitch in MQ Jabu
            if (Config_Flag(CFG_OOT_CUTSCENE_GLITCH_AID) && Config_IsMq(MQ_JABU_JABU) && play->sceneId == SCE_OOT_INSIDE_JABU_JABU && PARAMS_GET_U(actor->params, 0, 5) == 7)
            {
                shouldCsPlay = true;
                break;
            }
            OnePointCutscene_Sfx(&shouldCsPlay, true);
            break;
        }
        case ACTOR_DOOR_SHUTTER:
        {
            if (Config_Flag(CFG_OOT_CUTSCENE_GLITCH_AID))
            {
                if (play->sceneId == SCE_OOT_INSIDE_JABU_JABU && play->roomCtx.curRoom.num == 7 && actor->params == 0xb8)
                    break;
                if (play->sceneId == SCE_OOT_DODONGO_CAVERN && play->roomCtx.curRoom.num == 7 && actor->params == 0xa0)
                    break;
            }
            OnePointCutscene_Sfx(&shouldCsPlay, true);
            break;
        }
        case ACTOR_OBJ_LIGHTSWITCH:
        {
            ObjLightswitch *lightswitch = (ObjLightswitch *)actor;
            lightswitch->toggleDelay = 0;
            OnePointCutscene_Sfx(&shouldCsPlay, true);
            break;
        }
        case ACTOR_SHOT_SUN:
        case ACTOR_BG_HIDAN_FWBIG:
        case ACTOR_EN_EX_ITEM:
        case ACTOR_EN_DNT_NOMAL:
        case ACTOR_EN_DNT_DEMO:
        case ACTOR_BG_HAKA_ZOU:
        case ACTOR_EN_TA:
        case ACTOR_BG_ICE_SHUTTER:
        case ACTOR_OBJ_SYOKUDAI:
        case ACTOR_OBJ_TIMEBLOCK:
        case ACTOR_EN_PO_SISTERS:
        case ACTOR_OBJ_ICE_POLY:
        case ACTOR_BG_YDAN_MARUTA:
        case ACTOR_BG_SPOT18_SHUTTER:
        case ACTOR_BG_SPOT05_SOKO:
        case ACTOR_BG_SPOT06_OBJECTS:
        case ACTOR_BG_SPOT18_BASKET:
        case ACTOR_BG_HIDAN_CURTAIN:
        case ACTOR_BG_MORI_HINERI:
        case ACTOR_BG_MIZU_SHUTTER:
        case ACTOR_BG_HAKA_GATE:
        case ACTOR_EN_KAKASI2:
        case ACTOR_EN_DNT_JIJI:
            OnePointCutscene_Sfx(&shouldCsPlay, true);
            break;
        }
    }
    return shouldCsPlay;
}

s16 OnePointCutscene_InitHook(PlayState *play, s16 csId, s16 timer, Actor *actor, s16 parentCamId)
{
    csActor = actor;
    if (!OnePointCutscene_ShouldCutscenePlay(play, actor))
        return SUBCAM_NONE;

    return _OnePointCutscene_Init(play, csId, timer, actor, parentCamId);
}

s16 OnePointCutscene_EndCutsceneHook(PlayState *play, s16 subCamId)
{
    if (csActor && OnePointCutscene_ShouldCutscenePlay(play, csActor))
        return _OnePointCutscene_EndCutscene(play, subCamId);
    else
        return SUBCAM_NONE;
}
