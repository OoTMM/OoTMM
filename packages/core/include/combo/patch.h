#ifndef COMBO_PATCH_H
#define COMBO_PATCH_H

#include <combo/types.h>

#define CONCAT2(a, b) a ## b
#define CONCAT(a, b) CONCAT2(a, b)
#define PATCH_FUNC(dst, src)    __attribute__((section(".patch"), used, aligned(8))) static u32 CONCAT(kPatch, __COUNTER__)[] = { 0x6, (u32)dst, (u32)src };
#define PATCH_CALL(dst, src)    __attribute__((section(".patch"), used, aligned(8))) static u32 CONCAT(kPatch, __COUNTER__)[] = { 0x8, (u32)dst, (u32)src };
#define COSMETIC(name, sym)     __attribute__((section(".cosmetic_addr"),  used, aligned(4))) static u32 CONCAT(kCosmeticAddr, __COUNTER__) = ((u32)&sym - PAYLOAD_RAM); \
                                __attribute__((section(".cosmetic_name"), used, aligned(8))) static const char CONCAT(kCosmeticName, __COUNTER__)[] = #name
#endif
