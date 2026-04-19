#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
    const char* vsharderSource =
        "#version 410 \n"
        "void main(void) \n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    const char* fsharderSource =
        "#version 410 \n"
        "out vec4 color; \n"
        "void main(void) \n"
        "{ if (gl_FragCoord.x < 600) color = vec4(1.0, 0.0, 0.0, 1.0); else color = vec4(1.0, 1.0, 0.0, 1.0); }";
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vShader, 1, &vsharderSource, nullptr);
    glShaderSource(fShader, 1, &fsharderSource, nullptr);
    glCompileShader(vShader);
    glCompileShader(fShader);

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    return vfProgram;
}

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

float point_size{1.0f};
bool zoom_in{false};

void display(GLFWwindow* window, double currentTime) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    if (zoom_in) {
        if (point_size > 1) {
            point_size = point_size - 0.5f;
        } else {
            zoom_in = false;
        }
    } else {
        if (point_size < 100) {
            point_size = point_size + 0.5f;
        } else {
            zoom_in = true;
        }
    }
    glPointSize(point_size);
    glDrawArrays(GL_POINTS, 0, 1);
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
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2-Program 2.2", nullptr, nullptr);
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