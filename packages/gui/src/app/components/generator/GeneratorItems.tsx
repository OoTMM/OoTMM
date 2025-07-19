import { LuPackage, LuUsers, LuPuzzle, LuClock, LuTrendingUp } from 'react-icons/lu';

import { useSetting } from '@/app/contexts/SettingsContext';
import { TabView, TabViewRoute } from '../nav';
import { StartingItems } from '../StartingItems';
import { SettingsEditor } from '../settings';

export function GeneratorItems() {
  const games = useSetting('games');
  const routes: TabViewRoute[] = [
    { name: 'Progressive', icon: LuTrendingUp, component: () => <SettingsEditor category='items.progressive'/> },
    { name: 'Shared', icon: LuUsers, component: () => <SettingsEditor category='items.shared'/>, disabled: games !== 'ootmm' },
    { name: 'Extensions', icon: LuPuzzle, component: () => <SettingsEditor category='items.extensions'/> },
    { name: 'Ageless', icon: LuClock, component: () => <SettingsEditor category='items.ageless'/>, disabled: games === 'mm' },
    { name: 'Starting Items', icon: LuPackage, component: StartingItems }
  ];

  return <TabView routes={routes}/>;
}
