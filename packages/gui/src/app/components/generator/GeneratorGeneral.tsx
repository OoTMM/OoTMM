import { useSettings } from '../../contexts/SettingsContext';
import { useRandomSettings, usePatchRandomSettings } from '../../contexts/RandomSettingsContext';
import { useGenerator, useIsPatch, useRomConfig } from '../../contexts/GeneratorContext';
import { CheckboxField, FileSelectField, InputField } from '../ui';
import { PresetSelector } from '../PresetSelector';
import { SettingsImportExport } from '../SettingsImportExport';
import { Result } from '../Result';
import { Setting } from '../SettingsEditor';
import { Button, Select } from '../ui';

import logoOot from '../../../assets/oot.png';
import logoMm from '../../../assets/mm.png';
import logoOotmm from '../../../assets/logo.png';

export function GeneratorGeneral() {
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
    <main className="mt-8 flex flex-col items-center">
      <div className="bg-gray-100 dark:bg-gray-800 w-[1024px] mt-4 p-4 rounded border dark:border-slate-700 flex flex-col gap-4">
        <div className="m-auto text-center">
          <div className="font-bold text-2xl">OoTMM Web Generator</div>
          <div>{process.env.VERSION}</div>
        </div>
        {error && <div className="panel panel-error"><h2>Something went wrong</h2><p>{error}</p></div>}
        {result && <Result archive={archive} warnings={warnings}/>}
        <div className="m-auto flex gap-8">
          <FileSelectField imageSrc={logoOot} label="Ocarina of Time (1.0, U or J)" accept=".z64, .n64, .v64" file={romConfig.files.oot} onInput={(f) => setRomConfigFile('oot', f)}/>
          <FileSelectField imageSrc={logoMm} label="Majora's Mask (U only)" accept=".z64, .n64, .v64" file={romConfig.files.mm} onInput={(f) => setRomConfigFile('mm', f)}/>
          {isPatch && (<FileSelectField imageSrc={logoOotmm} label="OoTMM Patch File" accept=".ootmm" file={romConfig.files.patch} onInput={(f) => setRomConfigFile('patch', f)} />)}
        </div>
        {!isPatch && <CheckboxField label="Random Settings" checked={isRandomSettings} onChange={(x) => patchRandomSettings({ enabled: x })} />}
          {isRandomSettings && (
            <div>
              <Setting setting='games'/>
              <Setting setting='mode'/>
              <Setting setting='players'/>
              <Setting setting='teams'/>
              {settings.games !== 'mm' && <CheckboxField label="Random Settings: Master Quest" checked={randomSettings.mq} onChange={(x) => patchRandomSettings({ mq: x })} />}
              {settings.games !== 'oot' && <CheckboxField label="Random Settings: MM JP Layouts" checked={randomSettings.jp} onChange={(x) => patchRandomSettings({ jp: x })} />}
              <CheckboxField label="Random Settings: Entrances" checked={randomSettings.er} onChange={(x) => patchRandomSettings({ er: x })} />
              <CheckboxField label="Random Settings: Extra Shuffles" checked={randomSettings.extraShuffles} onChange={(x) => patchRandomSettings({ extraShuffles: x })} />
            </div>
          )}
          {!isPatch && !isRandomSettings && (
            <>
              <PresetSelector />
              <SettingsImportExport />
            </>
          )}
          {!isRandomSettings && <CheckboxField label="Use a patch file" checked={isPatch} onChange={setIsPatch} />}
          {!isPatch && !isRandomSettings && (
            <InputField type="text" label="Seed" placeholder="Leave blank to auto-generate" value={romConfig.seed} onInput={(e) => setSeed(e.currentTarget.value)}/>
          )}
          <Button disabled={!isReady}>Generate</Button>
      </div>
    </main>
  );
}
