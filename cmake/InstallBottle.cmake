if (NOT Bottle_FOUND)
    ExternalProject_Add(Bottle
            URL                 https://github.com/bottlepy/bottle/archive/0.12.13.tar.gz
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     cp <SOURCE_DIR>/bottle.py ${PROJECT_SOURCE_DIR}/tests
            TEST_COMMAND        ""
            )

    set (Bottle_FOUND             YES)
endif ()
