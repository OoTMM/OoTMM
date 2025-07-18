import { COLORS, COSMETICS, Cosmetics } from '@ootmm/core';

import { useCosmetics, useSetCosmetic } from '../contexts/CosmeticsContext';
import { Dropdown } from './Dropdown';
import { FileSelect } from './FileSelect';
import { CheckboxField } from './ui/CheckboxField';

const COLOR_OPTIONS: { name: string, value: string}[] = [{ value: 'default', name: 'Default' }, { value: 'auto', name: 'Auto' }, { value: 'random', name: 'Random' }, ...Object.entries(COLORS).map(([key, x]) => ({ name: x.name, value: key }))];

function Cosmetic({ cosmetic }: { cosmetic: keyof Cosmetics }) {
  const cosmetics = useCosmetics();
  const setCosmetic = useSetCosmetic();
  const data = COSMETICS.find(x => x.key === cosmetic)!;

  switch (data.type) {
  case 'color':
    return (
      <Dropdown
        key={cosmetic}
        value={cosmetics[cosmetic] as string}
        label={data.name}
        options={COLOR_OPTIONS}
        onInput={v => setCosmetic(cosmetic, v)}
      />
    );
  case 'file':
    return (
      <FileSelect
        logo="oot"
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

  return <main>
    <h1>Cosmetics</h1>
    <form>
      {nonFiles.map(c => <Cosmetic key={c.key} cosmetic={c.key}/>)}
    </form>
    <form>
      {files.map(c => <Cosmetic key={c.key} cosmetic={c.key}/>)}
    </form>
  </main>;
}
