project(examples CXX)

set(boost_lib "boost_program_options-vc120-mt-${Boost_MAJOR_VERSION}_${Boost_MINOR_VERSION}_${Boost_SUBMINOR_VERSION}")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(boost_lib "boost_program_options-vc120-mt-gd-${Boost_MAJOR_VERSION}_${Boost_MINOR_VERSION}_${Boost_SUBMINOR_VERSION}")
endif()

include_directories(.)
add_executable(example example.cpp)

if(WIN32)
    add_definitions(-DBOOST_ALL_DYN_LINK)
    set(TO_LINK_LIBS cppkin ${boost_lib}.dll Core${CMAKE_DEBUG_POSTFIX}.dll curl)
else()
    set(TO_LINK_LIBS cppkin boost_program_options Core${CMAKE_DEBUG_POSTFIX} curl)
endif()
target_link_libraries(example ${TO_LINK_LIBS})
