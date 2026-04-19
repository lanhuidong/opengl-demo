#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "util/render_util.h"

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

void init(GLFWwindow* window) {
    renderingProgram =
        RenderUtil::createShaderProgram("/Users/lanhuidong/CppProjects/opengl-demo/app/src/shaders/vshader_2_6.glsl",
                                        "/Users/lanhuidong/CppProjects/opengl-demo/app/src/shaders/fshader_2_6.glsl");
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

float x = 0.0f;
float inc = 0.01f;

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    x += inc;
    if (x > 0.75f) {
        inc = -0.01f;
    }
    if (x < -0.75f) {
        inc = 0.01f;
    }
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");
    glProgramUniform1f(renderingProgram, offsetLoc, x);

    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2-Program 2.6", nullptr, nullptr);
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