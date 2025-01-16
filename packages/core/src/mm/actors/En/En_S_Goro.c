#include <combo.h>
#include <combo/player.h>
#include <combo/mm/actors/En_S_Goro.h>

#define EN_S_GORO_ACTIONFLAG_ROLLEDUP (1 << 0)
#define EN_S_GORO_ACTIONFLAG_FACEPLAYER (1 << 1)
#define EN_S_GORO_ACTIONFLAG_EYESOPEN (1 << 2)
#define EN_S_GORO_ACTIONFLAG_EARSCOVERED (1 << 3)
#define EN_S_GORO_ACTIONFLAG_ENGAGED (1 << 4)
#define EN_S_GORO_ACTIONFLAG_LASTMESSAGE (1 << 5)
#define EN_S_GORO_ACTIONFLAG_GKQUIET_ACKNOWLEDGED (1 << 6)
#define EN_S_GORO_ACTIONFLAG_SNOREPHASE (1 << 7)
#define EN_S_GORO_ACTIONFLAG_UNK0100 (1 << 8)
#define EN_S_GORO_ACTIONFLAG_HANDTAP (1 << 9)
#define EN_S_GORO_ACTIONFLAG_TIRED (1 << 10)
#define EN_S_GORO_ACTIONFLAG_SUPPRESS_SNORE (1 << 11)

/* `this` isn't necessarily always current actor, but it is for these use-cases. */
u16 EnSGoro_OverrideBombshopGoronTextId(EnSGoro* this, u16 textId)
{
    if (Config_Flag(CFG_MM_KEG_STRENGTH_3) && gSave.info.inventory.upgrades.strength >= 3)
    {
        s32 hasKeg = gSave.info.inventory.items[ITS_MM_KEG] == ITEM_MM_POWDER_KEG;
        switch (textId)
        {
        /* My product is so heavy... */
        case 0x666: /* Human first time, after flavor text */
            return hasKeg
                ? 0x66f /* Goron first time with keg, after flavor text */
                : 0x67c; /* Goron first time no keg, after flavor text */
        case 0x66c: /* Human second time */
            this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
            return hasKeg
                ? 0x679 /* Goron second time with keg */
                : 0x681; /* Goron second time no keg */
        case 0x66a: /* Human first time, night 3 after flavor text */
            return hasKeg
                ? 0x678 /* Goron first time with keg, night 3 after flavor text */
                : 0x67f; /* Goron first time no keg, night 3 after flavor text */
        case 0x66d: /* Human second time, night 3 */
            this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
            return hasKeg
                ? 0x67a /* Goron second time with keg, night 3 */
                : 0x683; /* Goron second time no keg, night 3 */
        }
    }
    return textId;
}
