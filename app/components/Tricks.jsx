import React from 'react';
import { TRICKS } from '@ootmm/core';

import { Checkbox } from './Checkbox';

export const Tricks = ({ settings, setSetting }) => {
  const { tricks } = settings;
  const ootTricks = Object.keys(tricks).filter(
    (x) => x.substring(0, 3) === 'OOT'
  );
  const mmTricks = Object.keys(tricks).filter(
    (x) => x.substring(0, 2) === 'MM'
  );

  const styles = {
    fistDiv: {
      'margin-bottom': '24px',
    },
  };

  return (
    <form className="settings">
      <h1>Ocarina of Time</h1>
      <div className="checkboxes" style={styles.fistDiv}>
        {ootTricks.map((trick) => (
          <Checkbox
            key={trick}
            label={TRICKS[trick]}
            checked={tricks[trick]}
            onChange={(v) => setSetting({ tricks: { ...tricks, [trick]: v } })}
          />
        ))}
      </div>
      <h1>Majora's Mask</h1>
      <div className="checkboxes">
        {mmTricks.map((trick) => (
          <Checkbox
            key={trick}
            label={TRICKS[trick]}
            checked={tricks[trick]}
            onChange={(v) => setSetting({ tricks: { ...tricks, [trick]: v } })}
          />
        ))}
      </div>
    </form>
  );
};
