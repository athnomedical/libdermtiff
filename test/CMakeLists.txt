include_directories("../libdermtiff/include/")

macro(add_template_test name file)
    add_executable(${name} ${file})
    target_link_libraries(${name} PRIVATE dermtiff tiff)
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()

add_template_test(dermtiff-pencil "pencil.cpp")
add_template_test(dermtiff-tiff-read "tiff_read.cpp")
add_template_test(dermtiff-tiff-write "tiff_write.cpp")