set(TARGET stackmachine.test)

find_package(GTest)

if (GTEST_FOUND)
    set(SOURCES
        ../interpreter.cpp
        ../instructions.cpp
        interpreter_test.cpp
        main.cpp
    )

    add_executable(${TARGET} ${SOURCES})

    message("GTEST_INCLUDE_DIRS=${GTEST_INCLUDE_DIRS}")
    message("GTEST_BOTH_LIBRARIES=${GTEST_BOTH_LIBRARIES}")

    target_include_directories(${TARGET} PRIVATE ${GTEST_INCLUDE_DIRS})

    target_link_libraries(${TARGET} ${GTEST_BOTH_LIBRARIES})

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        find_package(Threads)
        target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT})
    endif ()

    add_test(NAME ${TARGET}
        COMMAND ${TARGET}
    )
ENDIF (GTEST_FOUND)
