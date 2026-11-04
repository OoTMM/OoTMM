import { SETTINGS } from '@ootmm/generator';

type State = Record<string, any>;
type Cond = (settings: State) => boolean;
type Change = { key: string; value: any };

const definitions = new Map<string, any>(
    SETTINGS.map(s => [s.key, s])
);

function inspect(cond: Cond, settings: State) {
    const reads = new Set<string>();

    const tracked = new Proxy(settings, {
        get(target, key, receiver) {
            if (typeof key === 'string') {
                reads.add(key);
            }

            return Reflect.get(target, key, receiver);
        },
    });

    return {
        passes: !!cond(tracked),
        reads,
    };
}

function alternatives(data: any, current: any, settings: State): any[] {
    switch (data.type) {
        case 'boolean':
            return [!current];

        case 'enum':
            return data.values
                .map((v: any) => v.value)
                .filter((v: any) => v !== current);

        case 'set':
            return [
                { type: 'none' },
                { type: 'all' },
                { type: 'random' },
                { type: 'specific', values: [] },
                { type: 'random-mixed', set: [], unset: [] },
            ].filter(v => v.type !== current?.type);

        case 'number': {
            const bound = (v: any) =>
                typeof v === 'function' ? v(settings) : v;

            const min = bound(data.min) ?? -Infinity;
            const max = bound(data.max) ?? Infinity;

            return [...new Set([
                data.default, 0, 1, 2,
                min, max,
                current - 1, current + 1,
            ])].filter(v =>
                typeof v === 'number' &&
                Number.isFinite(v) &&
                v !== current &&
                v >= min &&
                v <= max
            );
        }

        default:
            return [];
    }
}

export function findRequiredChanges(
    cond: Cond,
    settings: State
): Change[] | null {
    type Node = {
        settings: State;
        changes: Change[];
    };

    const queue: Node[] = [
        { settings, changes: [] },
    ];

    const visited = new Set<string>();

    const MAX_CHANGES = 6;
    const MAX_STATES = 5000;

    for (let i = 0; i < queue.length && i < MAX_STATES; i++) {
        const node = queue[i];

        const result = inspect(cond, node.settings);
        const reads = new Set(result.reads);

        let changesAvailable = true;

        for (const change of node.changes) {
            const data = definitions.get(change.key);

            const conditions: Cond[] = [];

            if (data?.cond) {
                conditions.push(data.cond);
            }

            if (data?.type === 'enum') {
                const option = data.values.find(
                    (v: any) => v.value === change.value
                );

                if (option?.cond) {
                    conditions.push(option.cond);
                }
            }

            for (const dependency of conditions) {
                const check = inspect(dependency, node.settings);

                if (!check.passes) {
                    changesAvailable = false;
                    check.reads.forEach(key => reads.add(key));
                }
            }
        }

        if (result.passes && changesAvailable) {
            return node.changes;
        }

        if (node.changes.length >= MAX_CHANGES) {
            continue;
        }
        for (const key of reads) {
            const data = definitions.get(key);

            if (!data) continue;

            if (node.changes.some(c => c.key === key)) {
                continue;
            }

            for (const value of alternatives(
                data,
                node.settings[key],
                node.settings
            )) {
                const changes = [...node.changes, { key, value }];

                const signature = JSON.stringify(
                    [...changes].sort((a, b) =>
                        a.key.localeCompare(b.key)
                    )
                );

                if (visited.has(signature)) continue;
                visited.add(signature);

                queue.push({
                    settings: { ...node.settings, [key]: value },
                    changes,
                });
            }
        }
    }

    return null;
}

function formatValue(key: string, value: any): string {
    const data = definitions.get(key);

    switch (data?.type) {
        case 'boolean':
            return value ? 'Enabled' : 'Disabled';

        case 'enum':
            return data.values.find(
                (v: any) => v.value === value
            )?.name ?? String(value);

        case 'set':
            return {
                none: 'None',
                all: 'All',
                random: 'Random',
                specific: 'Specific',
                'random-mixed': 'Mixed Random',
            }[value?.type as string] ?? String(value?.type);

        default:
            return String(value);
    }
}

export function explainUnavailable(
    cond: Cond,
    settings: State
): string {
    const changes = findRequiredChanges(cond, settings);

    if (!changes?.length) {
        return 'Unavailable with the current settings';
    }

    return 'To enable this setting:\n' +
        changes.map(({ key, value }) => {
            const name = definitions.get(key)?.name ?? key;

            return `${name}: Change ${formatValue(key, settings[key])} to ${formatValue(key, value)}`;
        }).join('\n');
}