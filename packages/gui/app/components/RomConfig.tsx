import ReactGA from 'react-ga4';

import { useGenerator, useIsPatch, useRandomSettings, useRomConfig, useSettings } from '../contexts/GeneratorContext';
import { Checkbox } from './Checkbox';
import { FileSelect } from './FileSelect';
import { PresetSelector } from './PresetSelector';
import { SettingsImportExport } from './SettingsImportExport';
import { Result } from './Result';
import { Setting } from './SettingsEditor';

export function RomConfig() {
  const { romConfig, setRomConfigFile, setSeed } = useRomConfig();
  const [isPatch, setIsPatch] = useIsPatch();
  const { error, result, warnings, archive, generate } = useGenerator();
  const [randomSettings, setRandomSettings] = useRandomSettings();
  const [settings] = useSettings();

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
          onInput={(f) => setRomConfigFile('oot', f)}
        />
        <FileSelect
          logo="mm"
          label="Majora's Mask (U only)"
          accept=".z64, .n64, .v64"
          file={romConfig.files.mm}
          onInput={(f) => setRomConfigFile('mm', f)}
        />
        {isPatch && (
          <FileSelect logo="ootmm" label="OoTMM Patch File" accept=".ootmm" file={romConfig.files.patch} onInput={(f) => setRomConfigFile('patch', f)} />
        )}
      </div>
      {!isPatch && <Checkbox label="Random Settings" checked={isRandomSettings} onInput={(x) => setRandomSettings({ enabled: x })} />}
        {isRandomSettings && (
          <>
            <Setting setting='games'/>
            <Setting setting='mode'/>
            <Setting setting='players'/>
            <Setting setting='teams'/>
            {settings.games !== 'mm' && <Checkbox label="Random Settings: Master Quest" checked={randomSettings.mq} onInput={(x) => setRandomSettings({ mq: x })} />}
            {settings.games !== 'oot' && <Checkbox label="Random Settings: MM JP Layouts" checked={randomSettings.jp} onInput={(x) => setRandomSettings({ jp: x })} />}
            <Checkbox label="Random Settings: Entrances" checked={randomSettings.er} onInput={(x) => setRandomSettings({ er: x })} />
            <Checkbox label="Random Settings: Extra Shuffles" checked={randomSettings.extraShuffles} onInput={(x) => setRandomSettings({ extraShuffles: x })} />
          </>
        )}
        {!isPatch && !isRandomSettings && (
          <>
            <PresetSelector />
            <SettingsImportExport />
          </>
        )}
        {!isRandomSettings && <Checkbox label="Use a patch file" checked={isPatch} onInput={setIsPatch} />}
        {!isPatch && !isRandomSettings && (
          <label>
            Seed (leave blank to auto-generate)
            <input type="text" value={romConfig.seed} onInput={(e) => setSeed(e.currentTarget.value)} />
          </label>
        )}
        <button disabled={!isReady} className="btn btn-primary" type="submit" style={{ width: '105px' }}>Generate</button>
    </form>
  </main>;
}
