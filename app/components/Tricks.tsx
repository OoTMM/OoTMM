import React from 'react';
import { TRICKS } from '@ootmm/core';

import { useSettings } from '../contexts/GeneratorContext';
import { ArrayList } from './ArrayList';

const NAMES = {
  OOT: 'Ocarina of Time',
  MM: 'Majora\'s Mask',
};

export function GameTricks({ game }: { game: keyof typeof NAMES }) {
  const [settings, setSettings] = useSettings();
  const tricks = Object.keys(TRICKS).filter(x => x.startsWith(game));
  const options = tricks.map(trick => ({ value: trick, label: TRICKS[trick as keyof typeof TRICKS] }));

  const add = (trick: string) => {
    setSettings({ tricks: { add: [trick as keyof typeof TRICKS] } });
  };

  const remove = (trick: string) => {
    setSettings({ tricks: { remove: [trick as keyof typeof TRICKS] } });
  }

  return (
    <div>
      <h1>{NAMES[game]}</h1>
      <ArrayList options={options} selected={settings.tricks.filter(x => tricks.includes(x))} add={add} remove={remove}/>
    </div>
  );
}

export function Tricks() {
  const [_, setSettings] = useSettings();
  const clear = () => {
    setSettings({ tricks: { set: [] } });
  }

  return (
    <div>
      <button className="btn-danger" onClick={clear}>Remove All</button>
      <div className="two-column-grid">
        <GameTricks game="OOT"/>
        <GameTricks game="MM"/>
      </div>
    </div>
  )
}
