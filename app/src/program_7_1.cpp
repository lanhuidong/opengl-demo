#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

#include "torus.h"
#include "util/render_util.h"

#define numVAOs 1
#define numVBOs 4

float camera_x, camera_y, camera_z;
float tor_loc_x, tor_loc_y, tor_loc_z;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

Torus my_torus(0.5f, 0.2f, 48);
int tor_vertices_num = my_torus.getVerticesNum();
int tor_indinces_num = my_torus.getIndicesNum();

glm::vec3 inital_light_loc = glm::vec3(5.0f, 2.0f, 2.0f);
float amt = 0.0f;

GLuint mv_loc, proj_loc, n_loc;
GLuint global_amb_loc, amb_loc, diff_loc, spec_loc, pos_loc, mamb_loc, mdiff_loc, mspec_loc, mshi_loc;
int width, height;
float aspect;
glm::mat4 p_mat, v_mat, m_mat, mv_mat, inv_tr_mat, r_mat;
glm::vec3 current_light_pos, transformed;
float light_pos[3];

// 白光
float globalAmbient[4] = {0.7f, 0.7f, 0.7f, 1.0f};
float lightAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};

// 黄金材质
float* mat_amb = RenderUtil::goldAmbient();
float* mat_dif = RenderUtil::goldDiffuse();
float* mat_spe = RenderUtil::goldSpecular();
float mat_shi = RenderUtil::goldShininess();

void installLights(glm::mat4 v_martix) {
    transformed = glm::vec3(v_martix * glm::vec4(current_light_pos, 1.0));
    light_pos[0] = transformed.x;
    light_pos[1] = transformed.y;
    light_pos[2] = transformed.z;

    global_amb_loc = glGetUniformLocation(renderingProgram, "globalAmbient");
    amb_loc = glGetUniformLocation(renderingProgram, "light.ambient");
    diff_loc = glGetUniformLocation(renderingProgram, "light.diffuse");
    spec_loc = glGetUniformLocation(renderingProgram, "light.specular");
    pos_loc = glGetUniformLocation(renderingProgram, "light.position");
    mamb_loc = glGetUniformLocation(renderingProgram, "material.ambient");
    mdiff_loc = glGetUniformLocation(renderingProgram, "material.diffuse");
    mspec_loc = glGetUniformLocation(renderingProgram, "material.specular");
    mshi_loc = glGetUniformLocation(renderingProgram, "material.shininess");

    glProgramUniform4fv(renderingProgram, global_amb_loc, 1, globalAmbient);
    glProgramUniform4fv(renderingProgram, amb_loc, 1, lightAmbient);
    glProgramUniform4fv(renderingProgram, diff_loc, 1, lightDiffuse);
    glProgramUniform4fv(renderingProgram, spec_loc, 1, lightSpecular);
    glProgramUniform3fv(renderingProgram, pos_loc, 1, light_pos);
    glProgramUniform4fv(renderingProgram, mamb_loc, 1, mat_amb);
    glProgramUniform4fv(renderingProgram, mdiff_loc, 1, mat_dif);
    glProgramUniform4fv(renderingProgram, mspec_loc, 1, mat_spe);
    glProgramUniform1f(renderingProgram, mshi_loc, mat_shi);
}

void setupVertices(void) {
    std::vector<int> ind = my_torus.getIndices();
    std::vector<glm::vec3> vert = my_torus.getVertices();
    std::vector<glm::vec2> tex = my_torus.getTexCoords();
    std::vector<glm::vec3> norm = my_torus.getNormals();

    std::vector<float> p_values;  // 顶点位置
    std::vector<float> t_values;  // 纹理坐标
    std::vector<float> n_values;  // 法向量

    int indices_num = my_torus.getVerticesNum();
    for (int i = 0; i < indices_num; ++i) {
        p_values.push_back(vert[i].x);
        p_values.push_back(vert[i].y);
        p_values.push_back(vert[i].z);

        t_values.push_back(tex[i].s);
        t_values.push_back(tex[i].t);

        n_values.push_back(norm[i].x);
        n_values.push_back(norm[i].y);
        n_values.push_back(norm[i].z);
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
}

std::string shader_selector{"1"};
void init(GLFWwindow* window) {
    std::string vp_path = "./app/src/shaders/vshader_7_1_" + shader_selector + ".glsl";
    std::string fp_path = "./app/src/shaders/fshader_7_1_" + shader_selector + ".glsl";
    renderingProgram = RenderUtil::createShaderProgram(vp_path, fp_path);
    camera_x = 0.0f;
    camera_y = 0.0f;
    camera_z = 1.0f;
    tor_loc_x = 0.0f;
    tor_loc_y = 0.0f;
    tor_loc_z = -1.0f;

    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    p_mat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    setupVertices();
}

float toRadians(float degrees) {
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    mv_loc = glGetUniformLocation(renderingProgram, "mv_matrix");
    proj_loc = glGetUniformLocation(renderingProgram, "proj_matrix");
    n_loc = glGetUniformLocation(renderingProgram, "norm_matrix");

    v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-camera_x, -camera_y, -camera_z));

    m_mat = glm::translate(glm::mat4(1.0f), glm::vec3(tor_loc_x, tor_loc_y, tor_loc_z));
    m_mat = glm::rotate(m_mat, toRadians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    current_light_pos = glm::vec3(inital_light_loc.x, inital_light_loc.y, inital_light_loc.z);
    amt = currentTime * 25.0f;
    r_mat = glm::rotate(glm::mat4(1.0f), toRadians(amt), glm::vec3(0.0f, 0.0f, 1.0f));
    current_light_pos = glm::vec3(r_mat * glm::vec4(current_light_pos, 1.0f));

    installLights(v_mat);

    mv_mat = v_mat * m_mat;
    inv_tr_mat = glm::transpose(glm::inverse(mv_mat));

    glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));
    glUniformMatrix4fv(n_loc, 1, GL_FALSE, glm::value_ptr(inv_tr_mat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    glDrawElements(GL_TRIANGLES, tor_indinces_num, GL_UNSIGNED_INT, nullptr);
}

void window_size_callback(GLFWwindow* win, int new_width, int new_height) {
    aspect = (float)new_width / (float)new_height;
    glViewport(0, 0, new_width, new_height);
    p_mat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        shader_selector = argv[1];
    }
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);  // macOS不支持高于4.1的OpenGL版本
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
    GLFWwindow* window = glfwCreateWindow(500, 500, "Chapter7-Program 7.1", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, window_size_callback);

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