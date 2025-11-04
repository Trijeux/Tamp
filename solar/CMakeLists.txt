file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(solar ${SRC_FILES} ${HEADER_FILES})

target_include_directories(solar PUBLIC include/)
target_link_libraries(solar PUBLIC common core my_common)
