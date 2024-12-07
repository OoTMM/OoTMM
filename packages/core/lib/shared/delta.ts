import { init, xd3_encode_memory, xd3_smatch_cfg } from 'xdelta3-wasm';

export async function deltaEncode(oldData: Uint8Array, newData: Uint8Array): Promise<Uint8Array> {
  await init();
  const ret = await xd3_encode_memory(newData, oldData, newData.length + 100, xd3_smatch_cfg.DEFAULT);
  if (ret.ret !== 0) {
    throw new Error('Delta error');
  }

  return ret.output;
}
