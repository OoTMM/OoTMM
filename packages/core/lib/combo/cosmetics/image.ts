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

export function recolorImage(image: Buffer, mask: Buffer, defaultColor: number, color: number) {
  const newBuffer = Buffer.alloc(image.length);

  /* Convert the color to HSL */
  const [colorH, colorS, colorL] = rgbToHsl(...packToRgb(color));
  const [defaultH, defaultS, defaultL] = rgbToHsl(...packToRgb(defaultColor));

  for (let i = 0; i < image.length; i += 4) {
    /* Fetch the pixel */
    const data = image.readUInt32BE(i);
    const [r, g, b, a] = packToRgba(data);

    /* Convert to HSL */
    const [h, s, l] = rgbToHsl(r, g, b);

    /* Compute ratios */
    const rS = s / defaultS;
    const rL = l / defaultL;

    /* Check if the pixel is inside the mask */
    const maskByte = mask.readUInt8(Math.floor(i / 32));
    const maskBit = (maskByte >> ((i / 4) % 8)) & 1;
    if (maskBit) {
      /* Pixel is within tolerance, recolor it */
      const newH = colorH;
      const newS = clamp01(colorS * rS);
      const newL = clamp01(colorL * rL);
      const [newR, newG, newB] = hslToRgb(newH, newS, newL).map(x => x * 255);
      newBuffer.writeUInt32BE(((newR << 24) | (newG << 16) | (newB << 8) | a * 255) >>> 0, i);
    } else {
      /* Pixel is not within tolerance, copy it over */
      newBuffer.writeUInt32BE(data, i);
    }
  }

  return newBuffer;
}
