import React from 'react';
import Select from 'react-select';
import { locationList } from '@ootmm/core';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { solid } from '@fortawesome/fontawesome-svg-core/import.macro';

import { useSettings } from '../contexts/GeneratorContext';

export function JunkLocations() {
  const [settings, setSettings] = useSettings();
  const locs = Object.keys(locationList(settings)).filter(x => !settings.junkLocations.includes(x)).sort();
  const locsOptions = locs.map(loc => ({ value: loc, label: loc }));

  const add = (loc: string) => {
    setSettings({ junkLocations: { add: [loc] } });
  };

  const remove = (loc: string) => {
    setSettings({ junkLocations: { remove: [loc] } });
  }

  const clear = () => {
    setSettings({ junkLocations: { set: [] } });
  }

  return (
    <div className='plando'>
      <div className='plando-search'>
        <Select className='plando-select' options={locsOptions} onChange={(v) => v && add(v.value)}/>
        <button className="btn-danger" onClick={clear}>Remove All</button>
      </div>
      <ol className='plando-list'>
        {settings.junkLocations.map(location => (
          <li key={location}>
            <span className="plando-remove" onClick={() => remove(location)}><FontAwesomeIcon icon={solid('xmark')}/></span>
            <span className="plando-item">{location}</span>
          </li>
        ))}
      </ol>
    </div>
  );

  /* <span className="plando-remove" onClick={() => removeItem(loc)}><FontAwesomeIcon icon={solid('xmark')}/></span> */
}

