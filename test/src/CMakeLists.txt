set(UT_SOURCES GraphRepresentationsTests.cpp
               ColoringAlgorithmsTest.cpp
               DeserializerTest.cpp
               SerializerTest.cpp
               SerializationHelpers.cpp)

add_executable(Ut ${UT_SOURCES})
target_include_directories(Ut PUBLIC ${PROJECT_SOURCE_DIR}/inc ${PROJECT_SOURCE_DIR}/test/inc)
set_target_properties(Ut PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries(Ut PRIVATE GraphCake gtest_main gtest)
