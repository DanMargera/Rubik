set(rubik_src Rubik.cpp Constants.cpp Algorithm.cpp)
add_library(rcore SHARED "${rubik_src}")
target_include_directories(rcore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rcore PUBLIC fmt)
