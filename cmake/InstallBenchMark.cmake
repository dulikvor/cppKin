if (NOT BENCHMARK_FOUND)

    ExternalProject_Add(BenchMark
            DOWNLOAD_NAME       benchmark-1.3.0.tar
            URL                 https://github.com/google/benchmark/archive/v1.3.0.tar.gz
            CONFIGURE_COMMAND   mkdir <SOURCE_DIR>/build && cd <SOURCE_DIR>/build && cmake -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBENCHMARK_ENABLE_INSTALL=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} ..
            BUILD_COMMAND       cd <SOURCE_DIR>/build && make
            INSTALL_COMMAND     cd <SOURCE_DIR>/build && make install
            TEST_COMMAND        ""
            )

    ExternalProject_Get_Property(BenchMark INSTALL_DIR)

    set (BENCHMARK_ROOT_DIR          ${INSTALL_DIR})
    set (BENCHMARK_INCLUDE_DIR       ${BENCHMARK_ROOT_DIR}/include)
    set (BENCHMARK_LIBRARY_DIR       ${BENCHMARK_ROOT_DIR}/lib)
    set (BENCHMARK_FOUND             YES)

endif ()
