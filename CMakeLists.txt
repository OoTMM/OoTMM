cmake_minimum_required(VERSION 3.10)
project(OoTMM)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(SOURCE_ROM_OOT  "${CMAKE_SOURCE_DIR}/roms/OoT.z64"  CACHE PATH "The Ocarina of Time ROM")
set(SOURCE_ROM_MM   "${CMAKE_SOURCE_DIR}/roms/MM.z64"   CACHE PATH "The Majora's Mask ROM")

set(RAW_DIR     "${CMAKE_BINARY_DIR}/raw")
set(RAW_DIR_OOT "${CMAKE_BINARY_DIR}/raw/oot")
set(RAW_DIR_MM  "${CMAKE_BINARY_DIR}/raw/mm")

set(ROM_OOTMM   "${CMAKE_BINARY_DIR}/OoTMM.z64")

add_subdirectory(src)

function(extract_rom rom dir)
  add_custom_command(
    OUTPUT "${dir}"
    COMMAND ruby "${CMAKE_SOURCE_DIR}/bin/extract" "${rom}" "${dir}"
    COMMENT "Extracting ${rom}"
    VERBATIM
  )
endfunction()

extract_rom("${SOURCE_ROM_OOT}" "${RAW_DIR_OOT}")

add_custom_command(
  OUTPUT "${ROM_OOTMM}"
  COMMAND ruby "${CMAKE_SOURCE_DIR}/bin/pack" "${ROM_OOTMM}" "${RAW_DIR_OOT}"
  DEPENDS "${RAW_DIR_OOT}"
  COMMENT "Packing ${ROM_OOTMM}"
  VERBATIM
)

add_custom_target(ootmm ALL DEPENDS "${ROM_OOTMM}" codex)
