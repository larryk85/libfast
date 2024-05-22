# ##################################################################################################
# Define the main test executable.
# ##################################################################################################
add_executable( 
   libfast_unit_tests 
   fast.cpp
)

target_link_libraries( libfast_unit_tests PRIVATE fast::fast Catch2::Catch2WithMain )
catch_discover_tests(libfast_unit_tests)
