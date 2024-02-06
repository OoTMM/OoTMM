#ifndef INCLUDED_COMBO_OOT_ENVIRONMENT_H
#define INCLUDED_COMBO_OOT_ENVIRONMENT_H

#include <combo/util.h>

typedef struct
{
    char unk[0xfc];
}
EnvironmentContext;

ASSERT_SIZE(EnvironmentContext, 0xfc);

#endif
