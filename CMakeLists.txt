cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles/modules/")

PROJECT(hermit)

INCLUDE (CheckIncludeFiles)
include (FindPkgConfig)
include(CheckCXXSourceCompiles)

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

# build type
set(CMAKE_BUILD_TYPE Debug)

# compile option (Debug) 
if (CMAKE_COMPILER_IS_CLANGXX)
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wno-unused-parameter -Werror -std=c++0x")
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -Wno-unused-parameter -Werror")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wno-array-bounds -Werror -std=c++0x")
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -Wno-array-bounds -Werror")
endif()

# compile option (Release)
if (CMAKE_COMPILER_IS_CLANGXX)
  set(CMAKE_CXX_FLAGS_DEBUG "-W -Wall -Wextra -Wno-unused-parameter -Werror -std=c++0x -O3")
  set(CMAKE_C_FLAGS_DEBUG "-W -Wall -Wextra -Wno-unused-parameter -Werror -O3")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-W -Wall -Wextra -Wno-array-bounds -Werror -std=c++0x -O3")
  set(CMAKE_C_FLAGS_DEBUG "-W -Wall -Wextra -Wno-array-bounds -Werror -O3")
endif()

#if you don't want the full compiler output, remove the following line
set(CMAKE_VERBOSE_MAKEFILE ON)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.49.0 COMPONENTS thread filesystem chrono system REQUIRED )
if (NOT Boost_FOUND)
  message(SEND_ERROR "Boost not found")
endif (NOT Boost_FOUND)

find_package( TomMath REQUIRED )
if (NOT TomMath_FOUND)
  message(SEND_ERROR "TomMath not found. Huge integer will not work." )
endif (NOT TomMath_FOUND)

pkg_check_modules( OPENEXR REQUIRED OpenEXR )

#pkg_check_modules(GLFW3 REQUIRED libglfw>=3.0)


INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include ${Boost_INCLUDE_DIRS} ${OPENEXR_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )
subdirs( include src )
