project(Raygen)
cmake_minimum_required(VERSION 3.21)

if(PROJECT_IS_TOP_LEVEL)
  message(STATUS "Raygen is the top level project")
  message("Finding raylib...")
  find_package(raylib REQUIRED)
  message("Finding Eigen3...")
  find_package(Eigen3 REQUIRED)
else()
  if(NOT TARGET raylib)
    find_package(raylib REQUIRED)
  endif()
  if(NOT TARGET Eigen3::Eigen)
    find_package(EIGEN3 REQUIRED)
  endif()
endif()

add_subdirectory(src)

set(Raygen_libs
  Eigen3::Eigen
  raylib  
  Raygen
)

option(BUILD_EXAMPLES "Build the examples" ON)
option(BUILD_ALEMBIC_EXAMPLES "Build the alembic examples" OFF)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

option(BUILD_TESTS "Build ctest" ON)
if(BUILD_TESTS)
enable_testing()
  add_subdirectory(test)
endif()
