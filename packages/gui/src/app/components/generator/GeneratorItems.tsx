import type { TabViewRoute } from '../nav';
import { LuPackage, LuPuzzle, LuClock } from 'react-icons/lu';

import { useStore } from '@/app/store';
import { TabView } from '../nav';
import { StartingItems } from '../StartingItems';
import { SettingsEditor } from '../settings';
import { ExtendedItemsEditor } from '../ExtendedItemsEditor.tsx';

const PageExtensions = () => <ExtendedItemsEditor/>;
const PageAgeless = () => <SettingsEditor category='items.ageless'/>;

export function GeneratorItems() {
  const games = useStore(state => state.settings.games);

  const routes: TabViewRoute[] = [
    { name: 'Extended Items', icon: LuPuzzle, component: PageExtensions },
    { name: 'Ageless', icon: LuClock, component: PageAgeless, disabled: games === 'mm' },
    { name: 'Starting Items', icon: LuPackage, component: StartingItems }
  ];

  return <TabView routes={routes}/>;
}
