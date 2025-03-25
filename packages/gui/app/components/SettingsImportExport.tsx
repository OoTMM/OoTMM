import { useMemo } from 'preact/hooks';
import { importSettings, exportSettings } from '@ootmm/core';

import { useOverrideSettings, useSettings } from '../contexts/GeneratorContext';

export const SettingsImportExport = () => {
  const [settings] = useSettings();
  const overrideSettings = useOverrideSettings();
  const settingsString = useMemo(() => exportSettings(settings), [settings]);

  const onInput = (newStr: string) => {
    const newSettings = importSettings(newStr);
    overrideSettings(newSettings);
  };

  return (
    <label>
      Import/Export Settings
      <input type="text" spellcheck={false} value={settingsString} onClick={(e) => { e.currentTarget.focus(); e.currentTarget.select(); }} onInput={x => onInput(x.currentTarget.value)}/>
    </label>
  )
};
