import { useCallback, useMemo } from 'react';
import Select from 'react-select';
import { FaXmark, FaArrowUp, FaArrowDown } from 'react-icons/fa6';

import { Dropdown } from './Dropdown';
import { HINT_TYPES, SETTINGS_DEFAULT_HINTS, SettingHint, itemName } from '@ootmm/core';
import { InputNumber } from './InputNumber';
import { Checkbox } from './Checkbox';
import { SettingsPanel } from './SettingsEditor';
import { useItemPool, usePatchSettings, useSetting } from '../contexts/SettingsContext';

const hintOptions: { name: string; value: string }[] = [];
for (const k in HINT_TYPES) {
  hintOptions.push({ name: HINT_TYPES[k as keyof typeof HINT_TYPES], value: k });
}

type HintEditorProps = {
  index: number;
};
export function HintEditor({ index }: HintEditorProps) {
  const hints = useSetting('hints');
  const hint = hints[index];
  const itemPool = useItemPool();
  const patchSettings = usePatchSettings();

  const itemOptions = useMemo(() => Object.keys(itemPool).map((item) => ({ value: item, label: itemName(item) })), [itemPool]);
  const selectedItem = itemOptions.find((x) => x.value === hint.item);

  const updateHint = useCallback((h: SettingHint) => {
    const newArray = [...hints];
    newArray[index] = h;
    patchSettings({ hints: { set: newArray } });
  }, [hints, index]);

  const onInputType = useCallback((type: keyof typeof HINT_TYPES) => {
    const newHint = { ...hint, type };
    if (type === 'item') {
      newHint.item = 'MM_SONG_SOARING';
    } else {
      delete newHint.item;
    }
    updateHint(newHint);
  }, [hint, updateHint]);

  const onInputAmount = useCallback((amount: number) => {
    const newHint = { ...hint, amount };
    updateHint(newHint);
  }, [hint, updateHint]);

  const onInputExtra = useCallback((extra: number) => {
    const newHint = { ...hint, extra };
    updateHint(newHint);
  }, [hint, updateHint]);

  const onInputMax = useCallback((max: boolean) => {
    const newHint = { ...hint };
    newHint.amount = max ? 'max' : 1;
    updateHint(newHint);
  }, [hint, updateHint]);

  const onInputItem = useCallback((item?: string) => {
    if (!item) return;
    const newHint = { ...hint, item };
    updateHint(newHint);
  }, [hint, updateHint]);

  const onRemove = useCallback(() => {
    const newArray = [...hints];
    newArray.splice(index, 1);
    patchSettings({ hints: { set: newArray } });
  }, [hints, index]);

  const onMoveUp = useCallback(() => {
    if (index === 0) return;
    const newArray = [...hints];
    const temp = newArray[index - 1];
    newArray[index - 1] = newArray[index];
    newArray[index] = temp;
    patchSettings({ hints: { set: newArray } });
  }, [hints, index]);

  const onMoveDown = useCallback(() => {
    if (index === hints.length - 1) return;
    const newArray = [...hints];
    const temp = newArray[index + 1];
    newArray[index + 1] = newArray[index];
    newArray[index] = temp;
    patchSettings({ hints: { set: newArray } });
  }, [hints, index]);

  return (
    <tr className="dashboard-table">
      <td>
        <span className="list-remove" onClick={onRemove}>
          <FaXmark/>
        </span>
        <span className="list-action" onClick={onMoveUp}>
          <FaArrowUp/>
        </span>
        <span className="list-action" onClick={onMoveDown}>
          <FaArrowDown/>
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
  const hints = useSetting('hints');
  const patchSettings = usePatchSettings();

  const onNew = useCallback(() => {
    const newArray = [...hints];
    newArray.push({ type: 'junk', amount: 1, extra: 0 });
    patchSettings({ hints: { set: newArray } });
  }, [hints]);

  return (
    <main>
      <h1>Hints</h1>
      <SettingsPanel category='hints'/>
      <>
        <button className="btn" onClick={onNew}>
          New
        </button>
        <button className="btn-danger" onClick={() => patchSettings({ hints: { set: [] } })}>
          Remove All
        </button>
        <button className="btn-danger" onClick={() => patchSettings({ hints: { set: SETTINGS_DEFAULT_HINTS } })}>
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
          {hints.map((_, i) => (
            <HintEditor key={i} index={i} />
          ))}
        </tbody>
      </table>
    </main>
  );
}
