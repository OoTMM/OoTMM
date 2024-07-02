#ifndef INCLUDED_COMBO_OOT_ENVIRONMENT_H
#define INCLUDED_COMBO_OOT_ENVIRONMENT_H

#include <combo/util.h>

typedef struct
{
    char unk_00[0x02];
    u16 sceneTimeSpeed;
    char unk_04[0xdc];
    u8 timeSeqState;
    char unk_e1[0x1b];
}
EnvironmentContext;

ASSERT_SIZE(EnvironmentContext, 0xfc);
ASSERT_OFFSET(EnvironmentContext, sceneTimeSpeed, 0x02);
ASSERT_OFFSET(EnvironmentContext, timeSeqState,   0xe0);

#endif
