import Select from 'react-select';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faXmark, faArrowUp, faArrowDown } from '@fortawesome/free-solid-svg-icons';

import { useItemPool, useSettings } from '../contexts/GeneratorContext';
import { Dropdown } from './Dropdown';
import { HINT_TYPES, SETTINGS_DEFAULT_HINTS, SettingHint, itemName } from '@ootmm/core';
import { InputNumber } from './InputNumber';
import { Checkbox } from './Checkbox';
import { SettingsPanel } from './SettingsEditor';

const hintOptions: { name: string; value: string }[] = [];
for (const k in HINT_TYPES) {
  hintOptions.push({ name: HINT_TYPES[k as keyof typeof HINT_TYPES], value: k });
}

type HintEditorProps = {
  index: number;
};
export function HintEditor({ index }: HintEditorProps) {
  const [settings, setSettings] = useSettings();
  const itemPool = useItemPool();

  const itemOptions = Object.keys(itemPool).map((item) => ({ value: item, label: itemName(item) }));
  const hint = settings.hints[index];

  const updateHint = (h: SettingHint) => {
    const newArray = [...settings.hints];
    newArray[index] = h;
    setSettings({ hints: { set: newArray } });
  };

  const onInputType = (type: keyof typeof HINT_TYPES) => {
    const newHint = { ...hint, type };
    if (type === 'item') {
      newHint.item = 'MM_SONG_SOARING';
    } else {
      delete newHint.item;
    }
    updateHint(newHint);
  };

  const onInputAmount = (amount: number) => {
    const newHint = { ...hint, amount };
    updateHint(newHint);
  };

  const onInputExtra = (extra: number) => {
    const newHint = { ...hint, extra };
    updateHint(newHint);
  };

  const onInputMax = (max: boolean) => {
    const newHint = { ...hint };
    newHint.amount = max ? 'max' : 1;
    updateHint(newHint);
  };

  const onInputItem = (item?: string) => {
    if (!item) return;
    const newHint = { ...hint, item };
    updateHint(newHint);
  };

  const onRemove = () => {
    const newArray = [...settings.hints];
    newArray.splice(index, 1);
    setSettings({ hints: { set: newArray } });
  };

  const onMoveUp = () => {
    if (index === 0) return;
    const newArray = [...settings.hints];
    const temp = newArray[index - 1];
    newArray[index - 1] = newArray[index];
    newArray[index] = temp;
    setSettings({ hints: { set: newArray } });
  };

  const onMoveDown = () => {
    if (index === settings.hints.length - 1) return;
    const newArray = [...settings.hints];
    const temp = newArray[index + 1];
    newArray[index + 1] = newArray[index];
    newArray[index] = temp;
    setSettings({ hints: { set: newArray } });
  };

  const selectedItem = itemOptions.find((x) => x.value === hint.item);

  return (
    <tr className="dashboard-table">
      <td>
        <span className="list-remove" onClick={onRemove}>
          <FontAwesomeIcon icon={faXmark} />
        </span>
        <span className="list-action" onClick={onMoveUp}>
          <FontAwesomeIcon icon={faArrowUp} />
        </span>
        <span className="list-action" onClick={onMoveDown}>
          <FontAwesomeIcon icon={faArrowDown} />
        </span>
      </td>
      <td>
        <Dropdown options={hintOptions} value={hint.type} onInput={onInputType as any} />
      </td>
      <td>
        {hint.type === 'item' && (
          <Select className="react-select-container" classNamePrefix="react-select" options={itemOptions} onChange={(v) => onInputItem(v?.value)} value={selectedItem}/>
        )}
      </td>
      <td>{hint.amount !== 'max' && <InputNumber value={hint.amount} onInput={onInputAmount} />}</td>
      <td>
        <Checkbox checked={hint.amount === 'max'} onInput={onInputMax} />
      </td>
      <td>
        <InputNumber value={hint.extra} onInput={onInputExtra} />
      </td>
    </tr>
  );
}

export function Hints() {
  const [settings, setSettings] = useSettings();

  const onNew = () => {
    const newArray = [...settings.hints];
    newArray.push({ type: 'junk', amount: 1, extra: 0 });
    setSettings({ hints: { set: newArray } });
  };

  return (
    <main>
      <h1>Hints</h1>
      <SettingsPanel category='hints'/>
      <>
        <button className="btn" onClick={onNew}>
          New
        </button>
        <button className="btn-danger" onClick={() => setSettings({ hints: { set: [] } })}>
          Remove All
        </button>
        <button className="btn-danger" onClick={() => setSettings({ hints: { set: SETTINGS_DEFAULT_HINTS } })}>
          Reset
        </button>
      </>

      <table className="hints">
        <thead>
          <tr>
            <th></th>
            <th>Type</th>
            <th>Item</th>
            <th>Amount</th>
            <th>Unlimited</th>
            <th>Extra Copies</th>
          </tr>
        </thead>
        <tbody>
          {settings.hints.map((_, i) => (
            <HintEditor key={i} index={i} />
          ))}
        </tbody>
      </table>
    </main>
  );
}
