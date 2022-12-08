import React, { useState } from 'react';
import { SETTINGS_CATEGORIES, DEFAULT_SETTINGS } from '@ootmm/core';

import { Tab, TabBar } from './Tab';
import { RomConfig } from './RomConfig';
import { Settings } from './Settings';

export const Generator = ({ onGenerate, error }) => {
  const [roms, setRoms] = useState({ oot: null, mm: null });
  const [settings, setSettings] = useState({...DEFAULT_SETTINGS});

  const setRom = (game, data) => setRoms({ ...roms, [game]: data });
  const setSetting = (setting) => setSettings({ ...settings, ...setting });

  return (
    <TabBar>
      <Tab name="ROM Config" component={<RomConfig roms={roms} setRom={setRom} error={error} onGenerate={() => onGenerate({ roms, settings })}/>}/>
      {SETTINGS_CATEGORIES.map(category =>
        <Tab key={category.key} name={category.name} component={<Settings category={category.key} settings={settings} setSetting={setSetting}/>}/>
      )}
    </TabBar>
  );
};
