file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)

set(binary app)

add_executable(${binary} ${srcs})

target_link_libraries(${binary} ${GLEW_LIBRARIES} ${glfw3_LIBRARIES})

target_include_directories(${binary} PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS})