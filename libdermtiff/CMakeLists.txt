if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

include_directories(
    "../deps/libtiff/"
    "../build/deps/libtiff/libtiff/"
    "./include/libdermtiff/dermtiff/"
)

add_library(dermtiff STATIC
    "./src/io.cpp"
    "./src/pencil.cpp"
    "./src/type.cpp"
    "./src/util.cpp"
)