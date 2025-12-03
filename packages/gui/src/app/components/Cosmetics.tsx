import { COLORS, COSMETICS, Cosmetics } from '@ootmm/core';

import { CheckboxField } from './ui/CheckboxField';
import { FileSelectField, SelectField } from './ui';

import ootmmLogo from '../../assets/logo.png';
import { useStore } from '../store';

const COLOR_OPTIONS: { label: string, value: string }[] = [{ value: 'default', label: 'Default' }, { value: 'auto', label: 'Auto' }, { value: 'random', label: 'Random' }, ...Object.entries(COLORS).map(([key, x]) => ({ label: x.name, value: key }))];

function CosmeticTooltips({ cosmetic }: { cosmetic: string }) {
  const data = COSMETICS.find(x => x.key === cosmetic)!;
  const description = (data as any).description;

  let defaultValue = '';

  if (!description) {
    return null;
  }

  switch(data.type) {
    case 'boolean':
      defaultValue = data.default ? 'true' : 'false';
      break;
    case 'color':
      defaultValue = 'Default';
      break;
  }

  return <>
    <pre className="whitespace-pre-line flex flex-col gap-4">
      {description.split('<br>').join('\n')}
      <span>Default: <strong>{defaultValue}</strong></span>
    </pre>
  </>;
}

function Cosmetic({ cosmetic }: { cosmetic: keyof Cosmetics }) {
  const cosmetics = useStore(state => state.cosmetics);
  const setCosmetic = useStore(state => state.setCosmetic);
  const data = COSMETICS.find(x => x.key === cosmetic)!;

  switch (data.type) {
    case 'color':
      return (
        <SelectField
          key={cosmetic}
          value={cosmetics[cosmetic] as string}
          label={data.name}
          options={COLOR_OPTIONS}
          tooltip={(data as any).description && <CosmeticTooltips cosmetic={cosmetic} />}
          onSelect={v => setCosmetic(cosmetic, v)}
        />
      );
    case 'file':
      return (
        <FileSelectField
          imageSrc={ootmmLogo}
          label={data.name}
          accept={`.${data.ext}`}
          file={cosmetics[cosmetic] as File | null}
          onInput={(f) => setCosmetic(cosmetic, f)}
        />
      );
    case 'boolean':
      return (
        <CheckboxField
          label={data.name}
          checked={!!(cosmetics[cosmetic])}
          tooltip={(data as any).description && <CosmeticTooltips cosmetic={cosmetic} />}
          onChange={(v) => setCosmetic(cosmetic, v)}
        />
      );
    default:
      return null;
  }
}

export function CosmeticsEditor() {
  const options: { name: string, value: string }[] = Object.entries(COLORS).map(([key, x]) => ({ name: x.name, value: key }));
  options.push({ name: "Random", value: "random" });
  const nonFiles = COSMETICS.filter(c => c.type !== 'file');
  const files = COSMETICS.filter(c => c.type === 'file');

  return <main className="p-8">
    <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
      {nonFiles.map(c => <Cosmetic key={c.key} cosmetic={c.key} />)}
    </div>
    <div className="flex gap-16 mt-16 justify-center">
      {files.map(c => <Cosmetic key={c.key} cosmetic={c.key} />)}
    </div>
  </main>;
}
