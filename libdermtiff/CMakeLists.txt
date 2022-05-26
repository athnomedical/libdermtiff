if(MSVC)
    set(CMAKE_DEBUG_POSTFIX d)
    if(BUILD_SHARED_LIBS)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

include_directories(
    "../deps/libtiff/"
    "../build/deps/libtiff/libtiff/"
    "./include/libdermtiff/dermtiff/"
)

add_library(dermtiff)

target_sources(dermtiff PRIVATE
    "./src/io.cpp"
    "./src/message.cpp"
    "./src/pencil.cpp"
    "./src/type.cpp"
    "./src/util.cpp"
)

target_link_libraries(dermtiff PRIVATE tiff)