#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

void logicRun(emscripten::val data) {
    // Example: Log the received data to the console
    emscripten::val console = emscripten::val::global("console");
    console.call<void>("log", data);
}

EMSCRIPTEN_BINDINGS(Logic) {
    emscripten::function("logicRun", &logicRun);
}
