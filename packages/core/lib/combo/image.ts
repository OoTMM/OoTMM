import { bufReadU16BE, bufReadU32BE, bufWriteU16BE, bufWriteU32BE } from "./util/buffer";

type ImageFormat = 'rgba32' | 'rgba16' | 'ia8';

function packToRgb(color: number) {
  const r = (color >> 16) & 0xff;
  const g = (color >> 8) & 0xff;
  const b = color & 0xff;

  return [r / 255, g / 255, b / 255] as const;
}

function packToRgba(color: number) {
  const r = ((color >> 24) & 0xff) >>> 0;
  const g = ((color >> 16) & 0xff) >>> 0;
  const b = ((color >> 8) & 0xff) >>> 0;
  const a = (color & 0xff) >>> 0;

  return [r / 255, g / 255, b / 255, a / 255] as const;
}

function rgbToHsl(r: number, g: number, b: number) {
  const max = Math.max(r, g, b);
  const min = Math.min(r, g, b);
  const l = (max + min) / 2;
  let h = 0;
  let s = 0;

  if (max !== min) {
    const d = max - min;
    s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
    switch (max) {
      case r: h = ((g - b) / d + (g < b ? 6 : 0)) / 6; break;
      case g: h = ((b - r) / d + 2) / 6; break;
      case b: h = ((r - g) / d + 4) / 6; break;
    }
  }

  return [h, s, l] as const;
}

function hueToRgb(p: number, q: number, t: number) {
  if (t < 0) {
    t += 1;
  }
  if (t > 1) {
    t -= 1;
  }
  if (t < 1 / 6) {
    return p + (q - p) * 6 * t;
  }
  if (t < 1 / 2) {
    return q;
  }
  if (t < 2 / 3) {
    return p + (q - p) * (2 / 3 - t) * 6;
  }
  return p;
}

function hslToRgb(h: number, s: number, l: number) {
  let r = 0;
  let g = 0;
  let b = 0;

  if (s !== 0) {
    const q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    const p = 2 * l - q;
    r = hueToRgb(p, q, h + 1 / 3);
    g = hueToRgb(p, q, h);
    b = hueToRgb(p, q, h - 1 / 3);
  } else {
    r = l;
    g = l;
    b = l;
  }

  return [r, g, b] as const;
}

function lerp(a: number, b: number, t: number) {
  return a + (b - a) * t;
}

function clamp(x: number, min: number, max: number) {
  return Math.min(Math.max(x, min), max);
}

function clamp01(x: number) {
  return clamp(x, 0, 1);
}

function bits5to8(x: number) {
  return (x << 3) | (x >>> 2);
}

function fromRgba16(image: Uint8Array) {
  const newBuf = new Uint8Array(image.length * 2);
  for (let i = 0; i < image.length / 2; ++i) {
    const srcIndex = i * 2;
    const dstIndex = i * 4;
    const rawColor = bufReadU16BE(image, srcIndex);
    const r5 = (rawColor >>> 11) & 0x1f;
    const g5 = (rawColor >>>  6) & 0x1f;
    const b5 = (rawColor >>>  1) & 0x1f;
    const a1 = (rawColor >>>  0) & 0x1;
    const r8 = bits5to8(r5);
    const g8 = bits5to8(g5);
    const b8 = bits5to8(b5);
    const a8 = a1 ? 0xff : 0x00;
    newBuf[dstIndex + 0] = r8;
    newBuf[dstIndex + 1] = g8;
    newBuf[dstIndex + 2] = b8;
    newBuf[dstIndex + 3] = a8;
  }
  return newBuf;
}

function fromIA8(image: Uint8Array) {
  const newBuf = new Uint8Array(image.length * 4);
  for (let i = 0; i < image.length; ++i) {
    const src = image[i];
    const r = src;
    const g = src;
    const b = src;
    const a = (src ? 0xff : 0x00);
    const dstIndex = i * 4;
    newBuf[dstIndex + 0] = r;
    newBuf[dstIndex + 1] = g;
    newBuf[dstIndex + 2] = b;
    newBuf[dstIndex + 3] = a;
  }
  return newBuf;
}

function toIA8(image: Uint8Array) {
  const newBuf = new Uint8Array(image.length / 4);
  for (let i = 0; i < newBuf.length; ++i) {
    const srcIndex = i * 4;
    const r = image[srcIndex + 0];
    const a = image[srcIndex + 3];
    const value = (a ? (r ? r : 1) : 0);
    newBuf[i] = value;
  }
  return newBuf;
}

function fromFormat(image: Uint8Array, format: ImageFormat) {
  if (format === 'rgba32') {
    return image;
  } else if (format === 'rgba16') {
    return fromRgba16(image);
  } else {
    return fromIA8(image);
  }
}

function toRgba16(image: Uint8Array) {
  const newBuf = new Uint8Array(image.length / 2);
  for (let i = 0; i < image.length / 4; ++i) {
    const srcIndex = i * 4;
    const dstIndex = i * 2;
    const r8 = image[srcIndex + 0];
    const g8 = image[srcIndex + 1];
    const b8 = image[srcIndex + 2];
    const a8 = image[srcIndex + 3];
    const r5 = (r8 >>> 3) & 0x1f;
    const g5 = (g8 >>> 3) & 0x1f;
    const b5 = (b8 >>> 3) & 0x1f;
    const a1 = (a8 >= 0x80) ? 0x01 : 0x00;
    const rawColor = (r5 << 11) | (g5 << 6) | (b5 << 1) | (a1 << 0);
    bufWriteU16BE(newBuf, dstIndex, rawColor);
  }
  return newBuf;
}

function toFormat(image: Uint8Array, format: ImageFormat) {
  if (format === 'rgba32') {
    return new Uint8Array(image);
  } else if (format === 'rgba16') {
    return toRgba16(image);
  } else {
    return toIA8(image);
  }
}

export function grayscale(image: Uint8Array, format: ImageFormat, gamma = 1) {
  image = fromFormat(image, format);

  for (let i = 0; i < image.length; i += 4) {
    const r = image[i + 0];
    const g = image[i + 1];
    const b = image[i + 2];
    const l = Math.max(r, g, b);
    const lGamma = Math.round(Math.pow(l / 255, gamma) * 255);
    image[i + 0] = lGamma;
    image[i + 1] = lGamma;
    image[i + 2] = lGamma;
  }

  return toFormat(image, format);
}

export function recolorImage(format: ImageFormat, image: Uint8Array, mask: Uint8Array | null, defaultColor: number, color: number) {
  image = fromFormat(image, format);
  const newBuffer = new Uint8Array(image.length);
  if (mask === null) {
    mask = new Uint8Array(image.length);
    mask.fill(0xff);
  }

  /* Extract new color */
  const [r, g, b] = packToRgb(color);

  /* Extract new color grayscale value */
  const l = Math.max(r, g, b);

  /* Extract default color grayscale */
  const [dr, dg, db] = packToRgb(defaultColor);
  const dl = Math.max(dr, dg, db);

  /* Compute lumi boost */
  const boost = l / dl;

  for (let i = 0; i < image.length; i += 4) {
    /* Fetch the pixel */
    const data = bufReadU32BE(image, i);
    const [pr, pg, pb, pa] = packToRgba(data);

    /* Convert to grayscale */
    const pl = Math.max(pr, pg, pb);

    /* Check if the pixel is inside the mask */
    const maskByte = mask[Math.floor(i / 32)];
    const maskBit = (maskByte >> ((i / 4) % 8)) & 1;
    if (maskBit) {
      /* Pixel is within tolerance, recolor it */
      const newR = Math.floor(clamp01(r * pl * boost) * 255);
      const newG = Math.floor(clamp01(g * pl * boost) * 255);
      const newB = Math.floor(clamp01(b * pl * boost) * 255);
      bufWriteU32BE(newBuffer, i, ((newR << 24) | (newG << 16) | (newB << 8) | pa * 255) >>> 0);
    } else {
      /* Pixel is not within tolerance, copy it over */
      bufWriteU32BE(newBuffer, i, data);
    }
  }

  return toFormat(newBuffer, format);
}
