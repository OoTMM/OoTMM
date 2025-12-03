import { LuPackage, LuUsers, LuPuzzle, LuClock, LuTrendingUp } from 'react-icons/lu';

import { useStore } from '@/app/store';
import { TabView, TabViewRoute } from '../nav';
import { StartingItems } from '../StartingItems';
import { SettingsEditor } from '../settings';

const PageProgressive = () => <SettingsEditor category='items.progressive'/>;
const PageShared = () => <SettingsEditor category='items.shared'/>;
const PageExtensions = () => <SettingsEditor category='items.extensions'/>;
const PageAgeless = () => <SettingsEditor category='items.ageless'/>;

export function GeneratorItems() {
  const games = useStore(state => state.settings.games);

  const routes: TabViewRoute[] = [
    { name: 'Progressive', icon: LuTrendingUp, component: PageProgressive },
    { name: 'Shared', icon: LuUsers, component: PageShared, disabled: games !== 'ootmm' },
    { name: 'Extensions', icon: LuPuzzle, component: PageExtensions },
    { name: 'Ageless', icon: LuClock, component: PageAgeless, disabled: games === 'mm' },
    { name: 'Starting Items', icon: LuPackage, component: StartingItems }
  ];

  return <TabView routes={routes}/>;
}
