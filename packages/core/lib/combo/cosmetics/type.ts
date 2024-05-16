import { ColorArg } from './color';
import { COSMETICS } from './data';

export type BufferPath = string | Buffer | ArrayBuffer | File;

type CosmeticsDataCommon = {
  readonly key: string;
  readonly name: string;
}

type CosmeticsDataColor = CosmeticsDataCommon & {
  readonly type: 'color';
}

type CosmeticsDataBoolean = CosmeticsDataCommon & {
  readonly type: 'boolean';
}

type CosmeticsDataFile = CosmeticsDataCommon & {
  readonly type: 'file';
  readonly ext: string;
}

type InputToShape<T> = T extends CosmeticsDataColor ? { [K in T['key']]: ColorArg }
  : T extends CosmeticsDataBoolean ? { [K in T['key']]: boolean }
  : T extends CosmeticsDataFile ? { [K in T['key']]: BufferPath | null }
  : never;

type CosmeticsDataEntry = typeof COSMETICS[number];

type CosmeticsShapes = InputToShape<CosmeticsDataEntry>;

type UnionToIntersection<U> =
  (U extends any ? (k: U)=>void : never) extends ((k: infer I)=>void) ? I : never

type CosmeticsBase = UnionToIntersection<CosmeticsShapes>;

export type Cosmetics = CosmeticsBase;
