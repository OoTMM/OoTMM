import { LuGlobe, LuLink, LuSettings, LuShuffle, LuTag, LuStar, LuTriangleAlert, LuFlaskConical } from 'react-icons/lu';

import { useSetting } from '@/app/contexts/SettingsContext';
import { TabView, TabViewRoute } from '../nav';
import { SpecialConds } from '../SpecialConds';
import { MultipleSettingsEditor, SettingsEditor } from '../settings';

const PageMain = () => <SettingsEditor category='main'/>;
const PageShuffle = () => <SettingsEditor category='main.shuffle'/>;
const PagePrices = () => <SettingsEditor category='main.prices'/>;
const PageEvents = () => <SettingsEditor category='main.events'/>;
const PageCross = () => <SettingsEditor category='main.cross'/>;
const PageWorld = () => <SettingsEditor category='main.world'/>;
const PageMisc = () => <MultipleSettingsEditor name="misc"/>;

export function GeneratorSettings() {
  const games = useSetting('games');
  const routes: TabViewRoute[] = [
    { name: 'Main', icon: LuSettings, component: PageMain },
    { name: 'Shuffle', icon: LuShuffle, component: PageShuffle },
    { name: 'Price', icon: LuTag, component: PagePrices },
    { name: 'Events', icon: LuTriangleAlert, component: PageEvents },
    { name: 'Cross-Game', icon: LuLink, component: PageCross, disabled: games !== 'ootmm' },
    { name: 'World', icon: LuGlobe, component: PageWorld },
    { name: 'Special Conditions', icon: LuStar, component: SpecialConds },
    { name: 'Misc.', icon: LuFlaskConical, component: PageMisc }
  ];

  return <TabView routes={routes}/>;
}
