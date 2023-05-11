import fs from 'fs/promises';
import path from 'path';
import { DATA_GI, DATA_NPC, DATA_SCENES, DATA_CONFIG } from './data';
import { Monitor } from './monitor';

import { fileExists } from './util';
import { PATCH_GROUP_VALUES } from './patch-build/group';

export class CodeGen {
  private defines: string[] = [];

  constructor(private filename: string, private guard: string) {
  }

  define(k: string, v: number) {
    this.defines.push(`#define ${k} 0x${v.toString(16)}`);
  }

  async emit() {
    if (!process.env.ROLLUP) {
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
  }
};

const codegenFile = async (data: {[k: string]: number}, prefix: string, filename: string, guard: string) => {
  if (!process.env.ROLLUP) {
    const cg = new CodeGen(path.resolve('build', 'include', 'combo', filename), guard);
    for (const [k, v] of Object.entries(data)) {
      cg.define(prefix + "_" + k, v);
    }
    await cg.emit();
  }
};

export const codegen = async (monitor: Monitor) => {
  monitor.log("Codegen");
  return Promise.all([
    codegenFile(DATA_GI,              "GI",   "gi_data.h",      "GENERATED_GI_DATA_H"),
    codegenFile(DATA_SCENES,          "SCE",  "scenes.h",       "GENERATED_SCENES_H"),
    codegenFile(DATA_NPC,             "NPC",  "npc.h",          "GENERATED_NPC_H"),
    codegenFile(DATA_CONFIG,          "CFG",  "config.h",       "GENERATED_CONFIG_H"),
    codegenFile(PATCH_GROUP_VALUES,   "PG",   "patch_group.h",  "GENERATED_PATCH_GROUP_H"),
  ]);
};
