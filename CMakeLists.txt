cmake_minimum_required(VERSION 3.26)

project(Demo
    VERSION 1.0.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#conan配置
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(${CMAKE_SOURCE_DIR}/cmake/conan.cmake)

conan_cmake_configure(REQUIRES glew/2.2.0
                      REQUIRES glfw/3.3.8
                      GENERATORS CMakeDeps
                      GENERATORS CMakeToolchain
                      IMPORTS "bin, *.dll -> ./bin"
                      IMPORTS "lib, *.dylib* -> ./bin"
                      OPTIONS *:shared=True)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(app)
