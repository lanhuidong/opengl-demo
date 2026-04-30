#include "render_util.h"

#include <SOIL2.h>

#include <fstream>
#include <iostream>
#include <memory>

void RenderUtil::printShaderLog(GLuint shader) {
    int max_len{0};
    int len{0};
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);
    if (max_len > 0) {
        log = (char*)malloc(max_len);
        glGetShaderInfoLog(shader, max_len, &len, log);
        std::cout << "Shader Info Log: " << log << std::endl;
        free(log);
    }
}

void RenderUtil::printProgramLog(int prog) {
    int max_len{0};
    int len{0};
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &max_len);
    if (max_len > 0) {
        log = (char*)malloc(max_len);
        glGetProgramInfoLog(prog, max_len, &len, log);
        std::cout << "Shader Info Log: " << log << std::endl;
        free(log);
    }
}

bool RenderUtil::checkOpenGLError() {
    bool found_error{false};
    int gl_err = glGetError();
    while (gl_err != GL_NO_ERROR) {
        std::cout << "glError: " << gl_err << std::endl;
        found_error = true;
        gl_err = glGetError();
    }
    return found_error;
}

std::string RenderUtil::readShaderSource(const std::string& file_path) {
    std::ifstream file(file_path);
    std::string content{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    return content;
}

GLuint RenderUtil::createShaderProgram(const std::string& vert_file, const std::string& frag_file) {
    GLint vert_compiled;
    GLint frag_compiled;
    GLint linked;

    GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);

    auto vert_shader_source = readShaderSource(vert_file);
    auto frag_shader_source = readShaderSource(frag_file);
    auto vert_shader_src = vert_shader_source.c_str();
    auto frag_shader_src = frag_shader_source.c_str();
    glShaderSource(v_shader, 1, &vert_shader_src, nullptr);
    glShaderSource(f_shader, 1, &frag_shader_src, nullptr);

    glCompileShader(v_shader);
    checkOpenGLError();
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &vert_compiled);
    if (vert_compiled != 1) {
        std::cout << "vertex compilation failed" << std::endl;
        printShaderLog(v_shader);
    }

    glCompileShader(f_shader);
    checkOpenGLError();
    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &frag_compiled);
    if (frag_compiled != 1) {
        std::cout << "fragment compilation failed" << std::endl;
        printShaderLog(f_shader);
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, v_shader);
    glAttachShader(shader_program, f_shader);
    glLinkProgram(shader_program);
    checkOpenGLError();
    glGetProgramiv(shader_program, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        std::cout << "linking failed" << std::endl;
        printProgramLog(shader_program);
    }

    return shader_program;
}

GLuint RenderUtil::loadTexture(const char* tex_image_path) {
    GLuint texture_id = SOIL_load_OGL_texture(tex_image_path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (texture_id == 0) {
        std::cout << "cound not find texture file" << tex_image_path << std::endl;
    }
    return texture_id;
}
