import ReactGA from 'react-ga4';

import { useSettings } from '../contexts/SettingsContext';
import { useRandomSettings, usePatchRandomSettings } from '../contexts/RandomSettingsContext';
import { useGenerator, useIsPatch, useRomConfig } from '../contexts/GeneratorContext';
import { CheckboxField } from './ui/CheckboxField';
import { FileSelect } from './FileSelect';
import { PresetSelector } from './PresetSelector';
import { SettingsImportExport } from './SettingsImportExport';
import { Result } from './Result';
import { Setting } from './SettingsEditor';
import { Button } from './ui';

export function RomConfig() {
  const { romConfig, setRomConfigFile, setSeed } = useRomConfig();
  const [isPatch, setIsPatch] = useIsPatch();
  const { error, result, warnings, archive, generate } = useGenerator();
  const randomSettings = useRandomSettings();
  const patchRandomSettings = usePatchRandomSettings();
  const settings = useSettings();

  const isRandomSettings = randomSettings.enabled;
  let isReady = !!romConfig.files.oot && !!romConfig.files.mm;
  if (isPatch) {
    isReady = isReady && !!romConfig.files.patch;
  }

  return (
    <main className="rom-config-wrapper">
      <div className="rom-config">
        <div className="rom-config-header">
          <span className="rom-config-header-title">OoTMM Web Generator</span>
          <span className="rom-config-header-version">{process.env.VERSION}</span>
        </div>
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
          {!isPatch && <CheckboxField label="Random Settings" checked={isRandomSettings} onChange={(x) => patchRandomSettings({ enabled: x })} />}
            {isRandomSettings && (
              <>
                <Setting setting='games'/>
                <Setting setting='mode'/>
                <Setting setting='players'/>
                <Setting setting='teams'/>
                {settings.games !== 'mm' && <CheckboxField label="Random Settings: Master Quest" checked={randomSettings.mq} onChange={(x) => patchRandomSettings({ mq: x })} />}
                {settings.games !== 'oot' && <CheckboxField label="Random Settings: MM JP Layouts" checked={randomSettings.jp} onChange={(x) => patchRandomSettings({ jp: x })} />}
                <CheckboxField label="Random Settings: Entrances" checked={randomSettings.er} onChange={(x) => patchRandomSettings({ er: x })} />
                <CheckboxField label="Random Settings: Extra Shuffles" checked={randomSettings.extraShuffles} onChange={(x) => patchRandomSettings({ extraShuffles: x })} />
              </>
            )}
            {!isPatch && !isRandomSettings && (
              <>
                <PresetSelector />
                <SettingsImportExport />
              </>
            )}
            {!isRandomSettings && <CheckboxField label="Use a patch file" checked={isPatch} onChange={setIsPatch} />}
            {!isPatch && !isRandomSettings && (
              <label>
                Seed (leave blank to auto-generate)
                <input type="text" value={romConfig.seed} onInput={(e) => setSeed(e.currentTarget.value)} />
              </label>
            )}
            <Button type="submit" disabled={!isReady}>Generate</Button>
        </form>
      </div>
    </main>
  );
}
