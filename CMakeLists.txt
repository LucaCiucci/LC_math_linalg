#[[LC_NOTICE_BEGIN
LC_NOTICE_END]]

cmake_minimum_required(VERSION 3.20)

################################
#           PROJECT
################################

project(
	LC_math_linalg
	VERSION 0.0.0
	DESCRIPTION "A C++ library for vectors, matrices and other basic linear algebra stuff"
	LANGUAGES CXX
)

################################
#          LIBRARY
################################

add_library(LC_math_linalg STATIC)

target_sources(LC_math_linalg PRIVATE 
	"src/tmp.cpp"
	"src/tmp/string_utils.cpp"
 "reasons.cpp")

set_target_properties(LC_math_linalg PROPERTIES CXX_STANDARD 20)

target_include_directories(
	LC_math_linalg
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>

		# once installed, users will use this path to include this library
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/LC_base_classes>
)
target_include_directories(
	LC_math_linalg
	PUBLIC
		include
)

#add_executable(e "tests/tmp.cpp")
#target_link_libraries(e PRIVATE LC_math_linalg)
#set_target_properties(e PROPERTIES CXX_STANDARD 20)

# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#top
find_package(Catch2 3 REQUIRED)
# Note: Catch2WithMain will provide us a default main
#target_link_libraries(e PRIVATE Catch2::Catch2WithMain)

add_test(NAME UnitTest COMMAND unit_test)


add_executable(reasons "reasons.cpp")
set_target_properties(reasons PROPERTIES CXX_STANDARD 20)
target_link_libraries(reasons PRIVATE LC_math_linalg)