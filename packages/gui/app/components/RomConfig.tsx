import React from 'react';
import ReactGA from 'react-ga4';

import { useGenerator, useIsPatch, useRandomSettings, useRomConfig } from '../contexts/GeneratorContext';
import { Checkbox } from './Checkbox';
import { FileSelect } from './FileSelect';
import { PresetSelector } from './PresetSelector';
import { SettingsImportExport } from './SettingsImportExport';
import { Result } from './Result';

export function RomConfig() {
  const { romConfig, setRomConfigFile, setSeed } = useRomConfig();
  const [isPatch, setIsPatch] = useIsPatch();
  const { error, result, warnings, archive, generate } = useGenerator();
  const [randomSettings, setRandomSettings] = useRandomSettings();

  const isRandomSettings = randomSettings.enabled;
  let isReady = !!romConfig.files.oot && !!romConfig.files.mm;
  if (isPatch) {
    isReady = isReady && !!romConfig.files.patch;
  }

  return <main>
    <h1>OoTMM Web Generator</h1>
    <h2>Version: {process.env.VERSION}</h2>
    {error && <div className="panel panel-error"><h2>Something went wrong</h2><p>{error}</p></div>}
    {result && <Result archive={archive} warnings={warnings}/>}
    <form className="rom-config-form"
      target="_self"
      onSubmit={(e) => {
        ReactGA.event('generate_seed');
        e.preventDefault();
        generate();
      }}
    >
      <div className="rom-config-files">
        <FileSelect
          logo="oot"
          label="Ocarina of Time (1.0, U or J)"
          accept=".z64, .n64, .v64"
          file={romConfig.files.oot}
          onChange={(f) => setRomConfigFile('oot', f)}
        />
        <FileSelect
          logo="mm"
          label="Majora's Mask (U only)"
          accept=".z64, .n64, .v64"
          file={romConfig.files.mm}
          onChange={(f) => setRomConfigFile('mm', f)}
        />
        {isPatch && (
          <FileSelect logo="ootmm" label="OoTMM Patch File" accept=".ootmm" file={romConfig.files.patch} onChange={(f) => setRomConfigFile('patch', f)} />
        )}
      </div>
      {!isPatch && <Checkbox label="Random Settings" checked={isRandomSettings} onChange={(x) => setRandomSettings({ enabled: x })} />}
        {isRandomSettings && (
          <>
            <Checkbox label="Random Settings: MQ" checked={randomSettings.mq} onChange={(x) => setRandomSettings({ mq: x })} />
            <Checkbox label="Random Settings: Entrances" checked={randomSettings.er} onChange={(x) => setRandomSettings({ er: x })} />
          </>
        )}
        {!isPatch && !isRandomSettings && (
          <>
            <PresetSelector />
            <SettingsImportExport />
          </>
        )}
        {!isRandomSettings && <Checkbox label="Use a patch file" checked={isPatch} onChange={setIsPatch} />}
        {!isPatch && !isRandomSettings && (
          <label>
            Seed (leave blank to auto-generate)
            <input type="text" value={romConfig.seed} onChange={(e) => setSeed(e.target.value)} />
          </label>
        )}
        <button disabled={!isReady} className="btn btn-primary" type="submit" style={{ width: '105px' }}>Generate</button>
    </form>
  </main>;
}
