include_directories("../libdermtiff/include/")

find_package(OpenMP)

macro(add_template_test name file)
    add_executable(${name} ${file})
    target_link_libraries(${name} PRIVATE dermtiff tiff)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(${name} PUBLIC OpenMP::OpenMP_CXX)
    endif()
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()

add_template_test(test-pencil-parse "testPencilParse.cpp")
add_template_test(test-pencil-tostring "testPencilToString.cpp")
add_template_test(test-tiff-read "testTIFFRead.cpp")
add_template_test(test-tiff-write "testTIFFWrite.cpp")