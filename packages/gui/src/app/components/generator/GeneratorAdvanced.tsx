import { NavTop, Tab } from '../nav';
import { Hints } from '../Hints';
import { useRandomSettings } from '@/app/contexts/RandomSettingsContext';
import { Plando } from '../Plando';

export function GeneratorAdvanced() {
  const randSettings = useRandomSettings();

  return (
    <NavTop>
      <Tab name="Hints"><Hints/></Tab>
      <Tab name="Plando" disabled={randSettings.enabled}><Plando/></Tab>
    </NavTop>
  );
}
