#include "multi.h"

#define WISPS_MAX 16

static int sWispsCount;
static Vec3f sWispsPos[WISPS_MAX];
static char sNames[9][WISPS_MAX];
static u8 sWispsColorIdx[WISPS_MAX];

typedef struct PACKED
{
    u16 key;
    u16 color;
    Vec3f pos;
    char name[8];
}
MultiExchangePosResponsePacket;

static const u32 kWispColors[] = {
    0xff0000cc, 0x00ff00cc, 0x0000ffcc, 0xffff00cc,
    0xff00ffcc, 0x00ffffcc, 0x000000cc, 0xffffffcc,
    0x7f0000cc, 0x007f00cc, 0x00007fcc, 0x7f7f00cc,
    0x7f007fcc, 0x007f7fcc, 0x7f7f7fcc, 0x3f3f3fcc,
};

void Multi_WispsReset(void)
{
    sWispsCount = 0;
}

static int sendPos(PlayState* play)
{
    Player* link;
    u16 tmp16;

    link = GET_PLAYER(play);
    if (!link || link->actor.id != ACTOR_PLAYER)
        return 1;

    gMultiCtx.pkt.size = 15;
    gMultiCtx.pkt.data[0] = OP_ECHANGE_POS;
    tmp16 = Multi_SceneKey(play);
    memcpy(&gMultiCtx.pkt.data[1], &tmp16, 2);
    memcpy(&gMultiCtx.pkt.data[3], &link->actor.world.pos, 12);
    return Multi_ProtocolSend();
}

static void addWisp(MultiExchangePosResponsePacket* pkt)
{
    int id;

    if (sWispsCount >= WISPS_MAX)
        return;
    id = sWispsCount++;
    sWispsColorIdx[id] = pkt->color;
    sWispsPos[id] = pkt->pos;
    memcpy(sNames[id], pkt->name, 8);
}

void Multi_WispsUpdate(PlayState* play)
{
    Multi_WispsReset();
    if (!sendPos(play))
        return;

    for (;;)
    {
        /* Get packet */
        if (!Multi_ProtocolRecv())
        {
            Multi_WispsReset();
            return;
        }

        /* If the packet is empty, it's the end of the list */
        if (gMultiCtx.pkt.size == 0)
            break;

        /* Else we need to parse it */
        addWisp((MultiExchangePosResponsePacket*)gMultiCtx.pkt.data);
    }
}

static void Multi_WispDraw(PlayState* play, int index)
{
    Vec3f* pos;

    pos = &sWispsPos[index];
    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_NEW);
    Gfx_DrawFlameColor(play, kWispColors[sWispsColorIdx[index] & 0xf], 0.35f, -50.0f);
    CLOSE_DISPS();
}

static void Multi_WispsDrawLabel(Gfx** gfxP, PlayState* play, int index)
{
    Player* player;
    int width;
    Vec3f wisp;
    Vec3f pos;
    float invW;
    int x;
    int y;
    float playerDist;

    player = GET_PLAYER(play);
    if (!player || player->actor.id != ACTOR_PLAYER)
        return;
    playerDist = Math_Vec3f_DistXYZ(&player->actor.world.pos, &sWispsPos[index]);
    if (playerDist > 200.f)
        return;

    width = (strlen(sNames[index]) * 7 + 1);
    wisp = sWispsPos[index];
    wisp.y += 55.f;
    Actor_ProjectPos(play, &wisp, &pos, &invW);
    pos.x *= invW;
    pos.y *= invW;
    pos.z *= invW;
    if (pos.z <= 0.f)
        return;
    pos.x *= pos.z;
    pos.y *= pos.z;
    x = ((pos.x + 1.f) * 0.5f) * 320 - width / 2;
    y = ((-pos.y + 1.f) * 0.5f) * 240;

    if (x < 0 || x + width > 320 || y < 0 || y + 8 > 240) return;

    FontCustom_DrawStr(gfxP, x, y, sNames[index]);
}

static void Multi_WispsDrawLabels(PlayState* play)
{
    Gfx* plusOne;
    Gfx* polyOpaP;

    OPEN_DISPS(play->state.gfxCtx);
    polyOpaP = POLY_OPA_DISP;
    plusOne = Gfx_Open(polyOpaP);
    gSPDisplayList(OVERLAY_DISP++, plusOne);

    FontCustom_Setup(&plusOne);
    for (int i = 0; i < sWispsCount; ++i)
        Multi_WispsDrawLabel(&plusOne, play, i);

    gSPEndDisplayList(plusOne++);
    Gfx_Close(polyOpaP, plusOne);
    POLY_OPA_DISP = plusOne;
    CLOSE_DISPS();
}

void Multi_WispsDraw(PlayState* play)
{
    if (!Config_Flag(CFG_MULTIPLAYER) || !sWispsCount)
        return;

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    for (int i = 0; i < sWispsCount; ++i)
        Multi_WispDraw(play, i);

    Multi_WispsDrawLabels(play);
}
