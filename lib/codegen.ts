import fs from 'fs/promises';
import path from 'path';
import { PATH_BUILD, PATH_DATA } from './config';
import { GI } from './data';

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
  const data = await GI;
  const cg = new CodeGen(path.resolve(PATH_BUILD, 'include', 'combo', 'gi_data.h'), 'GENERATED_GI_DATA_H');
  for (const [k, v] of Object.entries(data)) {
    cg.define("GI_" + k, v);
  }
  await cg.emit();
};

export const codegen = async () => {
  console.log("Codegen...");
  await codegenGI();
};
