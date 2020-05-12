function(mips_assemble dst src)
  get_filename_component(dstdir "${dst}" DIRECTORY)
  add_custom_command(
    OUTPUT "${dst}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${dstdir}"
    COMMAND mips64-elf-as "${src}" -o "${dst}"
    DEPENDS "${src}"
    COMMENT "Building ${dst}"
    VERBATIM
  )
endfunction()

function(mips_build_bin tgt lscript dst)
  set(srcs "${ARGN}")
  set(objects "")
  foreach(src ${srcs})
    get_filename_component(objname "${src}" NAME)
    set(obj "${CMAKE_BINARY_DIR}/objs/${tgt}/${objname}.o")
    mips_assemble("${obj}" "${src}")
    list(APPEND objects "${obj}")
  endforeach()

  add_custom_command(
    OUTPUT "${dst}"
    COMMAND mips64-elf-ld -o "${dst}" -T "${lscript}" ${objects}
    DEPENDS ${objects} ${lscript}
    COMMENT "Building ${dst}"
    VERBATIM
  )
  add_custom_target(${tgt} DEPENDS "${dst}")
endfunction()

add_subdirectory(codex)
add_subdirectory(tools)
