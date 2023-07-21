import React from 'react';
import { COLORS, COSMETICS, Cosmetics } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { useCosmetics } from '../contexts/GeneratorContext';
import { FileSelect } from './FileSelect';

const COLOR_OPTIONS: { name: string, value: string}[] = [{ value: 'random', name: 'Random' }, ...Object.entries(COLORS).map(([key, x]) => ({ name: x.name, value: key }))];

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
  default:
    return null;
  }
}

export function CosmeticsEditor() {
  const options: { name: string, value: string}[] = Object.entries(COLORS).map(([key, x]) => ({ name: x.name, value: key }));
  options.push({ name: "Random", value: "random" });

  return (
    <form className="settings">
      <div className="three-column-grid">
        {COSMETICS.map(c => <Cosmetic key={c.key} cosmetic={c.key}/>)}
      </div>
    </form>
  )
}
