#include <combo.h>

void EnChanger_Init(Actor* this, GameState_Play* play)
{
    int roomId;
    const float* kPosBottom;
    const float* kPosTop;

    /* Extract values */
    kPosBottom = actorAddr(0x155, 0x80ac3d3c);
    kPosTop = actorAddr(0x155, 0x80ac3d84);
    roomId = play->roomCtx.curRoom.num - 1;
    if (roomId < 0)
        roomId = 0;

    /* Spawn chests */
    if (roomId >= 5)
    {
        SpawnActor(&play->actorCtx, play, AC_EN_BOX, 20.f, 20.f, -2500.f, 0, 0x7fff, 0, 0x402a);
    }
    else
    {
        SpawnActor(&play->actorCtx, play, AC_EN_BOX, kPosBottom[roomId * 3 + 0], kPosBottom[roomId * 3 + 1], kPosBottom[roomId * 3 + 2], 0, 0xc001, 0, 0x4020 | (roomId * 2 + 0));
        SpawnActor(&play->actorCtx, play, AC_EN_BOX, kPosTop[roomId * 3 + 0], kPosTop[roomId * 3 + 1], kPosTop[roomId * 3 + 2], 0, 0x3fff, 0, 0x4020 | (roomId * 2 + 1));
    }

    ActorDestroy(this);
}
