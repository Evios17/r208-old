
# Configure catch3
include_directories(./catch3/)
add_library(cmi_catch3 SHARED ./catch3/catch_amalgamated.cpp)

# Add tests
add_executable(cmi_tests cmi_tests.cpp)
target_link_libraries(cmi_tests ChessMoveInterface cmi_catch3)
add_test(CMI_TESTS cmi_tests)


