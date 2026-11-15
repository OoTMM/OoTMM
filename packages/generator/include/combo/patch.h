#ifndef COMBO_PATCH_H
#define COMBO_PATCH_H

#include <combo/types.h>

#define CONCAT2(a, b) a ## b
#define CONCAT(a, b) CONCAT2(a, b)
#define PATCH_FUNC(dst, src)        __attribute__((section(".patch"), used, aligned(8))) static u32 CONCAT(kPatch, __COUNTER__)[] = { 0x6, (u32)dst, (u32)src };
#define PATCH_CALL(dst, src)        __attribute__((section(".patch"), used, aligned(8))) static u32 CONCAT(kPatch, __COUNTER__)[] = { 0x8, (u32)dst, (u32)src };
#define PATCH_VROM_U32(dst, value)  __attribute__((section(".patch"), used, aligned(8))) static u32 CONCAT(kPatch, __COUNTER__)[] = { 0x9, (u32)dst, 4, (u32)value };
#define EXPORT_SYMBOL(name, sym)    __attribute__((section(".symbols_addr"),  used, aligned(4))) static u32 CONCAT(kSymbolAddr, __COUNTER__) = ((u32)&sym); \
                                    __attribute__((section(".symbols_name"), used, aligned(8))) static const char CONCAT(kSymbolName, __COUNTER__)[] = #name
#define EXPORT_LINKABLE_SYMBOL(name, sym) EXPORT_SYMBOL(name, sym); __asm__(".global " #name "\n.set " #name ", " #sym)
#define EXPORT_ENEMY_PROFILE(prefix, profile, init, destroy, update, draw) \
    EXPORT_SYMBOL(prefix ## _PROFILE, profile); \
    EXPORT_SYMBOL(prefix ## _INIT, init); \
    EXPORT_SYMBOL(prefix ## _DESTROY, destroy); \
    EXPORT_SYMBOL(prefix ## _UPDATE, update); \
    EXPORT_SYMBOL(prefix ## _DRAW, draw)

#endif
