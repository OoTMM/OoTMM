import { useSetting } from '@/app/contexts/SettingsContext';
import { TabMultipleSettingsEditor, TabSettingsEditor } from '../legacy/TabLegacy';
import { NavTop, Tab, TabView, TabViewRoute } from '../nav';
import { SpecialConds } from '../SpecialConds';
import { MultipleSettingsEditor, SettingsEditor } from '../settings';

export function GeneratorSettings() {
  const games = useSetting('games');
  const routes: TabViewRoute[] = [
    { name: 'Main', component: () => <SettingsEditor category='main'/> },
    { name: 'Shuffle', component: () => <SettingsEditor category='main.shuffle'/> },
    { name: 'Price', component: () => <SettingsEditor category='main.prices'/> },
    { name: 'Events', component: () => <SettingsEditor category='main.events'/> },
    { name: 'Cross-Game', component: () => <SettingsEditor category='main.cross'/>, disabled: games !== 'ootmm' },
    { name: 'World', component: () => <SettingsEditor category='main.world'/> },
    { name: 'Special Conditions', component: SpecialConds },
    { name: 'Misc.', component: () => <MultipleSettingsEditor name="misc"/> }
  ]

  return <TabView routes={routes}/>;
}
