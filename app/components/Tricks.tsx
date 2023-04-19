import React from 'react';
import { TRICKS } from '@ootmm/core';

import { Checkbox } from './Checkbox';
import { useSettings } from '../contexts/GeneratorContext';

export function Tricks() {
  const [settings, setSettings] = useSettings();
  const { tricks } = settings;

  const ootTricks = Object.keys(TRICKS).filter(x => x.startsWith('OOT'));
  const mmTricks = Object.keys(TRICKS).filter(x => x.startsWith('MM'));

  const changeTrick = (trick: string, value: boolean) => {
    if (value) {
      setSettings({ tricks: { add: [trick as any] } });
    } else {
      setSettings({ tricks: { remove: [trick as any] } });
    }
  };

  return (
    <form className="settings">
      <h2>Ocarina of Time</h2>
      <div className="three-column-grid">
        {ootTricks.map((trick) => (
          <Checkbox
            key={trick}
            label={(TRICKS as {[k: string]: string})[trick]}
            checked={tricks.includes(trick as any)}
            onChange={v => changeTrick(trick, v)}
          />
        ))}
      </div>
      <h2 className="section-margin-top">Majora's Mask</h2>
      <div className="three-column-grid">
        {mmTricks.map((trick) => (
          <Checkbox
            key={trick}
            label={(TRICKS as {[k: string]: string})[trick]}
            checked={tricks.includes(trick as any)}
            onChange={v => changeTrick(trick, v)}
          />
        ))}
      </div>
    </form>
  );
}
