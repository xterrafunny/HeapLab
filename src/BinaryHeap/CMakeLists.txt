add_library(BinaryHeap "")

target_sources(
        BinaryHeap
        PRIVATE
        BinaryHeap.cpp
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/BinaryHeap.h
)

target_include_directories(
        BinaryHeap
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)