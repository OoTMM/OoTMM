#ifndef INCLUDED_COMBO_SPECIAL_CONDS_H
#define INCLUDED_COMBO_SPECIAL_CONDS_H

#define SPECIAL_BRIDGE      0
#define SPECIAL_MOON        1
#define SPECIAL_LACS        2
#define SPECIAL_GANON_BK    3
#define SPECIAL_MAJORA      4

#define SPF_STONES                  (1 <<  0)
#define SPF_MEDALLIONS              (1 <<  1)
#define SPF_REMAINS                 (1 <<  2)
#define SPF_SKULLS_GOLD             (1 <<  3)
#define SPF_SKULLS_SWAMP            (1 <<  4)
#define SPF_SKULLS_OCEAN            (1 <<  5)
#define SPF_FAIRIES_WF              (1 <<  6)
#define SPF_FAIRIES_SH              (1 <<  7)
#define SPF_FAIRIES_GB              (1 <<  8)
#define SPF_FAIRIES_ST              (1 <<  9)
#define SPF_FAIRY_TOWN              (1 << 10)
#define SPF_MASKS_REGULAR           (1 << 11)
#define SPF_MASKS_TRANSFORM         (1 << 12)
#define SPF_MASKS_OOT               (1 << 13)
#define SPF_TRIFORCE                (1 << 14)
#define SPF_COIN_RED                (1 << 15)
#define SPF_COIN_GREEN              (1 << 16)
#define SPF_COIN_BLUE               (1 << 17)
#define SPF_COIN_YELLOW             (1 << 18)

int SpecialConds_Eval(int special);

#endif

