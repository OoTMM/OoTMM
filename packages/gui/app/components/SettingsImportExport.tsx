import { useCallback, useMemo } from 'react';
import { importSettings, exportSettings } from '@ootmm/core';
import { useSetSettings, useSettings } from '../contexts/SettingsContext';

export const SettingsImportExport = () => {
  const settings = useSettings();
  const setSettings = useSetSettings();
  const settingsString = useMemo(() => exportSettings(settings), [settings]);

  const onInput = useCallback((e: Event) => {
    const target = e.currentTarget as HTMLInputElement;
    const newSettings = importSettings(target.value);
    setSettings(newSettings);
  }, []);

  return (
    <label>
      Import/Export Settings
      <input type="text" spellcheck={false} value={settingsString} onClick={(e) => { e.currentTarget.focus(); e.currentTarget.select(); }} onInput={onInput}/>
    </label>
  )
};
