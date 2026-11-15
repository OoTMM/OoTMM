#ifndef COMBO_ENEMY_SOUL_PREVIEW_H
#define COMBO_ENEMY_SOUL_PREVIEW_H

#include <combo/types.h>
#include <combo/math/vec.h>

typedef struct PlayState PlayState;
typedef struct {
    PlayState* play;
    u32 table;
    f32 x, y, z;
    s16 id, yaw, soul;
    u16 params;
    u8 sourceGame, soulGame;
    struct Actor* actor;
    Vec3f* position;
    f32 height;
    u16 radius;
} EnemySoulPreviewRequest;

int comboEnemyFindFloor(PlayState*, struct Actor*, Vec3f*, f32, u16, int);
void comboEnemySoulPreviewGateBegin(void);
extern u32 gEnemySoulPreviewSerial;
void comboEnemySoulPreviewObserve(int game, s16 gi, int present, int enemy);
int comboEnemySoulPreviewGate(PlayState* play, int sourceGame, s16 id, u16 params, f32 x, f32 y, f32 z, s16 yaw, int allowed);
void comboEnemySoulPreviewDraw(PlayState* play);
void comboEnemySoulPreviewReset(void);

#endif
