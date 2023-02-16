import React from 'react';

const styles = {
    div: {
      display: 'flex',
      'align-items': 'center',
      'flex-direction': 'row',
      with: '100%',
      width: '40%',
      'background-color': '#f0f0f0'
    },
  };

  export const TransferList = ({ label, locList, settings, setSettings }) => {
    const [selected, setSelected] = React.useState([]);


    return (
        <div style={styles.div}>
        </div>
    );
  }