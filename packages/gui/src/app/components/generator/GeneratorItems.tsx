import { useSetting } from '@/app/contexts/SettingsContext';
import { TabView, TabViewRoute } from '../nav';
import { StartingItems } from '../StartingItems';
import { SettingsEditor } from '../settings';

export function GeneratorItems() {
  const games = useSetting('games');
  const routes: TabViewRoute[] = [
    { name: 'Progressive', component: () => <SettingsEditor category='items.progressive'/> },
    { name: 'Shared', component: () => <SettingsEditor category='items.shared'/>, disabled: games !== 'ootmm' },
    { name: 'Extensions', component: () => <SettingsEditor category='items.extensions'/> },
    { name: 'Ageless', component: () => <SettingsEditor category='items.ageless'/>, disabled: games === 'mm' },
    { name: 'Starting Items', component: StartingItems }
  ];

  return <TabView routes={routes}/>;
}
