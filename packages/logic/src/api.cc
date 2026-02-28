#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>
#include "logic.hh"

void logicRun(emscripten::val data) {
    Logic logic;
    logic.load(data);
    logic.run();
}

EMSCRIPTEN_BINDINGS(Logic) {
    emscripten::function("logicRun", &logicRun);
}
