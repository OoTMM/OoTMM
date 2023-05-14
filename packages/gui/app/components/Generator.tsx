import React from 'react';
import { SETTINGS_CATEGORIES } from '@ootmm/core';

import { useIsPatch, useRandomSettings } from '../contexts/GeneratorContext';
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
import { Hints } from './Hints';

export function Generator() {
  const tabs: Tab[] = [];
  const [isPatch] = useIsPatch();
  const [randomSettings] = useRandomSettings();

  const isRandom = randomSettings.enabled;

  tabs.push({
    name: "ROM Config",
    component: <RomConfig/>
  });

  if (!isPatch) {
    if (!isRandom) {
      SETTINGS_CATEGORIES.forEach(category => {
        tabs.push({ name: category.name, component: <SettingsEditor category={category.key}/>});
      });
      tabs.push({
        name: "Dungeons",
        component: <Dungeons/>
      });
    }
    tabs.push({
      name: "Tricks",
      component: <Tricks/>
    });
    if (!isRandom) {
      tabs.push({
        name: "Special Conditions",
        component: <SpecialConds/>
      });
      tabs.push({
        name: "Starting Items",
        component: <StartingItems/>
      });
    }

    tabs.push({
      name: "Junk Locations",
      component: <JunkLocations/>
    });
    tabs.push({
      name: "Hints",
      component: <Hints/>
    });

    if (!isRandom) {
      tabs.push({
        name: "Plando",
        component: <Plando/>
      });
    }
  }

  tabs.push({
    name: "Cosmetics",
    component: <Cosmetics/>
  });

  return <TabBar tabs={tabs}/>
}
