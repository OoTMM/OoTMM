import React from 'react';
import { SETTINGS_CATEGORIES } from '@ootmm/core';

import { useIsPatch } from '../contexts/GeneratorContext';
import { RomConfig } from './RomConfig';
import { Tab, TabBar } from './Tab';
import { SettingsEditor } from './SettingsEditor';
import { StartingItems } from './StartingItems';
import { Tricks } from './Tricks';
import { JunkLocations } from './JunkLocations';
import { Dungeons } from './Dungeons';
import { SpecialConds } from './SpecialConds';
import { Plando } from './Plando';
import { Cosmetics } from './Cosmetics';

export function Generator() {
  const tabs: Tab[] = [];
  const [isPatch] = useIsPatch();

  tabs.push({
    name: "ROM Config",
    component: <RomConfig/>
  });

  if (!isPatch) {
    SETTINGS_CATEGORIES.forEach(category => {
      tabs.push({ name: category.name, component: <SettingsEditor category={category.key}/>});
    });
    tabs.push({
      name: "Dungeons",
      component: <Dungeons/>
    }, {
      name: "Tricks",
      component: <Tricks/>
    }, {
      name: "Special Conditions",
      component: <SpecialConds/>
    }, {
      name: "Starting Items",
      component: <StartingItems/>
    }, {
      name: "Junk Locations",
      component: <JunkLocations/>
    }, {
      name: "Plando",
      component: <Plando/>
    });
  }

  tabs.push({
    name: "Cosmetics",
    component: <Cosmetics/>
  });

  return <TabBar tabs={tabs}/>
}
