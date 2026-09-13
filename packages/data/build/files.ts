import { loadTxt } from './helpers';

async function loadFilesList(path: string) {
  const data = await loadTxt(path);
  return data.trim().split('\n');
}

export async function parseFiles() {
  const [oot, mm] = await Promise.all([
    loadFilesList('files/files-oot.txt'),
    loadFilesList('files/files-mm.txt'),
  ]);

  return { oot, mm };
}
