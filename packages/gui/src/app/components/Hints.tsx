import { useCallback, useMemo } from 'react';
import { Select, Input, Checkbox, Card } from './ui';
import { FaXmark, FaArrowUp, FaArrowDown } from 'react-icons/fa6';

import { HINT_TYPES, SETTINGS_DEFAULT_HINTS, SettingHint, itemName } from '@ootmm/core';
import { SettingsPanel } from './settings/SettingsEditor';
import { useItemPool, usePatchSettings, useSetting } from '../contexts/SettingsContext';
import { Button } from './ui';

const hintOptions: { label: string; value: keyof typeof HINT_TYPES }[] = [];
for (const k of Object.keys(HINT_TYPES)) {
  hintOptions.push({ label: HINT_TYPES[k as keyof typeof HINT_TYPES], value: k as keyof typeof HINT_TYPES });
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

  const onInputType = useCallback((type: keyof typeof HINT_TYPES | null) => {
    if (!type) return;

    const newHint = { ...hint, type };
    if (type === 'item') {
      newHint.item = 'MM_SONG_SOARING';
    } else {
      delete newHint.item;
    }
    updateHint(newHint);
  }, [hint, updateHint]);

  const onInputAmount = useCallback((amount: string) => {
    const newHint = { ...hint, amount: Number(amount) };
    updateHint(newHint);
  }, [hint, updateHint]);

  const onInputExtra = useCallback((extra: string) => {
    const newHint = { ...hint, extra: Number(extra) };
    updateHint(newHint);
  }, [hint, updateHint]);

  const onInputMax = useCallback((max: boolean) => {
    const newHint = { ...hint };
    newHint.amount = max ? 'max' : 1;
    updateHint(newHint);
  }, [hint, updateHint]);

  const onInputItem = useCallback((item: string | null) => {
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
    <tr>
      <td>
        <div className="flex justify-center">
          <span className="hover:text-gray-500 cursor-pointer" onClick={onRemove}><FaXmark/></span>
          <span className="hover:text-gray-500 cursor-pointer" onClick={onMoveUp}><FaArrowUp/></span>
          <span className="hover:text-gray-500 cursor-pointer" onClick={onMoveDown}><FaArrowDown/></span>
        </div>
      </td>
      <td><Select options={hintOptions} value={hint.type} onSelect={onInputType} /></td>
      <td>{hint.type === 'item' && <Select searcheable placeholder="Search..." options={itemOptions} onSelect={onInputItem} value={selectedItem?.value ?? null}/>}</td>
      <td>{hint.amount !== 'max' && <Input type="number" min="1" max="99" className="w-full" value={hint.amount.toString()} onChange={onInputAmount} />}</td>
      <td><div className="flex justify-center"><Checkbox checked={hint.amount === 'max'} onChange={onInputMax} /></div></td>
      <td>
        <Input type="number" min="0" max="99" className="w-full" value={hint.extra.toString()} onChange={onInputExtra} />
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
      <SettingsPanel category='hints'/>
      <div className="mt-8 mb-4 flex gap-2">
        <Button onClick={onNew}>New</Button>
        <Button variant="danger" onClick={() => patchSettings({ hints: { set: [] } })}>Remove All</Button>
        <Button variant="danger" onClick={() => patchSettings({ hints: { set: SETTINGS_DEFAULT_HINTS } })}>Reset</Button>
      </div>

      <Card>
        <table className="w-full border-separate border-spacing-y-2 border-spacing-x-2">
          <thead>
            <tr>
              <th>Actions</th>
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
      </Card>
    </main>
  );
}
