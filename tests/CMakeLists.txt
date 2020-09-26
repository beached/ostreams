
add_executable( console_test src/console_test.cpp )
target_link_libraries( console_test daw::ostreams )

add_executable( file_test src/file_test.cpp )
target_link_libraries( file_test daw::ostreams )

add_executable( memory_test src/memory_test.cpp )
target_link_libraries( memory_test daw::ostreams )

add_executable( simple_console src/simple_console.cpp )
target_link_libraries( simple_console daw::ostreams )

add_executable( benchmark include/benchmark.h src/benchmark.cpp )
target_include_directories( benchmark PRIVATE include/ )
target_link_libraries( benchmark daw::ostreams )

add_executable( double_benchmark include/benchmark.h src/double_benchmark.cpp )
target_include_directories( double_benchmark PRIVATE include/ )
target_link_libraries( double_benchmark daw::ostreams )

add_executable( floating_round_trip src/floating_round_trip.cpp )
target_link_libraries( floating_round_trip daw::ostreams )

add_executable( to_os_string_test src/to_os_string_test.cpp )
target_link_libraries( to_os_string_test daw::ostreams )

add_executable( bigint_test src/bigint_test.cpp )
target_link_libraries( bigint_test daw::ostreams )


