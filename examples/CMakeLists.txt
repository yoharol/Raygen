add_definitions(-DASSETSPATH="${PROJECT_SOURCE_DIR}/assets/")

message("Add example 1: Orbit Camera Control")
add_executable(orbit_cam orbit_camera.cpp)
target_link_libraries(orbit_cam PRIVATE Raygen)

message("Add example 2: 2D mass string game")
add_executable(mass_spring mass_spring.cpp)
target_link_libraries(mass_spring PRIVATE Raygen)

message("Add example 3: Parallel Computing")
add_executable(parallel_computing parallel_computing.cpp)
target_link_libraries(parallel_computing PRIVATE Raygen)

message("Add example 4: 2D paralleled mass string game")
add_executable(mass_spring_parallel mass_spring_parallel.cpp)
target_link_libraries(mass_spring_parallel PRIVATE Raygen)

message("Add example 4: alembic modeling")
find_package(Imath)
find_package(Alembic)
add_executable(alembic_modeling alembic_modeling.cpp)
target_link_libraries(alembic_modeling PRIVATE
  Raygen
  Imath::Imath
  Alembic::Alembic
)
