cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project (snow)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)




# add the executable
add_executable(snow src/main.cpp src/grid.cpp src/particle.cpp src/force.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE src/main.cpp)

target_link_libraries (snow Eigen3::Eigen)

include_directories("libraries/glm/")


set(GLFW_DIR "libraries/glfw")
add_subdirectory (${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# GLAD stuff
set(GLAD_DIR "libraries/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")
