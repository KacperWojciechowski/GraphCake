set(SOURCES AdjList.cpp
            AdjMatrix.cpp
            Benchmark.cpp
            ColoringAlgorithms.cpp
            Deserializer.cpp)

add_library(GraphCake STATIC ${SOURCES})
target_include_directories(GraphCake PUBLIC ${PROJECT_SOURCE_DIR}/inc)
set_target_properties(GraphCake PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})