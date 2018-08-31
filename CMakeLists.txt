cmake_minimum_required(VERSION 3.10)
project( ostreams )

set(CMAKE_CXX_STANDARD 14)

if( EXISTS "${CMAKE_SOURCE_DIR}/dependent_projects/build" )
	add_custom_target( dependency_stub )
	include_directories( SYSTEM "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/include" )
	#	link_directories( "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/lib" )
else( )
	set( SHOULD_BUILD_DEPS ON )
	include( "${CMAKE_SOURCE_DIR}/dependent_projects/dependencies.txt" )

	add_library( dependency_stub STATIC "${CMAKE_SOURCE_DIR}/dependent_projects/stub.cpp" )
	add_dependencies( dependency_stub header_libraries_prj )
include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
endif( )

set( HEADER_FOLDER "include" )
set( TEST_FOLDER "tests" )

include_directories( ${HEADER_FOLDER} )

set( HEADER_FILES
	${HEADER_FOLDER}/daw/io/console_stream.h
	${HEADER_FOLDER}/daw/io/file_stream.h
	${HEADER_FOLDER}/daw/io/memory_stream.h
	${HEADER_FOLDER}/daw/io/ostreams.h
	${HEADER_FOLDER}/daw/io/ostream_converters.h
	${HEADER_FOLDER}/daw/io/ostream_helpers.h
	${HEADER_FOLDER}/daw/io/static_string.h
)

add_executable( console_test ${HEADER_FILES} ${TEST_FOLDER}/console_test.cpp )
add_executable( file_test ${HEADER_FILES} ${TEST_FOLDER}/file_test.cpp )
add_executable( memory_test ${HEADER_FILES} ${TEST_FOLDER}/memory_test.cpp )
add_executable( memory_bench ${HEADER_FILES} ${TEST_FOLDER}/benchmark.h ${TEST_FOLDER}/memory_bench.cpp )

install( DIRECTORY ${HEADER_FOLDER}/daw DESTINATION include )
