#ifndef COMBO_ENEMY_ROOM_MODELS_H
#define COMBO_ENEMY_ROOM_MODELS_H

#include <combo/types.h>

typedef struct ObjectContext ObjectContext;
typedef struct PlayState PlayState;

void* comboObjectSlotSegmentAndPin(ObjectContext* ctx, int slot);
int comboEnemyRoomHasNativeModel(PlayState* play, u16 objectId);
u16 comboEnemyRoomModelHalfDays(PlayState* play, s32 room, u16 objectId);
int comboEnemyRoomModelsPreload(PlayState* play);
int comboEnemyRoomModelsOnlyOom(void);
void comboEnemyRoomModelsReset(void);
void comboEnemyObjectPreloadBegin(s8 room);
int comboEnemyObjectPreloadOom(u16 objectId, s8 room);
int comboEnemyObjectPreload(u16 objectId, s8 room);
void comboEnemyObjectPinFrames(u16 objectId, u16 frames);
void comboEnemyObjectReleaseUnloadedRooms(PlayState* play);
int comboEnemyRoomModelsLive(s32 room);
int comboEnemyRoomHasRetiringActors(s32 room, u16 currentHalfDay);

#endif
