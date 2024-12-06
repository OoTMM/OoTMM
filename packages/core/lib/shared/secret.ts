export function secretCreateKey(data: Uint8Array): Uint8Array {
  const keySize = 1024 * 64;
  const keyBuffer = new Uint8Array(keySize);
  const dataView = new Uint32Array(data.buffer);
  const keyView = new Uint32Array(keyBuffer.buffer);

  for (let i = 0; i < dataView.length; ++i) {
    keyView[i & (keySize / 4 - 1)] ^= dataView[i];
  }

  return keyBuffer;
}

export function secretEncode(data: Uint8Array, key: Uint8Array): Uint8Array {
  const dataCopy = new Uint8Array(data);
  for (let i = 0; i < key.length; ++i) {
    dataCopy[i] ^= key[i & (key.length - 1)];
  }

  return dataCopy;
}
