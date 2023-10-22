cmake_minimum_required(VERSION 3.25)
project(CW_ASD)

set(CMAKE_CXX_STANDARD 23)

add_executable(CW_ASD main.cpp)


set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
include_directories(C:/SFML-2.5.1/include)
target_link_libraries(CW_ASD sfml-graphics sfml-window sfml-system)

