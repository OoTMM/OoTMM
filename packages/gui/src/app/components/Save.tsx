import { localStorageExport, localStorageImport } from '../util';
import { Button, Card } from './ui';

function exportSettings() {
  const data = JSON.stringify(localStorageExport(), null, 2);
  const blob = new Blob([data], { type: 'application/json' });
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = 'ootmm-settings.json';
  a.click();
  URL.revokeObjectURL(url);
}

function importSettings() {
  const input = document.createElement('input');
  input.type = 'file';
  input.accept = 'application/json';
  input.onchange = (e) => {
    const file = (e.target as HTMLInputElement).files?.[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = (e) => {
      try {
        const data = JSON.parse(e.target?.result as string);
        localStorageImport(data);
        location.reload();
      } catch (err) {
        console.error(err);
      }
    };
    reader.readAsText(file);
  };
  input.click();
}

export function Save() {
  return (
    <main className="p-8">
      <Card>
        <h2>Import / Export Settings</h2>
        <div className="flex gap-4">
          <Button onClick={exportSettings}>Export</Button>
          <Button onClick={importSettings} variant="danger">Import</Button>
        </div>
      </Card>
    </main>
  )
}
