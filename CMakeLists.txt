cmake_minimum_required (VERSION 2.6)
project (snow)

# include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
# add_subdirectory (MathFunctions) 

include_directories ("${PROJECT_SOURCE_DIR}/glfw")
add_subdirectory (glfw) 

# include_directories ("${PROJECT_SOURCE_DIR}/glm")

# add the executable
add_executable(snow main.cpp)
# target_link_libraries (snow MathFunctions)
target_link_libraries (snow glfw)


# GLAD stuff
set(GLAD_DIR "glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")
