#include <combo.h>

#if defined(GAME_OOT)
# define ITEM_BOW ITEM_OOT_BOW
# define ITEM_ARROW_FIRE ITEM_OOT_ARROW_FIRE
# define ITEM_ARROW_ICE ITEM_OOT_ARROW_ICE
# define ITEM_ARROW_LIGHT ITEM_OOT_ARROW_LIGHT
# define ITEM_BOW_ARROW_FIRE ITEM_OOT_BOW_ARROW_FIRE
# define ITEM_BOW_ARROW_ICE ITEM_OOT_BOW_ARROW_ICE
# define ITEM_BOW_ARROW_LIGHT ITEM_OOT_BOW_ARROW_LIGHT

#define ITS_BOW ITS_OOT_BOW
#define ITS_ARROW_FIRE ITS_OOT_ARROW_FIRE
#define ITS_ARROW_ICE ITS_OOT_ARROW_ICE
#define ITS_ARROW_LIGHT ITS_OOT_ARROW_LIGHT

#define IA_BOW          0x08
#define IA_ARROW_FIRE   0x09
#define IA_ARROW_ICE    0x0a
#define IA_ARROW_LIGHT  0x0b

#define EN_ARROW_CTOR   0x8088441c
#define EN_ARROW_DTOR   0x80884624

#define BUTTON(x)   (gSave.equips.buttonItems[(x)])
#endif

#if defined(GAME_MM)
# define ITEM_BOW ITEM_MM_BOW
# define ITEM_ARROW_FIRE ITEM_MM_ARROW_FIRE
# define ITEM_ARROW_ICE ITEM_MM_ARROW_ICE
# define ITEM_ARROW_LIGHT ITEM_MM_ARROW_LIGHT
# define ITEM_BOW_ARROW_FIRE ITEM_MM_BOW_ARROW_FIRE
# define ITEM_BOW_ARROW_ICE ITEM_MM_BOW_ARROW_ICE
# define ITEM_BOW_ARROW_LIGHT ITEM_MM_BOW_ARROW_LIGHT

#define ITS_BOW ITS_MM_BOW
#define ITS_ARROW_FIRE ITS_MM_ARROW_FIRE
#define ITS_ARROW_ICE ITS_MM_ARROW_ICE
#define ITS_ARROW_LIGHT ITS_MM_ARROW_LIGHT

#define IA_BOW          0x09
#define IA_ARROW_FIRE   0x0a
#define IA_ARROW_ICE    0x0b
#define IA_ARROW_LIGHT  0x0c

#define EN_ARROW_CTOR   0x8088a240
#define EN_ARROW_DTOR   0x8088a464

#define BUTTON(x)   (gSave.itemEquips.buttonItems[0][(x)])
#endif

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
    { ITEM_BOW,         ITS_BOW,         ITEM_BOW,             IA_BOW,          0x2, 0 },
    { ITEM_ARROW_FIRE,  ITS_ARROW_FIRE,  ITEM_BOW_ARROW_FIRE,  IA_ARROW_FIRE,   0x3, 4 },
    { ITEM_ARROW_ICE,   ITS_ARROW_ICE,   ITEM_BOW_ARROW_ICE,   IA_ARROW_ICE,    0x4, 4 },
    { ITEM_ARROW_LIGHT, ITS_ARROW_LIGHT, ITEM_BOW_ARROW_LIGHT, IA_ARROW_LIGHT,  0x5, 8 },
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

    init = actorAddr(AC_EN_ARROW, EN_ARROW_CTOR);
    destroy = actorAddr(AC_EN_ARROW, EN_ARROW_DTOR);

    destroy(arrow, play);
    init(arrow, play);
}

static int IsArrowItem(u8 item)
{
    switch (item)
    {
    case ITEM_BOW:
    case ITEM_BOW_ARROW_FIRE:
    case ITEM_BOW_ARROW_ICE:
    case ITEM_BOW_ARROW_LIGHT:
        return 1;
    default:
        return 0;
    }
}

static void UpdateCButton(Actor_Player* link, GameState_Play* play, const ArrowInfo* info)
{
    /* Update the icon */
    BUTTON(link->heldItemButton) = info->icon;
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

#if defined(GAME_OOT)
    (void)prevEffectState;
#else
    prevEffectState = gSaveContext.magicState;
#endif

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
        if (curInfo->item != ITEM_BOW)
        {
#if defined(GAME_OOT)
            gSaveContext.magicState = 3;
#else
            gSaveContext.magicState = 2;
#endif
        }
        else
        {
#if defined(GAME_OOT)
            gSaveContext.magicState = 0;
#endif
        }

        /* Refund the previous arrow magic */
        if (!comboIsChateauActive())
        {
#if defined(GAME_OOT)
            gSave.playerData.magicAmount += sArrowCycleState.magicCost;
#else
            if (prevEffectState >= 2)
            {
                if (!MM_GET_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI))
                    gSave.playerData.magicAmount += sArrowCycleState.magicCost;
            }
#endif
        }

#if defined(GAME_OOT)
        if (!comboIsChateauActive())
            gSave.playerData.magicAmount -= curInfo->magicCost;
#else
        gSaveContext.magicToConsume = curInfo->magicCost;
#endif
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
    switch(play->sceneId)
    {
        case SCE_MM_SHOOTING_GALLERY:
        case SCE_MM_SHOOTING_GALLERY_SWAMP:
        case SCE_OOT_SHOOTING_GALLERY:
            return;
    }

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

    selectedItem = BUTTON(link->heldItemButton);
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
        item = BUTTON(link->heldItemButton);
        if (curInfo->var == 2 && item != ITEM_BOW && gSave.inventory.items[ITS_BOW] == ITEM_BOW)
        {
            BUTTON(link->heldItemButton) = ITEM_BOW;
            Interface_LoadItemIconImpl(play, link->heldItemButton);
        }
        PlaySound(0x4806);
        return;
    }

    if (curInfo->item == ITEM_BOW && gSaveContext.magicState != 0)
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

    if (curInfo->item == ITEM_BOW) {
        gSaveContext.magicState = 3;
    }
}
