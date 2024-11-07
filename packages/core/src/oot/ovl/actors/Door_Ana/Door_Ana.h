#ifndef OVL_DOOR_ANA
#define OVL_DOOR_ANA

#include <combo.h>

struct DoorAna;

typedef void (*DoorAnaActionFunc)(struct DoorAna*, PlayState*);

typedef struct DoorAna {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ ColliderCylinder collider;
    /* 0x0198 */ DoorAnaActionFunc actionFunc;
} DoorAna; // size = 0x019C

#endif
