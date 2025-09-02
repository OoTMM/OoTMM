import { useCallback, useEffect, useMemo, useState } from 'react';

import { useLocations, usePatchSettings, useSetting } from '../contexts/SettingsContext';
import { Button, Card, Select } from './ui';
import { FaXmark } from 'react-icons/fa6';

export function JunkLocations() {
  const junkLocations = useSetting('junkLocations');
  const [junkLocationsCache, setJunkLocationsCache] = useState(junkLocations);
  const locations = useLocations();
  const options = useMemo(() => locations.filter(x => !junkLocationsCache.includes(x)).sort().map(loc => ({ value: loc, label: loc })), [locations, junkLocationsCache]);
  const patchSettings = usePatchSettings();

  useEffect(() => {
    setJunkLocationsCache(junkLocations);
  }, [junkLocations]);

  const add = useCallback((loc: string) => {
    setJunkLocationsCache(prev => [...prev, loc]);
    patchSettings({ junkLocations: { add: [loc] } });
  }, []);

  const remove = useCallback((loc: string) => {
    setJunkLocationsCache(prev => prev.filter(x => x !== loc));
    patchSettings({ junkLocations: { remove: [loc] } });
  }, []);

  const clear = useCallback(() => {
    setJunkLocationsCache([]);
    patchSettings({ junkLocations: { set: [] } });
  }, []);

  return (
    <main className="h-full flex flex-col gap-4">
      <div className="flex gap-4">
        <div className="flex-1"><Select searcheable placeholder="Location" value={null} options={options} onSelect={add}/></div>
        <Button variant="danger" onClick={clear}>Clear All</Button>
      </div>
      <Card className="flex-[1_1_0] overflow-y-auto">
        {junkLocationsCache.length === 0 && (
          <div className="h-full flex justify-center items-center">
            <span className="text-gray-500 text-3xl">No Junk Locations</span>
          </div>
        )}
        {junkLocationsCache.sort().map((loc, i) => (
          <div key={i} className="flex gap-1 items-center">
            <span className="hover:text-gray-500 cursor-pointer" onClick={() => remove(loc)}><FaXmark/></span>
            <span>{loc}</span>
          </div>
        ))}
      </Card>
    </main>
  );
}
