message("Add Test")

# include(CTest)
macro(add_pass_test arg)
  add_executable(${arg} ${arg}.cpp)
  target_link_libraries(${arg} PRIVATE Raygen)
  add_test(NAME ${arg}_test COMMAND ${arg})
  set_property(TEST ${arg}_test PROPERTY PASS_REGULAR_EXPRESSION "pass")
endmacro(add_pass_test arg)

add_pass_test(polar_test)
add_pass_test(color_test)

#add_executable()
