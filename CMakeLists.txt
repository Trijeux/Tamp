cmake_minimum_required (VERSION 3.16)
project(GPR924-Physics-Engine)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(externals/GPR924-Engine)
add_subdirectory(solar/)
add_subdirectory(my_common/)


add_executable(solar_system_main src/solar_system_main.cc)
add_executable(collision_main src/triger_main.cc)
target_link_libraries(solar_system_main PRIVATE common core my_common solar)
target_link_libraries(collision_main PRIVATE common core my_common)