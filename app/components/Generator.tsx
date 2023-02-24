import React, { useState } from 'react';
import { OptionsInput, Settings as CoreSettings, SETTINGS_CATEGORIES, DEFAULT_SETTINGS, itemPool as makeItemPool, locationList as makeLocationList } from '@ootmm/core';
import { merge } from 'lodash';

import { Tab, TabBar } from './Tab';
import { RomConfig } from './RomConfig';
import { Settings } from './Settings';
import { StartingItems } from './StartingItems';
import { Tricks } from './Tricks';
import { JunkLocations } from './JunkLocations';

type Items = {[k: string]: number};

const displayJunkItems = !(makeLocationList === undefined);

const savedSettings = JSON.parse(localStorage.getItem('settings') || "{}");

const limitStartingItems = (startingItems: Items, itemPool: Items) => {
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

type GeneratorProps = {
  onGenerate: (x: any) => void;
  error: string;
}

export const Generator = ({ onGenerate, error }: GeneratorProps) => {
  const [roms, setRoms] = useState<{ oot: File | null, mm: File | null }>({ oot: null, mm: null });
  const [seed, setSeed] = useState("");
  const [usePatch, setUsePatch] = useState(false);
  const [patch, setPatch] = useState<File | null>(null);
  const [locList, setLocList] = useState(() => {
    if (displayJunkItems) {
      return makeLocationList(settings);
    }
    return null;
  });
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

  const setRom = (game: 'oot' | 'mm', data: File) => setRoms({ ...roms, [game]: data });
  const limitItemPool = (setting: Partial<CoreSettings>) => {
    const ip = makeItemPool(setting);
    setItemPool(ip);
    let { startingItems } = settings;
    startingItems = limitStartingItems(startingItems, ip);
    return startingItems;
  };

  const setSetting = (setting: Partial<CoreSettings>) => {
    if (!(setting.startingItems || setting.junkLocations)) {
      setting = { ...setting, startingItems: limitItemPool(setting) };
    }
    let newSettings = { ...settings, ...setting };
    if (displayJunkItems) {
      const [newLocList, junkLocations] = pruneLocationList(newSettings);
      newSettings = { ...newSettings, junkLocations: junkLocations };
      setLocList(newLocList);
    }
    localStorage.setItem('settings', JSON.stringify(newSettings));
    setSettings(newSettings);
  };

  const pruneLocationList = (newSettings: CoreSettings) => {
    const locList = makeLocationList(newSettings);
    const junkLocations = newSettings.junkLocations;
    const newJunkLocations = junkLocations.filter((v) => Object.keys(locList).indexOf(v) !== -1);
    return [locList, newJunkLocations] as const;
  };

  const tabs: Tab[] = [];
  tabs.push({
    name: "ROM Config",
    component:
      <RomConfig
      roms={roms}
      setRom={setRom}
      usePatch={usePatch}
      setUsePatch={setUsePatch}
      patch={patch}
      setPatch={setPatch}
      seed={seed}
      setSeed={setSeed}
      error={error}
      onGenerate={() => onGenerate({ roms, opts: { settings, seed, patch: usePatch ? patch : null } })}
    />
  });

  if (!usePatch) {
    SETTINGS_CATEGORIES.forEach(category => {
      tabs.push({ name: category.name, component: <Settings category={category.key} settings={settings} setSetting={setSetting}/>});
    });
    tabs.push({
      name: "Tricks",
      component: <Tricks settings={settings} setSetting={setSetting}/>
    }, {
      name: "Starting Items",
      component: <StartingItems settings={settings} setSetting={setSetting} itemPool={itemPool}/>
    }, {
      name: "Junk Locations",
      component: <JunkLocations settings={settings} setSetting={setSetting} locList={locList}/>
    })
  }

  return <TabBar tabs={tabs}/>;
};
