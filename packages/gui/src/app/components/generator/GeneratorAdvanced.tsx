import type { TabViewRoute } from '../nav';

import { LuLightbulb, LuWrench, LuMusic } from 'react-icons/lu';

import { TabView } from '../nav';
import { Hints } from '../Hints';
import { Plando } from '../Plando';
import { SongEventPlando } from '../SongEventPlando';
import { useStore } from '@/app/store';

export function GeneratorAdvanced() {
  const mode = useStore(state => state.config.mode);
  const settings = useStore(state => state.settings);
  const isRandom = mode === 'random';
  const hasSongEventPlando = settings.songEventsShuffleOot || settings.songEventsShuffleMm;
  const routes: TabViewRoute[] = [
    { name: 'Hints', icon: LuLightbulb, component: Hints },
    { name: 'Plando', icon: LuWrench, component: Plando, disabled: isRandom }, ...(hasSongEventPlando ? [{ name: 'Song Event Plando', icon: LuMusic, component: SongEventPlando, disabled: isRandom }] : []),
  ];

  return (
      <TabView routes={routes}/>
  );
}