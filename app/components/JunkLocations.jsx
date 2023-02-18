import React from 'react';
import { TransferList } from './TransferList';



export const JunkLocations = ({ settings, setSetting, locList }) => {
  return (
    <TransferList label='junkLocations' locList={locList} settings={settings} setSetting={setSetting} />
  );
}