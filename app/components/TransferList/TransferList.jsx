import React from 'react';
import { Scrollbox } from './Scrollbox';

// const styles = {
//     div: {
//       display: 'flex',
//       'align-items': 'center',
//       'flex-direction': 'row',
//       with: '100%',
//       width: '40%',
//       'background-color': '#f0f0f0'
//     },
//   };

  export const TransferList = ({ label, locList, settings, setSettings }) => {
    // params
    // label: name of the setting in the settings object
    // locList: full list of locations
    // settings: setting object
    // setSettings: hook to change the settings state variable
    const [selected, setSelected] = React.useState([]);


    return (
        <Scrollbox width="40%" height="400px">
          {locList}
        </Scrollbox>
    );
  }