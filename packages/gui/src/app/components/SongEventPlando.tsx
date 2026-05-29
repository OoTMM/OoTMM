import { useCallback, useEffect, useMemo, useState } from 'react';
import { FaXmark } from 'react-icons/fa6';
import { Button, Card, MultiSelect, Select } from './ui';
import { useStore } from '../store';

const SONG_EVENT_LOCATIONS_OOT = [
    'SONG_EVENT_TEMPLE_OF_TIME',
    'SONG_EVENT_WINDMILL',
    'SONG_EVENT_GRAVEYARD',
    'SONG_EVENT_ZORA_RIVER',
    'SONG_EVENT_GORON_CITY',
    'SONG_EVENT_GREAT_FAIRY_SPELL_WIND',
    'SONG_EVENT_GREAT_FAIRY_SPELL_FIRE',
    'SONG_EVENT_GREAT_FAIRY_SPELL_LOVE',
    'SONG_EVENT_GREAT_FAIRY_UPGRADE_MAGIC',
    'SONG_EVENT_GREAT_FAIRY_UPGRADE_MAGIC2',
    'SONG_EVENT_GREAT_FAIRY_UPGRADE_DEFENSE',
    'SONG_EVENT_TEMPLE_WATER',
    'SONG_EVENT_TEMPLE_SHADOW',
    'SONG_EVENT_TEMPLE_SPIRIT_STATUE',
    'SONG_EVENT_TEMPLE_SPIRIT_LOWER',
    'SONG_EVENT_TEMPLE_SPIRIT_HIGHER',
    'SONG_EVENT_TEMPLE_BOTW',
    'SONG_EVENT_TEMPLE_GANON',
] as const;

const SONG_EVENT_LOCATIONS_MM = [
    'SONG_EVENT_TEMPLE_WOODFALL',
    'SONG_EVENT_TEMPLE_SNOWHEAD',
    'SONG_EVENT_TEMPLE_GREATBAY',
    // commented out because its not currently in use in the pool
    //'SONG_EVENT_HEALING_POEHUT',
    'SONG_EVENT_HEALING_DARMANI',
    'SONG_EVENT_HEALING_PAMELA_FATHER',
    'SONG_EVENT_HEALING_KAMARO',
    'SONG_EVENT_HEALING_MIKAU',
    'SONG_EVENT_AWAKENING_KEETA',
    // commented out because its not currently use in the pool
    //'SONG_EVENT_AWAKENING_SCRUB',
    'SONG_EVENT_LULLABY_KID',
    'SONG_EVENT_STORMS_COMPOSER',
    'SONG_EVENT_CLOCK_TOWER_ROOF',
] as const;

const SONG_EVENT_SONGS = [
    'ZELDAS_LULLABY',
    'EPONAS',
    'SARIAS',
    'STORMS',
    'SUNS',
    'TIME',
    'MINUET',
    'BOLERO',
    'SERENADE',
    'REQUIEM',
    'NOCTURNE',
    'PRELUDE',
    'HEALING',
    'SOARING',
    'SONATA',
    'GORON_LULLABY',
    'GORON_LULLABY_INTRO',
    'NEW_WAVE',
    'ELEGY',
    'OATH',
] as const;

const RANDOM_SONG_EVENT_SONG = 'RANDOM' as const;
const SONG_EVENT_GROUPS_STORAGE_KEY = 'song-event-plando-groups';

type Game = 'oot' | 'mm';
type Settings = ReturnType<typeof useStore.getState>['settings'];
type SongEventSongName = typeof SONG_EVENT_SONGS[number];
type PlandoSongEventSong = number | 'random';
type SongEventLocationOot = typeof SONG_EVENT_LOCATIONS_OOT[number];
type SongEventLocationMm = typeof SONG_EVENT_LOCATIONS_MM[number];

type PlandoSongEventValue = {
    song: PlandoSongEventSong;
    group?: string;
};

type SongEventOption = {
    key: string;
    value: string;
    label: string;
    game: Game;
};

type SongEventGroup = {
    id: string;
    label: string;
    song: string;
    events: Array<{
        game: Game;
        event: string;
    }>;
};

type SongEventGroups = Record<string, SongEventGroup | null>;

const SONG_EVENT_SONG_NAMES: Record<SongEventSongName, string> = {
    ZELDAS_LULLABY: "Zelda's Lullaby",
    EPONAS: "Epona's Song",
    SARIAS: "Saria's Song",
    STORMS: "Song of Storms",
    SUNS: "Sun's Song",
    TIME: "Song of Time",
    MINUET: "Minuet of Forest",
    BOLERO: "Bolero of Fire",
    SERENADE: "Serenade of Water",
    REQUIEM: "Requiem of Spirit",
    NOCTURNE: "Nocturne of Shadow",
    PRELUDE: "Prelude of Light",
    HEALING: "Song of Healing",
    SOARING: "Song of Soaring",
    SONATA: "Sonata of Awakening",
    GORON_LULLABY: "Goron Lullaby",
    GORON_LULLABY_INTRO: "Goron Lullaby Intro",
    NEW_WAVE: "New Wave Bossa Nova",
    ELEGY: "Elegy of Emptiness",
    OATH: "Oath to Order",
};

const SONG_EVENT_LOCATION_NAMES: Record<string, string> = {
    SONG_EVENT_TEMPLE_OF_TIME: 'Temple of Time',
    SONG_EVENT_WINDMILL: 'Windmill',
    SONG_EVENT_GRAVEYARD: 'Graveyard',
    SONG_EVENT_ZORA_RIVER: 'Zora River',
    SONG_EVENT_GORON_CITY: 'Goron City',
    SONG_EVENT_GREAT_FAIRY_SPELL_WIND: 'Great Fairy - Farore’s Wind',
    SONG_EVENT_GREAT_FAIRY_SPELL_FIRE: 'Great Fairy - Din’s Fire',
    SONG_EVENT_GREAT_FAIRY_SPELL_LOVE: 'Great Fairy - Nayru’s Love',
    SONG_EVENT_GREAT_FAIRY_UPGRADE_MAGIC: 'Great Fairy - Magic Upgrade',
    SONG_EVENT_GREAT_FAIRY_UPGRADE_MAGIC2: 'Great Fairy - Double Magic',
    SONG_EVENT_GREAT_FAIRY_UPGRADE_DEFENSE: 'Great Fairy - Defense Upgrade',
    SONG_EVENT_TEMPLE_WATER: 'Water Temple',
    SONG_EVENT_TEMPLE_SHADOW: 'Shadow Temple',
    SONG_EVENT_TEMPLE_SPIRIT_STATUE: 'Spirit Temple Statue',
    SONG_EVENT_TEMPLE_SPIRIT_LOWER: 'Spirit Temple Lower',
    SONG_EVENT_TEMPLE_SPIRIT_HIGHER: 'Spirit Temple Higher',
    SONG_EVENT_TEMPLE_BOTW: 'Bottom of the Well',
    SONG_EVENT_TEMPLE_GANON: 'Ganon’s Castle',

    SONG_EVENT_TEMPLE_WOODFALL: 'Woodfall Temple',
    SONG_EVENT_TEMPLE_SNOWHEAD: 'Snowhead Temple',
    SONG_EVENT_TEMPLE_GREATBAY: 'Great Bay Temple',
    // commented out because its not currently use in the pool
    //SONG_EVENT_HEALING_POEHUT: 'Song of Healing - Poe Hut',
    SONG_EVENT_HEALING_DARMANI: 'Song of Healing - Darmani',
    SONG_EVENT_HEALING_PAMELA_FATHER: 'Song of Healing - Pamela’s Father',
    SONG_EVENT_HEALING_KAMARO: 'Song of Healing - Kamaro',
    SONG_EVENT_HEALING_MIKAU: 'Song of Healing - Mikau',
    SONG_EVENT_AWAKENING_KEETA: 'Sonata of Awakening - Keeta',
    // commented out because its not currently use in the pool
    //SONG_EVENT_AWAKENING_SCRUB: 'Sonata of Awakening - Deku Scrub',
    SONG_EVENT_LULLABY_KID: 'Goron Lullaby - Elder’s Son',
    SONG_EVENT_STORMS_COMPOSER: 'Song of Storms - Composer',
    SONG_EVENT_CLOCK_TOWER_ROOF: 'Clock Tower Roof',
};

function getPlandoSongEvent(
    plando: Settings['plando'],
    game: Game,
    event: string,
): PlandoSongEventValue | undefined {
    if (game === 'oot') {
        return plando.songEvents?.oot?.[event as SongEventLocationOot] as PlandoSongEventValue | undefined;
    }

    return plando.songEvents?.mm?.[event as SongEventLocationMm] as PlandoSongEventValue | undefined;
}

function songEventKey(game: Game, event: string) {
    return `${game}:${event}`;
}

function songEventSongValue(song: SongEventSongName): number {
    return SONG_EVENT_SONGS.indexOf(song);
}

function songEventSongName(song: number) {
    const key = SONG_EVENT_SONGS[song];
    return key ? SONG_EVENT_SONG_NAMES[key] : `Unknown Song ${song}`;
}

function songEventLabel(event: string) {
    return SONG_EVENT_LOCATION_NAMES[event] || event
        .replace(/^SONG_EVENT_/, '')
        .replace(/_/g, ' ')
        .toLowerCase()
        .replace(/\b\w/g, c => c.toUpperCase());
}

function unique<T>(items: T[]): T[] {
    return Array.from(new Set(items));
}

function intersectSongPools(pools: number[][]): number[] {
    if (pools.length === 0) {
        return [];
    }

    return pools.reduce((shared, pool) => {
        const poolSet = new Set(pool);
        return shared.filter(song => poolSet.has(song));
    });
}

function getEnabledSongEventGames(settings: Settings): Game[] {
    return [
        ...(settings.songEventsShuffleOot ? ['oot' as const] : []),
        ...(settings.songEventsShuffleMm ? ['mm' as const] : []),
    ];
}

function getAllowedSongPoolForGame(settings: Settings, game: Game): number[] {
    if (game === 'oot') {
        return unique([
            songEventSongValue('ZELDAS_LULLABY'),
            songEventSongValue('EPONAS'),
            songEventSongValue('SARIAS'),
            songEventSongValue('SUNS'),
            songEventSongValue('TIME'),
            songEventSongValue('STORMS'),
            songEventSongValue('MINUET'),
            songEventSongValue('BOLERO'),
            songEventSongValue('SERENADE'),
            songEventSongValue('REQUIEM'),
            songEventSongValue('NOCTURNE'),
            songEventSongValue('PRELUDE'),
            ...(settings.songHealingOot ? [songEventSongValue('HEALING')] : []),
            ...(settings.songSoaringOot ? [songEventSongValue('SOARING')] : []),
            ...(settings.songAwakeningOot ? [songEventSongValue('SONATA')] : []),
            ...(settings.songGoronOot ? [songEventSongValue('GORON_LULLABY')] : []),
            ...(
                settings.songGoronOot &&
                settings.progressiveGoronLullabyOot === 'progressive'
                    ? [songEventSongValue('GORON_LULLABY_INTRO')]
                    : []
            ),
            ...(settings.songZoraOot ? [songEventSongValue('NEW_WAVE')] : []),
            ...(settings.elegyOot ? [songEventSongValue('ELEGY')] : []),
            ...(settings.songOrderOot ? [songEventSongValue('OATH')] : []),
        ]);
    }

    return unique([
        songEventSongValue('TIME'),
        songEventSongValue('HEALING'),
        songEventSongValue('EPONAS'),
        songEventSongValue('SOARING'),
        songEventSongValue('STORMS'),
        songEventSongValue('SONATA'),
        songEventSongValue('GORON_LULLABY'),
        songEventSongValue('NEW_WAVE'),
        // commented out because elegy is currently glitched in MM for song events.
        // songEventSongValue('ELEGY'),
        songEventSongValue('OATH'),
        ...(settings.progressiveGoronLullabyMm === 'progressive'
            ? [songEventSongValue('GORON_LULLABY_INTRO')]
            : []),
        ...(settings.songZeldaLullabyMm ? [songEventSongValue('ZELDAS_LULLABY')] : []),
        ...(settings.songSariasMm ? [songEventSongValue('SARIAS')] : []),
        ...(settings.songSunMm ? [songEventSongValue('SUNS')] : []),
        ...(settings.songMinuetMm ? [songEventSongValue('MINUET')] : []),
        ...(settings.songBoleroMm ? [songEventSongValue('BOLERO')] : []),
        ...(settings.songSerenadeMm ? [songEventSongValue('SERENADE')] : []),
        ...(settings.songRequiemMm ? [songEventSongValue('REQUIEM')] : []),
        ...(settings.songNocturneMm ? [songEventSongValue('NOCTURNE')] : []),
        ...(settings.songPreludeMm ? [songEventSongValue('PRELUDE')] : []),
    ]);
}

function getAllowedSongPoolForGames(
    settings: Settings,
    games: Game[],
    mode: 'union' | 'intersection'
): number[] {
    const pools = unique(games).map(game => getAllowedSongPoolForGame(settings, game));

    return mode === 'intersection'
        ? intersectSongPools(pools)
        : unique(pools.flat());
}

function isSongAllowedForGame(settings: Settings, song: number, game: Game) {
    return getAllowedSongPoolForGame(settings, game).includes(song);
}

function nextGroupId(groups: SongEventGroups, extraUsedIds: Iterable<string> = []) {
    const used = new Set(
        Object.keys(groups || {})
            .map(id => Number(id.replace(/^group-/, '')))
            .filter(Number.isFinite)
    );

    for (const id of extraUsedIds) {
        const n = Number(id.replace(/^group-/, ''));
        if (Number.isFinite(n)) {
            used.add(n);
        }
    }

    let next = 1;

    while (used.has(next)) {
        next += 1;
    }

    return `group-${next}`;
}

function groupLabelFromId(id: string) {
    const n = Number(id.replace(/^group-/, ''));
    return Number.isFinite(n) ? `Group ${n}` : id;
}

function loadSongEventGroups(): SongEventGroups {
    if (typeof window === 'undefined') {
        return {};
    }

    try {
        const raw = window.localStorage.getItem(SONG_EVENT_GROUPS_STORAGE_KEY);
        return raw ? JSON.parse(raw) as SongEventGroups : {};
    } catch {
        return {};
    }
}

function saveSongEventGroups(groups: SongEventGroups) {
    if (typeof window === 'undefined') {
        return;
    }

    window.localStorage.setItem(
        SONG_EVENT_GROUPS_STORAGE_KEY,
        JSON.stringify(groups)
    );
}

function formatPlandoSong(song: PlandoSongEventSong) {
    return song === 'random'
        ? 'Random'
        : songEventSongName(song);
}

export function SongEventPlando() {
    const settings = useStore(state => state.settings);
    const plando = useStore(state => state.settings.plando);
    const patchSettings = useStore(state => state.patchSettings);

    const [selectedSong, setSelectedSongRaw] = useState<string | null>(null);
    const [selectedEvents, setSelectedEvents] = useState<string[]>([]);
    const [songEventGroups, setSongEventGroups] = useState<SongEventGroups>(() => loadSongEventGroups());

    useEffect(() => {
        saveSongEventGroups(songEventGroups);
    }, [songEventGroups]);

    useEffect(() => {
        const songEventsPatch: {
            oot?: Record<string, PlandoSongEventValue>;
            mm?: Record<string, PlandoSongEventValue>;
        } = {};

        let changed = false;

        for (const group of Object.values(songEventGroups || {})) {
            if (!group) {
                continue;
            }

            for (const event of group.events) {
                const data = getPlandoSongEvent(plando, event.game, event.event);

                if (!data) {
                    continue;
                }

                if (data.group === group.id) {
                    continue;
                }

                songEventsPatch[event.game] ||= {};
                songEventsPatch[event.game]![event.event] = {
                    ...data,
                    group: group.id,
                };

                changed = true;
            }
        }

        if (!changed) {
            return;
        }

        patchSettings({
            plando: {
                songEvents: songEventsPatch,
            },
        });
    }, [
        patchSettings,
        plando.songEvents,
        songEventGroups,
    ]);

    const plandoGroupIds = useMemo(() => {
        const ids = new Set<string>();

        for (const data of Object.values(plando.songEvents?.oot || {}) as PlandoSongEventValue[]) {
            if (data?.group) {
                ids.add(data.group);
            }
        }

        for (const data of Object.values(plando.songEvents?.mm || {}) as PlandoSongEventValue[]) {
            if (data?.group) {
                ids.add(data.group);
            }
        }

        return ids;
    }, [plando.songEvents]);

    const baseEventOptions = useMemo(() => {
        const options: SongEventOption[] = [];
        const usedOotEvents = new Set(Object.keys(plando.songEvents?.oot || {}));
        const usedMmEvents = new Set(Object.keys(plando.songEvents?.mm || {}));

        if (settings.songEventsShuffleOot) {
            options.push(...SONG_EVENT_LOCATIONS_OOT
                .filter(event => !usedOotEvents.has(event))
                .map(event => ({
                    key: songEventKey('oot', event),
                    value: event,
                    label: `OoT - ${songEventLabel(event)}`,
                    game: 'oot' as const,
                })));
        }

        if (settings.songEventsShuffleMm) {
            options.push(...SONG_EVENT_LOCATIONS_MM
                .filter(event => !usedMmEvents.has(event))
                .map(event => ({
                    key: songEventKey('mm', event),
                    value: event,
                    label: `MM - ${songEventLabel(event)}`,
                    game: 'mm' as const,
                })));
        }

        return options;
    }, [
        plando.songEvents,
        settings.songEventsShuffleOot,
        settings.songEventsShuffleMm,
    ]);

    const selectedEventOptions = useMemo(() => {
        return selectedEvents
            .map(key => baseEventOptions.find(option => option.key === key))
            .filter((option): option is SongEventOption => !!option);
    }, [baseEventOptions, selectedEvents]);

    const selectedEventGames = useMemo(() => {
        return unique(selectedEventOptions.map(event => event.game));
    }, [selectedEventOptions]);

    const songOptions = useMemo(() => {
        const games = selectedEventGames.length > 0
            ? selectedEventGames
            : getEnabledSongEventGames(settings);

        const allowedSongs = getAllowedSongPoolForGames(
            settings,
            games,
            selectedEventGames.length > 0 ? 'intersection' : 'union'
        );

        const allowedSongSet = new Set(allowedSongs);

        return [
            {
                value: RANDOM_SONG_EVENT_SONG,
                label: 'Random',
            },
            ...SONG_EVENT_SONGS
                .map(song => songEventSongValue(song))
                .filter(song => allowedSongSet.has(song))
                .map(song => ({
                    value: song.toString(),
                    label: songEventSongName(song),
                })),
        ];
    }, [
        settings,
        selectedEventGames,
    ]);

    const eventOptions = useMemo(() => {
        return baseEventOptions.filter(option => {
            if (!selectedSong) {
                return true;
            }

            if (selectedSong !== RANDOM_SONG_EVENT_SONG) {
                return isSongAllowedForGame(settings, Number(selectedSong), option.game);
            }

            const games = unique([
                ...selectedEventOptions.map(event => event.game),
                option.game,
            ]);

            return getAllowedSongPoolForGames(settings, games, 'intersection').length > 0;
        });
    }, [
        baseEventOptions,
        selectedSong,
        selectedEventOptions,
        settings,
    ]);

    useEffect(() => {
        if (!selectedSong || selectedSong === RANDOM_SONG_EVENT_SONG) {
            return;
        }

        if (!songOptions.some(option => option.value === selectedSong)) {
            setSelectedSongRaw(null);
        }
    }, [selectedSong, songOptions]);

    useEffect(() => {
        setSelectedEvents(current => {
            const validKeys = new Set(eventOptions.map(option => option.key));
            const next = current.filter(key => validKeys.has(key));

            return next.length === current.length
                ? current
                : next;
        });
    }, [eventOptions]);

    const setSelectedSong = (song: string | null) => {
        if (song) {
            setSelectedSongRaw(song);
        }
    };

    const placeSongEvent = useCallback(() => {
        if (!selectedSong || selectedEventOptions.length === 0) {
            return;
        }

        const selectedGames = unique(selectedEventOptions.map(event => event.game));
        const allowedSongs = getAllowedSongPoolForGames(settings, selectedGames, 'intersection');

        const song: PlandoSongEventSong =
            selectedSong === RANDOM_SONG_EVENT_SONG
                ? 'random'
                : Number(selectedSong);

        if (song === 'random') {
            if (allowedSongs.length === 0) {
                return;
            }
        } else if (
            !Number.isInteger(song) ||
            !allowedSongs.includes(song)
        ) {
            return;
        }

        const groupId = selectedEventOptions.length > 1
            ? nextGroupId(songEventGroups, plandoGroupIds)
            : undefined;

        const nextSongEvents: {
            oot?: Record<string, PlandoSongEventValue>;
            mm?: Record<string, PlandoSongEventValue>;
        } = {};

        for (const event of selectedEventOptions) {
            nextSongEvents[event.game] ||= {};
            nextSongEvents[event.game]![event.value] = {
                song,
                ...(groupId ? { group: groupId } : {}),
            };
        }

        patchSettings({
            plando: {
                songEvents: nextSongEvents,
            },
        });

        if (groupId) {
            setSongEventGroups(current => ({
                ...current,
                [groupId]: {
                    id: groupId,
                    label: groupLabelFromId(groupId),
                    song: selectedSong,
                    events: selectedEventOptions.map(event => ({
                        game: event.game,
                        event: event.value,
                    })),
                },
            }));
        }

        setSelectedSongRaw(null);
        setSelectedEvents([]);
    }, [
        patchSettings,
        plandoGroupIds,
        selectedSong,
        selectedEventOptions,
        settings,
        songEventGroups,
    ]);

    const removeSongEvent = useCallback((game: Game, event: string) => {
        patchSettings({
            plando: {
                songEvents: {
                    [game]: {
                        [event]: null,
                    },
                },
            },
        });

        setSelectedEvents(current =>
            current.filter(key => key !== songEventKey(game, event))
        );
    }, [patchSettings]);

    const removeGroup = useCallback((group: SongEventGroup) => {
        const songEventsPatch: {
            oot?: Record<string, null>;
            mm?: Record<string, null>;
        } = {};

        for (const event of group.events) {
            songEventsPatch[event.game] ||= {};
            songEventsPatch[event.game]![event.event] = null;
        }

        patchSettings({
            plando: {
                songEvents: songEventsPatch,
            },
        });

        setSongEventGroups(current => {
            const next = { ...current };
            delete next[group.id];
            return next;
        });

        setSelectedEvents(current =>
            current.filter(selected =>
                !group.events.some(grouped =>
                    songEventKey(grouped.game, grouped.event) === selected
                )
            )
        );
    }, [patchSettings]);

    const removeAll = useCallback(() => {
        patchSettings({
            plando: {
                songEvents: {
                    ...(settings.songEventsShuffleOot ? { oot: null } : {}),
                    ...(settings.songEventsShuffleMm ? { mm: null } : {}),
                },
            },
        });

        setSelectedEvents([]);
        setSongEventGroups({});
    }, [
        patchSettings,
        settings.songEventsShuffleOot,
        settings.songEventsShuffleMm,
    ]);

    const groupedEventKeys = useMemo(() => {
        const keys = new Set<string>();

        for (const group of Object.values(songEventGroups || {})) {
            if (!group) {
                continue;
            }

            for (const event of group.events) {
                keys.add(songEventKey(event.game, event.event));
            }
        }

        for (const [event, data] of Object.entries(plando.songEvents?.oot || {}) as Array<[string, PlandoSongEventValue]>) {
            if (data?.group) {
                keys.add(songEventKey('oot', event));
            }
        }

        for (const [event, data] of Object.entries(plando.songEvents?.mm || {}) as Array<[string, PlandoSongEventValue]>) {
            if (data?.group) {
                keys.add(songEventKey('mm', event));
            }
        }

        return keys;
    }, [
        plando.songEvents,
        songEventGroups,
    ]);

    const groups = useMemo(() => {
        return Object.values(songEventGroups || {})
            .filter((group): group is SongEventGroup => !!group)
            .sort((a, b) => a.label.localeCompare(b.label, undefined, { numeric: true }));
    }, [songEventGroups]);

    const entries = useMemo(() => {
        const next: Array<{
            game: Game;
            event: string;
            song: PlandoSongEventSong;
        }> = [];

        if (settings.songEventsShuffleOot) {
            next.push(...Object.entries(plando.songEvents?.oot || {})
                .filter((x): x is [string, PlandoSongEventValue] => !!x[1])
                .filter(([event]) => !groupedEventKeys.has(songEventKey('oot', event)))
                .map(([event, data]) => ({
                    game: 'oot' as const,
                    event,
                    song: data.song,
                })));
        }

        if (settings.songEventsShuffleMm) {
            next.push(...Object.entries(plando.songEvents?.mm || {})
                .filter((x): x is [string, PlandoSongEventValue] => !!x[1])
                .filter(([event]) => !groupedEventKeys.has(songEventKey('mm', event)))
                .map(([event, data]) => ({
                    game: 'mm' as const,
                    event,
                    song: data.song,
                })));
        }

        return next.sort((a, b) => {
            const ga = a.game.localeCompare(b.game);
            return ga !== 0 ? ga : a.event.localeCompare(b.event);
        });
    }, [
        groupedEventKeys,
        plando.songEvents,
        settings.songEventsShuffleOot,
        settings.songEventsShuffleMm,
    ]);

    const hasItems = groups.length > 0 || entries.length > 0;

    return (
        <main className="h-full min-h-0 flex flex-col">
            <nav className="flex gap-2">
                <div className="flex-1">
                    <Select
                        searcheable
                        placeholder="Song"
                        options={songOptions}
                        onSelect={setSelectedSong}
                        value={selectedSong}
                    />
                </div>

                <div className="flex-1">
                    <MultiSelect
                        searcheable
                        placeholder="Song Event"
                        options={eventOptions.map(option => ({
                            value: option.key,
                            label: option.label,
                        }))}
                        value={selectedEvents}
                        onSelect={event => {
                            setSelectedEvents(current =>
                                current.includes(event) ? current : [...current, event]
                            );
                        }}
                        onUnselect={event => {
                            setSelectedEvents(current =>
                                current.filter(x => x !== event)
                            );
                        }}
                        onClear={() => setSelectedEvents([])}
                    />
                </div>

                <Button onClick={placeSongEvent}>Add</Button>
                <Button variant="danger" onClick={removeAll}>Remove All</Button>
            </nav>

            <Card className="flex-1 min-h-0 overflow-y-auto mt-4 gap-0">
                {!hasItems && (
                    <div className="flex items-center justify-center h-full">
                        <span className="text-gray-500 text-3xl">
                            No Song Event Items
                        </span>
                    </div>
                )}

                {groups.map(group => (
                    <div
                        key={group.id}
                        className="shrink-0 mb-3 rounded border dark:border-gray-600 overflow-hidden"
                    >
                        <div className="ux-bg ux-border-b flex items-center gap-2 p-2 font-semibold">
                            <span
                                className="hover:text-gray-500 cursor-pointer"
                                onClick={() => removeGroup(group)}
                            >
                                <FaXmark />
                            </span>

                            <span>{group.label}</span>

                            <span className="text-gray-500 font-normal">
                                {group.song === RANDOM_SONG_EVENT_SONG
                                    ? 'Random'
                                    : songEventSongName(Number(group.song))}
                            </span>
                        </div>

                        <div className="flex flex-col">
                            {group.events.map(({ game, event }) => (
                                <div
                                    key={`${group.id}:${game}:${event}`}
                                    className="flex items-center gap-2 px-8 py-1.5 text-sm border-t dark:border-gray-700 first:border-t-0"
                                >
                                    <span>
                                        {game.toUpperCase()} - {songEventLabel(event)}
                                    </span>
                                </div>
                            ))}
                        </div>
                    </div>
                ))}

                {entries.map(({ game, event, song }) => (
                    <div key={`${game}:${event}`} className="shrink-0 flex items-center gap-1">
                        <span
                            className="hover:text-gray-500 cursor-pointer"
                            onClick={() => removeSongEvent(game, event)}
                        >
                            <FaXmark />
                        </span>

                        <span>
                            {formatPlandoSong(song)}: {game.toUpperCase()} - {songEventLabel(event)}
                        </span>
                    </div>
                ))}
            </Card>
        </main>
    );
}