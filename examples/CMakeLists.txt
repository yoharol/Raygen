message("Add example 1: Orbit Camera Control")
add_executable(orbit_cam orbit_camera.cpp)
target_link_libraries(orbit_cam PRIVATE Raygen)