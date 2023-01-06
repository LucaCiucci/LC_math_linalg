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
message(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION} (${CMAKE_BUILD_TYPE})")

set(LC_math_linalg_MAIN_PROJECT FALSE)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR AND CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(LC_math_linalg_MAIN_PROJECT TRUE)
endif()

################################
#          LIBRARY
################################

add_library(LC_math_linalg STATIC)

target_sources(LC_math_linalg PRIVATE 
	"src/tmp.cpp"
	"src/tmp/string_utils.cpp"
)

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


if (LC_math_linalg_MAIN_PROJECT)
	add_executable(tmp_elimina "tmp_elimina.cpp")
	set_target_properties(tmp_elimina PROPERTIES CXX_STANDARD 20)
	target_link_libraries(tmp_elimina PRIVATE LC_math_linalg)
endif()

if (LC_math_linalg_MAIN_PROJECT)
	# ??? copilot-> enable_testing()
	enable_testing()
	add_subdirectory(tests)
endif()
