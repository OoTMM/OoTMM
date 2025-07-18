import { ComponentType } from "preact";
import { Tab } from "../nav";
import { MultipleSettingsEditor, SettingsEditor } from "../SettingsEditor";

type TabSettingsEditorProps = {
  name: string;
  icon?: ComponentType;
  category: string;
  disabled?: boolean;
};
export function TabSettingsEditor({ name, icon, category, disabled }: TabSettingsEditorProps) {
  return <Tab name={name} icon={icon} disabled={disabled}><SettingsEditor name={name} category={category}/></Tab>;
}

type TabMultipleSettingsEditorProps = {
  name: string;
  icon?: ComponentType;
  disabled?: boolean;
};
export function TabMultipleSettingsEditor({ name, icon, disabled }: TabMultipleSettingsEditorProps) {
  return <Tab name={name} icon={icon} disabled={disabled}><MultipleSettingsEditor name={name}/></Tab>;
}
