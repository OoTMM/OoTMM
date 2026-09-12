import { useCallback, useEffect, useMemo } from 'react';
import { FaXmark } from 'react-icons/fa6';
import { MM_AGE_REQ_ITEMS, conflictingDependentItems, controllingItemsFor, getMmAgeReqRandomItemIds, isControllingItem, isMmAgeReqItemInPool, isPowderKegSplitAllowed } from '@ootmm/core';

import { Select, Button, Card, Tooltip } from './ui';
import { useStore } from '../store';

type AgeSide = 'child' | 'adult';
type AgeReqItem = typeof MM_AGE_REQ_ITEMS[number];

function itemSetting(item: AgeReqItem, side: AgeSide) {
    return side === 'child' ? item.childSetting : item.adultSetting;
}

function oppositeSetting(item: AgeReqItem, side: AgeSide) {
    return side === 'child' ? item.adultSetting : item.childSetting;
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
        .slice()
        .sort((a, b) => a.label.localeCompare(b.label))
        .map(item => {
            let disabled = false;

            if (!isControllingItem(item.id)) {
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
            poolItems,
            usedItems,
            childItems,
            adultItems,
        );
    }, [poolItems, usedItems, childItems, adultItems]);

    const randomItemOptions = useMemo(() => {
        return poolItems
            .filter(item => !usedItems.has(item.id))
            .slice()
            .sort((a, b) => a.label.localeCompare(b.label))
            .map(item => ({
                value: item.id,
                label: item.label,
            }));
    }, [poolItems, usedItems]);

    const adultItemOptions = useMemo(() => {
        return buildItemOptions(
            poolItems,
            usedItems,
            adultItems,
            childItems,
        );
    }, [poolItems, usedItems, childItems, adultItems]);

    useEffect(() => {
        const patch: Record<string, any> = {};
        const poolItemIds = new Set<AgeReqItem['id']>(poolItems.map(item => item.id) );

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
        const nextRandomIds = currentRandomIds.filter(id => poolItemIds.has(id));

        if (nextRandomIds.length !== currentRandomIds.length) {
            patch.mmAgeReqRandom = {
                type: 'specific',
                values: nextRandomIds,
            };
        }

        if (Object.keys(patch).length > 0) {
            patchSettings(patch as any);
        }
    }, [settings, poolItems, patchSettings]);

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
        for (const dependentItemId of conflictingDependentItems(item.id)) {
            const dependentItem = poolItems.find(x => x.id === dependentItemId);

            if (dependentItem) {
                patch[itemSetting(dependentItem, side === 'child' ? 'adult' : 'child')] = false;
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