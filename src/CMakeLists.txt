add_library(Raygen INTERFACE)

target_include_directories(Raygen
  INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(Raygen
  INTERFACE
  Eigen3::Eigen
  raylib
)