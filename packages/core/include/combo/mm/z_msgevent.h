#ifndef COMBO_MM_Z_MSGEVENT_H
#define COMBO_MM_Z_MSGEVENT_H

#include <combo.h>

typedef s32 (*MsgScriptCallback)(struct Actor*, struct PlayState*);
typedef s32 (*MsgScriptCmdHandler)(struct Actor*, struct PlayState*, u8**, MsgScriptCallback, s32*);
typedef u8 MsgScript;

extern u8 sMsgScriptCmdSizes[51];
extern MsgScriptCmdHandler sMsgScriptCmdHandlers[51];

#define SCRIPT_PACK_16(h, l) (((h) << 8) | (l))

typedef struct {
    /* 0x0 */ u8 cmd;
} MsgScriptCmdBase;

typedef struct {
    /* 0x0 */ MsgScriptCmdBase base;
    /* 0x1 */ u8 offsetH;
    /* 0x2 */ u8 offsetL;
} MsgScriptCmdCheckStrength3;

#endif
