import { LuGlobe, LuLink, LuSettings, LuShuffle, LuTag, LuStar, LuTrafficCone, LuFlaskConical } from 'react-icons/lu';

import { useSetting } from '@/app/contexts/SettingsContext';
import { TabView, TabViewRoute } from '../nav';
import { SpecialConds } from '../SpecialConds';
import { MultipleSettingsEditor, SettingsEditor } from '../settings';

export function GeneratorSettings() {
  const games = useSetting('games');
  const routes: TabViewRoute[] = [
    { name: 'Main', icon: LuSettings, component: () => <SettingsEditor category='main'/> },
    { name: 'Shuffle', icon: LuShuffle, component: () => <SettingsEditor category='main.shuffle'/> },
    { name: 'Price', icon: LuTag, component: () => <SettingsEditor category='main.prices'/> },
    { name: 'Events', icon: LuTrafficCone, component: () => <SettingsEditor category='main.events'/> },
    { name: 'Cross-Game', icon: LuLink, component: () => <SettingsEditor category='main.cross'/>, disabled: games !== 'ootmm' },
    { name: 'World', icon: LuGlobe, component: () => <SettingsEditor category='main.world'/> },
    { name: 'Special Conditions', icon: LuStar, component: SpecialConds },
    { name: 'Misc.', icon: LuFlaskConical, component: () => <MultipleSettingsEditor name="misc"/> }
  ]

  return <TabView routes={routes}/>;
}
