import { Color, ColorArg, ColorRandom } from './color';
import { COSMETICS } from './data';

export type BufferPath = string | Buffer;

type CosmeticsDataCommon = {
  readonly key: string;
  readonly name: string;
}

type CosmeticsDataColor = CosmeticsDataCommon & {
  readonly type: 'color';
}

type CosmeticsDataZobj = CosmeticsDataCommon & {
  readonly type: 'zobj';
}

type CosmeticsDataZip = CosmeticsDataCommon & {
  readonly type: 'zip';
}

type InputToShape<T> = T extends CosmeticsDataColor ? { [K in T['key']]: ColorArg }
  : T extends CosmeticsDataZobj ? { [K in T['key']]: BufferPath | null }
  : T extends CosmeticsDataZip ? { [K in T['key']]: BufferPath | null }
  : never;

type CosmeticsDataEntry = typeof COSMETICS[number];

type CosmeticsShapes = InputToShape<CosmeticsDataEntry>;

type UnionToIntersection<U> =
  (U extends any ? (k: U)=>void : never) extends ((k: infer I)=>void) ? I : never

type CosmeticsBase = UnionToIntersection<CosmeticsShapes>;

export type Cosmetics = CosmeticsBase;
