if(MSVC)
    set(CMAKE_DEBUG_POSTFIX d)
    if(BUILD_SHARED_LIBS)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

set(DERMTIFF_SOURCES
    "./src/io.cpp"
    "./src/message.cpp"
    "./src/pencil.cpp"
    "./src/type.cpp"
    "./src/util.cpp"
)

set(DERMTIFF_HEADERS
    "./include/libdermtiff/dermtiff.hpp"
    "./include/libdermtiff/dermtiff/io.hpp"
    "./include/libdermtiff/dermtiff/message.hpp"
    "./include/libdermtiff/dermtiff/pencil.hpp"
    "./include/libdermtiff/dermtiff/type.hpp"
)

if(WIN32)
    set(
        DERMTIFF_SOURCES
        ${DERMTIFF_SOURCES}
        dermtiff.def
    )
endif()

add_library(dermtiff ${DERMTIFF_SOURCES})

target_include_directories(dermtiff
    PRIVATE
    "../deps/libtiff/"
    "../build/deps/libtiff/libtiff/"
    "./include/libdermtiff/dermtiff/"
)

target_link_libraries(dermtiff PRIVATE tiff)

install(
    TARGETS dermtiff
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES
    ${DERMTIFF_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)