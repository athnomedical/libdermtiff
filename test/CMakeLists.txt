include_directories("../libdermtiff/include/")

macro(add_template_test name file)
    add_executable(${name} ${file})
    target_link_libraries(${name} PRIVATE dermtiff tiff)
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()

add_template_test(test-pencil-parse "testPencilParse.cpp")
add_template_test(test-pencil-tostring "testPencilToString.cpp")
add_template_test(test-tiff-read "testTIFFRead.cpp")
add_template_test(test-tiff-write "testTIFFWrite.cpp")
add_template_test(test-message "testMessage.cpp")