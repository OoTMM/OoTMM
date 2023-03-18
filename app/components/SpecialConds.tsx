import React from 'react';
import { Settings, SPECIAL_CONDS, SPECIAL_CONDS_KEYS } from '@ootmm/core';

import { useSettings } from '../contexts/GeneratorContext';
import { Checkbox } from './Checkbox';
import { InputNumber } from './InputNumber';

type SpecialCondsPanelProps = {
  cond: string;
};
function SpecialCondsPanel({ cond }: SpecialCondsPanelProps) {
  const [{ specialConds }, setSettings] = useSettings();
  const c = specialConds[cond as keyof typeof SPECIAL_CONDS];

  return (
    <form>
      <h2>{(SPECIAL_CONDS as any)[cond]}</h2>
      {Object.keys(SPECIAL_CONDS_KEYS).map(key =>
        <Checkbox
          key={key}
          label={(SPECIAL_CONDS_KEYS as any)[key]}
          checked={(c as any)[key]}
          onChange={x => setSettings({ specialConds: { [cond]: { [key]: x } }} as any)}
        />
      )}
      <InputNumber label="Amount" value={c.count} onChange={x => setSettings({ specialConds: { [cond]: { count: x } }} as any)}/>
    </form>
  );
}

export function SpecialConds() {
  return (
    <div className="three-column-grid">
      {Object.keys(SPECIAL_CONDS).map(x => <SpecialCondsPanel key={x} cond={x}/>)}
    </div>
  );
}
