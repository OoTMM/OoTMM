#pragma once

#include <emscripten/val.h>
#include "registry.hh"
#include "types.hh"

class Logic
{
public:
    Logic();
    void load(emscripten::val data);
    void run();

    Registry<LocationID> regLocations;

private:
    void loadPoolData(const char* game, emscripten::val pool);
};
