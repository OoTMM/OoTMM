import fs from 'fs/promises';
import path, { join } from 'path';
import { PATH_BUILD, PATH_DATA } from './config';

import { fileExists } from './util';

export class CodeGen {
  private defines: string[] = [];

  constructor(private filename: string, private guard: string) {
  }

  define(k: string, v: number) {
    this.defines.push(`#define ${k} 0x${v.toString(16)}`);
  }

  async emit() {
    const dir = path.dirname(this.filename);
    await fs.mkdir(dir, { recursive: true });
    const buf = ['#ifndef ' + this.guard, '#define ' + this.guard, '', ...this.defines, '', '#endif', ''].join("\n");
    let previousBuf = undefined;
    if (await fileExists(this.filename)) {
      previousBuf = await fs.readFile(this.filename, 'utf8');
    }
    if (previousBuf !== buf) {
      await fs.writeFile(this.filename, buf);
    }
  }
};

const codegenGI = async () => {
  const text = await fs.readFile(path.resolve(PATH_DATA, 'gi.json'), 'utf8');
  const data = JSON.parse(text) as any;
  const cg = new CodeGen(path.resolve(PATH_BUILD, 'include', 'combo', 'gi_data.h'), 'GENERATED_GI_DATA_H');
  for (const [k, v] of Object.entries(data)) {
    const name = "GI_" + k;
    const value = parseInt(v as string, 16);
    cg.define(name, value);
  }
  await cg.emit();
};

export const codegen = async () => {
  console.log("Codegen...");
  await codegenGI();
};
