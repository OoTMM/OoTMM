import fs from 'fs/promises';
import path from 'path';

import { fileExists } from './util';

export class CodeGen {
  private defines: string[] = [];

  constructor(private filename: string, private guard: string) {
  }

  define(k: string, v: number) {
    this.defines.push(`#define ${k} ${v}`);
  }

  async emit() {
    const dir = path.dirname(this.filename);
    await fs.mkdir(dir, { recursive: true });
    const buf = ['#ifndef ' + this.guard, '#define ' + this.guard, '', ...this.defines, '', '#endif'].join("\n");
    let previousBuf = undefined;
    if (await fileExists(this.filename)) {
      previousBuf = await fs.readFile(this.filename, 'utf8');
    }
    if (previousBuf !== buf) {
      await fs.writeFile(this.filename, buf);
    }
  }
};

export const codegen = async () => {

};
