add_executable(program_2_1 src/program_2_1.cpp)
target_link_libraries(program_2_1 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES})
target_include_directories(program_2_1 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(main src/main.cpp)
target_link_libraries(main ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES})
target_include_directories(main PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(cubes-1 src/cubes-1.cpp)
target_link_libraries(cubes-1 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES})
target_include_directories(cubes-1 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(cubes src/cubes.cpp)
target_link_libraries(cubes ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES})
target_include_directories(cubes PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(cube src/cube.cpp)
target_link_libraries(cube ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES})
target_include_directories(cube PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(pyramid src/pyramid.cpp)
target_link_libraries(pyramid ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES})
target_include_directories(pyramid PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})
