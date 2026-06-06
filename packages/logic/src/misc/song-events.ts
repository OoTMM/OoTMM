import type { Settings, PlandoSongEvents, SongEventSongs } from '@ootmm/core';
import type { World } from '../world';

import {
  Random,
  sample,
  SONG_EVENT_INDEX_OOT,
  SONG_EVENT_INDEX_MM,
  SONG_EVENT_SONG_IDS,
} from '@ootmm/core';

type LogicPassSongEventsState = {
  settings: Settings,
  worlds: World[],
  random: Random,
};

type Game = 'oot' | 'mm';

type SongPools = Record<Game, SongEventSongs[]>;

type RandomSongEventEntry = {
  game: Game,
  event: string,
  index: number,
  worldEvents: number[],
};

type PlandoSongEventsForGame = PlandoSongEvents[Game];

class LogicPassSongEvents {
  constructor(
      private readonly state: LogicPassSongEventsState,
  ) {
  }

  private songCheckIdsOot(): SongEventSongs[] {
    const songCheckIds: SongEventSongs[] = [
      'ZELDAS_LULLABY',
      'EPONAS',
      'SARIAS',
      'SUNS',
      'TIME',
      'STORMS',
      'MINUET',
      'BOLERO',
      'SERENADE',
      'REQUIEM',
      'NOCTURNE',
      'PRELUDE',
    ];
    if (this.state.settings.songHealingOot) {
      songCheckIds.push('HEALING');
    }
    if (this.state.settings.songSoaringOot) {
      songCheckIds.push('SOARING');
    }
    if (this.state.settings.songAwakeningOot) {
      songCheckIds.push('SONATA');
    }
    if (this.state.settings.songGoronOot) {
      songCheckIds.push('GORON_LULLABY');
    }
    if (this.state.settings.songGoronOot && this.state.settings.progressiveGoronLullabyOot === 'progressive') {
      songCheckIds.push('GORON_LULLABY_INTRO');
    }
    if (this.state.settings.songZoraOot) {
      songCheckIds.push('NEW_WAVE');
    }
    if (this.state.settings.elegyOot) {
      songCheckIds.push('ELEGY');
    }
    if (this.state.settings.songOrderOot) {
      songCheckIds.push('OATH');
    }
    return songCheckIds;
  }

  private songCheckIdsMm(): SongEventSongs[] {
    const songCheckIds: SongEventSongs[] = [
      'TIME',
      'HEALING',
      'EPONAS',
      'SOARING',
      'STORMS',
      'SONATA',
      'GORON_LULLABY',
      'NEW_WAVE',
      // SongEventSongs.ELEGY,
      'OATH',
    ];
    if (this.state.settings.progressiveGoronLullabyMm === 'progressive') {
      songCheckIds.push('GORON_LULLABY_INTRO');
    }
    if (this.state.settings.songZeldaLullabyMm) {
      songCheckIds.push('ZELDAS_LULLABY');
    }
    if (this.state.settings.songSariasMm) {
      songCheckIds.push('SARIAS');
    }
    if (this.state.settings.songSunMm) {
      songCheckIds.push('SUNS');
    }
    if (this.state.settings.songMinuetMm) {
      songCheckIds.push('MINUET');
    }
    if (this.state.settings.songBoleroMm) {
      songCheckIds.push('BOLERO');
    }
    if (this.state.settings.songSerenadeMm) {
      songCheckIds.push('SERENADE');
    }
    if (this.state.settings.songRequiemMm) {
      songCheckIds.push('REQUIEM');
    }
    if (this.state.settings.songNocturneMm) {
      songCheckIds.push('NOCTURNE');
    }
    if (this.state.settings.songPreludeMm) {
      songCheckIds.push('PRELUDE');
    }
    return songCheckIds;
  }

  private shuffleSongEventsOot(world: World, songCheckIds: SongEventSongs[]) {
    for (let i = 0; i < world.songEventsOot.length; ++i) {
      const v = sample(this.state.random, songCheckIds);
      world.songEventsOot[i] = SONG_EVENT_SONG_IDS[v];
    }
  }

  private shuffleSongEventsMm(world: World, songCheckIds: SongEventSongs[]) {
    for (let i = 0; i < world.songEventsMm.length; ++i) {
      const v = sample(this.state.random, songCheckIds);
      world.songEventsMm[i] = SONG_EVENT_SONG_IDS[v];
    }
  }

  private collectPlandoSongEvents(
      randomGroups: Map<string, RandomSongEventEntry[]>,
      worldEvents: number[],
      plando: PlandoSongEventsForGame,
      eventIndex: Record<string, number>,
      allowedSongs: Set<SongEventSongs>,
      game: Game,
  ) {
    for (const [event, data] of Object.entries(plando)) {
      if (!data) {
        continue;
      }

      const index = eventIndex[event];

      if (index === undefined) {
        throw new Error(`Invalid plando song event: ${event}`);
      }

      if (data.song === 'random') {
        const group =
            typeof data.group === 'string' && data.group.length > 0
                ? data.group
                : `${game}:${event}`;

        const entries = randomGroups.get(group) ?? [];
        entries.push({ game, event, index, worldEvents });
        randomGroups.set(group, entries);

        continue;
      }

      if (!allowedSongs.has(data.song)) {
        throw new Error(`Invalid plando song for ${event}: ${data.song}`);
      }

      worldEvents[index] = SONG_EVENT_SONG_IDS[data.song];
    }
  }

  private randomPoolForGroup(
      group: string,
      entries: RandomSongEventEntry[],
      pools: SongPools,
  ) {
    const games = new Set(entries.map(({ game }) => game));

    if (games.has('oot') && games.has('mm')) {
      const mmPool = new Set(pools.mm);
      return pools.oot.filter(song => mmPool.has(song));
    }

    if (games.has('oot')) {
      return pools.oot;
    }

    if (games.has('mm')) {
      return pools.mm;
    }

    throw new Error(`Invalid empty random song event group: ${group}`);
  }

  private applyRandomSongGroups(
      randomGroups: Map<string, RandomSongEventEntry[]>,
      pools: SongPools,
  ) {
    for (const [group, entries] of randomGroups) {
      const pool = this.randomPoolForGroup(group, entries, pools);

      if (pool.length === 0) {
        throw new Error(`No valid songs available for random song event group: ${group}`);
      }

      const resolvedSong = sample(this.state.random, pool);

      for (const { worldEvents, index } of entries) {
        worldEvents[index] = SONG_EVENT_SONG_IDS[resolvedSong];
      }
    }
  }

  private applyPlandoSongEvents(world: World, pools: SongPools) {
    const randomGroups = new Map<string, RandomSongEventEntry[]>();
    const allowedSongs = {
      oot: new Set(pools.oot),
      mm: new Set(pools.mm),
    };

    if (this.state.settings.songEventsShuffleOot) {
      this.collectPlandoSongEvents(
          randomGroups,
          world.songEventsOot,
          this.state.settings.plando.songEvents.oot,
          SONG_EVENT_INDEX_OOT,
          allowedSongs.oot,
          'oot',
      );
    }

    if (this.state.settings.songEventsShuffleMm) {
      this.collectPlandoSongEvents(
          randomGroups,
          world.songEventsMm,
          this.state.settings.plando.songEvents.mm,
          SONG_EVENT_INDEX_MM,
          allowedSongs.mm,
          'mm',
      );
    }

    this.applyRandomSongGroups(randomGroups, pools);
  }

  run() {
    const pools = {
      oot: this.songCheckIdsOot(),
      mm: this.songCheckIdsMm(),
    };

    for (const world of this.state.worlds) {
      if (this.state.settings.songEventsShuffleOot) {
        this.shuffleSongEventsOot(world, pools.oot);
      }

      if (this.state.settings.songEventsShuffleMm) {
        this.shuffleSongEventsMm(world, pools.mm);
      }

      this.applyPlandoSongEvents(world, pools);
    }

    return {};
  }
}

export function logicPassSongEvents(state: LogicPassSongEventsState) {
  return new LogicPassSongEvents(state).run();
}
