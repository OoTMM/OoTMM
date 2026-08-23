#ifndef INCLUDED_COMBO_CHECKS_H
#define INCLUDED_COMBO_CHECKS_H

#include <combo/types.h>

typedef struct ComboItemQuery ComboItemQuery;

u32 Checks_MakeOverrideKey(const ComboItemQuery* q);
u32 Checks_MakeNpcOverrideKey(int npc);

#endif
