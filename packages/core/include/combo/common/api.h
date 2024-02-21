#ifndef COMBO_COMMON_API_H
#define COMBO_COMMON_API_H

#include <combo/types.h>
#include <combo/gi.h>

#define PLAYER_ACTOR_STATE_TRANSITION           0x00000001
#define PLAYER_ACTOR_STATE_TRANSFORM            0x00000002
#define PLAYER_ACTOR_STATE_CLIMB                0x00000004
#define PLAYER_ACTOR_STATE_DEATH                0x00000080
#define PLAYER_ACTOR_STATE_FROZEN               0x00000200
#define PLAYER_ACTOR_STATE_GET_ITEM             0x00000400
#define PLAYER_ACTOR_STATE_HOLD_ITEM            0x00000800
#define PLAYER_ACTOR_STATE_CLIMB2               0x00200000
#define PLAYER_ACTOR_STATE_EPONA                0x00800000
#define PLAYER_ACTOR_STATE_WATER                0x08000000
#define PLAYER_ACTOR_STATE_USE_ITEM             0x10000000
#define PLAYER_ACTOR_STATE_CUTSCENE_FROZEN      0x20000000

#if defined(GAME_OOT)
# define PLAYER_MASK_NONE               0x00
# define PLAYER_MASK_KEATON             0x01
# define PLAYER_MASK_SKULL              0x02
# define PLAYER_MASK_SPOOKY             0x03
# define PLAYER_MASK_BUNNY              0x04
# define PLAYER_MASK_GORON              0x05
# define PLAYER_MASK_ZORA               0x06
# define PLAYER_MASK_GERUDO             0x07
# define PLAYER_MASK_TRUTH              0x08
# define PLAYER_MASK_BLAST              0x09
# define PLAYER_MASK_STONE              0x0a
#endif

#if defined(GAME_MM)
# define PLAYER_MASK_NONE               0x00
# define PLAYER_MASK_TRUTH              0x01
# define PLAYER_MASK_KAFEI              0x02
# define PLAYER_MASK_ALL_NIGHT          0x03
# define PLAYER_MASK_BUNNY              0x04
# define PLAYER_MASK_KEATON             0x05
# define PLAYER_MASK_GARO               0x06
# define PLAYER_MASK_ROMANI             0x07
# define PLAYER_MASK_TROUPE_LEADER      0x08
# define PLAYER_MASK_POSTMAN            0x09
# define PLAYER_MASK_COUPLE             0x0a
# define PLAYER_MASK_GREAT_FAIRY        0x0b
# define PLAYER_MASK_GIBDO              0x0c
# define PLAYER_MASK_DON_GERO           0x0d
# define PLAYER_MASK_KAMARO             0x0e
# define PLAYER_MASK_CAPTAIN            0x0f
# define PLAYER_MASK_STONE              0x10
# define PLAYER_MASK_BREMEN             0x11
# define PLAYER_MASK_BLAST              0x12
# define PLAYER_MASK_SCENTS             0x13
# define PLAYER_MASK_GIANT              0x14
# define PLAYER_MASK_FIERCE_DEITY       0x15
# define PLAYER_MASK_GORON              0x16
# define PLAYER_MASK_ZORA               0x17
# define PLAYER_MASK_DEKU               0x18
#endif

typedef struct GameState_Play GameState_Play;
typedef struct ActorContext ActorContext;
typedef struct Actor Actor;

float Actor_WorldDistXZToActor(Actor* a, Actor* b);
float Actor_HeightDiff(Actor* a, Actor* b);
u16   Actor_Angle(Actor* a, Actor* b);

void Interface_UpdateButtonsPart2(GameState_Play* play);

int    LoadFile(void* dst, u32 vromAddr, u32 size);

Actor*  SpawnActor(ActorContext* actorCtx, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable);
#if defined(GAME_MM)
Actor*  SpawnActorEx(ActorContext* actorCtx, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3);

int Schedule_CheckMiscS(GameState_Play* play, void* unk);
#endif

void    SkelAnime_DrawFlexOpa(GameState_Play* play, void** skeleton, Vec3s* jointTable, s32 dListCount,
                           void* overrideLimbDraw, void* postLimbDraw, void* arg);

void    AudioSeq_QueueSeqCmd(u32 unk);

void    ActorDestroy(Actor* actor);
int     Actor_HasParent(Actor* actor);
void    ActorSetScale(Actor* actor, float scale);
void    ActorSetUnk(Actor* actor, float unk);
void    ActorEnableGrab(Actor* actor, GameState_Play* play);
void    ActorEnableTalk(Actor* actor, GameState_Play* play, float range);
void    ActorEnableTalkEx(Actor* actor, GameState_Play* play, float range, u32 unk);
// AKA  Actor_UpdateBgCheckInfo
void    Actor_SetCollisionCylinder(GameState_Play* play, Actor* actor, float unk_3, float unk_4, float unk_5, u32 unk_6);
// AKA  Actor_MoveXZGravity AKA Actor_MoveWithGravity
void    ActorUpdateVelocity(Actor* actor);
int     ActorTalkedTo(Actor* actor);

void    EnableOwl(u8 owlId);

u32     GetChestFlag(GameState_Play* play, int flag);
void    SetChestFlag(GameState_Play* play, int flag);
u32     GetCollectibleFlag(GameState_Play* play, int flag);
void    SetCollectibleFlag(GameState_Play* play, int flag);
u32     GetSwitchFlag(GameState_Play* play, int flag);
u32     SetSwitchFlag(GameState_Play* play, int flag);
void    ClearSwitchFlag(GameState_Play* play, int flag);
void    SetRoomClear(GameState_Play* play, int flag);
u32     GetRoomClearFlag(GameState_Play* play, int flag);

void Sleep_Usec(u32 delay);

void Audio_PlayFanfare(int fanfareId);

void Actor_ProcessInitChain(Actor* this, void* data);

void DMARomToRam(u32 romAddr, void* dramAddr, u32 size);
void DmaCompressed(u32 pstart, void* dst, u32 size);
#if defined(GAME_OOT)
void _DmaCompressed(u32 pstart, void* dst, u32 size);
#endif

void ConvertMatrix(const float* in, u16* out);
void MatrixRotation(u16 angle, int unk_1);

void DisplayTextBox(GameState_Play* play, u16 textId);
void DisplayTextBox2(GameState_Play* play, u16 textId);
void DisplayTextBox3(GameState_Play* play, Actor* actor, u16 textId);

void DrawLink(GameState_Play* play, void** skeleton, Vec3s* jointTable, s32 dListCount, s32 lod, s32 tunic, s32 boots, s32 face, void* overrideLimbDraw, void* postLimbDraw, void* data);

int GiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b);
void GiveItemDefaultRange(Actor* actor, GameState_Play* play, s16 itemId);

void PlayerDisplayTextBox(GameState_Play* play, u16 messageId, void* unk);

#if defined(GAME_MM)
void Message_ShowMessageAtYPosition(GameState_Play* play, u16 messageId, u8 yPosition);
void PictoUpdateFlags(GameState_Play* play);
extern u16 gPictoboxState;
extern u16 gPictoboxPhotoTaken;
#endif

#define TEXT_STATE_NONE         0
#define TEXT_STATE_CLOSING      2
#define TEXT_STATE_DONE_FADING  3

#if defined(GAME_OOT)
int  Message_GetState(MessageContext* ctx);
#else
int  Message_GetState(void* ctx);
#endif

void Message_Close(GameState_Play* play);
int  Message_ShouldAdvance(GameState_Play* play);
int  Message_IsClosed(Actor* actor, GameState_Play* play);

void OcarinaAction(GameState_Play *play, u16 ocarinaAction);
void OcarinaAction2(GameState_Play *play, u16 ocarinaAction);

/* Matrix Ops */
#define MAT_SET 0
#define MAT_MUL 1

extern float* gModelView;

void ModelViewUnkTransform(MtxF* unk);
void ModelViewRotateX(float angle, int mode);
void ModelViewRotateY(float angle, int mode);
void ModelViewRotateZ(float angle, int mode);
void ModelViewTranslate(float tx, float ty, float tz, int mode);
void ModelViewScale(float sx, float sy, float sz, int mode);
void ModelViewMult(MtxF* mf, s32 mode);
void Matrix_SetTranslateRotateYXZ(f32 translateX, f32 translateY, f32 translateZ, Vec3s* rot);

f32 Math_CosS(s16 angle);
f32 Math_SinS(s16 angle);
s32 Math_StepToF(f32* pValue, f32 target, f32 step);
s32 Math_StepToS(s16* pValue, s16 target, s16 step);
f32 Math_SmoothStepToF(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep);
f32 sqrtf(f32 value);

void MatrixStackDup(void);
void MatrixStackPop(void);
void Matrix_MultVec3f(Vec3f* src, Vec3f* dest);

float RandFloat(void);
f32 Rand_CenteredFloat(f32 scale);

int GetActiveItem(GameState_Play* play);

#if defined(GAME_MM)
void AddMagic(GameState_Play* play, s16 amount);
#endif

u8 AddItem(GameState_Play* play, u8 itemId);
u8 AddItemWithIcon(GameState_Play* play, Actor_Player* link, const GetItem* giEntry);
u8 GetItemCollectBehavior(s16 itemId);
void Interface_SetDoAction(GameState_Play* play, u16 action);
void Interface_LoadActionLabelB(GameState_Play* play, u16 action);

#if defined(GAME_MM)
void PrepareSave(SramContext* sram);
void Sram_SaveEndOfCycle(GameState_Play* play);
void Sram_SaveNewDay(GameState_Play* play);
#endif

void Grayscale(void* buffer, u16 size);
s16 RandIntRange(s16 base, s16 amplitude);

extern u32 gSegments[16];

#define GET_LINK(play) ((Actor_Player*)(play->actorCtx.actors[2].first))

int  ActorCutscene_GetCanPlayNext(int cutscene);
s16  ActorCutscene_GetCurrentIndex(void);
void ActorCutscene_Stop(s16 cutsceneId);
void Cutscene_End(GameState_Play* play, void* unk);

void RemoveItem(s16 item, s16 slot);

void* ActorAlloc(u32 size);
void  ActorFree(void* data);

#if defined(GAME_OOT)
void Sram_OpenSave(void*);
#else
void Sram_OpenSave(void*, void*);
#endif

void Sram_CopySave(void*, void*);

void Play_Init(GameState_Play*);
void Play_Update(GameState_Play*);

s32 Player_InCsMode(GameState_Play*);
void Interface_LoadItemIconImpl(GameState_Play* play, int slot);
void UpdateEquipment(GameState_Play* play, Actor_Player* link);

void PlayStoreFlags(GameState_Play* play);
Camera* Play_GetCamera(GameState_Play* this, s16 camId);
s32 Play_CamIsNotFixed(GameState_Play* play);

void Player_Update(Actor_Player* this, GameState_Play* play);
void Play_SetupRespawnPoint(GameState_Play* this, s32 respawnMode, s32 playerParams);

void* OverlayAddr(u32 addr);

void LoadIcon(u32 vaddr, int iconId, void* buffer, int size);
void CmpDma_LoadAllFiles(u32 vrom, void* dst, size_t size);

s32 Player_ActionToModelGroup(Actor_Player* link, s32 itemAction);
void Player_SetModels(Actor_Player* link, s32 modelGroup);
int Player_UsingItem(Actor_Player* link);
int Player_GetEnvironmentalHazard(GameState_Play* play);

void PlaySound(u16 soundId);
void PlayMusic(int arg0, int arg1, int arg2, int arg3, int arg4);
void Player_PlaySfx(Actor_Player* player, u16 sfxId);
void Actor_PlaySfx(Actor* actor, u32 id);
void PlayLoopingSfxAtActor(Actor* actor, u32 id);
void Actor_PlaySfx_FlaggedCentered1(Actor* actor, u16 sfxId);
void Audio_PlaySfx_AtPos(Vec3f* pos, u16 sfxId);
void Audio_PlaySfx_MessageDecide(void);

#if defined(GAME_MM)
void AudioOcarina_SetInstrument(u8 ocarinaInstrumentId);
void Player_DrawHookshotReticle(GameState_Play* play, Actor_Player* player, f32 distance);
#endif

int Actor_RunByteCode(Actor* this, GameState_Play* play, void* bytecode, void* unk1, void* unk2);
void Enemy_StartFinishingBlow(GameState_Play* play, Actor* this);

void DeathWarp(GameState_Play* play);

#if defined(GAME_MM)
void DrawDungeonUnk2(void* unk);
#endif

void ActorRemove(ActorContext* ctx, Actor* actor, GameState_Play* play);

void DrawSimpleOpa(GameState_Play* play, u32 segAddr);

void AddRupees(s16 delta);

void AudioLoad_InitTable(void* unk1, u32 unk2, u32 unk3);

void ParseSceneRoomHeaders_SoundSettings(GameState_Play* play, void* cmd);

/* DrawGi */
void DrawGi_Opa0_Xlu1(GameState_Play*, s16);
void DrawGi_Opa0_Xlu12(GameState_Play*, s16);
void DrawGi_Opa0(GameState_Play*, s16);
void DrawGi_Opa01(GameState_Play*, s16);
void DrawGi_Opa1023(GameState_Play*, s16);
void DrawGi_Opa10_Xlu2(GameState_Play*, s16);
void DrawGi_Opa10_Xlu234(GameState_Play*, s16);
void DrawGi_Opa10_Xlu32(GameState_Play*, s16);
void DrawGi_Opa10234567(GameState_Play*, s16);
void DrawGi_Xlu01(GameState_Play*, s16);
void DrawGi_BlueFire(GameState_Play*, s16);
void DrawGi_BombchuMask(GameState_Play*, s16);
void DrawGi_Compass(GameState_Play*, s16);
void DrawGi_DekuNut(GameState_Play*, s16);
void DrawGi_Fairy(GameState_Play*, s16);
void DrawGi_Fish(GameState_Play*, s16);
void DrawGi_GiantKnife(GameState_Play*, s16);
void DrawGi_GS(GameState_Play*, s16);
void DrawGi_Heart(GameState_Play*, s16);
void DrawGi_Medallion(GameState_Play*, s16);
void DrawGi_MirrorShield(GameState_Play*, s16);
void DrawGi_Poe(GameState_Play*, s16);
void DrawGi_Potion(GameState_Play*, s16);
void DrawGi_Scale(GameState_Play*, s16);
void DrawGi_SoldOut(GameState_Play*, s16);
void DrawGi_Spell(GameState_Play*, s16);
void DrawGi_MoonTear(GameState_Play*, s16);
void DrawGi_BottleFairy(GameState_Play*, s16);
void DrawGi_BottleBlueFire(GameState_Play*, s16);
void DrawGi_VanillaRupee(GameState_Play*, s16);

void Interface_StartMoonCrash(GameState_Play* play);

/* GFX */
Gfx* Gfx_TexScroll(GfxContext* ctx, u32 x, u32 y, s32 width, s32 height);
void Gfx_DrawDListOpa(GameState_Play* play, Gfx* dlist);
void Gfx_DrawDListXlu(GameState_Play* play, Gfx* dlist);

void SpawnRoomActors(GameState_Play* play, int id);

extern u8 gPlayedOcarinaSong;
extern u8 gIsOcarinaEnabled;
extern u32 gOcarinaFlags;
extern u8 gOcarinaButtonPitchMap[];
extern u8 gOcarinaSongBuffer[];
extern u8 gOcarinaSongBufferCursor;

void HandleMessages(GameState_Play* play, void* unk);

#if defined(GAME_OOT)
void func_800C7200(u8 arg0);
void Letterbox_SetSizeTarget(s32 target);
u32 Letterbox_GetSizeTarget(void);
#endif

void Play_LoadKaleidoOverlay(void*);
void LoadMapMark(void*);

#define TINGLE_MAP_CLOCK_TOWN       0
#define TINGLE_MAP_WOODFALL         1
#define TINGLE_MAP_SNOWHEAD         2
#define TINGLE_MAP_ROMANI_RANCH     3
#define TINGLE_MAP_GREAT_BAY        4
#define TINGLE_MAP_STONE_TOWER      5
void Inventory_SetWorldMapCloudVisibility(s16 tingleId);

#endif

#if defined(GAME_MM)
s16 ActorCutscene_StartAndSetFlag(s16 index, Actor* actor);
#endif

void DrawHUD(GameState_Play* play);

int IsSceneValidEpona(int sceneId);

#if defined(GAME_OOT)
typedef enum {
    /* 0x0 */ MAGIC_STATE_IDLE, // Regular gameplay
    /* 0x1 */ MAGIC_STATE_CONSUME_SETUP, // Sets the speed at which magic border flashes
    /* 0x2 */ MAGIC_STATE_CONSUME, // Consume magic until target is reached or no more magic is available
    /* 0x3 */ MAGIC_STATE_METER_FLASH_1, // Flashes border and freezes Dark Link
    /* 0x4 */ MAGIC_STATE_METER_FLASH_2, // Flashes border and draws yellow magic to preview target consumption
    /* 0x5 */ MAGIC_STATE_RESET, // Reset colors and return to idle
    /* 0x6 */ MAGIC_STATE_METER_FLASH_3, // Flashes border with no additional behaviour
    /* 0x7 */ MAGIC_STATE_CONSUME_LENS, // Magic slowly consumed by lens.
    /* 0x8 */ MAGIC_STATE_STEP_CAPACITY, // Step `magicCapacity` to full capacity
    /* 0x9 */ MAGIC_STATE_FILL, // Add magic until magicFillTarget is reached.
    /* 0xA */ MAGIC_STATE_ADD // Add requested magic
} MagicState;

typedef enum {
    /* 0 */ MAGIC_CONSUME_NOW, // Consume Magic immediately without preview
    /* 1 */ MAGIC_CONSUME_WAIT_NO_PREVIEW, // Sets consume target but waits to consume. No yellow magic preview to target consumption. Unused
    /* 2 */ MAGIC_CONSUME_NOW_ALT, // Identical behaviour to MAGIC_CONSUME_NOW. Unused
    /* 3 */ MAGIC_CONSUME_LENS, // Lens consumption
    /* 4 */ MAGIC_CONSUME_WAIT_PREVIEW, // Sets consume target but waits to consume. Draws yellow magic to target consumption
    /* 5 */ MAGIC_ADD // Sets a target to add magic
} MagicChangeType;
#else
typedef enum {
    /* 0  */ MAGIC_STATE_IDLE, // Regular gameplay
    /* 1  */ MAGIC_STATE_CONSUME_SETUP, // Sets the speed at which the magic border flashes
    /* 2  */ MAGIC_STATE_CONSUME, // Consume magic until target is reached or no more magic is available
    /* 3  */ MAGIC_STATE_METER_FLASH_1, // Flashes border
    /* 4  */ MAGIC_STATE_METER_FLASH_2, // Flashes border and draws yellow magic to preview target consumption
    /* 5  */ MAGIC_STATE_RESET, // Reset colors and return to idle
    /* 6  */ MAGIC_STATE_METER_FLASH_3, // Flashes border with no additional behaviour
    /* 7  */ MAGIC_STATE_CONSUME_LENS, // Magic slowly consumed by Lens of Truth
    /* 8  */ MAGIC_STATE_STEP_CAPACITY, // Step `magicCapacity` to full capacity
    /* 9  */ MAGIC_STATE_FILL, // Add magic until magicFillTarget is reached
    /* 10 */ MAGIC_STATE_CONSUME_GORON_ZORA_SETUP,
    /* 11 */ MAGIC_STATE_CONSUME_GORON_ZORA, // Magic slowly consumed by Goron spiked rolling or Zora electric barrier.
    /* 12 */ MAGIC_STATE_CONSUME_GIANTS_MASK // Magic slowly consumed by Giant's Mask
} MagicState;

typedef enum {
    /* 0 */ MAGIC_CONSUME_NOW, // Consume magic immediately without preview
    /* 1 */ MAGIC_CONSUME_WAIT_NO_PREVIEW, // Sets consume target but waits to consume. No yellow magic preview to target consumption. Unused
    /* 2 */ MAGIC_CONSUME_NOW_ALT, // Identical behaviour to MAGIC_CONSUME_NOW. Unused
    /* 3 */ MAGIC_CONSUME_LENS, // Lens of Truth consumption
    /* 4 */ MAGIC_CONSUME_WAIT_PREVIEW, // Sets consume target but waits to consume. Show magic to be consumed in yellow.
    /* 5 */ MAGIC_CONSUME_GORON_ZORA, // Goron spiked rolling or Zora electric barrier slow consumption
    /* 6 */ MAGIC_CONSUME_GIANTS_MASK, // Giant's Mask slow consumption
    /* 7 */ MAGIC_CONSUME_DEITY_BEAM // Fierce Deity Beam consumption, consumed magic now and not via request
} MagicChangeType;
#endif

s32 Health_ChangeBy(GameState_Play* play, s16 amount);
s32 Magic_RequestChange(GameState_Play* play, s16 amount, s16 type);
void Magic_Reset(GameState_Play* play);
void Magic_Update(GameState_Play* play);
void Magic_Refill(GameState_Play*);

typedef struct ObjectContext ObjectContext;
int GetObjectSlot(ObjectContext* ctx, int objectId);
int IsObjectSlotLoaded(ObjectContext* ctx, int slot);

typedef struct Actor_EnItem00 Actor_EnItem00;

Actor_EnItem00* Item_DropCollectible(GameState_Play* play, Vec3f* pos, s16 params);

void PreDraw1(Actor* actor, GameState_Play* play, int unk);
void PreDraw2(Actor* actor, GameState_Play* play, int unk);

#if defined(GAME_OOT)
Actor* ActorFind(void* actorCtx, s32 actorId, s32 actorCategory);
void Play_InitEnvironment(GameState_Play *play, u16 skyboxId);
extern u8 gFogState;
#endif

Actor* SpawnCollectible(GameState_Play* play, const Vec3f* pos, u16 param);

#if defined(GAME_MM)
void SpawnCollectible2(GameState_Play* play, int unk, void* unk2, u16 unk3);
f32 VectDist(Vec3f* vec1, Vec3f* vec2);
f32 Math_Vec3f_DistXYZAndStoreDiff(Vec3f* a, Vec3f* b, Vec3f* dest);
#endif
void Math_Vec3f_Copy(Vec3f* dest, Vec3f* src);

void EffectSsIceSmoke_Spawn(GameState_Play* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale);
void EffectSsKiraKira_SpawnDispersed(GameState_Play* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s32 life);

s32 PlayerAnimation_Update(GameState_Play* play, SkelAnime* skelAnime);
void PlayerAnimation_PlayOnce(GameState_Play* play, SkelAnime* skelAnime, PlayerAnimationHeader* animation);
void PlayerAnimation_PlayOnceSetSpeed(GameState_Play* play, SkelAnime* skelAnime, PlayerAnimationHeader* animation, f32 playSpeed);
void PlayerAnimation_PlayLoopSetSpeed(GameState_Play* play, SkelAnime* skelAnime, PlayerAnimationHeader* animation, f32 playSpeed);
s32 PlayerAnimation_OnFrame(SkelAnime* skelAnime, f32 frame);

s16 Camera_SetFinishedFlag(Camera* camera);
s32 Camera_ChangeSetting(Camera* camera, s16 setting);
s16 Camera_GetCamDirPitch(Camera* camera);
s16 Camera_GetCamDirYaw(Camera* camera);
void Camera_SetCameraData(Camera* camera, s16 setDataFlags, void* data0, void* data1, s16 data2, s16 data3, s32 arg6);

s32 Collider_InitCylinder(struct GameState_Play* play, ColliderCylinder* collider);
s32 Collider_SetCylinder(struct GameState_Play* play, ColliderCylinder* collider, struct Actor* actor, ColliderCylinderInit* src);
void Collider_UpdateCylinder(struct Actor* actor, ColliderCylinder* collider);
void Collider_DestroyCylinder(struct GameState_Play* play, ColliderCylinder* collider);

s32 CollisionCheck_SetAT(GameState_Play* play, CollisionCheckContext* colCtxt, Collider* collider);
s32 CollisionCheck_SetOC(GameState_Play* play, CollisionCheckContext* colCtxt, Collider* collider);

void Map_SetAreaEntrypoint(GameState_Play* play);

typedef s32 (*OverrideCurveLimbDraw)(struct GameState_Play* play, SkelCurve* skelCuve, s32 limbIndex, struct Actor* actor);
typedef void (*PostCurveLimbDraw)(struct GameState_Play* play, SkelCurve* skelCuve, s32 limbIndex, struct Actor* actor);

s32 SkelCurve_Init(GameState_Play* play, SkelCurve* skelCurve, CurveSkeletonHeader* skeletonHeaderSeg, CurveAnimationHeader* animation);
void SkelCurve_Destroy(struct GameState_Play* play, SkelCurve* skelCurve);
void SkelCurve_SetAnim(SkelCurve* skelCurve, CurveAnimationHeader* animation, f32 arg2, f32 endFrame, f32 curFrame, f32 playSpeed);
s32 SkelCurve_Update(struct GameState_Play* play, SkelCurve* skelCurve);
void SkelCurve_Draw(Actor* actor, struct GameState_Play* play, SkelCurve* skelCurve, OverrideCurveLimbDraw overrideLimbDraw, PostCurveLimbDraw postLimbDraw, s32 lod, Actor* thisx);

typedef s32 (*OverrideLimbDrawOpa)(struct GameState_Play* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void*);
typedef void (*PostLimbDrawOpa)(struct GameState_Play* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void*);

void SkelAnime_DrawFlexLod(GameState_Play* play, void** skeleton, Vec3s* jointTable, s32 dListCount,
                            OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw, Actor* actor, s32 lod);

void Lights_PointNoGlowSetInfo(LightInfo* info, s16 x, s16 y, s16 z, u8 r, u8 g, u8 b, s16 radius);
LightNode* LightContext_InsertLight(GameState_Play* play, LightContext* lightCtx, LightInfo* info);

void Actor_DrawLensActors(GameState_Play* play, s32 numLensActors, Actor** lensActors);

f32 Player_GetHeight(Actor_Player* player);

s32 Entrance_GetSceneIdAbsolute(u16 entrance);

s32 Player_OverrideLimbDrawGameplayFirstPerson(struct GameState_Play* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void*);

void Player_SetBootData(GameState_Play* play, Actor_Player* player);
u8 Player_GetStrength(void);

/* SysFlashrom */
s32 SysFlashrom_IsInit(void);
s32 SysFlashrom_InitFlash(void);
s32 SysFlashrom_ReadData(void* addr, u32 pageNum, u32 pageCount);
s32 SysFlashrom_ExecWrite(void* addr, u32 pageNum, u32 pageCount);

EntranceTableEntry* Entrance_GetTableEntry(u16 entrance);

extern OSPiHandle* gCartHandle;
