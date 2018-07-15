set(BOOST_INCLUDEDIR ${PROJECT_DIR}/Third_Party/include)
set(BOOST_LIBRARYDIR ${PROJECT_DIR}/Third_Party/lib)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.58.0 COMPONENTS program_options)

if(Boost_FOUND)
    message(STATUS "Found Boost include dir - ${Green}${Boost_INCLUDE_DIR}${ColourReset}")
    message(STATUS "Found Boost library dir - ${Green}${Boost_LIBRARY_DIR_RELEASE}${ColourReset}")
else()
    message(WARNING ${Red}"Boost not found"${ColourReset})
endif()
