import React from 'react';
import { Settings, SPECIAL_CONDS, SPECIAL_CONDS_KEYS } from '@ootmm/core';

import { useSettings } from '../contexts/GeneratorContext';
import { Checkbox } from './Checkbox';
import { InputNumber } from './InputNumber';
import Group from './Group';

type SpecialCondsPanelProps = {
  cond: string;
};
function SpecialCondsPanel({ cond }: SpecialCondsPanelProps) {
  const [settings, setSettings] = useSettings();
  const { specialConds } = settings;
  const c = specialConds[cond as keyof typeof SPECIAL_CONDS];
  const enableCond = SPECIAL_CONDS[cond].cond || (() => true);
  const enabled = enableCond(settings);

  if (!enabled) {
    return null;
  }

  return (
    <form onSubmit={e => e.preventDefault()}>
      <Group direction="vertical">
        <h1>{SPECIAL_CONDS[cond].name}</h1>
        <Group direction="vertical" spacing='4px'> 
          {Object.keys(SPECIAL_CONDS_KEYS).map(key =>
            <Checkbox
              key={key}
              label={(SPECIAL_CONDS_KEYS as any)[key]}
              checked={(c as any)[key]}
              onChange={x => setSettings({ specialConds: { [cond]: { [key]: x } }} as any)}
            />
          )}
         </Group>
        <InputNumber label="Amount" value={c.count} onChange={x => setSettings({ specialConds: { [cond]: { count: x } }} as any)}/>
      </Group>
    </form>
  );
}

export function SpecialConds() {
  return (
    <Group direction="horizontal" spacing='4rem'>
      {Object.keys(SPECIAL_CONDS).map(x => <SpecialCondsPanel key={x} cond={x}/>)}
    </Group>
  );
}
