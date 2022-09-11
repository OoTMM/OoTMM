import fs from 'fs/promises';

export const fileExists = async (path: string) => {
  try {
    await fs.access(path, fs.constants.F_OK);
    return true;
  } catch {
    return false;
  }
}
