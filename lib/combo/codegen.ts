import fs from 'fs/promises';
import path from 'path';
import { PATH_BUILD } from './config';
import { DATA_GI, DATA_SCENES, DATA_NPC } from './data';

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

const codegenFile = async (data: Promise<{[k: string]: number}>, prefix: string, filename: string, guard: string) => {
  const d = await data;
  const cg = new CodeGen(path.resolve(PATH_BUILD, 'include', 'combo', filename), guard);
  for (const [k, v] of Object.entries(d)) {
    cg.define(prefix + "_" + k, v);
  }
  await cg.emit();
};

export const codegen = async () => {
  console.log("Codegen...");
  return Promise.all([
    codegenFile(DATA_GI,      "GI",  "gi_data.h", "GENERATED_GI_DATA_H"),
    codegenFile(DATA_SCENES,  "SCE", "scenes.h",  "GENERATED_SCENES_H"),
    codegenFile(DATA_NPC,     "NPC", "npc.h",     "GENERATED_NPC_H"),
  ]);
};
