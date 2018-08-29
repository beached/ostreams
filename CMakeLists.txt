cmake_minimum_required(VERSION 3.12)
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
set( SOURCE_FOLDER "src" )
set( TEST_FOLDER "tests" )

include_directories( ${HEADER_FOLDER} )

set( HEADER_FILES
	${HEADER_FOLDER}/ostreams.h
	${HEADER_FOLDER}/ostream_helpers.h
	${HEADER_FOLDER}/ostream_converters.h
)

add_executable( ostreams_test ${HEADER_FILES} ${TEST_FOLDER}/ostreams_test.cpp )

