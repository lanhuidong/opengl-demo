add_executable(program_2_1 src/program_2_1.cpp)
target_link_libraries(program_2_1 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES})
target_include_directories(program_2_1 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_2_2 src/program_2_2.cpp)
target_link_libraries(program_2_2 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES})
target_include_directories(program_2_2 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_2_3 src/program_2_3.cpp src/util/render_util.cpp)
target_link_libraries(program_2_3 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_2_3 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_2_5 src/program_2_5.cpp src/util/render_util.cpp)
target_link_libraries(program_2_5 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_2_5 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_2_6 src/program_2_6.cpp src/util/render_util.cpp)
target_link_libraries(program_2_6 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_2_6 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_4_1 src/program_4_1.cpp src/util/render_util.cpp)
target_link_libraries(program_4_1 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_4_1 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_4_2 src/program_4_2.cpp src/util/render_util.cpp)
target_link_libraries(program_4_2 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_4_2 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_4_3 src/program_4_3.cpp src/util/render_util.cpp)
target_link_libraries(program_4_3 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_4_3 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_4_4 src/program_4_4.cpp src/util/render_util.cpp)
target_link_libraries(program_4_4 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_4_4 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_5_1 src/program_5_1.cpp src/util/render_util.cpp)
target_link_libraries(program_5_1 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_5_1 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_6_1 src/program_6_1.cpp src/util/render_util.cpp src/sphere.cpp)
target_link_libraries(program_6_1 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_6_1 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_6_2 src/program_6_2.cpp src/util/render_util.cpp src/torus.cpp)
target_link_libraries(program_6_2 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_6_2 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_6_3 src/program_6_3.cpp src/util/render_util.cpp src/imported_model.cpp src/model_importer.cpp)
target_link_libraries(program_6_3 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_6_3 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

add_executable(program_7_1 src/program_7_1.cpp src/util/render_util.cpp src/torus.cpp)
target_link_libraries(program_7_1 ${GLEW_LIBRARIES} ${glfw3_LIBRARIES} ${glm_LIBRARIES} soil2)
target_include_directories(program_7_1 PUBLIC include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

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
