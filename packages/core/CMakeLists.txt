cmake_minimum_required(VERSION 3.10)

set(ARCH_CANDIDATES "mips64-ultra-elf" "mips-linux-gnu" "mips64-linux-gnu" "mipsel-linux-gnu" "mips64el-linux-gnu")

foreach(arch ${ARCH_CANDIDATES})
  find_program(CMAKE_C_COMPILER "${arch}-gcc")
  find_program(CMAKE_ASM_COMPILER "${arch}-gcc")
  if(CMAKE_C_COMPILER AND CMAKE_ASM_COMPILER)
    set(ARCH "${arch}")
    message(STATUS "Found compiler for ${ARCH}")
    break()
  endif()
endforeach()

project(OoTMM C ASM)

set(FLAGS "-EB -static -fomit-frame-pointer -ffreestanding -nostdlib -fno-PIC -mabi=32 -mno-shared -mno-abicalls -march=vr4300 -mtune=vr4300 -mfix4300 -mfix-r4000 -G 0 -Os -Wall -Werror=implicit-function-declaration -Werror=implicit-int -Wimplicit-fallthrough -Wl,--build-id=none")
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

# Tools
set(TOOL_DIR  "${CMAKE_SOURCE_DIR}/../tools/bin")
set(TOOL_ZOVL "${TOOL_DIR}/zovl")

# Linker scripts
set(LDSCRIPT_IN "${CMAKE_SOURCE_DIR}/src/link.ld.in")
set(LDSCRIPT_OVL "${CMAKE_SOURCE_DIR}/src/link_ovl.ld")

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
  file(GLOB_RECURSE SOURCES_OVL "src/${game}/ovl/*.c" "src/common/ovl/*.c")
  list(REMOVE_ITEM SOURCES ${SOURCES_OVL})

  add_executable(${game} ${SOURCES})
  set_target_properties(${game} PROPERTIES LINK_DEPENDS "${ldscript}")
  add_dependencies(${game} ldscript_${game})
  target_compile_definitions(${game} PRIVATE ${game_define})
  target_link_options(${game} PRIVATE "-T${ldscript}")

  # Extract map
  set(game_map "${CMAKE_BINARY_DIR}/${game}_map.txt")
  add_custom_command(
    OUTPUT "${game_map}"
    COMMAND "${CMAKE_SOURCE_DIR}/src/extract_sym.sh" "$<TARGET_FILE:${game}>" "${game_map}"
    DEPENDS ${game}
    COMMENT "Extracting map for ${game}"
    VERBATIM
  )
  add_custom_target(${game}_map ALL DEPENDS "${game_map}")

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

function (add_actor game name file)
  # Common stuff
  string(TOUPPER ${game} game_upper)
  set(target_name "${game}_ovl_${name}")
  set(ovl "${CMAKE_BINARY_DIR}/ovl/${game}/${name}.zovlx")
  set(game_define "-DGAME_${game_upper}=1")
  set(game_map "${CMAKE_BINARY_DIR}/${game}_map.txt")

  add_executable(${target_name} "${CMAKE_CURRENT_SOURCE_DIR}/src/${file}")
  set_target_properties(${target_name} PROPERTIES LINK_DEPENDS "${game_map};${LDSCRIPT_OVL}")
  target_link_options(${target_name} PRIVATE "-T${LDSCRIPT_OVL}" "-T${game_map}" "-Wl,--emit-relocs")
  target_compile_definitions(${target_name} PRIVATE ${game_define})
  target_compile_options(${target_name} PRIVATE "-fno-merge-constants" "-mno-explicit-relocs" "-mno-split-addresses")

  add_custom_command(
    OUTPUT "${ovl}"
    COMMAND "${TOOL_ZOVL}" "$<TARGET_FILE:${target_name}>" "${ovl}"
    DEPENDS ${target_name} "${TOOL_ZOVL}"
    COMMENT "Generating zactor for ${name}"
    VERBATIM
  )
  add_custom_target(
    ${target_name}_zovl ALL DEPENDS "${ovl}"
  )

  # Install
  install(FILES ${ovl} DESTINATION "bin/ovl/${game}")
endfunction()

function (add_actor_common)
  add_actor(oot ${ARGN})
  add_actor(mm ${ARGN})
endfunction()

build(oot)
build(mm)

add_actor_common(Obj_Kibako   "common/ovl/actors/Obj_Kibako/Obj_Kibako.c")
add_actor_common(Obj_Kibako2  "common/ovl/actors/Obj_Kibako2/Obj_Kibako2.c")
add_actor_common(Obj_Comb     "common/ovl/actors/Obj_Comb/Obj_Comb.c")
add_actor_common(En_Butte     "common/ovl/actors/En_Butte/En_Butte.c")

add_actor(oot Obj_Mure    "oot/ovl/actors/Obj_Mure/Obj_Mure.c")
add_actor(oot Obj_Mure2   "oot/ovl/actors/Obj_Mure2/Obj_Mure2.c")
add_actor(oot Obj_Tsubo   "oot/ovl/actors/Obj_Tsubo/Obj_Tsubo.c")
add_actor(oot En_Mag      "oot/ovl/actors/En_Mag/En_Mag.c")

add_actor(mm Boss_01            "mm/ovl/actors/Boss_01/Boss_01.c")
add_actor(mm Obj_Mure           "mm/ovl/actors/Obj_Mure/Obj_Mure.c")
add_actor(mm Obj_Tsubo          "mm/ovl/actors/Obj_Tsubo/Obj_Tsubo.c")
add_actor(mm Obj_Taru           "mm/ovl/actors/Obj_Taru/Obj_Taru.c")
add_actor(mm Obj_Flowerpot      "mm/ovl/actors/Obj_Flowerpot/Obj_Flowerpot.c")
add_actor(mm Obj_Snowball       "mm/ovl/actors/Obj_Snowball/Obj_Snowball.c")
add_actor(mm Obj_Snowball2      "mm/ovl/actors/Obj_Snowball2/Obj_Snowball2.c")
add_actor(mm En_Hs              "mm/ovl/actors/En_Hs/En_Hs.c")
add_actor(mm En_Goroiwa         "mm/ovl/actors/En_Goroiwa/En_Goroiwa.c")
add_actor(mm En_Nwc             "mm/ovl/actors/En_Nwc/En_Nwc.c")
add_actor(mm En_Test4           "mm/ovl/actors/En_Test4/En_Test4.c")
add_actor(mm En_Yb              "mm/ovl/actors/En_Yb/En_Yb.c")
add_actor(mm Bg_Ingate          "mm/ovl/actors/Bg_Ingate/Bg_Ingate.c")
add_actor(mm Custom_Magic_Fire  "mm/ovl/actors/Custom_Magic_Fire/Custom_Magic_Fire.c")
add_actor(mm Custom_Magic_Wind  "mm/ovl/actors/Custom_Magic_Wind/Custom_Magic_Wind.c")
add_actor(mm Custom_Magic_Dark  "mm/ovl/actors/Custom_Magic_Dark/Custom_Magic_Dark.c")
