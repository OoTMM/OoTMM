import fs from 'fs';
import path from 'path';
import * as DATA from '@ootmm/data';

async function fileExists(filename: string) {
  try {
    await fs.promises.access(filename, fs.constants.F_OK);
    return true;
  }
  catch (e) {
    return false;
  }
}

class CodeGen {
  private data: string[] = [];

  constructor(private filename: string, private guard?: string) {
  }

  define(k: string, v: number) {
    this.data.push(`#define ${k} 0x${v.toString(16)}`);
  }

  include(k: string) {
    this.data.push(`#include <${k}>`);
  }

  table(name: string, type: string, values: number[]) {
    const groups: number[][] = [];
    let remainder = [...values];
    for (;;) {
      if (remainder.length === 0) {
        break;
      }
      const group = remainder.slice(0, 16);
      remainder = remainder.slice(16);
      groups.push(group);
    }
    this.data.push(`const ${type} ${name}[] = {`);
    for (const group of groups) {
      this.data.push('  ' + group.map(v => (v < 0 ? '-' : '') + '0x' + Math.abs(v).toString(16) + ',').join(' '));
    }
    this.data.push('};');
    this.data.push('');
  }

  raw(data: string) {
    this.data.push(data);
  }

  async emit() {
    const dir = path.dirname(this.filename);
    await fs.promises.mkdir(dir, { recursive: true });
    const lines: string[] = [];
    if (this.guard) {
      lines.push('#ifndef ' + this.guard);
      lines.push('#define ' + this.guard);
      lines.push('');
    }
    for (const line of this.data) {
      lines.push(line);
    }
    if (this.guard) {
      lines.push('');
      lines.push('#endif');
    }
    const buf = lines.join('\n') + '\n';
    let previousBuf = undefined;
    if (await fileExists(this.filename)) {
      previousBuf = await fs.promises.readFile(this.filename, 'utf8');
    }
    if (previousBuf !== buf) {
      await fs.promises.writeFile(this.filename, buf);
    }
  }
};

async function genGI() {
  const cg = new CodeGen(path.resolve('build', 'include', 'combo', 'gi.h'), "GENERATED_GI_H");
  cg.define('GI_NONE', 0);
  let maxId = 0;
  for (let i = 0; i < DATA.RAW_GI.length; i++) {
    const rawGi = DATA.RAW_GI[i];
    cg.define(`GI_${rawGi.id}`, i + 1);
    maxId = i + 2;
  }
  cg.define('GI_MAX', maxId);
  await cg.emit();
}

async function genDrawGiFuncs() {
  const cgHeader = new CodeGen(path.resolve('build', 'include', 'combo', 'drawgi_funcs.h'), "GENERATED_DRAWGI_FUNCS_H");

  for (let i = 0; i < DATA.RAW_DRAWGI_FUNCS.length; i++) {
    const d = DATA.RAW_DRAWGI_FUNCS[i];
    cgHeader.define(`GIDF_${d.id}`, i);
  }

  cgHeader.raw('typedef struct PlayState PlayState;');
  cgHeader.raw('typedef void (*GetItemDrawFunc)(PlayState*, int, int);');
  cgHeader.raw('extern GetItemDrawFunc kGetItemDrawFuncs[];')
  for (let i = 0; i < DATA.RAW_DRAWGI_FUNCS.length; i++) {
    const d = DATA.RAW_DRAWGI_FUNCS[i];
    cgHeader.raw(`void GetItem_Draw${d.name}(PlayState* state, int index, int param);`);
  }

  const cgSource = new CodeGen(path.resolve('build', 'src', 'z64', 'drawgi_funcs.c'));
  cgSource.include('combo/drawgi_funcs.h');
  cgSource.raw('GetItemDrawFunc kGetItemDrawFuncs[] = {');
  for (let i = 0; i < DATA.RAW_DRAWGI_FUNCS.length; i++) {
    const d = DATA.RAW_DRAWGI_FUNCS[i];
    cgSource.raw(`  GetItem_Draw${d.name},`);
  }
  cgSource.raw('};');

  return Promise.all([
    cgHeader.emit(),
    cgSource.emit(),
  ]);
}

async function run() {
  await Promise.all([
    genGI(),
    genDrawGiFuncs(),
  ]);
}

run().catch((err) => {
  console.error(err);
  process.exit(1);
});
