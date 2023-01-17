import React, { useState } from 'react';
import { SETTINGS_CATEGORIES, DEFAULT_SETTINGS, itemPool as makeItemPool } from '@ootmm/core';

import { Tab, TabBar } from './Tab';
import { RomConfig } from './RomConfig';
import { Settings } from './Settings';
import { StartingItems } from './StartingItems';

export const Generator = ({ onGenerate, error }) => {
  const [roms, setRoms] = useState({ oot: null, mm: null });
  const [settings, setSettings] = useState({...DEFAULT_SETTINGS});
  const [itemPool, setItemPool] = useState(makeItemPool(settings));

  const setRom = (game, data) => setRoms({ ...roms, [game]: data });
  const limitItemPool = (setting) => {
    const ip = makeItemPool(setting);
    setItemPool(ip);
    const { startingItems } = settings;
    for (const i in startingItems) {
      if (!itemPool[i]) {
        delete startingItems[i];
      } else if (startingItems[i] > itemPool[i]) {
        startingItems[i] = itemPool[i];
      }
    }
    return startingItems;
  };
  const setSetting = (setting) => {
    if (!setting.startingItems) {
      setting = { ...setting, startingItems: limitItemPool(setting) };
    }
    setSettings({ ...settings, ...setting });
  };

  return (
    <TabBar>
      <Tab name="ROM Config" component={<RomConfig roms={roms} setRom={setRom} error={error} onGenerate={() => onGenerate({ roms, settings })}/>}/>
      {SETTINGS_CATEGORIES.map(category =>
        <Tab key={category.key} name={category.name} component={<Settings category={category.key} settings={settings} setSetting={setSetting}/>}/>
      )}
      <Tab name="Starting Items" component={<StartingItems settings={settings} setSetting={setSetting} itemPool={itemPool}/>}/>
    </TabBar>
  );
};
