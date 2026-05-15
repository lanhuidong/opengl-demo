#pragma once

#include <GL/glew.h>

#include <string>

class RenderUtil {
    RenderUtil() = delete;
    ~RenderUtil() = delete;

   private:
    static void printShaderLog(GLuint shader);
    static void printProgramLog(int prog);
    static std::string readShaderSource(const std::string& file_path);
    static GLuint prepareShader(int shader_type, const char* shader_path);
    static int finalizeShaderProgram(GLuint sprogram);

   public:
    static void displayComputeShaderLimits();
    static GLuint createShaderProgram(const char* cp);
    static GLuint createShaderProgram(const std::string& vert_file, const std::string& frag_file);
    static GLuint createShaderProgram(const char* vp, const char* gp, const char* fp);
    static GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp);
    static GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp);
    static GLuint loadTexture(const char* tex_image_path);
    static GLuint loadCubeMap(const char* map_dir);
    static bool checkOpenGLError();

    // 黄金材质的环境光、漫反射、镜面反射、光泽度
    static float* goldAmbient();
    static float* goldDiffuse();
    static float* goldSpecular();
    static float goldShininess();

    // 白银材质的环境光、漫反射、镜面反射、光泽度
    static float* silverAmbient();
    static float* silverDiffuse();
    static float* silverSpecular();
    static float silverShininess();

    // 青铜材质的环境光、漫反射、镜面反射、光泽度
    static float* bronzeAmbient();
    static float* bronzeDiffuse();
    static float* bronzeSpecular();
    static float bronzeShininess();

    // 玉石材质的环境光、漫反射、镜面反射、光泽度
    static float* jadeAmbient();
    static float* jadeDiffuse();
    static float* jadeSpecular();
    static float jadeShininess();

    // 珍珠材质的环境光、漫反射、镜面反射、光泽度
    static float* pearlAmbient();
    static float* pearlDiffuse();
    static float* pearlSpecular();
    static float pearlShininess();
};