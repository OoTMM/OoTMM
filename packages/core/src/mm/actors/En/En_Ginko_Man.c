#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

#define SET_HANDLER(a, h)   do { *(void**)(((char*)(a)) + 0x188) = (h); } while (0)
#define BANK_MAX            99999
#define BANK_FEE            (gSave.isNight ? 4 : 0)

static s8 sRewardId;
static u8 sIsDeposit;

static const u16 kRequiredRupeeReward[] = { 200, 500, 1000 };

static void EnGinkoMan_CheckRewardHandler(Actor* this, GameState_Play* play);
static void EnGinkoMan_DefaultHandler(Actor* this, GameState_Play* play);

static void EnGinkoMan_DisplayMainBox(Actor* this, GameState_Play* play)
{
    char* b;

    PlayerDisplayTextBox(play, 0x44c, this);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "You have " TEXT_COLOR_PINK);
    comboTextAppendNum(&b, gSave.bankRupees);
    comboTextAppendStr(&b, " Rupee");
    if (gSave.bankRupees != 1)
        comboTextAppendStr(&b, "s");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " in the bank." TEXT_NL TEXT_CHOICE3);
    comboTextAppendStr(&b, gSave.bankRupees == BANK_MAX ? TEXT_COLOR_RED : TEXT_COLOR_GREEN);
    comboTextAppendStr(&b, " Deposit" TEXT_NL);
    comboTextAppendStr(&b, gSave.bankRupees <= BANK_FEE ? TEXT_COLOR_RED : TEXT_COLOR_GREEN);
    comboTextAppendStr(&b, " Withdraw" TEXT_NL TEXT_COLOR_GREEN " Cancel" TEXT_END);
}

static int EnGinkoMan_ButtonPress(GameState_Play* play)
{
    u16 buttons;
    int msgState;

    msgState = Message_GetState(&play->msgCtx);
    if (msgState != 4 && msgState != 5 && msgState != 14)
        return 0;
    buttons = play->gs.input[0].pressed.buttons;
    if (buttons & B_BUTTON)
        return B_BUTTON;
    else if (buttons & A_BUTTON)
        return A_BUTTON;
    else if (buttons & R_TRIG)
        return R_TRIG;
    return 0;
}

static s32 EnGinkoMan_MaxRupees(void)
{
    return gMaxRupees[gSave.inventory.upgrades.wallet];
}

static s32 EnGinkoMan_CurrentRupees(void)
{
    s32 rupees;
    s32 maxRupees;

    rupees = gSave.playerData.rupees;
    rupees += gSave.rupeesDelta;
    maxRupees = EnGinkoMan_MaxRupees();
    if (rupees < 0)
        rupees = 0;
    if (rupees > maxRupees)
        rupees = maxRupees;
    return rupees;
}

static void EnGinkoMan_Deposit(GameState_Play* play, int amount)
{
    s32 rupees;

    rupees = EnGinkoMan_CurrentRupees();
    if (gSave.bankRupees + amount > BANK_MAX)
        amount = BANK_MAX - gSave.bankRupees;
    if (amount > rupees)
        amount = rupees;
    gSave.bankRupees += amount;
    AddRupeesRaw((s16)-amount);
}

static void EnGinkoMan_Withdraw(GameState_Play* play, int amount)
{
    s32 currentRupees;
    s32 maxRupees;

    /* Deduct the main amount */
    currentRupees = EnGinkoMan_CurrentRupees();
    maxRupees = EnGinkoMan_MaxRupees();
    if (amount > gSave.bankRupees)
        amount = gSave.bankRupees;
    if (amount > maxRupees - currentRupees)
        amount = maxRupees - currentRupees;
    if (amount == 0)
        return;
    gSave.bankRupees -= amount;
    AddRupeesRaw((s16)amount);

    /* Deduct the fee */
    if (gSave.bankRupees > BANK_FEE)
        gSave.bankRupees -= BANK_FEE;
    else
    {
        AddRupeesRaw(-(BANK_FEE - gSave.bankRupees));
        gSave.bankRupees = 0;
    }
}

static void EnGinkoMan_DoTransfer(GameState_Play* play, int amount)
{
    if (sIsDeposit)
        EnGinkoMan_Deposit(play, amount);
    else
        EnGinkoMan_Withdraw(play, amount);
}

static int EnGinkoMan_Reward(void)
{
    int npc;

    for (int i = 0; i < 3; ++i)
    {
        npc = NPC_MM_BANK_1 + i;
        if (BITMAP8_GET(gSharedCustomSave.mm.npc, npc))
            continue;
        if (gSave.bankRupees < kRequiredRupeeReward[i])
            return -1;
        return i;
    }

    return -1;
}

static void EnGinkoMan_GiveRewardHandler(Actor* this, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_PLAYER(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;
    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        BITMAP8_SET(gSharedCustomSave.mm.npc, NPC_MM_BANK_1 + sRewardId);
        SET_HANDLER(this, EnGinkoMan_CheckRewardHandler);
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_WALLET, NPC_MM_BANK_1 + sRewardId, 9999.f, 9999.f);
}

static void EnGinkoMan_CheckRewardHandler(Actor* this, GameState_Play* play)
{
    int reward;

    this->parent = NULL;
    reward = EnGinkoMan_Reward();
    if (reward == -1)
    {
        SET_HANDLER(this, EnGinkoMan_DefaultHandler);
        return;
    }

    sRewardId = reward;
    SET_HANDLER(this, EnGinkoMan_GiveRewardHandler);
    EnGinkoMan_GiveRewardHandler(this, play);
}

static void EnGinkoMan_TransferHandler(Actor* this, GameState_Play* play)
{
    int amount;

    switch (EnGinkoMan_ButtonPress(play))
    {
    case A_BUTTON:
        amount = play->msgCtx.bankRupeesSelected;
        break;
    case B_BUTTON:
        amount = 0;
        break;
    case R_TRIG:
        amount = 9999;
        break;
    default:
        return;
    }

    Message_Close(play);
    if (amount)
        EnGinkoMan_DoTransfer(play, amount);
    SET_HANDLER(this, EnGinkoMan_CheckRewardHandler);
    EnGinkoMan_CheckRewardHandler(this, play);
}

static void EnGinkoMan_MainBoxHandler(Actor* this, GameState_Play* play)
{
    int action;

    switch (EnGinkoMan_ButtonPress(play))
    {
    case A_BUTTON:
        action = play->msgCtx.choiceIndex;
        break;
    case B_BUTTON:
        action = 2;
        break;
    default:
        return;
    }

    sIsDeposit = 0;
    switch (action)
    {
    case 0:
        sIsDeposit = 1;
        /* Fallthrough */
    case 1:
        PlayerDisplayTextBox(play, 0x450, this);
        SET_HANDLER(this, EnGinkoMan_TransferHandler);
        break;
    case 2:
        Message_Close(play);
        SET_HANDLER(this, EnGinkoMan_DefaultHandler);
        break;
    }
}

static void EnGinkoMan_DefaultHandler(Actor* this, GameState_Play* play)
{
    if (!ActorTalkedTo(this))
    {
        ActorEnableTalk(this, play, 100.f);
        return;
    }

    EnGinkoMan_DisplayMainBox(this, play);
    SET_HANDLER(this, EnGinkoMan_MainBoxHandler);
}

PATCH_FUNC(0x80a645a4, EnGinkoMan_DefaultHandler);
