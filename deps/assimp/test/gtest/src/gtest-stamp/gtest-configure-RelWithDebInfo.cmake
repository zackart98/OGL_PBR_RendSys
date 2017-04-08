

set(command "/home/zack/programs/cmake-3.8.0/bin/cmake;-DCMAKE_BUILD_TYPE=RelWithDebInfo;-Dgtest_force_shared_crt=ON;-Dgtest_disable_pthreads:BOOL=OFF;-DBUILD_GTEST=ON;-GUnix Makefiles;/home/zack/workspace/PBR/deps/assimp/test/gtest/src/gtest")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/zack/workspace/PBR/deps/assimp/test/gtest/src/gtest-stamp/gtest-configure-out.log"
  ERROR_FILE "/home/zack/workspace/PBR/deps/assimp/test/gtest/src/gtest-stamp/gtest-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/zack/workspace/PBR/deps/assimp/test/gtest/src/gtest-stamp/gtest-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "gtest configure command succeeded.  See also /home/zack/workspace/PBR/deps/assimp/test/gtest/src/gtest-stamp/gtest-configure-*.log")
  message(STATUS "${msg}")
endif()
