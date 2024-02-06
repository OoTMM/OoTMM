cmake_minimum_required(VERSION 3.10)

set(ARCH "mips64-ultra-elf")
set(CMAKE_C_COMPILER "${ARCH}-gcc")
set(CMAKE_ASM_COMPILER "${ARCH}-gcc")

project(OoTMM C ASM)

set(FLAGS "-EB -ffreestanding -nostdlib -fno-PIC -mabi=32 -mno-shared -mno-abicalls -march=vr4300 -mtune=vr4300 -mfix4300 -mfix-r4000 -G 0 -Os -Wall -Werror=implicit-function-declaration -Werror=implicit-int -Wimplicit-fallthrough")
set(CMAKE_C_FLAGS "${FLAGS} -D_LANGUAGE_C=1")
set(CMAKE_ASM_FLAGS "${FLAGS}")

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
  set(ldscript_name "link_${game}.ld")
  set(ldscript "${CMAKE_BINARY_DIR}/${ldscript_name}")

  add_custom_command(
    OUTPUT "${ldscript}"
    COMMAND "${CMAKE_C_COMPILER}" ${game_define} -I "${CMAKE_SOURCE_DIR}/include" -E -P -x c "${LDSCRIPT_IN}" -o "${ldscript}" -MMD -MF "${ldscript}.d" -MT "${ldscript_name}"
    DEPENDS "${LDSCRIPT_IN}"
    DEPFILE "${ldscript}.d"
    COMMENT "Generating linker script for ${game}"
    VERBATIM
  )

  add_custom_target(
    ldscript_${game} DEPENDS "${ldscript}"
  )

  # Build
  file(GLOB_RECURSE SOURCES "src/${game}/*.c" "src/${game}/*.S" "src/common/*.c" "src/common/*.S" "build/src/${game}/*.c" "build/src/${game}/*.S" "build/src/common/*.c" "build/src/common/*.S")
  add_executable(${game} ${SOURCES})
  set_target_properties(${game} PROPERTIES LINK_DEPENDS "${ldscript}")
  add_dependencies(${game} ldscript_${game})
  target_compile_definitions(${game} PRIVATE ${game_define})
  target_link_options(${game} PRIVATE "-T${ldscript}")

  # Extract payload
  set(game_payload "${CMAKE_BINARY_DIR}/${game}_payload.bin")
  set(game_patch "${CMAKE_BINARY_DIR}/${game}_patch.bin")
  set(game_cosmetic_name "${CMAKE_BINARY_DIR}/${game}_cosmetic_name.bin")
  set(game_cosmetic_addr "${CMAKE_BINARY_DIR}/${game}_cosmetic_addr.bin")
  add_custom_command(
    OUTPUT
      "${game_payload}"
      "${game_patch}"
      "${game_cosmetic_name}"
      "${game_cosmetic_addr}"
    COMMAND "${ARCH}-objcopy" --only-section=.text -O binary "$<TARGET_FILE:${game}>" ${game_payload}
    COMMAND "${ARCH}-objcopy" --only-section=.patch -O binary "$<TARGET_FILE:${game}>" ${game_patch}
    COMMAND "${ARCH}-objcopy" --only-section=.cosmetic_name -O binary "$<TARGET_FILE:${game}>" ${game_cosmetic_name}
    COMMAND "${ARCH}-objcopy" --only-section=.cosmetic_addr -O binary "$<TARGET_FILE:${game}>" ${game_cosmetic_addr}
    DEPENDS ${game}
    COMMENT "Extracting sections for ${game}"
    VERBATIM
  )
  add_custom_target(
    ${game}_sections ALL DEPENDS "${game_payload}" "${game_patch}" "${game_cosmetic_name}" "${game_cosmetic_addr}"
  )

  # Install
  install(FILES ${game_payload} ${game_patch} ${game_cosmetic_name} ${game_cosmetic_addr} DESTINATION "bin")
endfunction()

build(oot)
build(mm)
