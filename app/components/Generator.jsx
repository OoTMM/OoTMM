import React, { useState } from 'react';

import { Tab, TabBar } from './Tab';
import { RomConfig } from './RomConfig';


export const Generator = ({ onGenerate, error }) => {
  const [roms, setRoms] = useState({ oot: null, mm: null });
  const setRom = (game, data) => setRoms({ ...roms, [game]: data });

  return (
    <TabBar>
      <Tab name="ROM Config" component={<RomConfig setRom={setRom} error={error} onGenerate={() => onGenerate({ roms })}/>}/>
      <Tab name="Toto"/>
    </TabBar>
  );
};
