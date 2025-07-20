import { Settings, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS } from '@ootmm/core';

import { useSettings, usePatchSettings } from '../contexts/SettingsContext';
import { InputField, CheckboxField, Card } from './ui';

type SpecialCondsPanelProps = {
  cond: string;
};
function SpecialCondsPanel({ cond }: SpecialCondsPanelProps) {
  const settings = useSettings();
  const patchSettings = usePatchSettings();
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
    <Card>
      <h2 className="mx-auto font-bold text-xl">{SPECIAL_CONDS[cond].name}</h2>
      {Object.keys(SPECIAL_CONDS_FIELDS).filter(key => { const cond = (SPECIAL_CONDS_FIELDS as any)[key].cond; return cond ? cond(settings) : true }).map(key =>
        <CheckboxField
          key={key}
          label={(SPECIAL_CONDS_FIELDS as any)[key].name}
          checked={(c as any)[key]}
          onChange={x => patchSettings({ specialConds: { [cond]: { [key]: x } }} as any)}
        />
      )}
      <InputField type="number" min="0" max={max} label={label} value={c.count.toString()} onChange={x => patchSettings({ specialConds: { [cond]: { count: Number(x) } }})}/>
    </Card>
  );
}

export function SpecialConds() {
  return (
    <main className="p-8 flex flex-wrap gap-8">
      {Object.keys(SPECIAL_CONDS).map(x => <SpecialCondsPanel key={x} cond={x}/>)}
    </main>
  );
}
