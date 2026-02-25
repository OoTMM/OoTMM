import { useCallback, useId, useMemo } from 'react';
import { importSettings, exportSettings } from '@ootmm/generator';
import { Input, Label } from '@/app/components/ui';
import { useStore } from '@/app/store';

export const SettingsImportExport = () => {
  const id = useId();
  const settings = useStore(state => state.settings);
  const setSettings = useStore(state => state.setSettings);
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
