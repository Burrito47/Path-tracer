cmake_minimum_required(VERSION 3.10)

project(PathTracer LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



file(GLOB SOURCES
    src/*.cpp
    
)

file(GLOB HEADER_FILES
    
    src/*.h
)
add_executable(PathTracer ${SOURCES} ${HEADER_FILES})

target_include_directories(PathTracer PRIVATE
    include
    
    include/glm
 
)

