project(dsd)
cmake_minimum_required(VERSION 2.8.11)

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

include(git_revision)
git_describe(GIT_TAG)

find_package(LibSndFile REQUIRED)
find_package(MBE REQUIRED)
find_package(ITPP REQUIRED)
find_package(LibPortAudio)

include_directories(SYSTEM ${LIBSNDFILE_INCLUDE_DIR} ${MBE_INCLUDE_DIR} ${ITPP_INCLUDE_DIR})
set(LIBS ${MBE_LIBRARY} ${LIBSNDFILE_LIBRARY} ${ITPP_LIBRARY})

if(PORTAUDIO_FOUND)
    include_directories(SYSTEM ${PORTAUDIO_INCLUDE_DIRS})
    list(APPEND LIBS ${PORTAUDIO_LIBRARIES})
    add_definitions(-DUSE_PORTAUDIO)
endif(PORTAUDIO_FOUND)

FILE(GLOB SRCS src/*.c src/*.cpp)
FILE(GLOB HEADERS include/*.h include/*.hpp)

configure_file("src/git_ver.c.in" "${CMAKE_CURRENT_BINARY_DIR}/git_ver.c" @ONLY)
list(APPEND SRCS "${CMAKE_CURRENT_BINARY_DIR}/git_ver.c")

include_directories("${PROJECT_SOURCE_DIR}/include")

ADD_EXECUTABLE(dsd ${SRCS} ${HEADERS})
TARGET_LINK_LIBRARIES(dsd ${LIBS})

include(GNUInstallDirs)
install(TARGETS dsd DESTINATION ${CMAKE_INSTALL_BINDIR})

# man page
find_program(HELP2MAN_FOUND help2man)
if (HELP2MAN_FOUND)
    add_custom_command(TARGET dsd POST_BUILD
        COMMAND help2man
        ARGS -n "Digital Speech Decoder"
            --no-discard-stderr
            --version-string=${GIT_TAG}
            -o ${CMAKE_CURRENT_BINARY_DIR}/dsd.1
            --no-info
            $<TARGET_FILE:dsd> 
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dsd.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
endif()

# uninstall target
configure_file(
    "cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

option(DISABLE_TEST "Disable building of test framework." OFF)

if (NOT DISABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
