#[[LC_NOTICE_BEGIN
LC_NOTICE_END]]

include(CTest)
Include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.2.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
	LC_testing_utils
	GIT_REPOSITORY https://github.com/LucaCiucci/LC_testing_utils
	GIT_TAG        v0.0.3
)
FetchContent_MakeAvailable(LC_testing_utils)

add_executable(unit_test)
set_target_properties(unit_test PROPERTIES CXX_STANDARD 20)

target_sources(
	unit_test
	PRIVATE
		"tmp.cpp"
		"Tensor/fixed_shape.cpp"
		"Tensor/shape/TensorDim.cpp"
		"Tensor/shape/TensorShape.cpp"
		"Tensor/shape/TensorPartialIndexValue.cpp"
		"Tensor/shape/TensorIndex.cpp"
		"Tensor/shape/PartialFixedTensorShapeHolder.cpp"
)

target_link_libraries(
	unit_test
	PRIVATE
		LC_math_linalg
		LC_testing_utils
		Catch2::Catch2WithMain
)

add_test(NAME UnitTest COMMAND unit_test)