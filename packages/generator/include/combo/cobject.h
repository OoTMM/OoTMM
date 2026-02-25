#ifndef COMBO_COBJECT_H
#define COMBO_COBJECT_H

#include <ultra64.h>

#define COBJECT_CONCAT(a, b) a ## b

#define COBJECT_DATA                __attribute__((section(".cobject.data"),used))
#define COBJECT_NAME(str)           __attribute__((section(".cobject.name"),used)) const char _name[] = #str
#define COBJECT_EXPORT(name, addr)   \
    __attribute__((section(".cobject.exports_addr"),used)) const void* COBJECT_CONCAT(name, _ADDR) = addr; \
    __attribute__((section(".cobject.exports_name"),used)) const char COBJECT_CONCAT(name, _NAME)[] = #name

#endif
