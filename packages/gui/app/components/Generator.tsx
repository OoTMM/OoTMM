import React from 'react';

import { useIsPatch, useRandomSettings, useSettings } from '../contexts/GeneratorContext';
import { RomConfig } from './RomConfig';
import { Tab, Tabs } from './Tab';
import { SettingsEditor } from './SettingsEditor';
import { StartingItems } from './StartingItems';
import { Tricks } from './Tricks';
import { JunkLocations } from './JunkLocations';
import { Dungeons } from './Dungeons';
import { SpecialConds } from './SpecialConds';
import { Plando } from './Plando';
import { CosmeticsEditor } from './Cosmetics';
import { Hints } from './Hints';

type TabSettingsEditorProps = {
  name: string;
  category: string;
  disabled?: boolean;
};
function TabSettingsEditor({ name, category, disabled }: TabSettingsEditorProps) {
  return <Tab name={name} disabled={disabled}><SettingsEditor name={name} category={category}/></Tab>;
}

export function Generator() {
  const [settings] = useSettings();
  const [isPatch] = useIsPatch();
  const [randomSettings] = useRandomSettings();
  const isRandom = randomSettings.enabled;

  return <Tabs>
    <Tab name="ROM Config"><RomConfig/></Tab>
    <Tab name="Settings" disabled={isPatch || isRandom}>
      <Tabs>
        <TabSettingsEditor name="Main" category="main"/>
        <TabSettingsEditor name="Shuffle" category="main.shuffle"/>
        <TabSettingsEditor name="Price" category="main.prices"/>
        <TabSettingsEditor name="Events" category="main.events"/>
        <TabSettingsEditor name="Cross-Game" category="main.cross"/>
        <Tab name="Dungeons"><Dungeons/></Tab>
        <Tab name="Special Conditions"><SpecialConds/></Tab>
        <Tab name="Starting Items"><StartingItems/></Tab>
        <TabSettingsEditor name="Misc." category="main.misc"/>
      </Tabs>
    </Tab>
    <Tab name="Items" disabled={isPatch || isRandom}>
      <Tabs>
        <TabSettingsEditor name="Progressive" category="items.progressive"/>
        <TabSettingsEditor name="Shared" disabled={settings.games !== 'ootmm'} category="items.shared"/>
        <TabSettingsEditor name="Extensions" category="items.extensions"/>
        <TabSettingsEditor name="Ageless" disabled={settings.games === 'mm'} category="items.ageless"/>
      </Tabs>
    </Tab>
    <Tab name="Logic" disabled={isPatch}>
      <Tabs>
        <Tab name="Tricks"><Tricks glitches={false}/></Tab>
        <Tab name="Glitches"><Tricks glitches={true}/></Tab>
        <Tab name="Junk Locations"><JunkLocations/></Tab>
      </Tabs>
    </Tab>
    <TabSettingsEditor name="Entrances" category="entrances" disabled={isPatch || isRandom}/>
    <Tab name="Advanced" disabled={isPatch}>
      <Tabs>
        <Tab name="Hints"><Hints/></Tab>
        <Tab name="Plando" disabled={isRandom}><Plando/></Tab>
      </Tabs>
    </Tab>
    <Tab name="Cosmetics"><CosmeticsEditor/></Tab>
  </Tabs>;
}
