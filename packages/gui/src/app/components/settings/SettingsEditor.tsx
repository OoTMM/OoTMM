import { SETTINGS, SUBCATEGORIES } from '@ootmm/generator';
import { Setting } from './Setting';

type SettingsPanelProps = {
  category: string;
  showDisabled?: boolean;
};
export function SettingsPanel({ category, showDisabled = false }: SettingsPanelProps) {
  const settingsData = SETTINGS.filter((s) => s.category === category);

  return (
    <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4 items-center">
      {settingsData.map(x => <Setting key={x.key} setting={x.key} showDisabled={showDisabled}/>)}
    </div>
  );
};

type SettingsEditorProps = {
  name?: string;
  category: string;
  showDisabled?: boolean;
};
export function SettingsEditor({ name, category, showDisabled = false }: SettingsEditorProps) {
  return (
    <main className="p-8">
      <SettingsPanel category={category} showDisabled={showDisabled}/>
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
    <main className="flex flex-col gap-16 p-8">
      {categoriesData.map((x) =>
        <div>
          <h1 className="font-bold text-xl mb-4">{x.key}</h1>
          <SettingsPanel category={x.category}/>
        </div>
      )}
    </main>
  )
}
