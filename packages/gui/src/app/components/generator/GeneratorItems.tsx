import { useSetting } from '@/app/contexts/SettingsContext';
import { TabSettingsEditor } from '../legacy/TabLegacy';
import { NavTop, Tab } from '../nav';
import { StartingItems } from '../StartingItems';

export function GeneratorItems() {
  const games = useSetting('games');

  return (
    <NavTop>
      <TabSettingsEditor name="Progressive" category="items.progressive"/>
      <TabSettingsEditor name="Shared" disabled={games !== 'ootmm'} category="items.shared"/>
      <TabSettingsEditor name="Extensions" category="items.extensions"/>
      <TabSettingsEditor name="Ageless" disabled={games === 'mm'} category="items.ageless"/>
      <Tab name="Starting Items"><StartingItems/></Tab>
    </NavTop>
  );
}
