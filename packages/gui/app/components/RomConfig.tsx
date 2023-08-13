import React from 'react';
import { useGenerator, useIsPatch, useRandomSettings, useRomConfig } from '../contexts/GeneratorContext';
import { Checkbox } from './Checkbox';

import { FileSelect } from './FileSelect';
import { PresetSelector } from './PresetSelector';
import { SettingsImportExport } from './SettingsImportExport';
import { Group } from './Group';
import { Text } from './Text';

export function RomConfig() {
  const { romConfig, setFileBuffer, setSeed } = useRomConfig();
  const [ isPatch, setIsPatch] = useIsPatch();
  const { error, generate } = useGenerator();
  const [randomSettings, setRandomSettings] = useRandomSettings();

  const isRandomSettings = randomSettings.enabled;

  return <Group direction='vertical' spacing=''>
    <Text size='mg'>OoTMM Web Generator</Text>
    <Text size='jb'>Version: {process.env.VERSION}</Text>
    {error && <div className="generator-error">{error}</div>}
    <form target="_self" onSubmit={(e) => { e.preventDefault(); generate(); }}>
      <div className='center'>
      <Group direction='horizontal' spacing='mg'>
        <FileSelect logo="oot" file='oot' label="Ocarina of Time (1.0, U or J)" accept=".z64, .n64, .v64" onChange={(f) => setFileBuffer('oot', f)}/>
        <FileSelect logo="mm" file='mm' label="Majora's Mask (U only)" accept=".z64, .n64, .v64" onChange={(f) => setFileBuffer('mm', f)} />
        {isPatch && <FileSelect file='patch' logo="ootmm" label="OoTMM Patch File" accept=".ootmm" onChange={(f) => setFileBuffer('patch', f)}/>}
      </Group>
      </div>
      {!isPatch && <Checkbox label="Random Settings" checked={isRandomSettings} onChange={x => setRandomSettings({ enabled: x })}/>}
      <Group direction='vertical' spacing={ isRandomSettings ? '2px' : '16px'}>
        {isRandomSettings && <>
          <Checkbox label="Random Settings: MQ" checked={randomSettings.mq} onChange={x => setRandomSettings({ mq: x })}/>
          <Checkbox label="Random Settings: Entrances" checked={randomSettings.er} onChange={x => setRandomSettings({ er: x })}/>
        </>}
        {!isPatch && !isRandomSettings && <>
          <Group direction='vertical'>
            <PresetSelector/>
            <SettingsImportExport/>
          </Group>
        </>}
        {!isRandomSettings && <Checkbox label="Use a patch file" checked={isPatch} onChange={setIsPatch}/>}
        {!isPatch && !isRandomSettings && <label>
          Seed (leave blank to auto-generate)
          <input
            type="text"
            value={romConfig.seed}
            onChange={(e) => setSeed(e.target.value)}
          />
        </label>}
        <button className="btn-primary" type="submit" style={{width: '105px'}}>
          Generate
        </button>
      </Group>
      
    </form>
  </Group>
}
