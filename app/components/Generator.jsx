import React, { useState } from 'react';
import { SETTINGS_CATEGORIES, DEFAULT_SETTINGS, itemPool as makeItemPool, locationList as makeLocationList } from '@ootmm/core';
import { merge } from 'lodash';

import { Tab, TabBar } from './Tab';
import { RomConfig } from './RomConfig';
import { Settings } from './Settings';
import { StartingItems } from './StartingItems';
import { Tricks } from './Tricks';
import { JunkLocations } from './JunkLocations';

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
  const [locList, setLocList] = useState(() => makeLocationList(settings) );
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
    if (!(setting.startingItems || setting.junkLocations)) {
      setting = { ...setting, startingItems: limitItemPool(setting) };
    }
    let newSettings = { ...settings, ...setting };
    const [newLocList, junkLocations] = pruneLocationList(newSettings);
    newSettings = { ...newSettings, junkLocations: junkLocations };
    setLocList(newLocList);
    localStorage.setItem('settings', JSON.stringify(newSettings));
    setSettings(newSettings);
  };

  const pruneLocationList = (newSettings) => {
    const locList = makeLocationList(newSettings);
    const junkLocations = newSettings.junkLocations;
    for (let i=0; i<junkLocations.length; i++) {
      if (Object.keys(locList).indexOf(junkLocations[i]) === -1) {
        delete junkLocations[i];
      }
    }
    return [locList, junkLocations];
  };
  

  return (
    <TabBar>
      <Tab name="ROM Config" component={<RomConfig roms={roms} setRom={setRom} seed={seed} setSeed={setSeed} error={error} onGenerate={() => onGenerate({ roms, settings, seed })}/>}/>
      {SETTINGS_CATEGORIES.map(category =>
        <Tab key={category.key} name={category.name} component={<Settings category={category.key} settings={settings} setSetting={setSetting}/>}/>
      )}
      <Tab name="Tricks" component={<Tricks settings={settings} setSetting={setSetting}/>}/>
      <Tab name="Starting Items" component={<StartingItems settings={settings} setSetting={setSetting} itemPool={itemPool}/>}/>
      <Tab name="Junk Locations" component={<JunkLocations settings={settings} setSetting={setSetting} locList={locList}/>}/>
    </TabBar>
  );
};
