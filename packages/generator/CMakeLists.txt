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

set(FLAGS "-EB -static -fomit-frame-pointer -ffreestanding -nostdlib -nostdinc -fno-PIC -mabi=32 -mno-shared -mno-abicalls -march=vr4300 -mtune=vr4300 -mfix4300 -mfix-r4000 -G 0 -Os -Wall -Werror=implicit-function-declaration -Werror=implicit-int -Wimplicit-fallthrough -Wl,--build-id=none")
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

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/../../third_party/ultralib/include" "${CMAKE_SOURCE_DIR}/../../third_party/ultralib/include/compiler/modern_gcc")
include_directories("${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/build/include" "${CMAKE_SOURCE_DIR}/../data/dist/include")

add_compile_definitions("F3DEX_GBI_2=1")

# Tools
set(TOOL_DIR  "${CMAKE_SOURCE_DIR}/../tools/bin")
set(TOOL_ZOVL "${TOOL_DIR}/zovl")

# Linker scripts
set(LDSCRIPT_IN "${CMAKE_SOURCE_DIR}/src/link.ld.in")
set(LDSCRIPT_OVL "${CMAKE_SOURCE_DIR}/src/link_ovl.ld")

function(build game)
  # Includes for OVLs
  set(OVL_INCLUDE_DIRS "")
  list(APPEND OVL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src/common/ovl")
  list(APPEND OVL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src/${game}/ovl")

  # Common stuff
  string(TOUPPER ${game} game_upper)
  set(game_define "-DGAME_${game_upper}=1")

  # Overlays
  set(OVL_OBJECTS "")
  set(OVL_NAMES "")
  set(shared_mm_overlays Bg_Ice_Shelter Standalone_GaroMaster Standalone_Gomess Standalone_Igos Standalone_Keeta Standalone_Slime Standalone_Snapper Standalone_Wart Standalone_Wiz En_Tanron1 En_Tanron3 En_Estone)
  set(Bg_Ice_Shelter_SOURCE "src/oot/ovl/actors/Bg_Ice_Shelter/Bg_Ice_Shelter.c")
  set(Standalone_GaroMaster_SOURCE "src/oot/enemy_ports/generated/mm_en_jso2.c")
  set(Standalone_Gomess_SOURCE "src/oot/enemy_ports/generated/mm_en_death.c")
  set(Standalone_Igos_SOURCE "src/oot/enemy_ports/generated/mm_en_knight.c")
  set(Standalone_Keeta_SOURCE "src/oot/enemy_ports/generated/mm_en_bsb.c")
  set(Standalone_Slime_SOURCE "src/oot/enemy_ports/generated/mm_en_bigslime.c")
  set(Standalone_Snapper_SOURCE "src/oot/enemy_ports/generated/mm_en_pametfrog.c")
  set(Standalone_Wiz_SOURCE "src/oot/enemy_ports/generated/mm_en_wiz.c")
  set(Standalone_Wart_SOURCE "src/oot/enemy_ports/generated/mm_boss_04.c")
  set(En_Tanron1_SOURCE "src/oot/enemy_ports/generated/mm_en_tanron1.c")
  set(En_Tanron3_SOURCE "src/oot/enemy_ports/generated/mm_en_tanron3.c")
  set(En_Estone_SOURCE "src/oot/enemy_ports/generated/mm_en_estone.c")
  foreach(ovl_category actors effects gamemodes)
    # Extract the list of OVLs for the current category
    set(ovls "")
    file(GLOB ovl_dirs "src/${game}/ovl/${ovl_category}/*" "src/common/ovl/${ovl_category}/*")
    foreach (ovl_dir ${ovl_dirs})
      get_filename_component(ovl_name "${ovl_dir}" NAME)
      list(APPEND ovls "${ovl_name}")
    endforeach()
    if(game STREQUAL "mm" AND ovl_category STREQUAL "actors")
      list(APPEND ovls ${shared_mm_overlays})
    endif()
    list(REMOVE_DUPLICATES ovls)

    # Process the OVLs
    foreach(ovl ${ovls})
      file(GLOB_RECURSE OVL_SOURCES "src/${game}/ovl/${ovl_category}/${ovl}/*.c" "src/${game}/ovl/${ovl_category}/${ovl}/*.S" "src/common/ovl/${ovl_category}/${ovl}/*.c" "src/common/ovl/${ovl_category}/${ovl}/*.S")

      if(game STREQUAL "mm" AND DEFINED ${ovl}_SOURCE)
        set(OVL_SOURCES "${${ovl}_SOURCE}")
      endif()

      # Compile the OVL into objects
      set(target_name "${game}_ovl_${ovl}")
      add_library(${target_name} OBJECT ${OVL_SOURCES})
      target_compile_options(${target_name} PRIVATE "-fno-merge-constants" "-mno-explicit-relocs" "-mno-split-addresses")
      target_compile_definitions(${target_name} PRIVATE ${game_define})
      target_include_directories(${target_name} PRIVATE ${OVL_INCLUDE_DIRS})

      # Merge the objects into a single file, with fixed segments
      set(target_file "${CMAKE_BINARY_DIR}/${game}_ovl_${ovl}.o")
      add_custom_command(
        OUTPUT "${target_file}"
        COMMAND "${ARCH}-ld" -r -o "${target_file}" "-T${LDSCRIPT_OVL}" $<TARGET_OBJECTS:${target_name}>
        COMMAND "${ARCH}-objcopy"
          --rename-section .text=.${ovl}.text
          --rename-section .data=.${ovl}.data
          --rename-section .rodata=.${ovl}.rodata
          --rename-section .bss=.${ovl}.bss
          --rename-section .meta=.${ovl}.meta
          "${target_file}"
        DEPENDS ${target_name} $<TARGET_OBJECTS:${target_name}> "${LDSCRIPT_OVL}"
        COMMENT "Merging objects for ${ovl}"
        VERBATIM
        COMMAND_EXPAND_LISTS
      )

      list(APPEND OVL_OBJECTS ${target_file})
      list(APPEND OVL_NAMES ${ovl})
    endforeach()
  endforeach()

  file(GLOB NATIVE_PORT_SOURCES "src/${game}/enemy_ports/generated/oot_*.c" "src/${game}/enemy_ports/generated/mm_*.c")
  list(SORT NATIVE_PORT_SOURCES)
  foreach(port_source ${NATIVE_PORT_SOURCES})
    get_filename_component(port_stem "${port_source}" NAME_WE)
    set(ovl "Native_Enemy_${port_stem}")
    set(target_name "${game}_native_${port_stem}")
    add_library(${target_name} OBJECT "${port_source}")
    target_compile_options(${target_name} PRIVATE "-fno-merge-constants" "-mno-explicit-relocs" "-mno-split-addresses")
    target_compile_definitions(${target_name} PRIVATE ${game_define})
    target_include_directories(${target_name} PRIVATE ${OVL_INCLUDE_DIRS})
    set(target_file "${CMAKE_BINARY_DIR}/${game}_ovl_${ovl}.o")
    add_custom_command(
      OUTPUT "${target_file}"
      COMMAND "${ARCH}-ld" -r -o "${target_file}" "-T${LDSCRIPT_OVL}" $<TARGET_OBJECTS:${target_name}>
      COMMAND "${ARCH}-objcopy"
        --rename-section .text=.${ovl}.text
        --rename-section .data=.${ovl}.data
        --rename-section .rodata=.${ovl}.rodata
        --rename-section .bss=.${ovl}.bss
        --rename-section .meta=.${ovl}.meta
        "${target_file}"
      DEPENDS ${target_name} $<TARGET_OBJECTS:${target_name}> "${LDSCRIPT_OVL}"
      COMMENT "Preparing native enemy overlay ${port_stem}"
      VERBATIM
      COMMAND_EXPAND_LISTS
    )
    list(APPEND OVL_OBJECTS ${target_file})
    list(APPEND OVL_NAMES ${ovl})
  endforeach()

  # Linker script
  set(ldscript_name "link_${game}.ld")
  set(ldscript_assets_name "link_assets_${game}.ld")
  set(ldscript "${CMAKE_BINARY_DIR}/${ldscript_name}")
  set(ldscript_assets "${CMAKE_SOURCE_DIR}/build/${ldscript_assets_name}")
  set(OVL_DEFINE "${OVL_NAMES}")
  list(TRANSFORM OVL_DEFINE PREPEND "OVL(")
  list(TRANSFORM OVL_DEFINE APPEND ")")
  string(REPLACE ";" " " OVL_DEFINE "${OVL_DEFINE}")

  add_custom_command(
    OUTPUT "${ldscript}"
    COMMAND "${CMAKE_C_COMPILER}" ${game_define} "-D __OVERLAYS__=${OVL_DEFINE}" -I "${CMAKE_SOURCE_DIR}/include" -I "${CMAKE_SOURCE_DIR}/build" -E -P -x c "${LDSCRIPT_IN}" -o "${ldscript}" -MMD -MF "${ldscript}.d" -MT "${ldscript_name}"
    DEPENDS "${LDSCRIPT_IN}" "${ldscript_assets}"
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
  list(REMOVE_ITEM SOURCES ${NATIVE_PORT_SOURCES})
  # These handwritten ports are included by their generated overlay wrappers.
  # Compile each only into its overlay, never into the resident payload as well.
  if(game STREQUAL "mm")
    foreach(port stalfos lizalfos moblin skull_kid)
      list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/mm/enemy_ports/${port}.c")
    endforeach()
  else()
    list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/oot/enemy_ports/garo.c")
  endif()

  add_executable(${game} ${SOURCES} ${OVL_OBJECTS})
  # Main-payload functions/data can be stripped individually. The linker script
  # keeps patch/export roots and every separately loaded overlay intact.
  target_compile_options(${game} PRIVATE "-ffunction-sections" "-fdata-sections")
  target_link_options(${game} PRIVATE "-Wl,--gc-sections")
  set_target_properties(${game} PROPERTIES LINK_DEPENDS "${ldscript}")
  add_dependencies(${game} ldscript_${game})
  target_compile_definitions(${game} PRIVATE ${game_define})
  target_link_libraries(${game})
  target_link_options(${game} PRIVATE "-Wl,--emit-relocs" "-T${ldscript}")
  target_include_directories(${game} PRIVATE ${OVL_INCLUDE_DIRS})

  # Extract main sections
  set(game_payload "${CMAKE_BINARY_DIR}/${game}_payload.bin")
  set(game_patch "${CMAKE_BINARY_DIR}/${game}_patch.bin")
  set(game_symbols_name "${CMAKE_BINARY_DIR}/${game}_symbols_name.bin")
  set(game_symbols_addr "${CMAKE_BINARY_DIR}/${game}_symbols_addr.bin")
  add_custom_command(
    OUTPUT
      "${game_payload}"
      "${game_patch}"
      "${game_symbols_name}"
      "${game_symbols_addr}"
    COMMAND "${ARCH}-objcopy" --only-section=.text -O binary "$<TARGET_FILE:${game}>" ${game_payload}
    COMMAND "${ARCH}-objcopy" --only-section=.patch -O binary "$<TARGET_FILE:${game}>" ${game_patch}
    COMMAND "${ARCH}-objcopy" --only-section=.symbols_name -O binary "$<TARGET_FILE:${game}>" ${game_symbols_name}
    COMMAND "${ARCH}-objcopy" --only-section=.symbols_addr -O binary "$<TARGET_FILE:${game}>" ${game_symbols_addr}
    DEPENDS ${game}
    COMMENT "Extracting sections for ${game}"
    VERBATIM
  )
  add_custom_target(
    ${game}_sections ALL DEPENDS "${game_payload}" "${game_patch}" "${game_symbols_name}" "${game_symbols_addr}"
  )
  install(FILES ${game_payload} ${game_patch} ${game_symbols_name} ${game_symbols_addr} DESTINATION "bin")

  # Extract overlays
  foreach(ovl ${OVL_NAMES})
    set(ovl_extracted "${CMAKE_BINARY_DIR}/${game}_ovl_${ovl}.zovlx")
    add_custom_command(
      OUTPUT "${ovl_extracted}"
      COMMAND "${TOOL_ZOVL}" "$<TARGET_FILE:${game}>" "${ovl}" "${ovl_extracted}"
      DEPENDS ${game}
      COMMENT "Generating zovlx for ${ovl}"
      VERBATIM
      USES_TERMINAL
    )
    add_custom_target(zovlx_${game}_${ovl} ALL DEPENDS "${ovl_extracted}")
    install(FILES ${ovl_extracted} DESTINATION "bin/ovl/${game}")
  endforeach()
endfunction()

# Build
foreach(game oot mm)
  build(${game})
endforeach()
