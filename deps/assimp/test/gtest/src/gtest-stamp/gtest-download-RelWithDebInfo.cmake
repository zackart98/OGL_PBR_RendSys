

set(command "/home/zack/programs/cmake-3.8.0/bin/cmake;-P;/home/zack/workspace/PBR/deps/assimp/test/gtest/tmp/gtest-gitclone.cmake")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/zack/workspace/PBR/deps/assimp/test/gtest/src/gtest-stamp/gtest-download-out.log"
  ERROR_FILE "/home/zack/workspace/PBR/deps/assimp/test/gtest/src/gtest-stamp/gtest-download-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/zack/workspace/PBR/deps/assimp/test/gtest/src/gtest-stamp/gtest-download-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "gtest download command succeeded.  See also /home/zack/workspace/PBR/deps/assimp/test/gtest/src/gtest-stamp/gtest-download-*.log")
  message(STATUS "${msg}")
endif()
