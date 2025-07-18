import { ComponentType } from 'react';
import { useIsPatch } from '../../contexts/GeneratorContext';
import { useRandomSettings } from '../../contexts/RandomSettingsContext';
import { useSetting } from '../../contexts/SettingsContext';
import { GeneratorGeneral } from './GeneratorGeneral';
import { Tab, NavSide, NavTop } from '../nav';
import { MultipleSettingsEditor, SettingsEditor } from '../SettingsEditor';
import { StartingItems } from '../StartingItems';
import { Tricks } from '../Tricks';
import { JunkLocations } from '../JunkLocations';
import { SpecialConds } from '../SpecialConds';
import { Plando } from '../Plando';
import { CosmeticsEditor } from '../Cosmetics';
import { Hints } from '../Hints';
import { LuSettings, LuGamepad2, LuBox, LuRoute, LuCpu, LuCompass, LuSparkle } from 'react-icons/lu';
import { TabMultipleSettingsEditor, TabSettingsEditor } from '../legacy/TabLegacy';
import { GeneratorSettings } from './GeneratorSettings';

export function Generator() {
  const games = useSetting('games');
  const [isPatch] = useIsPatch();
  const randomSettings = useRandomSettings();
  const isRandom = randomSettings.enabled;

  return (
    <NavSide>
      <Tab name="General" icon={LuSettings} component={GeneratorGeneral}/>
      <Tab name="Settings" icon={LuGamepad2} disabled={isPatch || isRandom} component={GeneratorSettings}/>
      <Tab name="Items" icon={LuBox} disabled={isPatch || isRandom}>
        <NavTop>
          <TabSettingsEditor name="Progressive" category="items.progressive"/>
          <TabSettingsEditor name="Shared" disabled={games !== 'ootmm'} category="items.shared"/>
          <TabSettingsEditor name="Extensions" category="items.extensions"/>
          <TabSettingsEditor name="Ageless" disabled={games === 'mm'} category="items.ageless"/>
          <Tab name="Starting Items"><StartingItems/></Tab>
        </NavTop>
      </Tab>
      <Tab name="Logic" icon={LuRoute} disabled={isPatch}>
        <NavTop>
          <Tab name="Tricks"><Tricks glitches={false}/></Tab>
          <Tab name="Glitches"><Tricks glitches={true}/></Tab>
          <Tab name="Junk Locations"><JunkLocations/></Tab>
        </NavTop>
      </Tab>
      <TabSettingsEditor name="Entrances" icon={LuCompass} category="entrances" disabled={isPatch || isRandom}/>
      <Tab name="Advanced" icon={LuCpu} disabled={isPatch}>
        <NavTop>
          <Tab name="Hints"><Hints/></Tab>
          <Tab name="Plando" disabled={isRandom}><Plando/></Tab>
        </NavTop>
      </Tab>
      <Tab name="Cosmetics" icon={LuSparkle}><CosmeticsEditor/></Tab>
    </NavSide>
  );
}
