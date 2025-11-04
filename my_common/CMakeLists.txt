file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(my_common ${SRC_FILES} ${HEADER_FILES})

target_include_directories(my_common PUBLIC include/)
target_link_libraries(my_common PUBLIC common core)
