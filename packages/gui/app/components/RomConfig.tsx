import React from 'react';
import { useGenerator, useIsPatch, useRandomSettings, useRomConfig } from '../contexts/GeneratorContext';
import { Checkbox } from './Checkbox';

import { FileSelect } from './FileSelect';
import { PresetSelector } from './PresetSelector';
import { SettingsImportExport } from './SettingsImportExport';

export function RomConfig() {
  const { romConfig, setFile, setSeed } = useRomConfig();
  const [isPatch, setIsPatch] = useIsPatch();
  const { error, generate } = useGenerator();
  const [randomSettings, setRandomSettings] = useRandomSettings();

  const isRandomSettings = randomSettings.enabled;

  return <>
    <h1>OoTMM Web Generator</h1>
    <h2>Version: {process.env.VERSION}</h2>
    {error && <div className="generator-error">{error}</div>}
    <form target="_self" onSubmit={(e) => { e.preventDefault(); generate(); }}>
      <div className="flex-h">
        <FileSelect logo="oot" label="Ocarina of Time (1.0, U or J)" accept=".z64, .n64, .v64" file={romConfig.files.oot} onChange={(f) => setFile('oot', f)}/>
        <FileSelect logo="mm" label="Majora's Mask (U only)" accept=".z64, .n64, .v64" file={romConfig.files.mm} onChange={(f) => setFile('mm', f)} />
        {isPatch && <FileSelect logo="ootmm" label="OoTMM Patch File" accept=".ootmm" file={romConfig.files.patch} onChange={(f) => setFile('patch', f)}/>}
      </div>
      {!isPatch && <Checkbox label="Random Settings" checked={isRandomSettings} onChange={x => setRandomSettings({ enabled: x })}/>}
      {!isPatch && !isRandomSettings && <>
        <PresetSelector/>
        <SettingsImportExport/>
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
      <button className="btn-primary sm-margin-top" type="submit">
        Generate
      </button>
    </form>
  </>
}
