cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "mips64-ultra-elf-gcc")
set(CMAKE_CXX_COMPILER "mips64-ultra-elf-g++")

project(OoTMM C ASM)

set(CMAKE_C_FLAGS "-EB -ffreestanding -nostdlib -fno-PIC -mabi=32 -mno-shared -mno-abicalls -march=vr4300 -mtune=vr4300 -mfix4300 -mfix-r4000 -G 0 -Os -Wall -Werror=implicit-function-declaration -Werror=implicit-int -Wimplicit-fallthrough")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")

# Override default envs
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG=1 -g -g3")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
set(CMAKE_C_FLAGS_MINSIZEREL "")

set(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
set(CMAKE_ASM_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/third_party/ultralib/include" "${CMAKE_SOURCE_DIR}/third_party/ultralib/include/gcc")
include_directories("${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/build/include")

add_compile_definitions("F3DEX_GBI_2=1")

set(LDSCRIPT_IN "${CMAKE_SOURCE_DIR}/src/link.ld.in")

function(build game)
  # Common stuff
  string(TOUPPER ${game} game_upper)
  set(game_define "-DGAME_${game_upper}=1")

  # Linker script
  set(ldscript "${CMAKE_BINARY_DIR}/link_${game}.ld")

  add_custom_command(
    OUTPUT "${ldscript}"
    COMMAND "${CMAKE_C_COMPILER}" ${game_define} -I "${CMAKE_SOURCE_DIR}/include" -E -P -x c "${LDSCRIPT_IN}" -o "${ldscript}"
    DEPENDS "${LDSCRIPT_IN}"
    COMMENT "Generating linker script for ${game}"
    VERBATIM
  )

  add_custom_target(
    ldscript_${game} DEPENDS "${ldscript}"
  )

  file(GLOB_RECURSE SOURCES "src/${game}/*.c" "src/${game}/*.S" "src/common/*.c" "src/common/*.S" "build/src/${game}/*.c" "build/src/${game}/*.S" "build/src/common/*.c" "build/src/common/*.S")
  add_link_options("-T${ldscript}")
  add_executable(${game} ${SOURCES})
  set_target_properties(${game} PROPERTIES LINK_DEPENDS "${ldscript}")
  add_dependencies(${game} ldscript_${game})
  target_compile_definitions(${game} PRIVATE ${game_define})
endfunction()

build(oot)
build(mm)
