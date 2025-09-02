import { COLORS, COSMETICS, Cosmetics } from '@ootmm/core';

import { useCosmetics, useSetCosmetic } from '../contexts/CosmeticsContext';
import { CheckboxField } from './ui/CheckboxField';

import ootmmLogo from '../../assets/logo.png';
import { FileSelectField, SelectField } from './ui';

const COLOR_OPTIONS: { label: string, value: string}[] = [{ value: 'default', label: 'Default' }, { value: 'auto', label: 'Auto' }, { value: 'random', label: 'Random' }, ...Object.entries(COLORS).map(([key, x]) => ({ label: x.name, value: key }))];

function Cosmetic({ cosmetic }: { cosmetic: keyof Cosmetics }) {
  const cosmetics = useCosmetics();
  const setCosmetic = useSetCosmetic();
  const data = COSMETICS.find(x => x.key === cosmetic)!;

  switch (data.type) {
  case 'color':
    return (
      <SelectField
        key={cosmetic}
        value={cosmetics[cosmetic] as string}
        label={data.name}
        options={COLOR_OPTIONS}
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
        onChange={(v) => setCosmetic(cosmetic, v)}
      />
    );
  default:
    return null;
  }
}

export function CosmeticsEditor() {
  const options: { name: string, value: string}[] = Object.entries(COLORS).map(([key, x]) => ({ name: x.name, value: key }));
  options.push({ name: "Random", value: "random" });
  const nonFiles = COSMETICS.filter(c => c.type !== 'file');
  const files = COSMETICS.filter(c => c.type === 'file');

  return <main className="p-8">
    <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
      {nonFiles.map(c => <Cosmetic key={c.key} cosmetic={c.key}/>)}
    </div>
    <div className="flex gap-16 mt-16 justify-center">
      {files.map(c => <Cosmetic key={c.key} cosmetic={c.key}/>)}
    </div>
  </main>;
}
