import clsx from 'clsx';
import { IconType } from 'react-icons';
import { LuHammer, LuDices, LuFileDiff } from 'react-icons/lu';

import { Setting, SettingsImportExport } from '@/app/components/settings';
import { useSettings } from '../../contexts/SettingsContext';
import { useRandomSettings, usePatchRandomSettings } from '../../contexts/RandomSettingsContext';
import { useGenerator, useRomConfig } from '../../contexts/GeneratorContext';
import { CheckboxField, FileSelectField, InputField, Button, RadioCardGroup, RadioCard, Label, Card } from '../ui';
import { PresetSelector } from '../PresetSelector';
import { Result } from '../Result';

import logoOot from '../../../assets/oot.png';
import logoMm from '../../../assets/mm.png';
import logoOotmm from '../../../assets/logo.png';

type ModeCardProps = {
  selected: boolean;
  onSelect: () => void;
  title: string;
  description: string;
  icon?: IconType;
};
function ModeCard({ selected, onSelect, title, description, icon: Icon }: ModeCardProps) {
  return (
    <RadioCard selected={selected} onSelect={onSelect}>
      <div className="flex items-center">
        <div className="flex-1 flex flex-col">
          <span className="font-bold">{title}</span>
          <span className="text-sm">{description}</span>
        </div>
        {Icon && <div className="opacity-20 mr-2">
          <Icon size={24}/>
        </div>}
      </div>
    </RadioCard>
  );
}

export function GeneratorGeneral() {
  const { romConfig, setRomConfigFile, setSeed, setMode } = useRomConfig();
  const { mode, seed } = romConfig;
  const { error, result, warnings, archive, generate } = useGenerator();
  const randomSettings = useRandomSettings();
  const patchRandomSettings = usePatchRandomSettings();
  const settings = useSettings();

  const isModeCreate = mode === 'create';
  const isModeRandom = mode === 'random';
  const isModePatch = mode === 'patch';

  let isReady = !!romConfig.files.oot && !!romConfig.files.mm;
  if (isModePatch) {
    isReady = isReady && !!romConfig.files.patch;
  }

  return (
    <main className="mt-8 flex flex-col items-center">
      {result && <Result archive={archive} warnings={warnings}/>}
      <Card className="w-[1024px] m-4 p-8 gap-4">
        <div className="m-auto mb-8 text-center">
          <div className="font-bold text-2xl">OoTMM Web Generator</div>
          <div>{process.env.VERSION}</div>
        </div>
        {error && <div className="panel panel-error"><h2>Something went wrong</h2><p>{error}</p></div>}
        <div className="m-auto flex gap-8">
          <FileSelectField imageSrc={logoOot} label="Ocarina of Time (1.0, U or J)" accept=".z64, .n64, .v64" file={romConfig.files.oot} onInput={(f) => setRomConfigFile('oot', f)}/>
          <FileSelectField imageSrc={logoMm} label="Majora's Mask (U only)" accept=".z64, .n64, .v64" file={romConfig.files.mm} onInput={(f) => setRomConfigFile('mm', f)}/>
        </div>
        <div className="mt-8 flex flex-col gap-6">
          <div>
            <span>Generator Mode</span>
            <RadioCardGroup>
              <ModeCard selected={isModeCreate} onSelect={() => setMode('create')} title="New Seed" description="Create a new seed with custom settings." icon={LuHammer}/>
              <ModeCard selected={isModeRandom} onSelect={() => setMode('random')} title="Random Settings" description="Play a seed where the settings themselves are random." icon={LuDices}/>
              <ModeCard selected={isModePatch} onSelect={() => setMode('patch')} title="Patch" description="Recreate an existing seed using a patch file." icon={LuFileDiff}/>
            </RadioCardGroup>
          </div>

          {isModeCreate && <>
            <PresetSelector />
            <SettingsImportExport />
            <InputField className={clsx(romConfig.seed ? 'font-mono' : '')} type="text" label="Seed" placeholder="Leave blank to auto-generate" value={romConfig.seed} onChange={setSeed}/>
          </>}

          {isModeRandom && (
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

          {isModePatch && <FileSelectField imageSrc={logoOotmm} label="Patch File" accept=".ootmm" file={romConfig.files.patch} onInput={(f) => setRomConfigFile('patch', f)} />}

          <Button variant="submit" disabled={!isReady} onClick={generate}>Generate</Button>
        </div>
      </Card>
    </main>
  );
}
