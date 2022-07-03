message("Add example 1: Orbit Camera Control")
add_executable(orbit_cam orbit_camera.cpp)
target_link_libraries(orbit_cam PRIVATE Raygen)

message("Add example 2: 2D mass string game")
add_executable(mass_spring mass_spring.cpp)
target_link_libraries(mass_spring PRIVATE Raygen)
