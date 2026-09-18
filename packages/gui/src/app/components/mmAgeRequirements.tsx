import { useCallback, useEffect, useMemo } from 'react';
import { FaXmark } from 'react-icons/fa6';
import { MM_AGE_REQ_ITEMS, conflictingDependentItems, controllingItemsFor, getMmAgeReqRandomItemIds, isControllingItem, isMmAgeReqItemInPool, isPowderKegSplitAllowed } from '@ootmm/core';

import { Select, Button, Card, Tooltip } from './ui';
import { useStore } from '../store';

type AgeSide = 'child' | 'adult';
type AgeReqItem = typeof MM_AGE_REQ_ITEMS[number];

const AGE_CHILD = 1;
const AGE_ADULT = 2;
const AGE_BOTH = AGE_CHILD | AGE_ADULT;

type AgeConstraint =
    | { itemSetting: string }
    | { all: readonly AgeConstraint[] }
    | { any: readonly AgeConstraint[] };

type AllLocationsAgeRule = {
    id: string;
    label: string;
    reliefChildSettings: readonly string[];
    when?: (settings: any) => boolean;
    requirement: AgeConstraint;
};

function req(itemSetting: string): AgeConstraint {
    return { itemSetting };
}

function all(...requirements: AgeConstraint[]): AgeConstraint {
    return { all: requirements };
}

function any(...requirements: AgeConstraint[]): AgeConstraint {
    return { any: requirements };
}

const ALL_LOCATIONS_AGE_RULES: readonly AllLocationsAgeRule[] = [
    {
        id: 'greatBayCoastBeanHp',
        label: 'Great Bay Coast HP',
        reliefChildSettings: [
            'mmAgeReqChildMagicBean',
        ],
        when: settings => settings.hookshotAnywhereMm !== 'logical',
        requirement: all(
            req('mmAgeReqChildMagicBean'),
            req('mmAgeReqChildHookshot'),
        ),
    },
    {
        id: 'snowheadStrayFairies',
        label: 'Snowhead Stray Fairies',
        reliefChildSettings: [
            'mmAgeReqChildMaskGreatFairy',
        ],
        requirement: all(
            req('mmAgeReqChildMaskGreatFairy'),
            req('mmAgeReqChildLensOfTruth'),
            any(
                req('mmAgeReqChildBow'),
                all(
                    req('mmAgeReqChildHookshot'),
                    req('mmAgeReqChildHookshotShort'),
                ),
            ),
        ),
    },
    {
        id: 'transformationMasks',
        label: 'Transformation Masks',
        reliefChildSettings: [
            'mmAgeReqChildMaskDeku',
            'mmAgeReqChildMaskGoron',
            'mmAgeReqChildMaskZora',
        ],
        requirement: all(
            req('mmAgeReqChildMaskDeku'),
            req('mmAgeReqChildMaskGoron'),
            req('mmAgeReqChildMaskZora'),
            req('mmAgeReqChildBow'),
            req('mmAgeReqChildArrowFire'),
            req('mmAgeReqChildArrowIce'),
            req('mmAgeReqChildArrowLight'),
            req('mmAgeReqChildHookshot'),
            req('mmAgeReqChildHookshotShort'),
        ),
    },
];

function itemSetting(item: AgeReqItem, side: AgeSide) {
    return side === 'child' ? item.childSetting : item.adultSetting;
}

function oppositeSetting(item: AgeReqItem, side: AgeSide) {
    return side === 'child' ? item.adultSetting : item.childSetting;
}

function isAdultMaskEnabled(settings: any) {
    return Boolean(settings.adultMaskMm);
}

function shouldEnforceAllLocationsAgeRules(settings: any) {
    return settings.logic === 'allLocations' && !isAdultMaskEnabled(settings);
}

function possibleAges(
    constraint: AgeConstraint,
    availableSettings: ReadonlySet<string>,
    childLockedSettings: ReadonlySet<string>,
    adultLockedSettings: ReadonlySet<string>,
): number {
    if ('itemSetting' in constraint) {
        if (!availableSettings.has(constraint.itemSetting)) {
            return 0;
        }

        if (childLockedSettings.has(constraint.itemSetting)) {
            return AGE_CHILD;
        }

        if (adultLockedSettings.has(constraint.itemSetting)) {
            return AGE_ADULT;
        }

        return AGE_BOTH;
    }

    if ('all' in constraint) {
        return constraint.all.reduce(
            (ages, requirement) =>
                ages & possibleAges(
                    requirement,
                    availableSettings,
                    childLockedSettings,
                    adultLockedSettings,
                ),
            AGE_BOTH,
        );
    }

    return constraint.any.reduce(
        (ages, requirement) =>
            ages | possibleAges(
                requirement,
                availableSettings,
                childLockedSettings,
                adultLockedSettings,
            ),
        0,
    );
}

function collectConstraintSettings(
    constraint: AgeConstraint,
    result: Set<string>,
) {
    if ('itemSetting' in constraint) {
        result.add(constraint.itemSetting);
        return;
    }

    const children = 'all' in constraint ? constraint.all : constraint.any;

    for (const child of children) {
        collectConstraintSettings(child, result);
    }
}

const ALL_LOCATIONS_COUPLED_SETTINGS = (() => {
    const result = new Set<string>();

    for (const rule of ALL_LOCATIONS_AGE_RULES) {
        collectConstraintSettings(rule.requirement, result);
    }

    return result;
})();

function itemParticipatesInAllLocationsRule(item: AgeReqItem) {
    return ALL_LOCATIONS_COUPLED_SETTINGS.has(item.childSetting);
}

function itemHasCoreAgeCoupling(item: AgeReqItem) {
    return (
        isControllingItem(item.id) ||
        controllingItemsFor(item.id).length > 0 ||
        conflictingDependentItems(item.id).length > 0
    );
}

function isUnsafeRandomItem(
    settings: any,
    item: AgeReqItem,
) {
    if (!shouldEnforceAllLocationsAgeRules(settings)) {
        return false;
    }

    return (
        itemParticipatesInAllLocationsRule(item) ||
        itemHasCoreAgeCoupling(item)
    );
}

function fixedSettingSets(
    childItems: readonly AgeReqItem[],
    adultItems: readonly AgeReqItem[],
) {
    const childLockedSettings = new Set<string>(
        childItems.map(item => item.childSetting),
    );

    const adultLockedSettings = new Set<string>(
        adultItems.map(item => item.childSetting),
    );

    return {
        childLockedSettings,
        adultLockedSettings,
    };
}

function conflictingAllLocationsRules(
    settings: any,
    poolItems: readonly AgeReqItem[],
    childItems: readonly AgeReqItem[],
    adultItems: readonly AgeReqItem[],
) {
    if (!shouldEnforceAllLocationsAgeRules(settings)) {
        return [];
    }

    const availableSettings = new Set<string>(
        poolItems.map(item => item.childSetting),
    );

    const {
        childLockedSettings,
        adultLockedSettings,
    } = fixedSettingSets(childItems, adultItems);

    return ALL_LOCATIONS_AGE_RULES.filter(rule => {
        if (rule.when && !rule.when(settings)) {
            return false;
        }

        return possibleAges(
            rule.requirement,
            availableSettings,
            childLockedSettings,
            adultLockedSettings,
        ) === 0;
    });
}

function wouldBreakAllLocationsRule(
    settings: any,
    poolItems: readonly AgeReqItem[],
    childItems: readonly AgeReqItem[],
    adultItems: readonly AgeReqItem[],
    item: AgeReqItem,
    side: AgeSide,
) {
    if (!shouldEnforceAllLocationsAgeRules(settings)) {
        return false;
    }

    const availableSettings = new Set<string>(
        poolItems.map(poolItem => poolItem.childSetting),
    );

    const {
        childLockedSettings,
        adultLockedSettings,
    } = fixedSettingSets(childItems, adultItems);

    childLockedSettings.delete(item.childSetting);
    adultLockedSettings.delete(item.childSetting);

    if (side === 'child') {
        childLockedSettings.add(item.childSetting);
    } else {
        adultLockedSettings.add(item.childSetting);
    }

    return ALL_LOCATIONS_AGE_RULES.some(rule => {
        if (rule.when && !rule.when(settings)) {
            return false;
        }

        return possibleAges(
            rule.requirement,
            availableSettings,
            childLockedSettings,
            adultLockedSettings,
        ) === 0;
    });
}

function buildItemOptions(
    settings: any,
    side: AgeSide,
    poolItems: readonly AgeReqItem[],
    usedItems: ReadonlySet<AgeReqItem['id']>,
    childItems: readonly AgeReqItem[],
    adultItems: readonly AgeReqItem[],
) {
    const sameSideItems = side === 'child' ? childItems : adultItems;
    const oppositeItems = side === 'child' ? adultItems : childItems;

    const sameSideItemIds = new Set<AgeReqItem['id']>(
        sameSideItems.map(item => item.id),
    );

    const oppositeItemIds = new Set<AgeReqItem['id']>(
        oppositeItems.map(item => item.id),
    );

    return poolItems
        .filter(item => !usedItems.has(item.id))
        .slice()
        .sort((a, b) => a.label.localeCompare(b.label))
        .map(item => {
            let disabled = false;

            if (
                shouldEnforceAllLocationsAgeRules(settings) &&
                !isControllingItem(item.id)
            ) {
                const powderKegSplitAllowed =
                    item.id === 'powderKeg' &&
                    isPowderKegSplitAllowed(
                        sameSideItemIds,
                        oppositeItemIds,
                    );

                const hasConflict = controllingItemsFor(item.id)
                    .some(controllingId =>
                        oppositeItemIds.has(controllingId)
                    );

                disabled = hasConflict && !powderKegSplitAllowed;
            }

            if (
                !disabled &&
                wouldBreakAllLocationsRule(
                    settings,
                    poolItems,
                    childItems,
                    adultItems,
                    item,
                    side,
                )
            ) {
                disabled = true;
            }

            return {
                value: item.id,
                label: item.label,
                disabled,
            };
        });
}

export function MmAgeRequirements() {
    const settings = useStore(state => state.settings);
    const patchSettings = useStore(state => state.patchSettings);

    const poolItems = useMemo(() => {
        return MM_AGE_REQ_ITEMS.filter(
            item => isMmAgeReqItemInPool(settings, item),
        );
    }, [settings]);

    const randomItemIds = useMemo(() => {
        return new Set<AgeReqItem['id']>(
            getMmAgeReqRandomItemIds(settings),
        );
    }, [settings]);

    const randomItems = useMemo(() => {
        return poolItems.filter(item => randomItemIds.has(item.id));
    }, [poolItems, randomItemIds]);

    const childItems = useMemo(() => {
        return poolItems.filter(item =>
            !randomItemIds.has(item.id) &&
            Boolean((settings as any)[item.childSetting])
        );
    }, [poolItems, randomItemIds, settings]);

    const adultItems = useMemo(() => {
        return poolItems.filter(item =>
            !randomItemIds.has(item.id) &&
            Boolean((settings as any)[item.adultSetting])
        );
    }, [poolItems, randomItemIds, settings]);

    const usedItems = useMemo(() => {
        return new Set(
            [...childItems, ...randomItems, ...adultItems]
                .map(item => item.id),
        );
    }, [childItems, randomItems, adultItems]);

    const childItemOptions = useMemo(() => {
        return buildItemOptions(
            settings,
            'child',
            poolItems,
            usedItems,
            childItems,
            adultItems,
        );
    }, [settings, poolItems, usedItems, childItems, adultItems]);

    const randomItemOptions = useMemo(() => {
        return poolItems
            .filter(item => !usedItems.has(item.id))
            .slice()
            .sort((a, b) => a.label.localeCompare(b.label))
            .map(item => ({
                value: item.id,
                label: item.label,
                disabled: isUnsafeRandomItem(settings, item),
            }));
    }, [poolItems, usedItems, settings]);

    const adultItemOptions = useMemo(() => {
        return buildItemOptions(
            settings,
            'adult',
            poolItems,
            usedItems,
            childItems,
            adultItems,
        );
    }, [settings, poolItems, usedItems, childItems, adultItems]);

    useEffect(() => {
        const patch: Record<string, any> = {};
        const poolItemIds = new Set<AgeReqItem['id']>(poolItems.map(item => item.id));

        for (const item of MM_AGE_REQ_ITEMS) {
            if (poolItemIds.has(item.id)) {
                continue;
            }

            if (Boolean((settings as any)[item.childSetting])) {
                patch[item.childSetting] = false;
            }

            if (Boolean((settings as any)[item.adultSetting])) {
                patch[item.adultSetting] = false;
            }
        }

        const currentRandomIds = getMmAgeReqRandomItemIds(settings);
        const nextRandomIds = currentRandomIds.filter(id => {
            if (!poolItemIds.has(id)) {
                return false;
            }

            if (!shouldEnforceAllLocationsAgeRules(settings)) {
                return true;
            }

            const item = poolItems.find(poolItem => poolItem.id === id);

            if (!item) {
                return false;
            }

            return !isUnsafeRandomItem(settings, item);
        });

        if (nextRandomIds.length !== currentRandomIds.length) {
            patch.mmAgeReqRandom = {
                type: 'specific',
                values: nextRandomIds,
            };
        }

        if (shouldEnforceAllLocationsAgeRules(settings)) {
            const childItemIds = new Set<AgeReqItem['id']>(
                childItems.map(item => item.id),
            );

            const adultItemIds = new Set<AgeReqItem['id']>(
                adultItems.map(item => item.id),
            );

            for (const item of childItems) {
                if (isControllingItem(item.id)) {
                    continue;
                }

                const powderKegSplitAllowed =
                    item.id === 'powderKeg' &&
                    isPowderKegSplitAllowed(
                        childItemIds,
                        adultItemIds,
                    );

                const hasConflict = controllingItemsFor(item.id)
                    .some(controllingId =>
                        adultItemIds.has(controllingId)
                    );

                if (hasConflict && !powderKegSplitAllowed) {
                    patch[item.childSetting] = false;
                }
            }

            for (const item of adultItems) {
                if (isControllingItem(item.id)) {
                    continue;
                }

                const powderKegSplitAllowed =
                    item.id === 'powderKeg' &&
                    isPowderKegSplitAllowed(
                        adultItemIds,
                        childItemIds,
                    );

                const hasConflict = controllingItemsFor(item.id)
                    .some(controllingId =>
                        childItemIds.has(controllingId)
                    );

                if (hasConflict && !powderKegSplitAllowed) {
                    patch[item.adultSetting] = false;
                }
            }

            const conflicts = conflictingAllLocationsRules(
                settings,
                poolItems,
                childItems,
                adultItems,
            );

            for (const rule of conflicts) {
                for (const reliefChildSetting of rule.reliefChildSettings) {
                    const reliefItem = poolItems.find(
                        item => item.childSetting === reliefChildSetting,
                    );

                    if (!reliefItem) {
                        continue;
                    }

                    if (Boolean((settings as any)[reliefItem.childSetting])) {
                        patch[reliefItem.childSetting] = false;
                    }

                    if (Boolean((settings as any)[reliefItem.adultSetting])) {
                        patch[reliefItem.adultSetting] = false;
                    }
                }
            }
        }

        if (Object.keys(patch).length > 0) {
            patchSettings(patch as any);
        }
    }, [settings, poolItems, childItems, adultItems, patchSettings]);

    const addItem = useCallback((side: AgeSide, itemId: string | null) => {
        if (!itemId) {
            return;
        }

        const item = poolItems.find(x => x.id === itemId);

        if (!item) {
            return;
        }

        const sameSideItems = side === 'child' ? childItems : adultItems;
        const oppositeItems = side === 'child' ? adultItems : childItems;

        const sameSideItemIds = new Set<AgeReqItem['id']>(
            sameSideItems.map(sideItem => sideItem.id),
        );

        const oppositeItemIds = new Set<AgeReqItem['id']>(
            oppositeItems.map(oppositeItem => oppositeItem.id),
        );

        if (
            shouldEnforceAllLocationsAgeRules(settings) &&
            !isControllingItem(item.id)
        ) {
            const powderKegSplitAllowed =
                item.id === 'powderKeg' &&
                isPowderKegSplitAllowed(sameSideItemIds, oppositeItemIds);

            const hasConflict = controllingItemsFor(item.id)
                .some(controllingId => oppositeItemIds.has(controllingId));

            if (hasConflict && !powderKegSplitAllowed) {
                return;
            }
        }

        if (
            wouldBreakAllLocationsRule(
                settings,
                poolItems,
                childItems,
                adultItems,
                item,
                side,
            )
        ) {
            return;
        }

        const randomValues = getMmAgeReqRandomItemIds(settings)
            .filter(id => id !== item.id);

        const patch: Record<string, any> = {
            [itemSetting(item, side)]: true,
            [oppositeSetting(item, side)]: false,
            mmAgeReqRandom: {
                type: 'specific',
                values: randomValues,
            },
        };

        if (shouldEnforceAllLocationsAgeRules(settings)) {
            for (const dependentItemId of conflictingDependentItems(item.id)) {
                const dependentItem = poolItems.find(x => x.id === dependentItemId);

                if (dependentItem) {
                    patch[itemSetting(dependentItem, side === 'child' ? 'adult' : 'child')] = false;
                }
            }
        }

        patchSettings(patch as any);
    }, [patchSettings, poolItems, childItems, adultItems, settings]);

    const addRandomItem = useCallback((itemId: string | null) => {
        if (!itemId) {
            return;
        }

        const item = poolItems.find(x => x.id === itemId);

        if (!item) {
            return;
        }

        if (isUnsafeRandomItem(settings, item)) {
            return;
        }

        const values = new Set(getMmAgeReqRandomItemIds(settings));
        values.add(item.id);

        patchSettings({
            [item.childSetting]: false,
            [item.adultSetting]: false,
            mmAgeReqRandom: {
                type: 'specific',
                values: [...values],
            },
        } as any);
    }, [patchSettings, poolItems, settings]);

    const removeItem = useCallback((side: AgeSide, item: AgeReqItem) => {
        patchSettings({
            [itemSetting(item, side)]: false,
        } as any);
    }, [patchSettings]);

    const removeRandomItem = useCallback((item: AgeReqItem) => {
        const values = getMmAgeReqRandomItemIds(settings)
            .filter(id => id !== item.id);

        patchSettings({
            [item.childSetting]: false,
            [item.adultSetting]: false,
            mmAgeReqRandom: {
                type: 'specific',
                values,
            },
        } as any);
    }, [patchSettings, settings]);

    const removeAll = useCallback(() => {
        const patch: Record<string, any> = {};

        for (const item of MM_AGE_REQ_ITEMS) {
            patch[item.childSetting] = false;
            patch[item.adultSetting] = false;
        }

        patch.mmAgeReqRandom = {
            type: 'specific',
            values: [],
        };

        patchSettings(patch as any);
    }, [patchSettings]);

    return (
        <main className="h-full flex flex-col">
            <nav className="flex justify-end items-center gap-2">
                <Tooltip>
                    <div className="space-y-2">
                        <div>
                            These settings let you choose which items are restricted to a specific age.
                        </div>

                        <div>
                            Mutually Exclusive Items:
                        </div>

                        <ul className="list-disc pl-5 space-y-1">
                            <li>Deku Mask | Swamp Title Deed</li>
                            <li>Goron Mask | Mountain Title Deed</li>
                            <li>Zora Mask | Ocean Title Deed</li>
                            <li>Goron Mask / Strength | Powder Keg</li>
                        </ul>

                        <div className="text-m">
                            These items must be usable by the same age. Powder Keg
                            may be assigned to the opposite age when Goron Mask and Strength
                            are restricted to opposite ages.
                        </div>

                        <div>
                            All Locations Requirements:
                        </div>

                        <ul className="list-disc pl-5 space-y-1">
                            <li>Magic Bean + Hookshot</li>
                            <li>Great Fairy Mask + Lens + Bow or Hookshot</li>
                            <li>Deku Mask + Goron Mask + Zora Mask</li>
                            <li>Transformation Masks + Bow + Elemental Arrows + Hookshots</li>
                        </ul>

                        <div className="text-m">
                            These additional restrictions only apply when Logic is set
                            to All Locations and Adult Mask is disabled.
                        </div>
                    </div>
                </Tooltip>

                <Button variant="danger" onClick={removeAll}>
                    Remove All
                </Button>
            </nav>

            <div className="grid grid-cols-3 gap-4 min-h-0 flex-1 mt-4">
                <AgeRequirementTable
                    title="Child"
                    items={childItems}
                    options={childItemOptions}
                    onSelect={item => addItem('child', item)}
                    onRemove={item => removeItem('child', item)}
                />

                <AgeRequirementTable
                    title="Random"
                    items={randomItems}
                    options={randomItemOptions}
                    onSelect={addRandomItem}
                    onRemove={removeRandomItem}
                />

                <AgeRequirementTable
                    title="Adult"
                    items={adultItems}
                    options={adultItemOptions}
                    onSelect={item => addItem('adult', item)}
                    onRemove={item => removeItem('adult', item)}
                />
            </div>
        </main>
    );
}

type AgeRequirementTableProps = {
    title: string;
    items: readonly AgeReqItem[];
    options: Array<{
        value: string;
        label: string;
        disabled?: boolean;
    }>;
    onSelect: (item: string | null) => void;
    onRemove: (item: AgeReqItem) => void;
};

function AgeRequirementTable({
                                 title,
                                 items,
                                 options,
                                 onSelect,
                                 onRemove,
                             }: AgeRequirementTableProps) {
    return (
        <Card className="min-h-0 flex flex-col gap-3">
            <h2 className="text-xl font-semibold text-center">{title}</h2>

            <Select
                searcheable
                placeholder="Add item"
                options={options}
                value={null}
                onSelect={onSelect}
            />

            <div className="min-h-0 overflow-y-auto flex flex-col gap-1">
                {items.length === 0 && (
                    <div className="flex items-center justify-center h-full">
                        <span className="text-gray-500 text-2xl text-center">
                            No Age Requirements
                        </span>
                    </div>
                )}

                {items
                    .slice()
                    .sort((a, b) => a.label.localeCompare(b.label))
                    .map(item => (
                        <div key={item.id} className="flex items-center gap-1">
                            <span
                                className="hover:text-gray-500 cursor-pointer"
                                onClick={() => onRemove(item)}
                            >
                                <FaXmark />
                            </span>

                            <span>{item.label}</span>
                        </div>
                    ))}
            </div>
        </Card>
    );
}