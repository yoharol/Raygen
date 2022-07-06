message("Add alembic examples")

find_package(Imath)
find_package(Alembic)
add_executable(alembic_modeling alembic_modeling.cpp)
target_link_libraries(alembic_modeling PRIVATE
  Raygen
  Alembic::Alembic
)

add_executable(alembic_animation alembic_animation.cpp)
target_link_libraries(alembic_animation PRIVATE
  Alembic::Alembic
)