#ifndef COMBO_COMMON_ACTOR_ENGIRLA_H
#define COMBO_COMMON_ACTOR_ENGIRLA_H

#include <combo/common/actor.h>
#include <combo/util.h>
#include <combo/types.h>

typedef struct Actor_EnGirlA Actor_EnGirlA;

typedef int  (*ShopPrecondFn)(GameState_Play*, Actor_EnGirlA*);
typedef void (*ShopBuyFn)(GameState_Play*, Actor_EnGirlA*);

struct Actor_EnGirlA
{
    Actor           base;
    char            unk_13c[0x54];
    u16             messageId2;
    s32             gi;
    u16             disabled;
    char            unk_19a[0x0e];
    u16             angle;
    char            unk_1aa[2];
    ShopPrecondFn   precond;
    ShopBuyFn       quickBuy;
    ShopBuyFn       postBuy;
    s16             price;
    s16             count;
    s16             drawGiId;
    s16             unk;
    void*           func1;

    /* Custom fields */
    u8 shopId;
};

#if defined(GAME_OOT)
_Static_assert(sizeof(Actor_EnGirlA) == 0x1c8, "OoT Actor_EnGirlA size is wrong");
#else
_Static_assert(sizeof(Actor_EnGirlA) == 0x1d0, "MM Actor_EnGirlA size is wrong");
#endif

typedef struct ComboItemQuery ComboItemQuery;
typedef struct ComboItemOverride ComboItemOverride;

void EnGirlA_ItemQuery(ComboItemQuery* q, Actor_EnGirlA* girlA);
void EnGirlA_ItemOverride(ComboItemOverride* o, Actor_EnGirlA* girlA);

#endif
