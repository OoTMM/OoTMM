import React from 'react';
import { Settings, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS } from '@ootmm/core';

import { useSettings } from '../contexts/GeneratorContext';
import { Checkbox } from './Checkbox';
import { InputNumber } from './InputNumber';
import { Group } from './Group';
import { Text } from './Text';

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
        <Text size='jb'>{SPECIAL_CONDS[cond].name}</Text>
        <Group direction="vertical" spacing='xs'> 
        {Object.keys(SPECIAL_CONDS_FIELDS).filter(key => { const cond = (SPECIAL_CONDS_FIELDS as any)[key].cond; return cond ? cond(settings) : true }).map(key =>
          <Checkbox
            key={key}
            label={(SPECIAL_CONDS_FIELDS as any)[key].name}
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
    <Group direction='vertical' spacing='xxl'>
      <Text size='mg'>Special Conditions</Text>
      <Group direction="horizontal" spacing='mg'>
        {Object.keys(SPECIAL_CONDS).map(x => <SpecialCondsPanel key={x} cond={x}/>)}
      </Group>
    </Group>
  );
}
