add_library(BinomialHeap "")

target_sources(
        BinomialHeap
        PRIVATE
        BinomialHeap.cpp
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/BinomialHeap.h
)

target_include_directories(
        BinomialHeap
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)