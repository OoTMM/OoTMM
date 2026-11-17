import type { Monitor } from '@ootmm/core';

import * as Yaz0 from 'yaz0';
import { FileResolver } from '../file-resolver';

export async function loadDataZip(): Promise<Uint8Array> {
  const response = await fetch(new URL('../../../dist/data.zip', import.meta.url));
  if (!response.ok) {
    throw new Error(`Failed to load generator data: HTTP ${response.status}`);
  }
  return new Uint8Array(await response.arrayBuffer());
}

export async function compressFile(data: Uint8Array): Promise<Uint8Array> {
  return Yaz0.compress(data, 7);
}

export async function png(filename: string, mode: 'rgba32' | 'rgba16' | 'i4' | 'ia4' | 'ia8' | 'bitmask'): Promise<Uint8Array> {
  return new FileResolver().fetch(`${filename}.bin`);
}

export async function raw(filename: string): Promise<Uint8Array> {
  return new FileResolver().fetch(filename);
}

export async function codegen(monitor: Monitor): Promise<void> {}

export async function writeGeneratedFile(filename: string, data: string): Promise<void> {}

export async function writeCustomObject(name: string, data: Uint8Array): Promise<void> {}

export async function readCosmeticFile(filename: string): Promise<Uint8Array> {
  throw new Error('Cannot load buffers from path');
}
