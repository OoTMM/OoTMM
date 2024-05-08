import React from 'react';
import { Settings, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS } from '@ootmm/core';

import { useSettings } from '../contexts/GeneratorContext';
import { Checkbox } from './Checkbox';
import { InputNumber } from './InputNumber';

type SpecialCondsPanelProps = {
  cond: string;
};
function SpecialCondsPanel({ cond }: SpecialCondsPanelProps) {
  const [settings, setSettings] = useSettings();
  const { specialConds } = settings;
  const c = specialConds[cond as keyof typeof SPECIAL_CONDS];
  const enableCond = SPECIAL_CONDS[cond].cond || (() => true);
  const enabled = enableCond(settings);
  let max = 0;

  if (!enabled) {
    return null;
  }

  for (const f in SPECIAL_CONDS_FIELDS) {
    if(c[f as keyof typeof SPECIAL_CONDS_FIELDS]) {
      const condm = SPECIAL_CONDS_FIELDS[f as keyof typeof SPECIAL_CONDS_FIELDS].max;
      if(typeof condm === 'number')
        max += condm;
      else
        max += condm(settings);
    }
  }
  if (c['masksOot'] && c['masksRegular'])
    max -= Object.keys(settings).filter(key => key.includes('sharedMask')).filter(x => settings[x as keyof Settings]).length;

  const label = `Amount (max: ${max > 0 ? max : 0})`
  c.count = c.count > max ? max : c.count;

  return (
    <form className="vertical" onSubmit={e => e.preventDefault()}>
      <h2>{SPECIAL_CONDS[cond].name}</h2>
      <>
        {Object.keys(SPECIAL_CONDS_FIELDS).filter(key => { const cond = (SPECIAL_CONDS_FIELDS as any)[key].cond; return cond ? cond(settings) : true }).map(key =>
          <Checkbox
            key={key}
            label={(SPECIAL_CONDS_FIELDS as any)[key].name}
            checked={(c as any)[key]}
            onChange={x => setSettings({ specialConds: { [cond]: { [key]: x } }} as any)}
          />
        )}
      </>
      <InputNumber max={max} label={label} value={c.count} onChange={x => setSettings({ specialConds: { [cond]: { count: x } }} as any)}/>
    </form>
  );
}

export function SpecialConds() {
  return (
    <main>
      <h1>Special Conditions</h1>
      <>
        {Object.keys(SPECIAL_CONDS).map(x => <SpecialCondsPanel key={x} cond={x}/>)}
      </>
    </main>
  );
}
