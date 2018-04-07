cmake_minimum_required (VERSION 2.6)
project (snow)

include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
add_subdirectory (MathFunctions) 

include_directories ("${PROJECT_SOURCE_DIR}/glfw")
add_subdirectory (glfw) 
 
# add the executable
add_executable(snow main.cpp)
target_link_libraries (snow MathFunctions)
target_link_libraries (snow glfw)
