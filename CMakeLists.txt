cmake_minimum_required(VERSION 3.25)
project(CW_ASD)

set(CMAKE_CXX_STANDARD 23)

add_executable(CW_ASD main.cpp)
add_compile_options(-O3)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/Users/User/Desktop/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML 2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
include_directories(C:/Users/User/Desktop/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML 2.5.1/include)
target_link_libraries(CW_ASD sfml-graphics sfml-window sfml-system)

