file(GLOB_RECURSE SOURCES "*.s")

mips_build_bin(codex "${CMAKE_CURRENT_SOURCE_DIR}/codex.ld" "${CMAKE_BINARY_DIR}/codex" ${SOURCES})
