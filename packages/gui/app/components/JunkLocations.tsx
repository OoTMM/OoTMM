import { useCallback, useMemo } from 'react';

import { useLocations, usePatchSettings, useSetting } from '../contexts/SettingsContext';
import { ArrayList } from './ArrayList';

export function JunkLocations() {
  const junkLocations = useSetting('junkLocations');
  const locations = useLocations();
  const options = useMemo(() => locations.map(loc => ({ value: loc, label: loc })), [locations]);
  const patchSettings = usePatchSettings();

  const add = useCallback((loc: string) => {
    patchSettings({ junkLocations: { add: [loc] } });
  }, []);

  const remove = useCallback((loc: string) => {
    patchSettings({ junkLocations: { remove: [loc] } });
  }, []);

  const clear = useCallback(() => {
    patchSettings({ junkLocations: { set: [] } });
  }, []);

  return (
    <main>
      <h1>Junk Locations</h1>
      <ArrayList options={options} selected={junkLocations} add={add} remove={remove} clear={clear}/>
    </main>
  );
}
