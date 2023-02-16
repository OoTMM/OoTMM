import React from 'react';
import { TransferList } from './TransferList';



export const JunkLocations = ({ settings, setSettings, locList }) => {
    return (<TransferList label='junkLocations' locList={locList} settings={settings} setSettings={setSettings} />);
}