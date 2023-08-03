#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x1d4) = (h); } while (0)

void EnGk_HandlerNull(void)
{
}

void EnGk_LearnSongLullaby(Actor* this, GameState_Play* play)
{
    if (gMmExtraFlags.songGoron)
    {
        SET_HANDLER(this, EnGk_HandlerNull);
        return;
    }

    if (Actor_HasParent(this))
    {
        gMmExtraFlags.songGoron = 1;
        SET_HANDLER(this, EnGk_HandlerNull);
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_SONG_GORON, NPC_MM_GORON_BABY, 9999.f, 9999.f);
}

void EnGk_CheckLullaby(Actor* this, GameState_Play* play)
{
    if (play->msgCtx.ocarinaMode == 3 && gSave.playerForm == MM_PLAYER_FORM_GORON)
    {
        switch (play->msgCtx.ocarinaSong)
        {
        case 0x01:
        case 0x0e:
            if (gMmExtraFlags.songGoron)
            {
                play->msgCtx.ocarinaSong = 0x01;
            }
            else
            {
                play->msgCtx.ocarinaSong = 0x0e;
                SET_HANDLER(this, EnGk_LearnSongLullaby);
                SetSwitchFlag(play, 0x14);
            }
            break;
        }
    }
}

PATCH_FUNC(0x80b51410, EnGk_CheckLullaby);

void EnGk_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
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
