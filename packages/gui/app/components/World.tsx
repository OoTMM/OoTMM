import { DUNGEONS } from '@ootmm/core';

import { useSettings } from '../contexts/GeneratorContext';
import { Dropdown } from './Dropdown';
import { Setting } from './SettingsEditor';

export function World() {
  const [settings, setSettings] = useSettings();
  const { dungeon } = settings;
  const options = [{
    name: 'Vanilla',
    value: 'vanilla',
   }, {
    name: 'Master Quest',
    value: 'mq',
  }, {
    name: 'Random',
    value: 'random',
  }];

  return (
    <main>
      <h1>World</h1>
      {settings.games !== 'mm' && <div>
          <h2>Master Quest</h2>
          <form>
            {Object.keys(DUNGEONS).map(d =>
              <Dropdown
                key={d}
                label={(DUNGEONS as any)[d]}
                options={options}
                value={(dungeon as any)[d]}
                onInput={v => setSettings({ dungeon: { [d as keyof typeof DUNGEONS]: v } })}
              />
            )}
          </form>
        </div>}
        {settings.games !== 'mm' && <div>
          <h2>Other</h2>
          <form>
            <Setting setting='worldLayoutMm'/>
          </form>
        </div>}
    </main>
  );
}
