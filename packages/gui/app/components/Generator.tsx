import React from 'react';
import { SETTINGS_CATEGORIES } from '@ootmm/core';

import { useIsPatch, useRandomSettings } from '../contexts/GeneratorContext';
import { RomConfig } from './RomConfig';
import { Tab, Tabs } from './Tab';
import { SettingsEditor } from './SettingsEditor';
import { StartingItems } from './StartingItems';
import { Tricks } from './Tricks';
import { Glitches } from './Glitches';
import { JunkLocations } from './JunkLocations';
import { Dungeons } from './Dungeons';
import { SpecialConds } from './SpecialConds';
import { Plando } from './Plando';
import { CosmeticsEditor } from './Cosmetics';
import { Hints } from './Hints';

export function Generator() {
  const [isPatch] = useIsPatch();
  const [randomSettings] = useRandomSettings();
  const isRandom = randomSettings.enabled;

  return <Tabs>
    <Tab name="ROM Config"><RomConfig/></Tab>
    {SETTINGS_CATEGORIES.map(c => (
      <Tab key={c.key} name={c.name} disabled={isPatch || isRandom}><SettingsEditor category={c.key}/></Tab>
    ))}
    <Tab name="Dungeons" disabled={isPatch || isRandom}><Dungeons/></Tab>
    <Tab name="Tricks" disabled={isPatch}><Tricks/></Tab>
    <Tab name="Glitches" disabled={isPatch}><Glitches/></Tab>
    <Tab name="Special Conditions" disabled={isPatch || isRandom}><SpecialConds/></Tab>
    <Tab name="Starting Items" disabled={isPatch || isRandom}><StartingItems/></Tab>
    <Tab name="Junk Locations" disabled={isPatch}><JunkLocations/></Tab>
    <Tab name="Hints" disabled={isPatch}><Hints/></Tab>
    <Tab name="Plando" disabled={isPatch || isRandom}><Plando/></Tab>
    <Tab name="Cosmetics"><CosmeticsEditor/></Tab>
  </Tabs>;
}
