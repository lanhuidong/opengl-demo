#pragma once

#include <GL/glew.h>

#include <string>

class RenderUtil {
    RenderUtil() = delete;
    ~RenderUtil() = delete;

   public:
    static void printShaderLog(GLuint shader);
    static void printProgramLog(int prog);
    static bool checkOpenGLError();
    static std::string readShaderSource(const std::string& file_path);
    static GLuint createShaderProgram(const std::string& vert_file, const std::string& frag_file);
    static GLuint loadTexture(const char* tex_image_path);
};