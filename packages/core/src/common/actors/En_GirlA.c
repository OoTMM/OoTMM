#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define SOLD_OUT (GI_MM_SOLD_OUT | MASK_FOREIGN_GI)
#else
# define SOLD_OUT GI_MM_SOLD_OUT
#endif

int shopReadFlag(int);

void EnGirlA_ItemQuery(ComboItemQuery* q, Actor_EnGirlA* girlA, int flags)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_SHOP;
    q->id = girlA->shopId;
    q->ovFlags = flags | OVF_PRECOND;
    q->gi = girlA->gi;
    q->giRenew = SOLD_OUT;

    if (shopReadFlag(girlA->shopId))
    {
        q->ovFlags |= OVF_RENEW;

#if defined(GAME_MM)
        switch (girlA->shopId)
        {
        case 0x02:
        case 0x03:
        case 0x04:
            bzero(q, sizeof(*q));
            q->ovType = OV_NONE;
            q->gi = GI_MM_SOLD_OUT;
            break;
        }
#endif
    }
}

void EnGirlA_ItemOverride(ComboItemOverride* o, Actor_EnGirlA* girlA, int flags)
{
    ComboItemQuery q;

    EnGirlA_ItemQuery(&q, girlA, flags);
    comboItemOverride(o, &q);
}
