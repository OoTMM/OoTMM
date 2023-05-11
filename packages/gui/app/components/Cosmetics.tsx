import React from 'react';
import { COLORS, COSMETIC_NAMES } from '@ootmm/core';

import { Dropdown } from './Dropdown';
import { useCosmetics } from '../contexts/GeneratorContext';

export function Cosmetics() {
  const [cosmetics, setCosmetics] = useCosmetics();
  const options: { name: string, value: string}[] = Object.entries(COLORS).map(([key, x]) => ({ name: x.name, value: key }));
  options.push({ name: "Random", value: "random" });

  return (
    <form className="settings">
      <div className="three-column-grid">
        {(Object.keys(COSMETIC_NAMES) as Array<keyof typeof COSMETIC_NAMES>).map(key =>
          <Dropdown
            key={key}
            value={cosmetics[key]}
            label={COSMETIC_NAMES[key]}
            options={options}
            onChange={v => setCosmetics({ [key]: v })}
          />
        )}
      </div>
    </form>
  )
}
