import { Random } from './random';

export type MmAgeRequirement = 'none' | 'child' | 'adult';
export type MmAgeReqSide = Exclude<MmAgeRequirement, 'none'>;

export type MmAgeReqItem = {
    id: string;
    label: string;
    cItem?: string;
    setting: string;
};

export const MM_AGE_REQ_ITEMS = [
    {
        id: 'hookshot',
        label: 'Hookshot',
        cItem: 'ITEM_MM_HOOKSHOT',
        setting: 'mmAgeReqHookshot',
        
    },
    {
        id: 'hookshotShort',
        label: 'Short Hookshot',
        cItem: 'ITEM_MM_HOOKSHOT_SHORT',
        setting: 'mmAgeReqHookshotShort',
        
    },
    {
        id: 'bow',
        label: 'Bow',
        cItem: 'ITEM_MM_BOW',
        setting: 'mmAgeReqBow',
        
    },
    {
        id: 'ocarinaOfTime',
        label: 'Ocarina of Time',
        cItem: 'ITEM_MM_OCARINA_OF_TIME',
        setting: 'mmAgeReqOcarinaOfTime',
        
    },
    {
        id: 'ocarinaFairy',
        label: 'Fairy Ocarina',
        cItem: 'ITEM_MM_OCARINA_FAIRY',
        setting: 'mmAgeReqOcarinaFairy',
        
    },
    {
        id: 'bomb',
        label: 'Bombs',
        cItem: 'ITEM_MM_BOMB',
        setting: 'mmAgeReqBomb',
        
    },
    {
        id: 'bombchu',
        label: 'Bombchu',
        cItem: 'ITEM_MM_BOMBCHU',
        setting: 'mmAgeReqBombchu',
        
    },
    {
        id: 'arrowFire',
        label: 'Fire Arrow',
        cItem: 'ITEM_MM_ARROW_FIRE',
        setting: 'mmAgeReqArrowFire',
        
    },
    {
        id: 'arrowIce',
        label: 'Ice Arrow',
        cItem: 'ITEM_MM_ARROW_ICE',
        setting: 'mmAgeReqArrowIce',
        
    },
    {
        id: 'arrowLight',
        label: 'Light Arrow',
        cItem: 'ITEM_MM_ARROW_LIGHT',
        setting: 'mmAgeReqArrowLight',
        
    },
    {
        id: 'hammer',
        label: 'Megaton Hammer',
        cItem: 'ITEM_MM_HAMMER',
        setting: 'mmAgeReqHammer',
        
    },
    {
        id: 'spellWind',
        label: "Farore's Wind",
        cItem: 'ITEM_MM_SPELL_WIND',
        setting: 'mmAgeReqSpellWind',
        
    },
    {
        id: 'spellLove',
        label: "Nayru's Love",
        cItem: 'ITEM_MM_SPELL_LOVE',
        setting: 'mmAgeReqSpellLove',
        
    },
    {
        id: 'spellFire',
        label: "Din's Fire",
        cItem: 'ITEM_MM_SPELL_FIRE',
        setting: 'mmAgeReqSpellFire',
        
    },
    {
        id: 'bootsIron',
        label: 'Iron Boots',
        cItem: 'ITEM_MM_BOOTS_IRON',
        setting: 'mmAgeReqBootsIron',
        
    },
    {
        id: 'bootsHover',
        label: 'Hover Boots',
        cItem: 'ITEM_MM_BOOTS_HOVER',
        setting: 'mmAgeReqBootsHover',
        
    },
    {
        id: 'tunicGoron',
        label: 'Goron Tunic',
        cItem: 'ITEM_MM_TUNIC_GORON',
        setting: 'mmAgeReqTunicGoron',
        
    },
    {
        id: 'tunicZora',
        label: 'Zora Tunic',
        cItem: 'ITEM_MM_TUNIC_ZORA',
        setting: 'mmAgeReqTunicZora',
        
    },

    {
        id: 'greatFairySword',
        label: 'Great Fairy Sword',
        cItem: 'ITEM_MM_GREAT_FAIRY_SWORD',
        setting: 'mmAgeReqGreatFairySword',
        
    },
    {
        id: 'boomerang',
        label: 'Boomerang',
        cItem: 'ITEM_MM_BOOMERANG',
        setting: 'mmAgeReqBoomerang',
        
    },
    {
        id: 'slingshot',
        label: 'Slingshot',
        cItem: 'ITEM_MM_SLINGSHOT',
        setting: 'mmAgeReqSlingshot',
        
    },
    {
        id: 'stick',
        label: 'Deku Stick',
        cItem: 'ITEM_MM_STICK',
        setting: 'mmAgeReqStick',
        
    },
    {
        id: 'maskDeku',
        label: 'Deku Mask',
        cItem: 'ITEM_MM_MASK_DEKU',
        setting: 'mmAgeReqMaskDeku',
        
    },
    {
        id: 'nut',
        label: 'Deku Nuts',
        cItem: 'ITEM_MM_NUT',
        setting: 'mmAgeReqDekuNuts',
        
    },
    {
        id: 'magicBean',
        label: 'Magic Bean',
        cItem: 'ITEM_MM_MAGIC_BEAN',
        setting: 'mmAgeReqMagicBean',
        
    },
    {
        id: 'powderKeg',
        label: 'Powder Keg',
        cItem: 'ITEM_MM_POWDER_KEG',
        setting: 'mmAgeReqPowderKeg',
        
    },
    {
        id: 'pictographBox',
        label: 'Pictograph Box',
        cItem: 'ITEM_MM_PICTOGRAPH_BOX',
        setting: 'mmAgeReqPictographBox',
        
    },
    {
        id: 'lensOfTruth',
        label: 'Lens of Truth',
        cItem: 'ITEM_MM_LENS_OF_TRUTH',
        setting: 'mmAgeReqLensOfTruth',
        
    },
    {
        id: 'maskGoron',
        label: 'Goron Mask',
        cItem: 'ITEM_MM_MASK_GORON',
        setting: 'mmAgeReqMaskGoron',
        
    },
    {
        id: 'maskZora',
        label: 'Zora Mask',
        cItem: 'ITEM_MM_MASK_ZORA',
        setting: 'mmAgeReqMaskZora',
        
    },
    {
        id: 'maskFierceDeity',
        label: 'Fierce Deity Mask',
        cItem: 'ITEM_MM_MASK_FIERCE_DEITY',
        setting: 'mmAgeReqMaskFierceDeity',
        
    },
    {
        id: 'maskPostman',
        label: 'Postman Hat',
        cItem: 'ITEM_MM_MASK_POSTMAN',
        setting: 'mmAgeReqMaskPostman',
        
    },
    {
        id: 'maskAllNight',
        label: 'All-Night Mask',
        cItem: 'ITEM_MM_MASK_ALL_NIGHT',
        setting: 'mmAgeReqMaskAllNight',
        
    },
    {
        id: 'maskBlast',
        label: 'Blast Mask',
        cItem: 'ITEM_MM_MASK_BLAST',
        setting: 'mmAgeReqMaskBlast',
        
    },
    {
        id: 'maskStone',
        label: 'Stone Mask',
        cItem: 'ITEM_MM_MASK_STONE',
        setting: 'mmAgeReqMaskStone',
        
    },
    {
        id: 'maskGreatFairy',
        label: 'Great Fairy Mask',
        cItem: 'ITEM_MM_MASK_GREAT_FAIRY',
        setting: 'mmAgeReqMaskGreatFairy',
        
    },
    {
        id: 'maskKeaton',
        label: 'Keaton Mask',
        cItem: 'ITEM_MM_MASK_KEATON',
        setting: 'mmAgeReqMaskKeaton',
        
    },
    {
        id: 'maskBremen',
        label: 'Bremen Mask',
        cItem: 'ITEM_MM_MASK_BREMEN',
        setting: 'mmAgeReqMaskBremen',
        
    },
    {
        id: 'maskBunny',
        label: 'Bunny Hood',
        cItem: 'ITEM_MM_MASK_BUNNY',
        setting: 'mmAgeReqMaskBunny',
        
    },
    {
        id: 'maskDonGero',
        label: 'Don Gero Mask',
        cItem: 'ITEM_MM_MASK_DON_GERO',
        setting: 'mmAgeReqMaskDonGero',
        
    },
    {
        id: 'maskScents',
        label: 'Mask of Scents',
        cItem: 'ITEM_MM_MASK_SCENTS',
        setting: 'mmAgeReqMaskScents',
        
    },
    {
        id: 'maskRomani',
        label: 'Romani Mask',
        cItem: 'ITEM_MM_MASK_ROMANI',
        setting: 'mmAgeReqMaskRomani',
        
    },
    {
        id: 'maskTroupeLeader',
        label: 'Troupe Leader Mask',
        cItem: 'ITEM_MM_MASK_TROUPE_LEADER',
        setting: 'mmAgeReqMaskTroupeLeader',
        
    },
    {
        id: 'maskKafei',
        label: 'Kafei Mask',
        cItem: 'ITEM_MM_MASK_KAFEI',
        setting: 'mmAgeReqMaskKafei',
        
    },
    {
        id: 'maskCouple',
        label: "Couple's Mask",
        cItem: 'ITEM_MM_MASK_COUPLE',
        setting: 'mmAgeReqMaskCouple',
        
    },
    {
        id: 'maskTruth',
        label: 'Mask of Truth',
        cItem: 'ITEM_MM_MASK_TRUTH',
        setting: 'mmAgeReqMaskTruth',
        
    },
    {
        id: 'maskKamaro',
        label: 'Kamaro Mask',
        cItem: 'ITEM_MM_MASK_KAMARO',
        setting: 'mmAgeReqMaskKamaro',
        
    },
    {
        id: 'maskGibdo',
        label: 'Gibdo Mask',
        cItem: 'ITEM_MM_MASK_GIBDO',
        setting: 'mmAgeReqMaskGibdo',
        
    },
    {
        id: 'maskGaro',
        label: 'Garo Mask',
        cItem: 'ITEM_MM_MASK_GARO',
        setting: 'mmAgeReqMaskGaro',
        
    },
    {
        id: 'maskCaptain',
        label: "Captain's Hat",
        cItem: 'ITEM_MM_MASK_CAPTAIN',
        setting: 'mmAgeReqMaskCaptain',
        
    },
    {
        id: 'maskGiant',
        label: "Giant's Mask",
        cItem: 'ITEM_MM_MASK_GIANT',
        setting: 'mmAgeReqMaskGiant',
        
    },
    {
        id: 'maskGerudo',
        label: 'Gerudo Mask',
        cItem: 'ITEM_MM_MASK_GERUDO',
        setting: 'mmAgeReqMaskGerudo',
        
    },
    {
        id: 'maskSkull',
        label: 'Skull Mask',
        cItem: 'ITEM_MM_MASK_SKULL',
        setting: 'mmAgeReqMaskSkull',
        
    },
    {
        id: 'maskSpooky',
        label: 'Spooky Mask',
        cItem: 'ITEM_MM_MASK_SPOOKY',
        setting: 'mmAgeReqMaskSpooky',
        
    },
    {
        id: 'moonTear',
        label: "Moon's Tear",
        cItem: 'ITEM_MM_MOON_TEAR',
        setting: 'mmAgeReqMoonTear',
    },
    {
        id: 'deedLand',
        label: 'Land Title Deed',
        cItem: 'ITEM_MM_DEED_LAND',
        setting: 'mmAgeReqDeedLand',
    },
    {
        id: 'deedSwamp',
        label: 'Swamp Title Deed',
        cItem: 'ITEM_MM_DEED_SWAMP',
        setting: 'mmAgeReqDeedSwamp',
    },
    {
        id: 'deedMountain',
        label: 'Mountain Title Deed',
        cItem: 'ITEM_MM_DEED_MOUNTAIN',
        setting: 'mmAgeReqDeedMountain',
    },
    {
        id: 'deedOcean',
        label: 'Ocean Title Deed',
        cItem: 'ITEM_MM_DEED_OCEAN',
        setting: 'mmAgeReqDeedOcean',
    },
    {
        id: 'letterToMama',
        label: 'Priority Mail to Mama',
        cItem: 'ITEM_MM_LETTER_TO_MAMA',
        setting: 'mmAgeReqLetterToMama',
    },
    {
        id: 'letterToKafei',
        label: 'Letter to Kafei',
        cItem: 'ITEM_MM_LETTER_TO_KAFEI',
        setting: 'mmAgeReqLetterToKafei',
    },
    {
        id: 'pendantOfMemories',
        label: 'Pendant of Memories',
        cItem: 'ITEM_MM_PENDANT_OF_MEMORIES',
        setting: 'mmAgeReqPendantOfMemories',
    },
    {
        id: 'strength',
        label: 'Silver/Golden Gauntlets',
        setting: 'mmAgeReqStrength',
    },
    {
        id: 'swordKokiri',
        label: 'Kokiri Sword',
        cItem: 'ITEM_MM_SWORD_KOKIRI',
        setting: 'mmAgeReqSwordKokiri',
    },
    {
        id: 'swordRazor',
        label: 'Razor Sword',
        cItem: 'ITEM_MM_SWORD_RAZOR',
        setting: 'mmAgeReqSwordRazor',
    },
    {
        id: 'swordGilded',
        label: 'Gilded Sword',
        cItem: 'ITEM_MM_SWORD_GILDED',
        setting: 'mmAgeReqSwordGilded',
    },
    {
        id: 'swordMaster',
        label: 'Master Sword',
        cItem: 'ITEM_MM_SWORD_MASTER',
        setting: 'mmAgeReqSwordMaster',
    },
    {
        id: 'swordGoron',
        label: 'Goron Swords',
        cItem: 'ITEM_MM_SWORD_GORON',
        setting: 'mmAgeReqSwordGoron',
    },
    {
        id: 'shieldDeku',
        label: 'Deku Shield',
        cItem: 'ITEM_MM_SHIELD_DEKU',
        setting: 'mmAgeReqShieldDeku',
    },
    {
        id: 'shieldHero',
        label: "Hero's Shield",
        cItem: 'ITEM_MM_SHIELD_HERO',
        setting: 'mmAgeReqShieldHero',
    },
    {
        id: 'shieldHylian',
        label: 'Hylian Shield',
        cItem: 'ITEM_MM_SHIELD_HYLIAN',
        setting: 'mmAgeReqShieldHylian',
    },
    {
        id: 'shieldMirror',
        label: 'Mirror Shield',
        cItem: 'ITEM_MM_SHIELD_MIRROR',
        setting: 'mmAgeReqShieldMirror',
    },
] as const;

export type MmAgeReqItemDef = typeof MM_AGE_REQ_ITEMS[number];
export type MmAgeReqItemId = MmAgeReqItemDef['id'];

export const MM_AGE_REQ_ITEM_POOL_SETTINGS: Partial<Record<MmAgeReqItemId, string>> = {
    hookshotShort: 'shortHookshotMm',
    ocarinaFairy: 'fairyOcarinaMm',

    hammer: 'hammerMm',
    boomerang: 'boomerangMm',
    slingshot: 'slingshotMm',

    spellFire: 'spellFireMm',
    spellWind: 'spellWindMm',
    spellLove: 'spellLoveMm',

    bootsIron: 'bootsIronMm',
    bootsHover: 'bootsHoverMm',

    tunicGoron: 'tunicGoronMm',
    tunicZora: 'tunicZoraMm',

    maskGerudo: 'gerudoMaskMm',
    maskSkull: 'skullMaskMm',
    maskSpooky: 'spookyMaskMm',

    strength: 'strengthMm',

    swordMaster: 'masterSwordMm',
    swordGoron: 'goronSwordsMm',
    shieldDeku: 'dekuShieldMm',
    shieldHylian: 'hylianShieldMm',
};

export const MM_AGE_REQ_RESTRICTION_DEPENDENCIES: Partial<
    Record<MmAgeReqItemId, readonly MmAgeReqItemId[]>
> = {
    maskDeku: ['deedSwamp'],
    maskGoron: ['powderKeg', 'deedMountain'],
    maskZora: ['deedOcean'],
    strength: ['powderKeg'],
};

export function isMmAgeReqItemInPool(settings: any, item: MmAgeReqItemDef) {
    const poolSetting = MM_AGE_REQ_ITEM_POOL_SETTINGS[item.id];

    if (!poolSetting) {
        return true;
    }

    return Boolean(settings[poolSetting]);
}

export function conflictingDependentItems(
    controllingItemId: MmAgeReqItemId,
): readonly MmAgeReqItemId[] {
    return MM_AGE_REQ_RESTRICTION_DEPENDENCIES[controllingItemId] ?? [];
}

export function controllingItemsFor(
    dependentItemId: MmAgeReqItemId,
): MmAgeReqItemId[] {
    return Object.entries(MM_AGE_REQ_RESTRICTION_DEPENDENCIES)
        .filter(([, dependentIds]) => dependentIds?.includes(dependentItemId))
        .map(([controllingItemId]) => controllingItemId as MmAgeReqItemId);
}

export function isControllingItem(itemId: MmAgeReqItemId) {
    return MM_AGE_REQ_RESTRICTION_DEPENDENCIES[itemId] !== undefined;
}

export function isPowderKegSplitAllowed(
    sameSideItemIds: ReadonlySet<MmAgeReqItemId>,
    oppositeItemIds: ReadonlySet<MmAgeReqItemId>,
) {
    return (
        (
            sameSideItemIds.has('maskGoron') &&
            oppositeItemIds.has('strength')
        ) ||
        (
            sameSideItemIds.has('strength') &&
            oppositeItemIds.has('maskGoron')
        )
    );
}

type MmAgeReqAssignments = Partial<Record<MmAgeReqItemId, MmAgeReqSide>>;

function assignmentsPotentiallyValid(
    assignments: MmAgeReqAssignments,
    unresolved: ReadonlySet<MmAgeReqItemId>,
): boolean {
    const goronSide = assignments.maskGoron;
    const strengthSide = assignments.strength;

    for (const [controllerKey, dependentIds] of Object.entries(
        MM_AGE_REQ_RESTRICTION_DEPENDENCIES,
    )) {
        const controllerId = controllerKey as MmAgeReqItemId;
        const controllerSide = assignments[controllerId];

        if (!controllerSide || !dependentIds) {
            continue;
        }

        for (const dependentId of dependentIds) {
            const dependentSide = assignments[dependentId];

            if (!dependentSide || dependentSide === controllerSide) {
                continue;
            }

            if (dependentId === 'powderKeg') {
                if (
                    goronSide !== undefined &&
                    strengthSide !== undefined &&
                    goronSide !== strengthSide
                ) {
                    continue;
                }
                if (
                    goronSide === undefined &&
                    unresolved.has('maskGoron')
                ) {
                    continue;
                }

                if (
                    strengthSide === undefined &&
                    unresolved.has('strength')
                ) {
                    continue;
                }
            }

            return false;
        }
    }

    return true;
}

export function getMmAgeReqRandomItemIds(settings: any): MmAgeReqItemId[] {
    const value = settings.mmAgeReqRandom;

    if (!value || value.type === 'none') {
        return [];
    }

    if (value.type === 'all') {
        return MM_AGE_REQ_ITEMS.map(item => item.id);
    }

    if (value.type !== 'specific' || !Array.isArray(value.values)) {
        return [];
    }

    const validIds = new Set<string>(MM_AGE_REQ_ITEMS.map(item => item.id));

    return value.values.filter(
        (id: unknown): id is MmAgeReqItemId =>
            typeof id === 'string' && validIds.has(id),
    );
}

export function randomizeMmAgeRequirements(
    settings: any,
    random: Random,
    itemIds: readonly MmAgeReqItemId[],
) {
    const poolItems = MM_AGE_REQ_ITEMS.filter(
        item => isMmAgeReqItemInPool(settings, item),
    );

    const poolIds = new Set<MmAgeReqItemId>(poolItems.map(item => item.id));
    const randomIds = new Set<MmAgeReqItemId>(
        itemIds.filter(id => poolIds.has(id)),
    );

    if (randomIds.size === 0) {
        return;
    }

    const assignments: MmAgeReqAssignments = {};

    for (const item of poolItems) {
        if (randomIds.has(item.id)) {
            settings[item.setting] = 'none';
            continue;
        }

        const age = settings[item.setting] as MmAgeRequirement;
        if (age === 'child' || age === 'adult') {
            assignments[item.id] = age;
        }
    }

    const randomItems = poolItems.filter(item => randomIds.has(item.id));
    const preferredSides = new Map<MmAgeReqItemId, MmAgeReqSide>();

    for (const item of randomItems) {
        preferredSides.set(
            item.id,
            (random.next() & 1) ? 'adult' : 'child',
        );
    }

    const orderedItems = [
        ...randomItems.filter(item => isControllingItem(item.id)),
        ...randomItems.filter(item => !isControllingItem(item.id)),
    ];

    const unresolved = new Set<MmAgeReqItemId>(
        orderedItems.map(item => item.id),
    );

    const solve = (index: number): boolean => {
        if (index === orderedItems.length) {
            return assignmentsPotentiallyValid(assignments, new Set());
        }

        const item = orderedItems[index];
        const preferred = preferredSides.get(item.id)!;
        const sides: MmAgeReqSide[] = [
            preferred,
            preferred === 'child' ? 'adult' : 'child',
        ];

        unresolved.delete(item.id);

        for (const side of sides) {
            assignments[item.id] = side;

            if (
                assignmentsPotentiallyValid(assignments, unresolved) &&
                solve(index + 1)
            ) {
                return true;
            }
        }

        delete assignments[item.id];
        unresolved.add(item.id);
        return false;
    };

    if (!solve(0)) {
        throw new Error('Unable to resolve random MM age requirements');
    }

    for (const item of randomItems) {
        const side = assignments[item.id]!;
        settings[item.setting] = side;
    }
}

export async function resolveMmAgeRequirements(
    settings: any,
    seed: string,
) {
    const itemIds = getMmAgeReqRandomItemIds(settings);

    if (itemIds.length === 0) {
        return;
    }

    const random = new Random();
    await random.seed([
        'mm-age-requirements',
        seed.length.toString(16),
        seed,
    ].join('\0'));

    randomizeMmAgeRequirements(settings, random, itemIds);
}
