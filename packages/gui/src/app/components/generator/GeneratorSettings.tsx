import { useSetting } from '@/app/contexts/SettingsContext';
import { TabMultipleSettingsEditor, TabSettingsEditor } from '../legacy/TabLegacy';
import { NavTop, Tab } from '../nav';
import { SpecialConds } from '../SpecialConds';

export function GeneratorSettings() {
  const games = useSetting('games');

  return (
    <NavTop>
      <TabSettingsEditor name="Main" category="main"/>
      <TabSettingsEditor name="Shuffle" category="main.shuffle"/>
      <TabSettingsEditor name="Price" category="main.prices"/>
      <TabSettingsEditor name="Events" category="main.events"/>
      <TabSettingsEditor name="Cross-Game" disabled={games !== 'ootmm'} category="main.cross" />
      <TabSettingsEditor name="World" category="main.world"/>
      <Tab name="Special Conditions"><SpecialConds/></Tab>
      <TabMultipleSettingsEditor name="Misc."/>
    </NavTop>
  );
}
