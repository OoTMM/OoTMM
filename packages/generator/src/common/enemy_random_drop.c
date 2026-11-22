#include <combo.h>
#include <combo/enemy.h>
#include <combo/enemy_native.h>
#include <combo/global.h>
#include <combo/rand.h>
#include <combo/sfx_source.h>
#include <combo/common/actors/En_Item00.h>

typedef struct
{
    u32 mask;
    u8 rows[17][16][2];
#if defined(GAME_MM)
    u8 overrides[7][2];
#endif
} EnemyDropRows;

static volatile EnemyDropRows sRows __attribute__((section(".data"))) = { 0 };
#if defined(GAME_OOT)
EXPORT_SYMBOL(FOREIGN_ENEMY_RANDOM_DROPS_OOT, sRows);
EXPORT_SYMBOL(FOREIGN_ENEMY_RANDOM_DROPS_V2_OOT, sRows);
#else
EXPORT_SYMBOL(FOREIGN_ENEMY_RANDOM_DROPS_V2_MM, sRows);
#endif

extern void EnemyPickupTossed(Actor_EnItem00*, PlayState*);

static int enemyFlexible(PlayState* play, Vec3f* pos, int* quantity)
{
    int hp = gSave.info.playerData.health;
    *quantity = 1;
    if (hp <= 0x10)
    {
#if defined(GAME_MM)
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, pos->x, pos->y + 40.f, pos->z, 0, 0, 0, 2);
        SfxSource_PlaySfxAtFixedWorldPos(play, pos, 40, NA_SE_EV_BUTTERFRY_TO_FAIRY);
#else
        Item_DropCollectible(play, pos, ITEM00_FLEXIBLE);
#endif
        return -1;
    }
    if (hp <= 0x50)
    {
        *quantity = hp <= 0x30 ? 3 : 1;
        return ITEM00_RECOVERY_HEART;
    }
#if defined(GAME_MM)
    if (gSave.info.playerData.magicLevel && !gSave.info.playerData.magic)
        return ITEM00_MAGIC_LARGE;
    if (gSave.info.playerData.magicLevel && gSave.info.playerData.magic <= (gSave.info.playerData.magicLevel >> 1))
        return ITEM00_MAGIC_SMALL;
    if (!comboIsLinkAdult() && gMmExtraAmmo.slingshotSeeds < 6)
        return ITEM00_SEEDS;
    if (comboIsLinkAdult() && gSave.info.inventory.ammo[ITS_NATIVE_BOW] < 6)
        return ITEM00_ARROWS_10;
#else
    if (gSave.info.playerData.magicLevel && gSave.info.playerData.magic <= (gSave.info.playerData.magicLevel >> 1))
        return ITEM00_MAGIC_LARGE;
    if (gSave.info.inventory.ammo[ITS_NATIVE_BOW] < 6)
        return ITEM00_ARROWS_30;
#endif
    if (gSave.info.inventory.ammo[ITS_NATIVE_BOMBS] < 6)
        return ITEM00_BOMB;
    return gSave.info.playerData.rupees < 11 ? ITEM00_RUPEE_RED : -1;
}

void comboEnemyRandomDrop(PlayState* play, Actor* from, Vec3f* pos, s16 params)
{
    EnemyNativeSegments scope = { 0 };
    int row, roll, item, quantity;
    Actor_EnItem00* pickup;
    row = (u16)params >> 4;
    if (!play || !pos || (params & 15) || row >= 17 || !(sRows.mask & (1u << row)) ||
        (from && from != comboEnemyCurrentActor()))
    {
        g.roomEnemyLackSoul = 1;
        return;
    }
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    roll = (s32)(Rand_ZeroOne() * 16.f);
    item = sRows.rows[row][roll][0];
    quantity = sRows.rows[row][roll][1];
#if defined(GAME_OOT)
    if (from && (from->dropFlag & 0x23))
    {
        if (from->dropFlag & 1)
            item = ITEM00_ARROWS_30;
        else if (from->dropFlag & 2)
            item = ITEM00_RECOVERY_HEART;
        else
            item = ITEM00_RUPEE_PURPLE;
        quantity = 1;
    }
    int singleNut = item == 0xfb;
    if (singleNut)
        item = ITEM00_NUT;
#else
    if (from && (from->dropFlag & 0x7f))
    {
        unsigned bit = 0;
        while (!(from->dropFlag & (1u << bit)))
            ++bit;
        item = (from->dropFlag & 0x20) ? ITEM00_RUPEE_PURPLE : sRows.overrides[bit][0];
        quantity = sRows.overrides[bit][1];
    }
    int fivePack = -1;
    if (item == 0xfd)
        fivePack = item = ITEM00_ARROWS_10;
    if (item == 0xfc)
        fivePack = item = ITEM00_NUT;
#endif
    if (item == 0xfe)
        item = enemyFlexible(play, pos, &quantity);
    while (quantity-- && item >= 0 && item != 0xff)
    {
        item = EnItem00_FixDrop(item);
        if (item < 0)
            break;
        pickup = (Actor_EnItem00*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ITEM00,
                                              pos->x, pos->y, pos->z, 0, 0, 0, item);
        if (!pickup)
            continue;
#if defined(GAME_MM)
        pickup->enemySourceQuantity = item == fivePack && pickup->actor.params == fivePack;
#else
        pickup->enemySourceQuantity = singleNut && item == ITEM00_NUT && pickup->actor.params == ITEM00_NUT;
#endif
        pickup->actor.velocity.y = 8.f;
        pickup->actor.speed = 2.f;
        pickup->actor.gravity = -0.9f;
        pickup->actor.world.rot.y = (s32)(Rand_ZeroOne() * 40000.f);
        Actor_SetScale(&pickup->actor, 0.f);
        pickup->handler = EnemyPickupTossed;
        pickup->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        pickup->actor.room = -1;
        pickup->timer = 220;
    }
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_RANDOM_DROP, comboEnemyRandomDrop);

#if defined(GAME_OOT)
typedef struct
{
    u16 params[3];
    u8 count;
    u8 last;
} EnemyDrop;

static int enemyDropTranslate(u32 source, EnemyDrop* drop)
{
    u16 flags;
    u16 item;
    if (source & ~0x180ffu)
        return 0;
    flags = (source & 0x8000) | ((source >> 2) & 0x4000);
    drop->count = 1;
    drop->last = 0;
    switch (source & 0xff)
    {
    case 0x00: item = ITEM00_RUPEE_GREEN; break;
    case 0x01: item = ITEM00_RUPEE_BLUE; break;
    case 0x02: item = ITEM00_RUPEE_RED; break;
    case 0x03: item = ITEM00_RECOVERY_HEART; break;
    case 0x04: item = ITEM00_BOMB; break;
    case 0x05: item = ITEM00_ARROWS_10; break;
    case 0x08: item = ITEM00_ARROWS_30; break;
    case 0x09:
    case 0x0a:
        item = ITEM00_ARROWS_30;
        drop->count = (source & 0xff) == 0x09 ? 2 : 3;
        drop->params[1] = drop->params[2] = flags | ITEM00_ARROWS_10;
        break;
    case 0x0b: item = ITEM00_BOMBS_5; break;
    case 0x0c: item = ITEM00_NUT; break;
    case 0x0d: item = ITEM00_STICK; break;
    case 0x0e: item = ITEM00_MAGIC_LARGE; break;
    case 0x0f: item = ITEM00_MAGIC_SMALL; break;
    case 0x12: item = ITEM00_FLEXIBLE; break;
    case 0x13: item = ITEM00_RUPEE_ORANGE; break;
    case 0x14: item = ITEM00_RUPEE_PURPLE; break;
    case 0x15:
        flags = source & 0x8000;
        item = ITEM00_RECOVERY_HEART;
        drop->count = 3;
        drop->last = 1;
        drop->params[1] = drop->params[2] = flags | item;
        break;
    case 0x17:
        item = ITEM00_NUT;
        drop->count = 2;
        drop->params[1] = flags | item;
        break;
    default:
        return 0;
    }
    drop->params[0] = flags | item;
    return 1;
}

static Actor* enemyDrop(PlayState* play, Vec3f* position, u32 source,
                        Actor* (*spawn)(PlayState*, const Vec3f*, int))
{
    EnemyDrop drop;
    EnemyNativeSegments scope = { 0 };
    Actor* first = NULL;
    Actor* last = NULL;
    if (!play || !position || !enemyDropTranslate(source, &drop))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return NULL;
    for (u32 i = 0; i < drop.count; ++i)
    {
        last = spawn(play, position, (s16)drop.params[i]);
        if (last && (source & 0xff) == 0x0c && last->id == ACTOR_EN_ITEM00 && last->params == ITEM00_NUT)
            ((Actor_EnItem00*)last)->enemySourceQuantity = 1;
        if (!i)
            first = last;
    }
    comboEnemyNativeLeave(&scope);
    return drop.last ? last : first;
}

Actor* comboEnemyDropCollectible(PlayState* play, Vec3f* position, u32 source)
{
    return enemyDrop(play, position, source, Item_DropCollectible);
}

Actor* comboEnemyDropCollectible2(PlayState* play, Vec3f* position, u32 source)
{
    if ((source & 0xff) == 0x15)
        return NULL;
    if ((source & 0xff) == 0x12)
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    return enemyDrop(play, position, source, Item_DropCollectible2);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DROP, comboEnemyDropCollectible);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DROP_STATIONARY, comboEnemyDropCollectible2);
#elif defined(GAME_MM)
static int enemyDropTranslate(u16 source, u32* params, u8* count)
{
    u32 flags;
    u32 item;
    if (source & 0x3f00)
        return 0;
    flags = (source & 0x8000) | ((source & 0x4000) << 2);
    *count = 1;
    switch (source & 0xff)
    {
    case 0x00: item = ITEM00_RUPEE_GREEN; break;
    case 0x01: item = ITEM00_RUPEE_BLUE; break;
    case 0x02: item = ITEM00_RUPEE_RED; break;
    case 0x03: item = ITEM00_RECOVERY_HEART; break;
    case 0x04: item = ITEM00_BOMB; break;
    case 0x09: item = ITEM00_ARROWS_10; break;
    case 0x0a: item = ITEM00_ARROWS_30; break;
    case 0x0b: item = ITEM00_BOMBS_5; break;
    case 0x0c:
        item = ITEM00_NUT;
        *count = 5;
        break;
    case 0x0d: item = ITEM00_STICK; break;
    case 0x0e: item = ITEM00_MAGIC_LARGE; break;
    case 0x0f: item = ITEM00_MAGIC_SMALL; break;
    case 0x10: item = ITEM00_SEEDS; break;
    case 0x12: item = ITEM00_FLEXIBLE; break;
    case 0x13: item = ITEM00_RUPEE_ORANGE; break;
    case 0x14: item = ITEM00_RUPEE_PURPLE; break;
    default:
        return 0;
    }
    *params = flags | item;
    return 1;
}

static Actor* enemyDrop(PlayState* play, Vec3f* position, s16 source,
                        Actor* (*spawn)(PlayState*, const Vec3f*, int))
{
    u32 params;
    u8 count;
    EnemyNativeSegments scope = { 0 };
    Actor* first = NULL;
    if (!play || !position || !enemyDropTranslate((u16)source, &params, &count))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return NULL;
    for (u32 i = 0; i < count; ++i)
    {
        Actor* item = spawn(play, position, params);
        if (!i)
            first = item;
    }
    comboEnemyNativeLeave(&scope);
    return first;
}

Actor* comboEnemyDropCollectible(PlayState* play, Vec3f* position, s16 source)
{
    return enemyDrop(play, position, source, Item_DropCollectible);
}

Actor* comboEnemyDropCollectible2(PlayState* play, Vec3f* position, s16 source)
{
    if ((source & 0xff) == 0x12)
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    return enemyDrop(play, position, source, Item_DropCollectible2);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DROP, comboEnemyDropCollectible);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DROP_STATIONARY, comboEnemyDropCollectible2);
#endif
