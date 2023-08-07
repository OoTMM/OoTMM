import { fileExists } from '../util';
import fs from 'fs/promises';
import path from 'path';

export class CodeGen {
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

  async emit() {
    if (!process.env.ROLLUP) {
      const dir = path.dirname(this.filename);
      await fs.mkdir(dir, { recursive: true });
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
        previousBuf = await fs.readFile(this.filename, 'utf8');
      }
      if (previousBuf !== buf) {
        await fs.writeFile(this.filename, buf);
      }
    }
  }
};
