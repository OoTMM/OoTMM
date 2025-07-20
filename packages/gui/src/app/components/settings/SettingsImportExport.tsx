import { useCallback, useId, useMemo } from 'react';
import { importSettings, exportSettings } from '@ootmm/core';
import { useSetSettings, useSettings } from '@/app/contexts/SettingsContext';
import { Input, Label } from '@/app/components/ui';

export const SettingsImportExport = () => {
  const id = useId();
  const settings = useSettings();
  const setSettings = useSetSettings();
  const settingsString = useMemo(() => exportSettings(settings), [settings]);

  const onChange = useCallback((data: string) => {
    const newSettings = importSettings(data);
    setSettings(newSettings);
  }, []);

  return (
    <div className="flex flex-col gap-1">
      <Label htmlFor={id}>Import/Export Settings</Label>
      <Input
        id={id}
        className="font-mono"
        type="text"
        spellcheck={false}
        value={settingsString}
        onClick={(e: any) => {
          e.currentTarget.focus();
          e.currentTarget.select();
        }}
        onChange={onChange}
      />
    </div>
  );
}
