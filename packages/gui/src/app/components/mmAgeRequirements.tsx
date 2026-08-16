import { useCallback, useEffect, useMemo } from 'react';
import { FaXmark } from 'react-icons/fa6';
import { MM_AGE_REQ_ITEMS } from '@ootmm/core';

import { Select, Button, Card } from './ui';
import { useStore } from '../store';

type AgeSide = 'child' | 'adult';
type Settings = ReturnType<typeof useStore.getState>['settings'];
type AgeReqItem = typeof MM_AGE_REQ_ITEMS[number];

function itemSetting(item: AgeReqItem, side: AgeSide) {
    return side === 'child' ? item.childSetting : item.adultSetting;
}

function oppositeSetting(item: AgeReqItem, side: AgeSide) {
    return side === 'child' ? item.adultSetting : item.childSetting;
}

const MM_AGE_REQ_ITEM_POOL_SETTINGS: Partial<Record<AgeReqItem['id'], keyof Settings>> = {
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
};

function isMmAgeReqItemInPool(settings: Settings, item: AgeReqItem) {
    const poolSetting = MM_AGE_REQ_ITEM_POOL_SETTINGS[item.id];

    if (!poolSetting) {
        return true;
    }

    return Boolean(settings[poolSetting]);
}

const MM_AGE_REQ_RESTRICTION_DEPENDENCIES: Partial<
    Record<AgeReqItem['id'], readonly AgeReqItem['id'][]>
> = {
    maskDeku: ['deedSwamp'],
    maskGoron: ['powderKeg', 'deedMountain'],
    maskZora: ['deedOcean'],
    strength: ['powderKeg'],
};

function conflictingDependentItems(
    controllingItemId: AgeReqItem['id'],
): readonly AgeReqItem['id'][] {
    return MM_AGE_REQ_RESTRICTION_DEPENDENCIES[controllingItemId] ?? [];
}

function controllingItemsFor(
    dependentItemId: AgeReqItem['id'],
): AgeReqItem['id'][] {
    return Object.entries(MM_AGE_REQ_RESTRICTION_DEPENDENCIES)
        .filter(([, dependentIds]) => dependentIds?.includes(dependentItemId))
        .map(([controllingItemId]) => controllingItemId as AgeReqItem['id']);
}

function isControllingItem(itemId: AgeReqItem['id']) {
    return MM_AGE_REQ_RESTRICTION_DEPENDENCIES[itemId] !== undefined;
}

function isPowderKegSplitAllowed(
    sameSideItemIds: ReadonlySet<AgeReqItem['id']>,
    oppositeItemIds: ReadonlySet<AgeReqItem['id']>,
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

function buildItemOptions(
    poolItems: readonly AgeReqItem[],
    usedItems: ReadonlySet<AgeReqItem['id']>,
    sameSideItems: readonly AgeReqItem[],
    oppositeItems: readonly AgeReqItem[],
) {
    const sameSideItemIds = new Set<AgeReqItem['id']>(
        sameSideItems.map(item => item.id),
    );

    const oppositeItemIds = new Set<AgeReqItem['id']>(
        oppositeItems.map(item => item.id),
    );

    return poolItems
        .filter(item => !usedItems.has(item.id))
        .filter(item => {
            if (isControllingItem(item.id)) {
                return true;
            }
            if (
                item.id === 'powderKeg' &&
                isPowderKegSplitAllowed(sameSideItemIds, oppositeItemIds)
            ) {
                return true;
            }

            return controllingItemsFor(item.id)
                .every(controllingId => !oppositeItemIds.has(controllingId));
        })
        .slice()
        .sort((a, b) => a.label.localeCompare(b.label))
        .map(item => ({
            value: item.id,
            label: item.label,
        }));
}

export function MmAgeRequirements() {
    const settings = useStore(state => state.settings);
    const patchSettings = useStore(state => state.patchSettings);

    const poolItems = useMemo(() => {
        return MM_AGE_REQ_ITEMS.filter(item => isMmAgeReqItemInPool(settings, item));
    }, [settings]);

    const childItems = useMemo(() => {
        return poolItems.filter(item => Boolean((settings as any)[item.childSetting]));
    }, [poolItems, settings]);

    const adultItems = useMemo(() => {
        return poolItems.filter(item => Boolean((settings as any)[item.adultSetting]));
    }, [poolItems, settings]);

    const usedItems = useMemo(() => {
        return new Set([...childItems, ...adultItems].map(item => item.id));
    }, [childItems, adultItems]);

    const childItemOptions = useMemo(() => {
        return buildItemOptions(
            poolItems,
            usedItems,
            childItems,
            adultItems,
        );
    }, [poolItems, usedItems, childItems, adultItems]);

    const adultItemOptions = useMemo(() => {
        return buildItemOptions(
            poolItems,
            usedItems,
            adultItems,
            childItems,
        );
    }, [poolItems, usedItems, childItems, adultItems]);

    useEffect(() => {
        const patch: Record<string, boolean> = {};

        for (const item of MM_AGE_REQ_ITEMS) {
            if (isMmAgeReqItemInPool(settings, item)) {
                continue;
            }

            if (Boolean((settings as any)[item.childSetting])) {
                patch[item.childSetting] = false;
            }

            if (Boolean((settings as any)[item.adultSetting])) {
                patch[item.adultSetting] = false;
            }
        }

        if (Object.keys(patch).length > 0) {
            patchSettings(patch as any);
        }
    }, [settings, patchSettings]);

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

        if (!isControllingItem(item.id)) {
            const powderKegSplitAllowed =
                item.id === 'powderKeg' &&
                isPowderKegSplitAllowed(sameSideItemIds, oppositeItemIds);

            const hasConflict = controllingItemsFor(item.id)
                .some(controllingId => oppositeItemIds.has(controllingId));

            if (hasConflict && !powderKegSplitAllowed) {
                return;
            }
        }

        const patch: Record<string, boolean> = {
            [itemSetting(item, side)]: true,
            [oppositeSetting(item, side)]: false,
        };
        for (const dependentItemId of conflictingDependentItems(item.id)) {
            const dependentItem = poolItems.find(x => x.id === dependentItemId);

            if (dependentItem) {
                patch[itemSetting(dependentItem, side === 'child' ? 'adult' : 'child')] = false;
            }
        }

        patchSettings(patch as any);
    }, [patchSettings, poolItems, childItems, adultItems]);

    const removeItem = useCallback((side: AgeSide, item: AgeReqItem) => {
        patchSettings({
            [itemSetting(item, side)]: false,
        } as any);
    }, [patchSettings]);

    const removeAll = useCallback(() => {
        const patch: Record<string, boolean> = {};

        for (const item of MM_AGE_REQ_ITEMS) {
            patch[item.childSetting] = false;
            patch[item.adultSetting] = false;
        }

        patchSettings(patch as any);
    }, [patchSettings]);

    return (
        <main className="h-full flex flex-col">
            <nav className="flex justify-end gap-2">
                <Button variant="danger" onClick={removeAll}>Remove All</Button>
            </nav>

            <div className="grid grid-cols-2 gap-4 min-h-0 flex-1 mt-4">
                <AgeRequirementTable
                    title="Child"
                    items={childItems}
                    options={childItemOptions}
                    onSelect={item => addItem('child', item)}
                    onRemove={item => removeItem('child', item)}
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
    options: Array<{ value: string; label: string }>;
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
            <h2 className="text-xl font-semibold">{title}</h2>

            <Select
                searchable
                placeholder="Add item"
                options={options}
                value={null}
                onSelect={onSelect}
            />

            <div className="min-h-0 overflow-y-auto flex flex-col gap-1">
                {items.length === 0 && (
                    <div className="flex items-center justify-center h-full">
                        <span className="text-gray-500 text-2xl">No Age Requirements</span>
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