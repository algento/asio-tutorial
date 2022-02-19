# FindAsio- locate the asio library.
#
# This module defines the following variables (on success):
# ASIO_INCLUDE_DIR - where to find asio.hpp
# ASIO_FOUND - if the library was successfully located
#
# ASIO_ROOT_DIR - root directory of a glm installation

message(STATUS "Using FindASIO.cmake")

if(UNIX)
    set(_asio_HEADER_SEARCH_DIRS
        "/usr/include/"
        "/usr/local/include"
        #custom location
    )
else()
    set(_asio_HEADER_SEARCH_DIRS
        "${CMAKE_SOURCE_DIR}/apsim/external/asio/include" #custom location
    )
endif()

# check environment variable
set(_asio_ENV_ROOT_DIR "$ENV{ASIO_ROOT_DIR}")

if(NOT ASIO_ROOT_DIR AND _asio_ENV_ROOT_DIR)
	set(ASIO_ROOT_DIR "${_asio_ENV_ROOT_DIR}")
endif(NOT ASIO_ROOT_DIR AND _asio_ENV_ROOT_DIR)

if(ASIO_ROOT_DIR)
    set(_asio_HEADER_SEARCH_DIRS "${ASIO_ROOT_DIR}"
        "${ASIO_ROOT_DIR}/include"
        ${_asio_HEADER_SEARCH_DIRS}
    )
endif(ASIO_ROOT_DIR)

# locate header
find_path(ASIO_INCLUDE_DIRS "asio.hpp"
    PATHS 
        ${_asio_HEADER_SEARCH_DIRS}
        # ${CMAKE_CURRENT_LIST_DIR}/../../asio/asio
    )
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASIO DEFAULT_MSG
    ASIO_INCLUDE_DIRS)

if(ASIO_FOUND)
	set(ASIO_INCLUDE_DIR "${ASIO_INCLUDE_DIRS}")
	message(STATUS "    ASIO_INCLUDE_DIRS = ASIO_INCLUDE_DIR = ${ASIO_INCLUDE_DIRS}")
endif(ASIO_FOUND)

# set(ASIO_FOUND FALSE)
# if(TARGET ASIO::ASIO)
#     set(ASIO_FOUND TRUE)
# else()

# find_package(Threads QUIET)

# if(NOT Threads_FOUND)
#     message(FATAL_ERROR "ASIO requires thread support, but it wasn't found.")
# else()
#     add_library(ASIO::ASIO INTERFACE IMPORTED GLOBAL)
#     set_target_properties(ASIO::ASIO PROPERTIES
#     INTERFACE_COMPILE_DEFINITIONS "ASIO_STANDALONE"
#     INTERFACE_INCLUDE_DIRECTORIES "${ASIO_ROOT}/include"
#     )
#     target_link_libraries(ASIO::ASIO INTERFACE Threads::Threads)
#     set(ASIO_FOUND TRUE)
# endif()

# if(ASIO_FOUND)
# 	set(ASIO_INCLUDE_DIR "${ASIO_INCLUDE_DIRS}")
# 	message(STATUS "    ASIO_INCLUDE_DIRS = ASIO_INCLUDE_DIR = ${ASIO_INCLUDE_DIRS}")
# endif(ASIO_FOUND)