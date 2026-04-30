#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

#include "util/render_util.h"

#define numVAOs 1
#define numVBOs 2

float camera_x, camera_y, camera_z;
float pyr_loc_x, pyr_loc_y, pyr_loc_z;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mv_loc, proj_loc;
int width, height;
float aspect, time_factor;
glm::mat4 p_mat, v_mat, m_mat, mv_mat;

void setupVertices(void) {
    float pyramid_positions[54] = {-1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  -1.0f,
                                   1.0f,  1.0f,  -1.0f, -1.0f, 0.0f,  1.0f,  0.0f,  1.0f,  -1.0f, -1.0f, -1.0f,
                                   -1.0f, -1.0f, 0.0f,  1.0f,  0.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
                                   0.0f,  1.0f,  0.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,
                                   1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f};
    float pyr_tex_coords[36] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,  //
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,  //
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,  //
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,  //
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f   //
    };
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_positions), pyramid_positions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyr_tex_coords), pyr_tex_coords, GL_STATIC_DRAW);
}

GLuint texture_id;
void init(GLFWwindow* window) {
    renderingProgram =
        RenderUtil::createShaderProgram("/Users/lanhuidong/CppProjects/opengl-demo/app/src/shaders/vshader_5_1.glsl",
                                        "/Users/lanhuidong/CppProjects/opengl-demo/app/src/shaders/fshader_5_1.glsl");
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    p_mat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    camera_x = 0.0f;
    camera_y = 0.0f;
    camera_z = 4.0f;
    pyr_loc_x = 0.0f;
    pyr_loc_y = 0.0f;
    pyr_loc_z = 0.0f;
    setupVertices();
    texture_id = RenderUtil::loadTexture("./app/resources/ice.jpg");
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    mv_loc = glGetUniformLocation(renderingProgram, "mv_matrix");
    proj_loc = glGetUniformLocation(renderingProgram, "proj_matrix");

    v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-camera_x, -camera_y, -camera_z));

    m_mat = glm::translate(glm::mat4(1.0f), glm::vec3(pyr_loc_x, pyr_loc_y, pyr_loc_z));

    m_mat = glm::rotate(m_mat, -0.45f, glm::vec3(1.0f, 0.0f, 0.0f));
    m_mat = glm::rotate(m_mat, 0.61f, glm::vec3(0.0f, 1.0f, 0.0f));
    m_mat = glm::rotate(m_mat, 0.00f, glm::vec3(0.0f, 0.0f, 1.0f));

    mv_mat = v_mat * m_mat;

    glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);
}

int main(void) {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);  // macOS不支持高于4.1的OpenGL版本
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter4-Program 5.1", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}