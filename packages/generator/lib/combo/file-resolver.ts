import JSZip from 'jszip';
import { loadDataZip } from '#generator-platform';

let dataZipPromise: Promise<JSZip> | null = null;

async function makeDataZipPromise(): Promise<JSZip> {
  return JSZip.loadAsync(await loadDataZip());
}

async function getDataZip() {
  if (!dataZipPromise) {
    dataZipPromise = makeDataZipPromise().catch((err) => {
      dataZipPromise = null;
      throw err;
    });
  }

  return dataZipPromise;
}

export class FileResolver {
  private zipPromise: Promise<JSZip>;

  constructor() {
    this.zipPromise = getDataZip();
  }

  async fetch(filename: string): Promise<Uint8Array> {
    const zip = await this.zipPromise;
    const file = zip.file(filename);

    if (!file) {
      throw new Error(`File not found: ${filename}`);
    }

    return file.async('uint8array');
  }

  async glob(pattern: RegExp): Promise<string[]> {
    const zip = await this.zipPromise;
    return zip.file(pattern).map(f => f.name);
  }
};

