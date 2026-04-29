#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stack>
#include <string>

#include "util/render_util.h"

#define numVAOs 1
#define numVBOs 2

float camera_x, camera_y, camera_z;
float cube_loc_x, cube_loc_y, cube_loc_z;
float pyr_loc_x, pyr_loc_y, pyr_loc_z;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mv_loc, proj_loc;
int width, height;
float aspect;
glm::mat4 p_mat, v_mat, m_mat, mv_mat;

void setupVertices(void) {
    float vertex_positions[108] = {
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
        -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f};

    float pyramid_positions[54] = {-1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  -1.0f,
                                   1.0f,  1.0f,  -1.0f, -1.0f, 0.0f,  1.0f,  0.0f,  1.0f,  -1.0f, -1.0f, -1.0f,
                                   -1.0f, -1.0f, 0.0f,  1.0f,  0.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
                                   0.0f,  1.0f,  0.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,
                                   1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f};
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_positions), pyramid_positions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    renderingProgram =
        RenderUtil::createShaderProgram("/Users/lanhuidong/CppProjects/opengl-demo/app/src/shaders/vshader_4_4.glsl",
                                        "/Users/lanhuidong/CppProjects/opengl-demo/app/src/shaders/fshader_4_4.glsl");
    camera_x = 0.0f;
    camera_y = 0.0f;
    camera_z = 16.0f;
    cube_loc_x = 0.0f;
    cube_loc_y = -2.0f;
    cube_loc_z = 0.0f;
    pyr_loc_x = 2.0f;
    pyr_loc_y = 2.0f;
    pyr_loc_z = 0.0f;
    setupVertices();
}

std::stack<glm::mat4> mv_stack;

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    mv_loc = glGetUniformLocation(renderingProgram, "mv_matrix");
    proj_loc = glGetUniformLocation(renderingProgram, "proj_matrix");
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    p_mat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));

    v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-camera_x, -camera_y, -camera_z));
    mv_stack.push(v_mat);

    // 四棱锥：🌞
    mv_stack.push(mv_stack.top());
    mv_stack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    mv_stack.push(mv_stack.top());
    mv_stack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_stack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    mv_stack.pop();

    // 大立方体：🌍
    mv_stack.push(mv_stack.top());
    mv_stack.top() *=
        glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
    mv_stack.push(mv_stack.top());
    mv_stack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));

    glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_stack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mv_stack.pop();

    // 小立方体：🌛
    mv_stack.push(mv_stack.top());
    mv_stack.top() *=
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0));
    mv_stack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));
    mv_stack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));

    glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_stack.top()));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mv_stack.pop();
    mv_stack.pop();
    mv_stack.pop();
    mv_stack.pop();
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
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter4-Program 4.4", nullptr, nullptr);
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