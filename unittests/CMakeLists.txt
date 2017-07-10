cmake_minimum_required(VERSION 2.6)

set(PROGRAM_NAME test-lazy-gltf2)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(ADDITIONAL_INCLUDES
    ../include
    ../external-deps/include
)
include_directories(${ADDITIONAL_INCLUDES})

set(UNITTEST_SRC
    ../include/lazy_gltf2.hpp
    src/common.hpp
    src/main_tests.cpp
    src/test_AnimatedMorphCube.cpp
    src/test_boombox.cpp
    src/test_box.cpp
    src/test_cameras.cpp
    src/test_lantern.cpp
    src/test_monster.cpp
    src/test_TwoSidedPlane.cpp
    src/test_strings.cpp
)
 
add_executable(${PROGRAM_NAME} ${UNITTEST_SRC})
target_link_libraries(${PROGRAM_NAME} ${GTEST_LIBRARIES} pthread)
