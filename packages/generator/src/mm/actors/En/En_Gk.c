#include <combo.h>
#include <combo/item.h>
#include <combo/mm/ocarina.h>
#include <combo/data/song_events.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x1d4) = (h); } while (0)

void EnGk_HandlerNull(void)
{
}

void EnGk_LearnSongLullaby(Actor* this, PlayState* play)
{
    if (gMmExtraFlags.songGoron)
    {
        SET_HANDLER(this, EnGk_HandlerNull);
        return;
    }

    if (Actor_HasParentZ(this))
    {
        gMmExtraFlags.songGoron = 1;
        SET_HANDLER(this, EnGk_HandlerNull);
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_SONG_GORON, NPC_MM_GORON_BABY, 9999.f, 9999.f);
    MM_SET_EVENT_WEEK(EV_MM_WEEK_CALMED_GORON_BABY);
}

void EnGk_CheckLullaby(Actor* this, PlayState* play)
{
    if (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT && gSave.playerForm == MM_PLAYER_FORM_GORON)
    {
        if (Ocarina_CheckSongEventSong(play->msgCtx.ocarinaSong, SONG_EVENT_LULLABY_KID))
        {
            Ocarina_ClearLastPlayedSong(play);
            if (gMmExtraFlags.songGoron)
            {
                play->msgCtx.ocarinaSong = OCARINA_SONG_GORON_LULLABY;
            }
            else
            {
                play->msgCtx.ocarinaSong = OCARINA_SONG_GORON_LULLABY_INTRO;
                SET_HANDLER(this, EnGk_LearnSongLullaby);
                Flags_SetSwitch(play, 0x14);
            }
        }
    }
}

PATCH_FUNC(0x80b51410, EnGk_CheckLullaby);

void EnGk_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_BOTTLED_GOLD_DUST)
        npc = NPC_MM_GORON_RACE;
    else
        gi = GI_MM_RECOVERY_HEART;
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80b525c8, EnGk_GiveItem);
