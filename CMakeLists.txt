cmake_minimum_required(VERSION 3.12)
project( ostreams )

set(CMAKE_CXX_STANDARD 17)

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

