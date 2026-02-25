import JSZip from 'jszip';

let dataZipPromise: Promise<JSZip> | null = null;

async function makeDataZipPromise(): Promise<JSZip> {
  let data: Uint8Array;

  if (!process.env.__IS_BROWSER__) {
    const fs = await import('fs/promises');
    const path = await import('path');
    data = await fs.readFile(path.resolve(__dirname, '..', '..', 'dist', 'data.zip'));
  } else {
    const dataZipUrl = new URL('../../dist/data.zip', import.meta.url).href;
    const response = await fetch(dataZipUrl);
    data = new Uint8Array(await response.arrayBuffer());
  }

  return JSZip.loadAsync(data);
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

