#ifndef COMBO_DRAW_H
#define COMBO_DRAW_H

#include <PR/gbi.h>

#define DRAW_NO_PRE1    0x01
#define DRAW_NO_PRE2    0x02
#define DRAW_RAW        (DRAW_NO_PRE1 | DRAW_NO_PRE2)

typedef struct GfxContext GfxContext;
typedef struct GameState_Play GameState_Play;
typedef struct Actor Actor;

void Draw_SetObjectSegment(GfxContext* gfx, void* buffer);
void Draw_Gi(GameState_Play* play, Actor* actor, s16 gi, int flags);
void Draw_Init2D(Gfx** dl);
void Draw_Blit2D_RGBA32(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale);
void Draw_Blit2D_RGBA16(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale);
void Draw_Blit2D_IA4(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale);
void Draw_GlitterGi(GameState_Play* play, Actor* actor, s16 gi);

#endif
