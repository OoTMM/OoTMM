import React from 'react';

// const styles = {
//     div: {
//       display: 'flex',
//       'align-items': 'center',
//       'flex-direction': 'row',
//       with: '100%',
//     },
//   };

  export const Scrollbox = ({ width, height }) => {
    const [selected, setSelected] = React.useState([]);

    const boxStyle = {
        div: {
            width: width,
            height: height,
            'overflow-y': 'auto',
            'background-color': '#f0f0f0',
        }
    }

    return (
        <div style={boxStyle.div}>
            
        </div>
    );
  }