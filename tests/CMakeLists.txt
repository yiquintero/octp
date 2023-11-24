cmake_minimum_required(VERSION 3.14)

# Suppress warning msgs
cmake_policy(SET CMP0135 NEW)

project(octptests)

# TEST_IN_GITHUB_ACTIONS is ON if tests are run as part of GitHub Actions workflow, it's OFF otherwise
set (TEST_IN_GITHUB_ACTIONS OFF CACHE INTERNAL "Tests are built and run as part of a GitHub actions workflow")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch and download Google Test
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip
)
# Windows: Prevent overriding the parent project's compiler/linker settings
# Currently no parent CMake project, but good to keep for the future
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#Enable testing in CMake
enable_testing()

# Create an executable for the tests
add_executable(
    testoctp
    testoctp.cpp
)

# link the googletest library to the tests
target_link_libraries(
    testoctp
    GTest::gtest_main
)

# Enable CMake’s test runner to discover the tests included in the binary,
# using the GoogleTest CMake module.
include(GoogleTest)
gtest_discover_tests(testoctp)

# Get LAMMPS
if (TEST_IN_GITHUB_ACTIONS)
  # lammps was previously downloaded and built as part of the workflow
  set (LAMMPS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../lammps/src/")
else()
  message(STATUS "Downloading LAMMPS")
  # download lammps
  FetchContent_Declare(
    lammps
    URL https://github.com/lammps/lammps/archive/refs/tags/stable_2Aug2023_update1.zip
  )
  FetchContent_Populate(lammps)
  set (LAMMPS_SOURCE_DIR "${lammps_SOURCE_DIR}/src/")
    # copy octp files to lammps dir
    file(GLOB filesToCopy "${CMAKE_SOURCE_DIR}/../src/*")
    file(COPY ${filesToCopy} DESTINATION ${LAMMPS_SOURCE_DIR})
    # build lammps+octp as a post-build event of the testoctp
    add_custom_command(
      TARGET testoctp POST_BUILD 
      COMMAND cd ${LAMMPS_SOURCE_DIR} &&
        make yes-asphere &&
        make yes-body &&
        make yes-class2 &&
        make yes-dipole &&
        make yes-granular &&
        make yes-kspace &&
        make yes-manybody &&
        make yes-molecule &&
        make yes-rigid &&
        make yes-shock &&
        make -j 8 mpi
      WORKING_DIRECTORY ${lammps_SOURCE_DIR}
      VERBATIM
      COMMENT "Build LAMMPS with OCTP plugin"
    )
endif()

# Add preprocessor definitions used in the tests
add_compile_definitions(TEST_DATA_PATH="${CMAKE_SOURCE_DIR}/data/")
add_compile_definitions(LAMMPS_PATH=${LAMMPS_SOURCE_DIR})



