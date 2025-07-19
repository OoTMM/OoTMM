import { SETTINGS, SUBCATEGORIES } from '@ootmm/core';
import { Setting } from './Setting';

type SettingsPanelProps = {
  category: string;
};
export function SettingsPanel({ category }: SettingsPanelProps) {
  const settingsData = SETTINGS.filter((s) => s.category === category);

  return (
    <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4 items-center">
      {settingsData.map(x => <Setting key={x.key} setting={x.key}/>)}
    </div>
  );
};

type SettingsEditorProps = {
  name?: string;
  category: string;
};
export function SettingsEditor({ name, category }: SettingsEditorProps) {
  return (
    <main className="p-8">
      <SettingsPanel category={category}/>
    </main>
  )
}

type MultipleSettingsEditorProps = {
  name: string;
};
export function MultipleSettingsEditor({ name }: MultipleSettingsEditorProps) {
  const categoriesData = SUBCATEGORIES.find((x) => x.key === name)?.categories;

  if (!categoriesData) {
    return null;
  }

  return (
    <main className="flex flex-col gap-16">
      {categoriesData.map((x) =>
        <div>
          <h1 className="font-bold text-xl mb-4">{x.key}</h1>
          <SettingsPanel category={x.category}/>
        </div>
      )}
    </main>
  )
}
