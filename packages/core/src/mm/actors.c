#include <combo.h>
#include <combo/souls.h>

void EnGo_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnDnh_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnShn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnPm_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b);
void EnAn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnPst_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnNb_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnAl_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnBjt_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnTab_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);

typedef void (*TextBoxCallback)(Actor*, GameState_Play*, s16);
void EnGo_AfterTextBox(Actor* this, GameState_Play* play, s16 messageId);
void EnTab_AfterTextBox(Actor* this, GameState_Play* play, s16 messageId);

static Actor* sByteCodeActor;

/* TODO: Patch every bytecode and remove this */
static s16 convertGi(s16 initial)
{
    switch (initial)
    {
    case 0x01: return GI_MM_RUPEE_GREEN;
    case 0x02: return GI_MM_RUPEE_BLUE;
    case 0x04: return GI_MM_RUPEE_RED;
    case 0x05: return GI_MM_RUPEE_PURPLE;
    case 0x06: return GI_MM_RUPEE_SILVER;
    case 0x07: return GI_MM_RUPEE_GOLD;
    case 0x0a: return GI_MM_RECOVERY_HEART;
    case 0x0c: return GI_MM_HEART_PIECE;
    case 0x34: return GI_MM_POWDER_KEG;
    case 0x43: return GI_MM_PICTOGRAPH_BOX;
    case 0x6f: return GI_MM_BOTTLE_CHATEAU;
    case 0x84: return GI_MM_MASK_POSTMAN;
    case 0x8f: return GI_MM_MASK_KAFEI;
    case 0x91: return GI_MM_CHATEAU;
    case 0x92: return GI_MM_MILK;
    case 0xa0: return GI_MM_ROOM_KEY;
    case 0xaa: return GI_MM_LETTER_TO_KAFEI;
    }

    return initial;
}

static void Actor_ByteCode_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    void (*func)(Actor*, GameState_Play*, s16, float, float);

    gi = convertGi(gi);

    switch (actor->id)
    {
    case AC_EN_GO:
        func = EnGo_GiveItem;
        break;
    case AC_EN_DNH:
        func = EnDnh_GiveItem;
        break;
    case AC_EN_SHN:
        func = EnShn_GiveItem;
        break;
    case AC_EN_PM:
        func = EnPm_GiveItem;
        break;
    case AC_EN_PST:
        func = EnPst_GiveItem;
        break;
    case AC_EN_AN:
        func = EnAn_GiveItem;
        break;
    case AC_EN_NB:
        func = EnNb_GiveItem;
        break;
    case AC_EN_AL:
        func = EnAl_GiveItem;
        break;
    case AC_EN_TAB:
        func = EnTab_GiveItem;
        break;
    case AC_EN_BJT:
        func = EnBjt_GiveItem;
        break;
    default:
        func = (void*)GiveItem;
        break;
    }

    func(actor, play, gi, a, b);
}

PATCH_CALL(0x8010aa34, Actor_ByteCode_GiveItem);

void EnGo_AfterGivingItem(Actor* actor);
void EnDnh_AfterGivingItem(Actor* actor);
void EnPm_AfterGivingItem(Actor* actor);
void EnAn_AfterGivingItem(Actor* actor);
void EnNb_AfterGivingItem(Actor* actor);
void EnTab_AfterGivingItem(Actor* actor);

static int Actor_ByteCode_HasParent(Actor* actor)
{
    int ret;
    void (*func)(Actor*);

    ret = Actor_HasParent(actor);
    if (ret)
    {
        switch (actor->id)
        {
        case AC_EN_GO:
            func = EnGo_AfterGivingItem;
            break;
        case AC_EN_DNH:
            func = EnDnh_AfterGivingItem;
            break;
        case AC_EN_PM:
            func = EnPm_AfterGivingItem;
            break;
        case AC_EN_AN:
            func = EnAn_AfterGivingItem;
            break;
        case AC_EN_TAB:
            func = EnTab_AfterGivingItem;
            break;
        case AC_EN_NB:
            func = EnNb_AfterGivingItem;
            break;
        default:
            func = NULL;
            break;
        }

        if (func)
            func(actor);
    }
    return ret;
}

PATCH_CALL(0x8010a9f8, Actor_ByteCode_HasParent);

static void Actor_ByteCode_RemoveItem(s16 item, s16 slot)
{
    switch (item)
    {
    case ITEM_MM_LETTER_TO_KAFEI:
        comboRemoveTradeItem3(XITEM_MM_TRADE3_LETTER_TO_KAFEI);
        break;
    case ITEM_MM_PENDANT_OF_MEMORIES:
        comboRemoveTradeItem3(XITEM_MM_TRADE3_PENDANT_OF_MEMORIES);
        break;
    case ITEM_MM_LETTER_TO_MAMA:
        comboRemoveTradeItem2(XITEM_MM_TRADE2_LETTER_TO_MAMA);
        break;
    default:
        RemoveItem(item, slot);
        break;
    }
}

PATCH_CALL(0x8010bc60, Actor_ByteCode_RemoveItem);

int Actor_RunByteCodeWrapper(Actor* this, GameState_Play* play, void* bytecode, void* unk1, void* unk2)
{
    sByteCodeActor = this;
    return Actor_RunByteCode(this, play, bytecode, unk1, unk2);
}

PATCH_CALL(0x8092d53c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80950de0, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80a157f8, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80a50f58, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80a52fe0, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80a88208, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80ae6a84, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80afa61c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80b2be74, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80b3d41c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80b3efb4, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80b57a64, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bc0ecc, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bc2328, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bd379c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bdf6e4, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80be137c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bf2bf4, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bfda74, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80c22a14, Actor_RunByteCodeWrapper);

static void Actor_ByteCode_DispatchTextBox(GameState_Play* play, s16 messageId)
{
    Actor* this;
    TextBoxCallback cb;

    this = sByteCodeActor;
    cb = NULL;
    switch (this->id)
    {
    case AC_EN_GO:
        cb = EnGo_AfterTextBox;
        break;
    case AC_EN_TAB:
        cb = EnTab_AfterTextBox;
        break;
    }
    if (cb)
    {
        cb(this, play, messageId);
    }
}

static void Actor_ByteCode_PlayerDisplayTextBox_Hook(GameState_Play* play, s16 messageId, void* unk)
{
    PlayerDisplayTextBox(play, messageId, unk);
    Actor_ByteCode_DispatchTextBox(play, messageId);
}

PATCH_CALL(0x8010af04, Actor_ByteCode_PlayerDisplayTextBox_Hook);

static void Actor_ByteCode_DisplayTextBox2_Hook(GameState_Play* play, s16 messageId)
{
    DisplayTextBox2(play, messageId);
    Actor_ByteCode_DispatchTextBox(play, messageId);
}

PATCH_CALL(0x8010af50, Actor_ByteCode_DisplayTextBox2_Hook);

static int canSpawnSoul(GameState_Play* play, s16 actorId, u16 variable)
{
    switch (actorId)
    {
    case AC_EN_OKUTA:
    case AC_EN_BIGOKUTA:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_OCTOROK);
    case AC_EN_WALLMAS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WALLMASTER);
    case AC_EN_DODONGO:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DODONGO);
    case AC_EN_FIREFLY:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_KEESE);
    case AC_EN_TITE:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_TEKTITE);
    case AC_EN_PEEHAT:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_PEAHAT);
    case AC_EN_DINOFOS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_LIZALFOS_DINALFOS);
    case AC_EN_ST:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SKULLTULA);
    case AC_EN_AM:
    case AC_EN_FAMOS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_ARMOS);
    case AC_EN_DEKUBABA:
    case AC_EN_KAREBABA:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEKU_BABA);
    case AC_EN_DEKUNUTS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEKU_SCRUB);
    case AC_EN_BBFALL:
    case AC_EN_BB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BUBBLE);
    case AC_EN_VM:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BEAMOS);
    case AC_EN_RD:
    case AC_EN_TALK_GIBUD:
    case AC_EN_RAILGIBUD:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_REDEAD_GIBDO);
    case AC_EN_SW:
        if (variable & 0x03)
            return 1;
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SKULLWALLTULA);
    case AC_EN_SB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SHELL_BLADE);
    case AC_EN_RR:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_LIKE_LIKE);
    case AC_EN_IK:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_IRON_KNUCKLE);
    case AC_EN_FZ:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_FREEZARD);
    case AC_EN_WF:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WOLFOS);
    case AC_EN_CROW:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GUAY);
    case AC_EN_TUBO_TRAP:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_FLYING_POT) ? 1 : -1;
    case AC_EN_FLOORMAS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_FLOORMASTER);
    case AC_EN_SLIME:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_CHUCHU);
    case AC_EN_DRAGON:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEEP_PYTHON);
    case AC_EN_PR:
    case AC_EN_PR2:
    case AC_EN_PRZ:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SKULLFISH);
    case AC_EN_WDHAND:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEXIHAND);
    case AC_EN_GRASSHOPPER:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DRAGONFLY);
    case AC_EN_SNOWMAN:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_EENO);
    case AC_EN_EGOL:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_EYEGORE);
    case AC_EN_PP:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_HIPLOOP);
    case AC_EN_RAT:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_REAL_BOMBCHU);
    case AC_EN_THIEFBIRD:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_TAKKURI);
    case AC_EN_MKK:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BOE);
    case AC_EN_BAGUO:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_NEJIRON);
    case AC_BOSS_05:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BIO_BABA);
    case AC_EN_JSO:
    case AC_EN_JSO2:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GARO);
    case AC_EN_WIZ:
    case AC_EN_WIZ_BROCK:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WIZZROBE);
    case AC_EN_DEATH:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GOMESS);
    case AC_EN_PAMETFROG:
    case AC_EN_BIGSLIME:
    case AC_EN_BIGPAMET:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GEKKO);
    case AC_EN_BAT:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BAD_BAT);
    case AC_EN_KAME:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SNAPPER);
    case AC_BOSS_04:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WART);
    case AC_EN_BSB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_CAPTAIN_KEETA);
    case AC_BOSS_01:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_ODOLWA);
    case AC_BOSS_HAKUGIN:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_GOHT);
    case AC_BOSS_03:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_GYORG);
    case AC_BOSS_02:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_TWINMOLD);
    case AC_EN_KNIGHT:
    case AC_BOSS_06:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_IGOS);
    case AC_EN_NEO_REEBA:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_LEEVER);
    case AC_EN_SKB:
        if ((play->sceneId == SCE_MM_IKANA_GRAVEYARD) && (play->roomCtx.curRoom.id == 0x01)) /* Upper graveyard */
            return 1;
        /* Fallthrough */
    case AC_EN_HINT_SKB:
    case AC_EN_RAIL_SKB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_STALCHILD);
    default:
        return 1;
    }
}

static int canSpawnActor(GameState_Play* play, s16 actorId, u16 variable)
{
    switch (actorId)
    {
    case AC_EN_SYATEKI_OKUTA:
        if (play->sceneId == SCE_MM_SHOOTING_GALLERY && gSave.day > 3)
            return 0;
        /* Fallthrough */
    default:
        return 1;
    }
}

static s16 sActorIdToSpawn;

Actor* comboSpawnActorEx(ActorContext* actorCtx, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3)
{
    int ret;

    if (!canSpawnActor(play, actorId, variable))
        return NULL;

    ret = canSpawnSoul(play, actorId, variable);
    if (ret <= 0)
    {
        if (ret == 0)
            g.roomEnemyLackSoul = 1;
        return NULL;
    }
    sActorIdToSpawn = actorId;
    return SpawnActorEx(actorCtx, play, actorId, x, y, z, rx, ry, rz, variable, ex1, ex2, ex3);
}

static int GetRoomClearFlagForActor(GameState_Play* play, int flag)
{
    int res;

    res = GetRoomClearFlag(play, flag);
    if (comboConfig(CFG_ER_WALLMASTERS) && sActorIdToSpawn == AC_EN_WALLMAS)
        res = 0;
    return res;
}

PATCH_CALL(0x800baea0, GetRoomClearFlagForActor);

static LightInfo D_8015BC00;
static LightNode* D_8015BC10;
static s32 D_8015BC14;
static f32 D_8015BC18;

void Actor_InitFaroresWind(GameState_Play* play) {
    Vec3f lightPos;

    if (gSave.fw.data)
    {
        gSaveContext.respawn[RESPAWN_MODE_HUMAN] = gSave.fw;
    }
    else
    {
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 0;
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.x = 0.0f;
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.y = 0.0f;
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.z = 0.0f;
    }

    lightPos = gSaveContext.save.fw.pos;
    lightPos.y += 60.0f;

    Lights_PointNoGlowSetInfo(&D_8015BC00, lightPos.x, lightPos.y, lightPos.z, 0xFF, 0xFF, 0xFF, -1);

    D_8015BC10 = LightContext_InsertLight(play, &play->lightCtx, &D_8015BC00);
    D_8015BC14 = 0;
    D_8015BC18 = 0.0f;
}

void Actor_DrawFaroresWindPointer(GameState_Play* play)
{
    s32 lightRadius = -1;
    s32 params;

    OPEN_DISPS(play->gs.gfx);

    params = gSaveContext.respawn[RESPAWN_MODE_HUMAN].data;

    if (params)
    {
        f32 yOffset = comboIsLinkAdult() ? 80.0f : 60.0f;
        f32 ratio = 1.0f;
        s32 alpha = 255;
        s32 temp = params - 40;

        if (temp < 0)
        {
            gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = ++params;
            ratio = ABS(params) * 0.025f;
            D_8015BC14 = 60;
            D_8015BC18 = 1.0f;
        }
        else if (D_8015BC14)
        {
            D_8015BC14--;
        }
        else if (D_8015BC18 > 0.0f)
        {
            static Vec3f effectVel = { 0.0f, -0.05f, 0.0f };
            static Vec3f effectAccel = { 0.0f, -0.025f, 0.0f };
            static Color_RGBA8 effectPrimCol = { 255, 255, 255, 0 };
            static Color_RGBA8 effectEnvCol = { 100, 200, 0, 0 };
            Vec3f* curPos = &gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos;
            Vec3f* nextPos = &gSaveContext.respawn[RESPAWN_MODE_DOWN].pos;
            f32 prevNum = D_8015BC18;
            Vec3f dist;
            f32 diff = Math_Vec3f_DistXYZAndStoreDiff(nextPos, curPos, &dist);
            Vec3f effectPos;
            f32 factor;
            f32 length;
            f32 dx;
            f32 speed;

            if (diff < 20.0f)
            {
                D_8015BC18 = 0.0f;
                Math_Vec3f_Copy(curPos, nextPos);
            }
            else
            {
                length = diff * (1.0f / D_8015BC18);
                speed = 20.0f / length;
                speed = CLAMP_MIN(speed, 0.05f);
                Math_StepToF(&D_8015BC18, 0.0f, speed);
                factor = (diff * (D_8015BC18 / prevNum)) / diff;
                curPos->x = nextPos->x + (dist.x * factor);
                curPos->y = nextPos->y + (dist.y * factor);
                curPos->z = nextPos->z + (dist.z * factor);
                length *= 0.5f;
                dx = diff - length;
                yOffset += sqrtf(SQ(length) - SQ(dx)) * 0.2f;
            }

            effectPos.x = curPos->x + Rand_CenteredFloat(6.0f);
            effectPos.y = curPos->y + 80.0f + (6.0f * RandFloat());
            effectPos.z = curPos->z + Rand_CenteredFloat(6.0f);

            EffectSsKiraKira_SpawnDispersed(play, &effectPos, &effectVel, &effectAccel, &effectPrimCol, &effectEnvCol,
                                            1000, 16);

            if (D_8015BC18 == 0.0f)
            {
                gSaveContext.respawn[RESPAWN_MODE_HUMAN] = gSaveContext.respawn[RESPAWN_MODE_DOWN];
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].playerParams = 0x6ff;
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 40;
            }
        }
        else if (temp > 0)
        {
            Vec3f* curPos = &gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos;
            f32 nextRatio = 1.0f - temp * 0.1f;
            f32 curRatio = 1.0f - (f32)(temp - 1) * 0.1f;
            Vec3f eye;
            Vec3f dist;
            f32 diff;

            if (nextRatio > 0.0f)
            {
                eye.x = play->view.eye.x;
                eye.y = play->view.eye.y - yOffset;
                eye.z = play->view.eye.z;
                diff = Math_Vec3f_DistXYZAndStoreDiff(&eye, curPos, &dist);
                diff = (diff * (nextRatio / curRatio)) / diff;
                curPos->x = eye.x + (dist.x * diff);
                curPos->y = eye.y + (dist.y * diff);
                curPos->z = eye.z + (dist.z * diff);
            }

            alpha = 255 - (temp * 30);

            if (alpha < 0)
            {
                gSave.fw.data = 0;
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 0;
                alpha = 0;
            }
            else
            {
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = ++params;
            }

            ratio = 1.0f + ((f32)temp * 0.2); /* required to match */
        }

        lightRadius = 500.0f * ratio;

        s32 fwSceneId = Entrance_GetSceneIdAbsolute(gSaveContext.respawn[RESPAWN_MODE_HUMAN].entrance);
        s32 fwRoomId = gSaveContext.respawn[RESPAWN_MODE_HUMAN].roomIndex;

        s32 sceneMatches = fwSceneId == play->sceneId;

        f32 yPos = gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.y + yOffset;

        if (!sceneMatches)
        {
            switch (play->sceneId)
            {
            case SCE_MM_TWIN_ISLANDS_WINTER:
                sceneMatches = fwSceneId == SCE_MM_TWIN_ISLANDS_SPRING;
                break;
            case SCE_MM_GORON_VILLAGE_WINTER:
                sceneMatches = fwSceneId == SCE_MM_GORON_VILLAGE_SPRING;
                break;
            case SCE_MM_MOUNTAIN_VILLAGE_WINTER:
                sceneMatches = fwSceneId == SCE_MM_MOUNTAIN_VILLAGE_SPRING;
                break;
            case SCE_MM_TWIN_ISLANDS_SPRING:
                sceneMatches = fwSceneId == SCE_MM_TWIN_ISLANDS_WINTER;
                break;
            case SCE_MM_GORON_VILLAGE_SPRING:
                sceneMatches = fwSceneId == SCE_MM_GORON_VILLAGE_WINTER;
                break;
            case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
                sceneMatches = fwSceneId == SCE_MM_MOUNTAIN_VILLAGE_WINTER;
                break;
            case SCE_MM_SOUTHERN_SWAMP_CLEAR:
                sceneMatches = fwSceneId == SCE_MM_SOUTHERN_SWAMP;
                break;
            case SCE_MM_SOUTHERN_SWAMP:
                sceneMatches = fwSceneId == SCE_MM_SOUTHERN_SWAMP_CLEAR;
                break;
            case SCE_MM_TEMPLE_STONE_TOWER:
                sceneMatches = fwSceneId == SCE_MM_TEMPLE_STONE_TOWER_INVERTED;
                yPos *= -1.0f;
                break;
            case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
                sceneMatches = fwSceneId == SCE_MM_TEMPLE_STONE_TOWER;
                yPos *= -1.0f;
                break;
            }
        }

        s32 shouldDraw = sceneMatches
                && (D_8015BC18 != 0.0f || fwRoomId == play->roomCtx.curRoom.id || fwRoomId == play->roomCtx.prefRoom.id);

        if ((play->csCtx.state == CS_STATE_IDLE) && shouldDraw)
        {
            f32 scale = 0.025f * ratio;

            POLY_XLU_DISP = Gfx_SetupDL(POLY_XLU_DISP, 25); /* SETUPDL_25 */

            ModelViewTranslate(gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.x, yPos, gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.z, MAT_SET);
            ModelViewScale(scale, scale, scale, MAT_MUL);
            ModelViewMult(&play->billboardMtxF, MAT_MUL);
            MatrixStackDup();

            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 255, 255, 200, alpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 100, 200, 0, 255);

            ModelViewRotateZ(BINANG_TO_RAD_ALT2((play->gameplayFrames * 1500) & 0xFFFF), MAT_MUL);
            gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | 0x23210); /* gEffFlash1DL */

            MatrixStackPop();
            ModelViewRotateZ(BINANG_TO_RAD_ALT2(~((play->gameplayFrames * 1200) & 0xFFFF)), MAT_MUL);

            gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | 0x23210); /* gEffFlash1DL */

            Lights_PointNoGlowSetInfo(&D_8015BC00, gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.x,
                                    yPos,
                                    gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.z, 255, 255, 255, lightRadius);
        }
        CLOSE_DISPS();
    }
}

void Actor_AfterDrawAll(GameState_Play* play)
{
    /* Displaced code: */
    if (play->actorCtx.lensMaskSize != 0)
    {
        play->actorCtx.lensActorsDrawn = 1;
        Actor_DrawLensActors(play, play->actorCtx.numLensActors, play->actorCtx.lensActors);
    }

    Actor_DrawFaroresWindPointer(play);
}

typedef struct
{
    u32 offset;
    u16 value;
}
OverlayPatch;

static OverlayPatch adultEponaPatch[] = {
    { 0x865a, 0x0600 },
    { 0x865e, 0x1e2c },
    { 0x867a, 0x0600 },
    { 0x868e, 0x3cec },
    { 0x8706, 0x0600 },
    { 0x870a, 0x3cec },
    { 0x872a, 0x0600 },
    { 0x8736, 0x3cec },
    { 0x879a, 0x0600 },
    { 0x87a2, 0x75f0 },
    { 0x87ae, 0x0600 },
    { 0x87be, 0x75f0 },
    { 0x880e, 0x0600 },
    { 0x8812, 0x6d50 },
    { 0x8832, 0x0600 },
    { 0x883e, 0x6d50 },
    { 0x887e, 0x0600 },
    { 0x8886, 0x6d50 },
    { 0x8c22, 0x0600 },
    { 0x8c2e, 0x3cec },
    { 0x8c3a, 0x0600 },
    { 0x8c4e, 0x3cec },
    { 0x8d52, 0x0600 },
    { 0x8d5e, 0x1e2c },
    { 0x8d6a, 0x0600 },
    { 0x8d7e, 0x1e2c },
    { 0x8dde, 0x0600 },
    { 0x8de6, 0x1e2c },
    { 0x8df2, 0x0600 },
    { 0x8e06, 0x1e2c },
    { 0x8e72, 0x0600 },
    { 0x8e7e, 0x6d50 },
    { 0x8e92, 0x0600 },
    { 0x8e9e, 0x6d50 },
    { 0x8f06, 0x0600 },
    { 0x8f0e, 0x6d50 },
    { 0x8f1a, 0x0600 },
    { 0x8f2e, 0x6d50 },
    { 0xbbde, 0x3c23 },
    { 0xc8ac, 0x1000 },
    { 0xd792, 0x6d50 },
    { 0xd796, 0x5584 },
    { 0xd79a, 0x4dec },
    { 0xd79e, 0x3cec },
    { 0xd7a2, 0x75f0 },
    { 0xd7a6, 0x32b0 },
    { 0xd7aa, 0x1e2c },
    { 0xd7ae, 0x2470 },
    { 0xd7b2, 0x2c38 },
    { 0xd81a, 0x9d74 },
    { 0xdad6, 0x9f80 },
    { 0xdada, 0xa180 },
    { 0xdade, 0xa380 },
};

static void Actor_PatchAdultHorse(void* loadedRam)
{
    if (!comboIsLinkAdult())
    {
        return;
    }

    for (int i = 0; i < ARRAY_SIZE(adultEponaPatch); i++)
    {
        u16* patchPointer = (u16*)(((u8*)loadedRam) + adultEponaPatch[i].offset);
        *patchPointer = adultEponaPatch[i].value;
    }
}

static PlayerAgeProperties sPlayerAgePropertiesAdult = {
    56.0f,
    90.0f,
    1.0f,
    111.0f,
    70.0f,
    79.4f,
    59.0f,
    41.0f,
    19.0f,
    36.0f,
    50.0f,
    56.0f,
    68.0f,
    70.0f,
    18.0f,
    23.0f,
    70.0f,
    { 9, 0x123F, 0x167 },
    {
        { 8, 0x1256, 0x17C },
        { 9, 0x17EA, 0x167 },
        { 8, 0x1256, 0x17C },
        { 9, 0x17EA, 0x167 },
    },
    {
        { 9, 0x17EA, 0x167 },
        { 9, 0x1E0D, 0x17C },
        { 9, 0x17EA, 0x167 },
        { 9, 0x1E0D, 0x17C },
    },
    {
        { 8, 0x1256, 0x17C },
        { 9, 0x17EA, 0x167 },
        { -0x638, 0x1256, 0x17C },
        { -0x637, 0x17EA, 0x167 },
    },
    0,
    0x80,
    22.0f,
    36.0f,
    (PlayerAnimationHeader*)0x0400e300, /* gPlayerAnim_pz_Tbox_open */
    (PlayerAnimationHeader*)0x0400d548, /* gPlayerAnim_link_demo_back_to_past */
    (PlayerAnimationHeader*)0x0400d660, /* gPlayerAnim_link_demo_return_to_past */
    (PlayerAnimationHeader*)0x0400e378, /* gPlayerAnim_pz_climb_startA */
    (PlayerAnimationHeader*)0x0400e380, /* gPlayerAnim_pz_climb_startB */
    {
        (PlayerAnimationHeader*)0x0400e388, /* gPlayerAnim_pz_climb_upL */
        (PlayerAnimationHeader*)0x0400e390, /* gPlayerAnim_pz_climb_upR */
        (PlayerAnimationHeader*)0x0400dab0, /* gPlayerAnim_link_normal_Fclimb_upL */
        (PlayerAnimationHeader*)0x0400dab8, /* gPlayerAnim_link_normal_Fclimb_upR */
    },
    {
        (PlayerAnimationHeader*)0x0400da90, /* gPlayerAnim_link_normal_Fclimb_sideL */
        (PlayerAnimationHeader*)0x0400da98, /* gPlayerAnim_link_normal_Fclimb_sideR */
    },
    {
        (PlayerAnimationHeader*)0x0400e358, /* gPlayerAnim_pz_climb_endAL */
        (PlayerAnimationHeader*)0x04008360, /* gPlayerAnim_pz_climb_endAR */
    },
    {
        (PlayerAnimationHeader*)0x0400e370, /* gPlayerAnim_pz_climb_endBR */
        (PlayerAnimationHeader*)0x0400e368, /* gPlayerAnim_pz_climb_endBL */
    },
};

static void Actor_PatchPlayerFormProperties()
{
    if (comboIsLinkAdult())
    {
        /* Maybe better to move this to when the overlay is loaded? */
        PlayerAgeProperties* playerAgeProperties = (PlayerAgeProperties*) OverlayAddr(0x8085ba38);
        playerAgeProperties[MM_PLAYER_FORM_HUMAN] = sPlayerAgePropertiesAdult;

        u32* playerAnimationHeaders = (u32*)OverlayAddr(0x8085be84);
        playerAnimationHeaders[0x36] = 0x0400e3c8;
        playerAnimationHeaders[0x37] = 0x0400e3c8;
        playerAnimationHeaders[0x38] = 0x0400e3c8;
        playerAnimationHeaders[0x39] = 0x0400e3c8;
        playerAnimationHeaders[0x3a] = 0x0400e3c8;
        playerAnimationHeaders[0x3b] = 0x0400e3c8;

        playerAnimationHeaders[0x42] = 0x0400e3d0;
        playerAnimationHeaders[0x43] = 0x0400e3d0;
        playerAnimationHeaders[0x44] = 0x0400e3d0;
        playerAnimationHeaders[0x45] = 0x0400e3d0;
        playerAnimationHeaders[0x46] = 0x0400e3d0;
        playerAnimationHeaders[0x47] = 0x0400e3d0;
    }
}

ActorInit* Actor_LoadOverlayCustom(ActorContext* actorCtx, s16 index)
{
    ActorInit* actorInit = Actor_LoadOverlay(actorCtx, index);

    ActorOvl* overlayEntry = &gActorOvl[index];

    if (overlayEntry->count == 0 && overlayEntry->data)
    {
        switch (index)
        {
        case AC_EN_HORSE:
            Actor_PatchAdultHorse(overlayEntry->data);
            break;
        }
    }

    return actorInit;
}

PATCH_CALL(0x800bae44, Actor_LoadOverlayCustom);

void KaleidoManager_LoadPlayerOvl(void* ovl)
{
    KaleidoManager_LoadOvl(ovl);
    Actor_PatchPlayerFormProperties();
}

PATCH_CALL(0x801639d4, KaleidoManager_LoadPlayerOvl);
