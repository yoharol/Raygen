message("Add Test")

# include(CTest)

add_executable(polar_test polar_test.cpp)
target_link_libraries(polar_test PRIVATE Raygen)
add_test(NAME Polar_Angle_Test COMMAND polar_test)
set_property(TEST Polar_Angle_Test PROPERTY PASS_REGULAR_EXPRESSION "pass")

