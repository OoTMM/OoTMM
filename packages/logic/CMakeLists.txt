cmake_minimum_required(VERSION 3.20)
project(logic CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE SOURCES src/*.cc src/*.c src/*.hh src/*.h)
add_executable(logic ${SOURCES})

set(EMSCRIPTEN_COMMON_FLAGS
)

set(EMSCRIPTEN_COMPILE_FLAGS
)

set(EMSCRIPTEN_LINK_FLAGS
  -sMODULARIZE=1
  -sEXPORT_ES6=1
  -sENVIRONMENT=web,worker,node
  -sALLOW_MEMORY_GROWTH=1
  -sINITIAL_MEMORY=67108864
  --bind
  --no-entry
)

target_compile_options(logic PRIVATE ${EMSCRIPTEN_COMPILE_FLAGS})
target_link_options(logic PRIVATE ${EMSCRIPTEN_LINK_FLAGS})
target_include_directories(logic PUBLIC src)
