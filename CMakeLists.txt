cmake_minimum_required(VERSION 3.13)
project( ostreams )

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include("${CMAKE_SOURCE_DIR}/glean/CMakeListsCompiler.txt")
include("${CMAKE_SOURCE_DIR}/glean/CMakeLists.txt")

set( HEADER_FOLDER "include" )
set( TEST_FOLDER "tests" )

include_directories( ${HEADER_FOLDER} )

set( HEADER_FILES
	${HEADER_FOLDER}/daw/io/bigint.h
	${HEADER_FOLDER}/daw/io/impl/bigint_impl.h
	${HEADER_FOLDER}/daw/io/console_stream.h
	${HEADER_FOLDER}/daw/io/file_stream.h
	${HEADER_FOLDER}/daw/io/memory_stream.h
	${HEADER_FOLDER}/daw/io/ostreams.h
	${HEADER_FOLDER}/daw/io/ostream_converters.h
	${HEADER_FOLDER}/daw/io/ostream_converters_float.h
	${HEADER_FOLDER}/daw/io/ostream_converters_impl.h
	${HEADER_FOLDER}/daw/io/ostream_converters_int.h
	${HEADER_FOLDER}/daw/io/ostream_helpers.h
	${HEADER_FOLDER}/daw/io/static_string.h
)

add_executable( console_test ${HEADER_FILES} ${TEST_FOLDER}/console_test.cpp )
add_executable( file_test ${HEADER_FILES} ${TEST_FOLDER}/file_test.cpp )
add_executable( memory_test ${HEADER_FILES} ${TEST_FOLDER}/memory_test.cpp )
add_executable( simple_console ${HEADER_FILES} ${TEST_FOLDER}/simple_console.cpp )
add_executable( benchmark ${HEADER_FILES} ${TEST_FOLDER}/benchmark.h ${TEST_FOLDER}/benchmark.cpp )
add_executable( double_benchmark ${HEADER_FILES} ${TEST_FOLDER}/benchmark.h ${TEST_FOLDER}/double_benchmark.cpp )
add_executable( floating_round_trip ${HEADER_FILES} ${TEST_FOLDER}/floating_round_trip.cpp )
add_executable( to_os_string_test ${HEADER_FILES} ${TEST_FOLDER}/to_os_string_test.cpp )
add_executable( bigint_test ${HEADER_FILES} ${TEST_FOLDER}/bigint_test.cpp )

install( DIRECTORY ${HEADER_FOLDER}/daw DESTINATION include )
