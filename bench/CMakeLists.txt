project(benchMark CXX)

include_directories(.)

add_executable(benchMark BenchMark.cpp)
set(TO_LINK_LIBS benchmark cppkin pthread)
target_link_libraries(benchMark ${TO_LINK_LIBS})
