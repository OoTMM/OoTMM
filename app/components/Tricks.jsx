import React from 'react';
import { TRICKS } from '@ootmm/core';

import { Checkbox } from './Checkbox';

export const Tricks = ({ settings, setSetting }) => {
  const { tricks } = settings;

  return (
    <form className='settings'>
      {Object.keys(tricks).map(trick =>
        <Checkbox
          key={trick}
          label={TRICKS[trick]}
          checked={tricks[trick]}
          onChange={v => setSetting({ tricks: { ...tricks, [trick]: v } }) }
        />
      )}
    </form>
  );
};
