#include <combo.h>

typedef struct
{
    u16 frameDelay;
    s8 magicCost;
    Actor* arrow;
}
ArrowCycleState;

static ArrowCycleState sArrowCycleState;

typedef struct
{
    u8  item;
    u8  slot;
    u8  icon;
    u8  action;
    u16 var;
    u8  magicCost;
}
ArrowInfo;

static const ArrowInfo kArrowsInfo[] = {
    { ITEM_MM_BOW,         ITS_MM_BOW,         ITEM_MM_BOW,             0x9, 0x2, 0 },
    { ITEM_MM_ARROW_FIRE,  ITS_MM_ARROW_FIRE,  ITEM_MM_BOW_ARROW_FIRE,  0xA, 0x3, 4 },
    { ITEM_MM_ARROW_ICE,   ITS_MM_ARROW_ICE,   ITEM_MM_BOW_ARROW_ICE,   0xB, 0x4, 4 },
    { ITEM_MM_ARROW_LIGHT, ITS_MM_ARROW_LIGHT, ITEM_MM_BOW_ARROW_LIGHT, 0xC, 0x5, 8 },
};

static const ArrowInfo* GetArrowInfo(u16 variable)
{
    for (int i = 0; i < ARRAY_SIZE(kArrowsInfo); i++)
    {
        if (kArrowsInfo[i].var == variable)
            return &kArrowsInfo[i];
    }

    return NULL;
}

static u16 GetNextArrowVariable(u16 variable)
{
    switch (variable)
    {
        case 2: return 3;
        case 3: return 4;
        case 4: return 5;
        case 5: return 2;
        default: UNREACHABLE();
    }
}

static int HasEnoughMagicForArrow(s8 prevCost, s8 curCost)
{
    return gSave.playerData.magicAmount >= (curCost - prevCost);
}

static const ArrowInfo* GetNextArrowInfo(u16 variable)
{
    s8 magicCost;
    u16 current;
    const ArrowInfo* info;
    s8 magic;
    int hasMagic;

    magicCost = GetArrowInfo(variable)->magicCost;
    current = variable;
    for (int i = 0; i < ARRAY_SIZE(kArrowsInfo); ++i)
    {
        current = GetNextArrowVariable(current);
        info = GetArrowInfo(current);
        magic = info->magicCost;
        hasMagic = HasEnoughMagicForArrow(magicCost, magic);
        if (info && info->item == gSave.inventory.items[info->slot] && hasMagic)
            return info;
    }

    return NULL;
}

static void ReinitializeArrow(Actor* arrow, GameState_Play* play)
{
    ActorFunc init;
    ActorFunc destroy;

    init = actorAddr(AC_EN_ARROW, 0x8088a240);
    destroy = actorAddr(AC_EN_ARROW, 0x8088a464);

    destroy(arrow, play);
    init(arrow, play);
}

static int IsArrowItem(u8 item)
{
    switch (item)
    {
    case ITEM_MM_BOW:
    case ITEM_MM_BOW_ARROW_FIRE:
    case ITEM_MM_BOW_ARROW_ICE:
    case ITEM_MM_BOW_ARROW_LIGHT:
        return 1;
    default:
        return 0;
    }
}

static void UpdateCButton(Actor_Player* link, GameState_Play* play, const ArrowInfo* info)
{
    /* Update the icon */
    gSave.itemEquips.buttonItems[0][link->heldItemButton] = info->icon;
    Interface_LoadItemIconImpl(play, link->heldItemButton);

    /* Update the action */
    link->itemAction = info->action;
    link->heldItemAction = info->action;
}

static int ActorHelper_DoesActorExist(Actor* actor, GameState_Play* play, int category)
{
    Actor* list;

    list = play->actorCtx.actors[category].first;
    while (list)
    {
        if (list == actor)
            return 1;
        list = list->next;
    }
    return 0;
}

static void HandleFrameDelay(Actor_Player* link, GameState_Play* play, Actor* arrow)
{
    s16 prevEffectState;
    const ArrowInfo* curInfo;
    Actor* special;

    /* Sanity check */
    if (!ActorHelper_DoesActorExist(arrow, play, 0x07)) {
        return;
    }

    prevEffectState = gSaveContext.magicState;
    curInfo = GetArrowInfo(arrow->variable);
    if (arrow && curInfo)
    {
        special = arrow->attachedB;
        if (special)
        {
            ActorRemove(&play->actorCtx, special, play);
            arrow->attachedB = NULL;
        }

        /* Update the magic state */
        if (curInfo->item != ITEM_MM_BOW)
        {
            gSaveContext.magicState = 2;
        }

        /* Refund the previous arrow magic */
        if (prevEffectState >= 2 && !MM_GET_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI))
            gSave.playerData.magicAmount += sArrowCycleState.magicCost;

        /* Set the magic cost */
        gSaveContext.magicToConsume = curInfo->magicCost;
    }
}

static Actor* ArrowCycle_FindArrow(Actor_Player* link, GameState_Play* play)
{
    Actor* attached;

    attached = link->base.attachedB;
    if (attached && attached->id == AC_EN_ARROW && attached->attachedA == &link->base) {
        return attached;
    } else {
        return NULL;
    }
}

void ArrowCycle_Handle(Actor_Player* link, GameState_Play* play)
{
    Actor* arrow;
    Actor* special;
    u8 selectedItem;
    u8 item;
    const ArrowInfo* curInfo;
    const ArrowInfo* nextInfo;

    if (sArrowCycleState.frameDelay >= 1)
    {
        HandleFrameDelay(link, play, sArrowCycleState.arrow);
        sArrowCycleState.arrow = NULL;
        sArrowCycleState.frameDelay = 0;
        sArrowCycleState.magicCost = 0;
        return;
    }

    /* Find the arrow */
    arrow = ArrowCycle_FindArrow(link, play);
    if (!arrow)
        return;

    /* Filter deku bubbles */
    if (!(2 <= arrow->variable && arrow->variable < 6))
        return;

    selectedItem = gSave.itemEquips.buttonItems[0][link->heldItemButton];
    if (!IsArrowItem(selectedItem)) {
        return;
    }

    if (!(play->gs.input[0].pressed.buttons & R_TRIG))
        return;

    play->gs.input[0].pressed.buttons &= ~R_TRIG;

    /* get the various infos */
    curInfo = GetArrowInfo(arrow->variable);
    nextInfo = GetNextArrowInfo(arrow->variable);

    /* Handle not having anything to cycle */
    if (!curInfo || !nextInfo || curInfo->var == nextInfo->var)
    {
        item = gSave.itemEquips.buttonItems[0][link->heldItemButton];
        if (curInfo->var == 2 && item != ITEM_MM_BOW && gSave.inventory.items[ITS_MM_BOW] == ITEM_MM_BOW)
        {
            gSave.itemEquips.buttonItems[0][link->heldItemButton] = ITEM_MM_BOW;
            Interface_LoadItemIconImpl(play, link->heldItemButton);
        }
        PlaySound(0x4806);
        return;
    }

    if (curInfo->item == ITEM_MM_BOW && gSaveContext.magicState != 0)
    {
        PlaySound(0x4806);
        return;
    }

    /* Update the arrow and reload it */
    arrow->variable = nextInfo->var;
    ReinitializeArrow(arrow, play);

    special = arrow->attachedB;
    if (special)
        special->draw = NULL;

    /* Update the C buttons */
    UpdateCButton(link, play, nextInfo);

    /* Prepare the frame skip stuff */
    sArrowCycleState.arrow = arrow;
    sArrowCycleState.frameDelay++;
    sArrowCycleState.magicCost = curInfo->magicCost;

    if (curInfo->item == ITEM_MM_BOW) {
        gSaveContext.magicState = 3;
    }
}
