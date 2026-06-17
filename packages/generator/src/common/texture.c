#include <combo.h>

void Grayscale(void* dst, u16 size)
{
    Color_RGBA8* c;
    u16 acc;

    for (u16 i = 0; i < size; ++i)
    {
        c = (Color_RGBA8*)dst + i;
        acc = (c->r + c->g * 2 + c->b) / 7;
        c->r = acc & 0xff;
        c->g = acc & 0xff;
        c->b = acc & 0xff;
    }
}

static inline float fmodf(float x, float y) {
    int n = (int)(x / y);
    float r = x - (float)n * y;
    if (r < 0) r += y;
    return r;
}

// Convert RGB [0,1] → HSV
static void rgb_to_hsv(float r, float g, float b, float *h, float *s, float *v) {
    float max = MAX(r, MAX(g, b));
    float min = MIN(r, MIN(g, b));
    float d = max - min;
    *v = max;
    *s = max == 0 ? 0 : d / max;

    if (d == 0) {
        *h = 0;
    } else {
        if (max == r)
            *h = fmodf((g - b) / d, 6.0f);
        else if (max == g)
            *h = (b - r) / d + 2.0f;
        else
            *h = (r - g) / d + 4.0f;
        *h *= 60.0f;
        if (*h < 0) *h += 360.0f;
    }
}

// Convert HSV [0,1] → RGB
static void hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b) {
    float c = v * s;
    float x = c * (1 - fabsf(fmodf(h / 60.0f, 2.0f) - 1));
    float m = v - c;
    float r1, g1, b1;

    if (h < 60)      { r1 = c; g1 = x; b1 = 0; }
    else if (h < 120){ r1 = x; g1 = c; b1 = 0; }
    else if (h < 180){ r1 = 0; g1 = c; b1 = x; }
    else if (h < 240){ r1 = 0; g1 = x; b1 = c; }
    else if (h < 300){ r1 = x; g1 = 0; b1 = c; }
    else             { r1 = c; g1 = 0; b1 = x; }

    *r = (r1 + m);
    *g = (g1 + m);
    *b = (b1 + m);
}

void change_hsv(Color_RGBA8 *pixels, size_t size,
                float hue_shift,
                float saturation_mul,
                float brightness_mul)
{
    for (size_t i = 0; i < size; i++) {
        float r = pixels[i].r / 255.0f;
        float g = pixels[i].g / 255.0f;
        float b = pixels[i].b / 255.0f;
        float h, s, v;

        rgb_to_hsv(r, g, b, &h, &s, &v);

        // Hue shift in degrees
        h = fmodf(h + hue_shift, 360.0f);
        if (h < 0.0f) {
            h += 360.0f;
        }
        s = CLAMP(s * saturation_mul, 0.0f, 1.0f);
        v = CLAMP(v * brightness_mul, 0.0f, 1.0f);

        hsv_to_rgb(h, s, v, &r, &g, &b);

        pixels[i].r = (u8)(CLAMP(r * 255.0f, 0.0f, 255.0f));
        pixels[i].g = (u8)(CLAMP(g * 255.0f, 0.0f, 255.0f));
        pixels[i].b = (u8)(CLAMP(b * 255.0f, 0.0f, 255.0f));
    }
}
