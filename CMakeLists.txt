cmake_minimum_required(VERSION 3.15)
project(InterSect)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} g++ -g -Wall -Wextra -std=c++14 -O3 main.cpp -o main -lGL -lglfw -lGLU -lGLEW")
#add_library(-lGL -lGLEW -lglfw3)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED STATIC)

add_executable(InterSect main.cpp Ellipsoid.cpp Ellipsoid.h Point.cpp Point.h)

target_link_libraries(InterSect glfw ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY}
        ${GLEW_LIBRARY})


# sudo apt-get install libglfw3
# sudo apt-get install libglfw3-dev
# sudo apt-get install libglew-dev
#
# sudo apt-get install pkg-config