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

add_subdirectory("deps/libtiff")
add_subdirectory("libdermtiff")
add_subdirectory("test")

if(MSVC)
	set_target_properties(tiff PROPERTIES PREFIX "lib")
	if(NOT (CMAKE_BUILD_TYPE EQUAL "DEBUG"))
		FILE(STRINGS "${PROJECT_SOURCE_DIR}/deps/libtiff/configure.ac" configure REGEX "^LIBTIFF_.*=")
		foreach(line ${configure})
			foreach(var LIBTIFF_CURRENT LIBTIFF_AGE)
				if(NOT ${var} AND line MATCHES "^${var}=(.*)")
					set(${var} "${CMAKE_MATCH_1}")
					break()
				endif()
			endforeach()
		endforeach()

		math(EXPR SO_MAJOR "${LIBTIFF_CURRENT} - ${LIBTIFF_AGE}")

		set_target_properties(tiff PROPERTIES SUFFIX "-${SO_MAJOR}.dll")
	endif()
endif()