cmake_minimum_required (VERSION 3.15.0)

enable_language(CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(dermtiff)
set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 4)
set(CMAKE_PROJECT_VERSION_PATCH 1)

if (MSVC)
	set(CMAKE_CXX_FLAGS "/W4 /EHsc")
else()
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unknown-pragmas")
endif()

set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build shared libraries")

set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR})

find_package(ZLIB REQUIRED)

option(tiff-tools "build TIFF tools" OFF)
option(tiff-tests "build TIFF tests" OFF)
option(tiff-contrib "build TIFF contrib" OFF)
option(tiff-docs "build TIFF documentation" OFF)

if (APPLE)
	set(VERSION_FILE_BEFORE ${PROJECT_SOURCE_DIR}/deps/libtiff/VERSION)
	set(VERSION_FILE_AFTER ${PROJECT_SOURCE_DIR}/deps/libtiff/VERSION.txt)
	if(EXISTS ${VERSION_FILE_AFTER})
		execute_process(COMMAND mv ${VERSION_FILE_AFTER} ${VERSION_FILE_BEFORE})
	endif()
endif()

add_subdirectory("deps/libtiff")
add_subdirectory("libdermtiff")
add_subdirectory("test")

if (APPLE)
	execute_process(COMMAND mv ${VERSION_FILE_BEFORE} ${VERSION_FILE_AFTER})
endif()
