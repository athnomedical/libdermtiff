include_directories("../libdermtiff/include/")

macro(add_template_test name file)
    add_executable(${name} ${file})
    target_link_libraries(${name} PRIVATE dermtiff tiff)
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()

add_template_test(dermtiff-pencil-parse "pencilParse.cpp")
add_template_test(dermtiff-pencil-tostring "pencilToString.cpp")
add_template_test(dermtiff-tiff-read "tiffRead.cpp")
add_template_test(dermtiff-tiff-write "tiffWrite.cpp")
add_template_test(dermtiff-message "message.cpp")