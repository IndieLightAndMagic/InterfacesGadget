cmake_minimum_required(VERSION 3.5.1)

set(TARGET_NAME NetworkInterfacesUtil)
project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_FLAGS "-g -Werror=unused-parameter")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(DISTRO_INCLUDE ${CMAKE_SOURCE_DIR}/dist/include )
set(DISTRO_LIBRARY ${CMAKE_SOURCE_LIB}/dist/lib)



add_executable(networktest networktest.cpp networkinterfacesutil.cpp)
add_library(${TARGET_NAME} STATIC networkinterfacesutil.cpp networkinterfacesutil.h)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} cp lib${TARGET_NAME}.a ${DISTRO_LIBRARY}
    COMMAND ${CMAKE_COMMAND} cp ${CMAKE_SOURCE_DIR}/*.h ${DISTRO_INCLUDE}
)