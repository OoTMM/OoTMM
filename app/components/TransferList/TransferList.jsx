import React from 'react';
import { Scrollbox } from './Scrollbox';
import { ListElement } from './ListElement';
import './transferlist.css';

const intersection = (a, b) => {
  return a.filter((v) => b.indexOf(v) !== -1);
}

export const TransferList = ({ label, locList, settings, setSetting }) => {
  // params
  // label: name of the setting in the settings object
  // locList: full list of locations
  // settings: setting object
  // setSettings: hook to change the settings state variable
  const [searchString, setSearchString] = React.useState('');
  const [selected, setSelected] = React.useState([]);
  const [left, setLeft] = React.useState(() => {
    const newLocList = [...Object.keys(locList)];
    if (settings[label].length === 0) {
      return newLocList;
    } else {
      for (const loc of settings[label]) {
        const idx = newLocList.indexOf(loc);
        if (idx === -1) {
          console.log(`Error: ${loc} from ${label} not found in location list.`);
        } else {
          newLocList.splice(idx, 1);
        }
      }
      return newLocList;
    }
  });

  const handleSelect = (loc) => {
    const newSelected = [ ...selected ];
    const idx = newSelected.indexOf(loc);
    if (idx === -1) {
      newSelected.push(loc);
    } else {
      newSelected.splice(idx, 1);
    }
    setSelected(newSelected);
  };

  const buildListElements = (locations) => (
    <Scrollbox width="50%" height="400px">
      {filterList(locations).map((loc) => (
        <ListElement
          key={loc}
          label={loc}
          selected={selected.includes(loc)}
          onClick={() => handleSelect(loc)}
        />
      ))}
    </Scrollbox>
  );

  const moveSelectedRight = () => {
    const toMove = intersection(left, selected);
    const newRight = [ ...toMove, ...settings[label] ];
    const newLeft = left.filter((v) => toMove.indexOf(v) === -1);
    const newSelected = selected.filter((v) => toMove.indexOf(v) === -1);
    setSetting({ [label]: newRight });
    setLeft(newLeft);
    setSelected(newSelected);
  };

  const moveSelectedLeft = () => {
    const toMove = intersection(settings[label], selected);
    const newLeft = [ ...toMove, ...left ];
    const newRight = settings[label].filter((v) => toMove.indexOf(v) === -1);
    const newSelected = selected.filter((v) => toMove.indexOf(v) === -1);
    setSetting({ [label]: newRight });
    setLeft(newLeft);
    setSelected(newSelected);
  };

  const resetLists = () => {
    setLeft(Object.keys(locList));
    setSetting({ [label]: [] });
    setSelected([]);
  };

  const filterList = (locations) => {
    return locations.filter((v) => v.toLowerCase().includes(searchString.toLowerCase()));
  };

  return (
    <div className='transfer-list'>
      <div className='tl-buttons'>
        <button
          className="tl-button"
          onClick={() => moveSelectedRight()}
          disabled={intersection(left, selected).length === 0}
        >Add {"\u25b6"}</button>
        <button
          className="tl-button"
          onClick={() => resetLists()}
        >Reset</button>
        <button
          className="tl-button"
          onClick={() => moveSelectedLeft()}
          disabled={intersection(settings[label], selected).length === 0}
        >{"\u25c0"} Remove</button>
      </div>
      <div className='tl-lists'>
          {buildListElements(left)}
          {buildListElements(settings[label])}
      </div>
      <div className='tl-filters'>
        <input type="text" className='tl-text-input' value={searchString}
          onChange={e => setSearchString(e.target.value)}
        />
        <button className="filter-clear" onClick={e => setSearchString('')}
          hidden={searchString === ''}
        >X</button>
      </div>
    </div>
  );
}