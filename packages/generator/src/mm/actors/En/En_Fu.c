#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

static u8 sIsFirstReward;

int EnFu_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParentZ(this);
    if (ret && sIsFirstReward)
    {
        gMmExtraFlags2.honeyDarling = 1;
        sIsFirstReward = 0;
    }
    return ret;
}

PATCH_CALL(0x80963574, EnFu_HasGivenItem);

void EnFu_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_MM_RUPEE_PURPLE:
        if (!gMmExtraFlags2.honeyDarling)
        {
            npc = NPC_MM_HONEY_DARLING_1;
            sIsFirstReward = 1;
        }
        break;
    case GI_MM_HEART_PIECE:
        npc = NPC_MM_HONEY_DARLING_2;
        break;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x809635cc, EnFu_GiveItem);
PATCH_CALL(0x809635ec, EnFu_GiveItem);

static int EnFu_CheckAgeReq(int adultFlag, int childFlag)
{
    if (Config_Flag(adultFlag))
        return comboIsLinkAdult();
    if (Config_Flag(childFlag))
        return !comboIsLinkAdult();
    return 1;
}

static int EnFu_CheckCurrentGameAgeReq(PlayState* play)
{

    Player* player;
    player = GET_PLAYER(play);
    switch (CURRENT_DAY)
    {
    case 1:
        return EnFu_CheckAgeReq(
            CFG_MM_AGE_REQ_ADULT_BOMBCHU,
            CFG_MM_AGE_REQ_CHILD_BOMBCHU
        );
    case 2:
        return EnFu_CheckAgeReq(
            CFG_MM_AGE_REQ_ADULT_BOMB,
            CFG_MM_AGE_REQ_CHILD_BOMB
        );
    case 3:
        if (player->transformation == MM_PLAYER_FORM_DEKU)
            return 1;

        return EnFu_CheckAgeReq(
            CFG_MM_AGE_REQ_ADULT_BOW,
            CFG_MM_AGE_REQ_CHILD_BOW
        );

    default:
        return 1;
    }
}

static u16 EnFu_GetWrongFormText(void)
{
    switch (CURRENT_DAY)
    {
        case 1:
        case 2:
            return 0x286F;
        case 3:
            return 0x2841;
    }
}

static void EnFu_SetTextId(Actor* actor, u16 textId)
{
    *(u16*)((u8*)actor + 0x552) = textId;
}

static int EnFu_MessageShouldAdvance_Age(PlayState* play)
{
    Actor* actor;
    u16 textId;

    if (!Message_ShouldAdvance(play))
        return 0;
    if (play->msgCtx.currentTextId == 0x2871 && play->msgCtx.choiceIndex == 0 && !EnFu_CheckCurrentGameAgeReq(play))
    {
        actor = play->msgCtx.unkActor;
        textId = EnFu_GetWrongFormText();
        Message_StartTextbox(play, textId, actor);
        EnFu_SetTextId(actor, textId);
        return 0;
    }
    return 1;
}

PATCH_CALL(0x8096259c, EnFu_MessageShouldAdvance_Age);