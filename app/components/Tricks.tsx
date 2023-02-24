import React from 'react';
import { TRICKS, Settings } from '@ootmm/core';

import { Checkbox } from './Checkbox';

type TricksProps = {
  settings: Settings;
  setSetting: (settings: Partial<Settings>) => void;
};

export const Tricks = ({ settings, setSetting }: TricksProps) => {
  const { tricks } = settings;
  const ootTricks = Object.keys(tricks).filter(
    (x) => x.substring(0, 3) === 'OOT'
  );
  const mmTricks = Object.keys(tricks).filter(
    (x) => x.substring(0, 2) === 'MM'
  );

  return (
    <form className="settings">
      <h2>Ocarina of Time</h2>
      <div className="three-column-grid">
        {ootTricks.map((trick) => (
          <Checkbox
            key={trick}
            label={(TRICKS as {[k: string]: string})[trick]}
            checked={(tricks as {[k: string]: boolean})[trick]}
            onChange={(v) => setSetting({ tricks: { ...tricks, [trick]: v } })}
          />
        ))}
      </div>
      <h2 className="section-margin-top">Majora's Mask</h2>
      <div className="three-column-grid">
        {mmTricks.map((trick) => (
          <Checkbox
            key={trick}
            label={(TRICKS as {[k: string]: string})[trick]}
            checked={(tricks as {[k: string]: boolean})[trick]}
            onChange={(v) => setSetting({ tricks: { ...tricks, [trick]: v } })}
          />
        ))}
      </div>
    </form>
  );
};
