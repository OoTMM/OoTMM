import React from 'react';
import { COLORS, COSMETICS, Cosmetics } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { useCosmetics } from '../contexts/GeneratorContext';
import { FileSelect } from './FileSelect';
import { Checkbox } from './Checkbox';

const COLOR_OPTIONS: { name: string, value: string}[] = [{ value: 'default', name: 'Default' }, { value: 'auto', name: 'Auto' }, { value: 'random', name: 'Random' }, ...Object.entries(COLORS).map(([key, x]) => ({ name: x.name, value: key }))];

function Cosmetic({ cosmetic }: { cosmetic: keyof Cosmetics }) {
  const [cosmetics, setCosmetics] = useCosmetics();
  const data = COSMETICS.find(x => x.key === cosmetic)!;

  switch (data.type) {
  case 'color':
    return (
      <Dropdown
        key={cosmetic}
        value={cosmetics[cosmetic] as string}
        label={data.name}
        options={COLOR_OPTIONS}
        onChange={v => setCosmetics({ [cosmetic]: v })}
      />
    );
  case 'zobj':
    return (
      <FileSelect
        logo="oot"
        label={data.name}
        accept=".zobj"
        file={`cosmetics.${cosmetic}`}
        onChange={(f) => setCosmetics({ [cosmetic]: f })}
      />
    );
  case 'zip':
    return (
      <FileSelect
        logo="oot"
        label={data.name}
        accept=".zip"
        file={`cosmetics.${cosmetic}`}
        onChange={(f) => setCosmetics({ [cosmetic]: f })}
      />
    );
  case 'boolean':
    return (
      <Checkbox
        label={data.name}
        checked={!!(cosmetics[cosmetic])}
        onChange={(v) => setCosmetics({ [cosmetic]: v })}
      />
    );
  default:
    return null;
  }
}

export function CosmeticsEditor() {
  const options: { name: string, value: string}[] = Object.entries(COLORS).map(([key, x]) => ({ name: x.name, value: key }));
  options.push({ name: "Random", value: "random" });
  const fileTypes = ['zobj', 'zip'];
  const nonFiles = COSMETICS.filter(c => !fileTypes.includes(c.type));
  const files = COSMETICS.filter(c => fileTypes.includes(c.type));

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
