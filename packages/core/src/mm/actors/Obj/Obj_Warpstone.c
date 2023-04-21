#include <combo.h>

static void ObjWarpstone_Text(GameState_Play* play)
{
    char* b;

    b = play->textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "Save the game and keep playing?" TEXT_NL TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN "Yes" TEXT_NL "No" TEXT_END);
}

int ObjWarpstone_TalkedTo(Actor* this, GameState_Play* play)
{
    if (ActorTalkedTo(this))
    {
        ObjWarpstone_Text(play);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80b92ea8, ObjWarpstone_TalkedTo);

void ObjWarpstone_Save(GameState_Play* play)
{
    gSave.isOwlSave = 1;
    PrepareSave(&play->sramCtx);
    comboWriteSave();
    PlaySound(0x4823);
    Message_Close(play);
}
