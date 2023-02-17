import React, { useState } from 'react';
import { merge } from 'lodash';
import { SETTINGS_CATEGORIES, DEFAULT_SETTINGS, itemPool as makeItemPool } from '@ootmm/core';

import { Tab, TabBar } from './Tab';
import { RomConfig } from './RomConfig';
import { Settings } from './Settings';
import { StartingItems } from './StartingItems';
import { Tricks } from './Tricks';

const savedSettings = JSON.parse(localStorage.getItem('settings') || "{}");

const limitStartingItems = (startingItems, itemPool) => {
  const items = { ...startingItems };
  for (const i in items) {
    if (!itemPool[i]) {
      delete items[i];
    } else if (items[i] > itemPool[i]) {
      items[i] = itemPool[i];
    }
  }
  return items;
};

export const Generator = ({ onGenerate, error }) => {
  const [roms, setRoms] = useState({ oot: null, mm: null });
  const [seed, setSeed] = useState("");
  const [settings, setSettings] = useState(merge({}, DEFAULT_SETTINGS, savedSettings));
  const [itemPool, setItemPool] = useState(() => {
    const pool = makeItemPool(settings);
    let { startingItems } = settings;
    startingItems = limitStartingItems(startingItems, pool);
    const newSettings = { ...settings, startingItems };
    localStorage.setItem('settings', JSON.stringify(newSettings));
    setSettings(newSettings);
    return pool;
  });

  const setRom = (game, data) => setRoms({ ...roms, [game]: data });
  const limitItemPool = (setting) => {
    const ip = makeItemPool(setting);
    setItemPool(ip);
    let { startingItems } = settings;
    startingItems = limitStartingItems(startingItems, ip);
    return startingItems;
  };
  const setSetting = (setting) => {
    if (!setting.startingItems) {
      setting = { ...setting, startingItems: limitItemPool(setting) };
    }
    const newSettings = { ...settings, ...setting };
    localStorage.setItem('settings', JSON.stringify(newSettings));
    setSettings(newSettings);
  };

  return (
    <TabBar>
      <Tab name="ROM Config" component={<RomConfig roms={roms} setRom={setRom} seed={seed} setSeed={setSeed} error={error} onGenerate={() => onGenerate({ roms, settings, seed })}/>}/>
      {SETTINGS_CATEGORIES.map(category =>
        <Tab key={category.key} name={category.name} component={<Settings category={category.key} settings={settings} setSetting={setSetting}/>}/>
      )}
      <Tab name="Tricks" component={<Tricks settings={settings} setSetting={setSetting}/>}/>
      <Tab name="Starting Items" component={<StartingItems settings={settings} setSetting={setSetting} itemPool={itemPool}/>}/>
    </TabBar>
  );
};
