add_library(FibHeap "")
target_sources(
        FibHeap
        PRIVATE
        FibHeap.cpp
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/FibHeap.h
)

target_include_directories(
        FibHeap
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)