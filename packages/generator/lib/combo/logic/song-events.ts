import { Settings, Random, sample } from '@ootmm/core';
import { World } from './world';

export enum SongEventSongs {
  ZELDAS_LULLABY,
  EPONAS,
  SARIAS,
  STORMS,
  SUNS,
  TIME,
  MINUET,
  BOLERO,
  SERENADE,
  REQUIEM,
  NOCTURNE,
  PRELUDE,
  HEALING,
  SOARING,
  SONATA,
  GORON_LULLABY,
  GORON_LULLABY_INTRO,
  NEW_WAVE,
  ELEGY,
  OATH,
}

export class LogicPassSongEvents {
  constructor(
    private state: {
      settings: Settings,
      worlds: World[],
      random: Random,
    }
  ) {
  }

  private shuffleSongEventsOot(world: World) {
    const songCheckIds = [
      SongEventSongs.ZELDAS_LULLABY,
      SongEventSongs.EPONAS,
      SongEventSongs.SARIAS,
      SongEventSongs.SUNS,
      SongEventSongs.TIME,
      SongEventSongs.STORMS,
      SongEventSongs.MINUET,
      SongEventSongs.BOLERO,
      SongEventSongs.SERENADE,
      SongEventSongs.REQUIEM,
      SongEventSongs.NOCTURNE,
      SongEventSongs.PRELUDE,
    ];
    if (this.state.settings.songHealingOot) {
      songCheckIds.push(SongEventSongs.HEALING);
    }
    if (this.state.settings.songSoaringOot) {
      songCheckIds.push(SongEventSongs.SOARING);
    }
    if (this.state.settings.songAwakeningOot) {
      songCheckIds.push(SongEventSongs.SONATA);
    }
    if (this.state.settings.songGoronOot) {
      songCheckIds.push(SongEventSongs.GORON_LULLABY);
    }
    if (this.state.settings.songGoronOot && this.state.settings.progressiveGoronLullabyOot === 'progressive') {
      songCheckIds.push(SongEventSongs.GORON_LULLABY_INTRO);
    }
    if (this.state.settings.songZoraOot) {
      songCheckIds.push(SongEventSongs.NEW_WAVE);
    }
    if (this.state.settings.elegyOot) {
      songCheckIds.push(SongEventSongs.ELEGY);
    }
    if (this.state.settings.songOrderOot) {
      songCheckIds.push(SongEventSongs.OATH);
    }
    for (let i = 0; i < world.songEventsOot.length; ++i) {
      const v = sample(this.state.random, songCheckIds);
      world.songEventsOot[i] = v;
    }
  }

  private shuffleSongEventsMm(world: World) {
    const songCheckIds = [
      SongEventSongs.TIME,
      SongEventSongs.HEALING,
      SongEventSongs.EPONAS,
      SongEventSongs.SOARING,
      SongEventSongs.STORMS,
      SongEventSongs.SONATA,
      SongEventSongs.GORON_LULLABY,
      SongEventSongs.NEW_WAVE,
      // SongEventSongs.ELEGY,
      SongEventSongs.OATH,
    ];
    if (this.state.settings.progressiveGoronLullabyMm === 'progressive') {
      songCheckIds.push(SongEventSongs.GORON_LULLABY_INTRO);
    }
    if (this.state.settings.songZeldaLullabyMm) {
      songCheckIds.push(SongEventSongs.ZELDAS_LULLABY);
    }
    if (this.state.settings.songSariasMm) {
      songCheckIds.push(SongEventSongs.SARIAS);
    }
    if (this.state.settings.songSunMm) {
      songCheckIds.push(SongEventSongs.SUNS);
    }
    if (this.state.settings.songMinuetMm) {
      songCheckIds.push(SongEventSongs.MINUET);
    }
    if (this.state.settings.songBoleroMm) {
      songCheckIds.push(SongEventSongs.BOLERO);
    }
    if (this.state.settings.songSerenadeMm) {
      songCheckIds.push(SongEventSongs.SERENADE);
    }
    if (this.state.settings.songRequiemMm) {
      songCheckIds.push(SongEventSongs.REQUIEM);
    }
    if (this.state.settings.songNocturneMm) {
      songCheckIds.push(SongEventSongs.NOCTURNE);
    }
    if (this.state.settings.songPreludeMm) {
      songCheckIds.push(SongEventSongs.PRELUDE);
    }
    for (let i = 0; i < world.songEventsMm.length; ++i) {
      const v = sample(this.state.random, songCheckIds);
      world.songEventsMm[i] = v;
    }
  }

  run() {
    if (this.state.settings.songEventsShuffleOot) {
      for (const world of this.state.worlds) {
        this.shuffleSongEventsOot(world);
      }
    }

    if (this.state.settings.songEventsShuffleMm) {
      for (const world of this.state.worlds) {
        this.shuffleSongEventsMm(world);
      }
    }

    return {};
  }
}
