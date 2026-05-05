#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

#include "imported_model.h"
#include "util/render_util.h"

#define numVAOs 1
#define numVBOs 3

float camera_x, camera_y, camera_z;
float pyr_loc_x, pyr_loc_y, pyr_loc_z;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mv_loc, proj_loc;
int width, height;
float aspect;
glm::mat4 p_mat, v_mat, m_mat, mv_mat;
ImportedModel my_model("./app/resources/shuttle.obj");

void setupVertices(void) {
    std::vector<glm::vec3> vert = my_model.getVertices();
    std::vector<glm::vec2> tex = my_model.getTexCoords();
    std::vector<glm::vec3> norm = my_model.getNormals();

    std::vector<float> p_values;  // 顶点位置
    std::vector<float> t_values;  // 纹理坐标
    std::vector<float> n_values;  // 法向量

    int indices_num = my_model.getVerticesNum();
    for (int i = 0; i < indices_num; ++i) {
        p_values.push_back((vert[i]).x);
        p_values.push_back((vert[i]).y);
        p_values.push_back((vert[i]).z);

        t_values.push_back((tex[i]).s);
        t_values.push_back((tex[i]).t);

        n_values.push_back((norm[i]).x);
        n_values.push_back((norm[i]).y);
        n_values.push_back((norm[i]).z);
    }

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    // 把顶点放入缓冲区0
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, p_values.size() * 4, &p_values[0], GL_STATIC_DRAW);

    // 把纹理坐标放入缓冲区1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, t_values.size() * 4, &t_values[0], GL_STATIC_DRAW);

    // 把法向量放入缓冲区2
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, n_values.size() * 4, &n_values[0], GL_STATIC_DRAW);
}

GLuint texture_id;
void init(GLFWwindow* window) {
    renderingProgram =
        RenderUtil::createShaderProgram("./app/src/shaders/vshader_6_3.glsl", "./app/src/shaders/fshader_6_3.glsl");
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    p_mat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    camera_x = -0.1f;
    camera_y = 0.0f;
    camera_z = 1.5f;
    pyr_loc_x = 0.0f;
    pyr_loc_y = 0.0f;
    pyr_loc_z = 0.0f;
    setupVertices();
    texture_id = RenderUtil::loadTexture("./app/resources/spstob_1.jpg");
}

float toRadians(float degrees) {
    return (degrees * 2.0f * 3.14159f) / 360.0f;
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

    m_mat = glm::rotate(m_mat, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    m_mat = glm::rotate(m_mat, toRadians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_mat = glm::rotate(m_mat, toRadians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));

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
    glDrawArrays(GL_TRIANGLES, 0, my_model.getVerticesNum());
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
    GLFWwindow* window = glfwCreateWindow(800, 800, "Chapter6-Program 6.3", nullptr, nullptr);
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