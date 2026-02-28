cmake_minimum_required(VERSION 3.20)
project(logic CXX C)

file(GLOB_RECURSE SOURCES *.cc *.c *.hh *.h)
add_executable(logic ${SOURCES})
add_compile_options(-s MODULARIZE=1 -s EXPORT_NAME="Logic" -s EXPORT_ES6=1 -s ENVIRONMENT=web,worker,node -s ALLOW_MEMORY_GROWTH=1 -s INITIAL_MEMORY=67108864)
target_include_directories(logic PUBLIC src)
