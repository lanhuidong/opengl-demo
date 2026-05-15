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

void RenderUtil::displayComputeShaderLimits() {
    int work_grp_cnt[3];
    int work_grp_size[3];
    int work_grp_inv;
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    std::cout << "maximum number of workgroups is: " << work_grp_cnt[0] << " " << work_grp_cnt[1] << " "
              << work_grp_cnt[2] << std::endl;
    std::cout << "maximum size of workgroups is: " << work_grp_size[0] << " " << work_grp_size[1] << " "
              << work_grp_size[2] << std::endl;
    std::cout << "max local work group invocations " << work_grp_inv << std::endl;
}

GLuint RenderUtil::prepareShader(int shader_type, const char* shader_path) {
    GLint shaderCompiled;
    std::string shader_str = readShaderSource(shader_path);
    const char* shader_src = shader_str.c_str();
    GLuint shader_ref = glCreateShader(shader_type);

    if (shader_ref == 0 || shader_ref == GL_INVALID_ENUM) {
        std::cout << "Error: Could not create shader " << shader_path << " of type: " << shader_type << std::endl;
        return 0;
    }
    glShaderSource(shader_ref, 1, &shader_src, nullptr);
    glCompileShader(shader_ref);
    checkOpenGLError();
    glGetShaderiv(shader_ref, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE) {
        if (shader_type == GL_VERTEX_SHADER) {
            std::cout << "Vertex ";
        } else if (shader_type == GL_TESS_CONTROL_SHADER) {
            std::cout << "Tess Control ";
        } else if (shader_type == GL_TESS_EVALUATION_SHADER) {
            std::cout << "Tess Eval ";
        } else if (shader_type == GL_GEOMETRY_SHADER) {
            std::cout << "Geometry ";
        } else if (shader_type == GL_FRAGMENT_SHADER) {
            std::cout << "Fragment  ";
        } else if (shader_type == GL_COMPUTE_SHADER) {
            std::cout << "Compute ";
        }
        std::cout << "shader compilation error for shader: '" << shader_path << "'." << std::endl;
        printShaderLog(shader_ref);
    }

    return shader_ref;
}

int RenderUtil::finalizeShaderProgram(GLuint sprogram) {
    GLint linked;
    glLinkProgram(sprogram);
    checkOpenGLError();
    glGetProgramiv(sprogram, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        std::cout << "linking failed" << std::endl;
        printProgramLog(sprogram);
    }
    return sprogram;
}

GLuint RenderUtil::createShaderProgram(const char* cp) {
    GLuint shader = prepareShader(GL_COMPUTE_SHADER, cp);
    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    finalizeShaderProgram(program);
    return program;
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

GLuint RenderUtil::createShaderProgram(const char* vp, const char* gp, const char* fp) {
    GLuint v_shader = prepareShader(GL_VERTEX_SHADER, vp);
    GLuint g_shader = prepareShader(GL_GEOMETRY_SHADER, gp);
    GLuint f_shader = prepareShader(GL_FRAGMENT_SHADER, fp);
    GLuint program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, g_shader);
    glAttachShader(program, f_shader);
    finalizeShaderProgram(program);
    return program;
}

GLuint RenderUtil::createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp) {
    GLuint v_shader = prepareShader(GL_VERTEX_SHADER, vp);
    GLuint tc_shader = prepareShader(GL_TESS_CONTROL_SHADER, tCS);
    GLuint te_shader = prepareShader(GL_TESS_EVALUATION_SHADER, tES);
    GLuint f_shader = prepareShader(GL_FRAGMENT_SHADER, fp);
    GLuint program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, tc_shader);
    glAttachShader(program, te_shader);
    glAttachShader(program, f_shader);
    finalizeShaderProgram(program);
    return program;
}

GLuint RenderUtil::createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp,
                                       const char* fp) {
    GLuint v_shader = prepareShader(GL_VERTEX_SHADER, vp);
    GLuint tc_shader = prepareShader(GL_TESS_CONTROL_SHADER, tCS);
    GLuint te_shader = prepareShader(GL_TESS_EVALUATION_SHADER, tES);
    GLuint g_shader = prepareShader(GL_GEOMETRY_SHADER, gp);
    GLuint f_shader = prepareShader(GL_FRAGMENT_SHADER, fp);
    GLuint program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, tc_shader);
    glAttachShader(program, te_shader);
    glAttachShader(program, g_shader);
    glAttachShader(program, f_shader);
    finalizeShaderProgram(program);
    return program;
}

GLuint RenderUtil::loadTexture(const char* tex_image_path) {
    GLuint texture_id = SOIL_load_OGL_texture(tex_image_path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (texture_id == 0) {
        std::cout << "cound not find texture file" << tex_image_path << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat anisoset = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoset);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoset);
    }
    return texture_id;
}

GLuint RenderUtil::loadCubeMap(const char* map_dir) {
    GLuint texture_id;
    std::string xp = map_dir;
    xp = xp + "/xp.jpg";
    std::string xn = map_dir;
    xn = xn + "/xn.jpg";
    std::string yp = map_dir;
    yp = yp + "/yp.jpg";
    std::string yn = map_dir;
    yn = yn + "/yn.jpg";
    std::string zp = map_dir;
    zp = zp + "/zp.jpg";
    std::string zn = map_dir;
    zn = zn + "/zn.jpg";
    texture_id = SOIL_load_OGL_cubemap(xp.c_str(), xn.c_str(), yp.c_str(), yn.c_str(), zp.c_str(), zn.c_str(),
                                       SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if (texture_id == 0) {
        std::cout << "didnt find cube map image file" << std::endl;
    }
    return texture_id;
}

float* RenderUtil::goldAmbient() {
    static float a[4] = {0.2473f, 0.1995f, 0.0745f, 1};
    return (float*)a;
}

float* RenderUtil::goldDiffuse() {
    static float a[4] = {0.7516f, 0.6065f, 0.2265f, 1};
    return (float*)a;
}

float* RenderUtil::goldSpecular() {
    static float a[4] = {0.6283f, 0.5559f, 0.3661f, 1};
    return (float*)a;
}

float RenderUtil::goldShininess() {
    return 51.2f;
}

float* RenderUtil::silverAmbient() {
    static float a[4] = {0.1923f, 0.1923f, 0.1923f, 1};
    return (float*)a;
}

float* RenderUtil::silverDiffuse() {
    static float a[4] = {0.5075f, 0.5075f, 0.5075f, 1};
    return (float*)a;
}

float* RenderUtil::silverSpecular() {
    static float a[4] = {0.5083f, 0.5083f, 0.5083f, 1};
    return (float*)a;
}

float RenderUtil::silverShininess() {
    return 51.2f;
}

float* RenderUtil::bronzeAmbient() {
    static float a[4] = {0.2125f, 0.1275f, 0.0540f, 1};
    return (float*)a;
}

float* RenderUtil::bronzeDiffuse() {
    static float a[4] = {0.7140f, 0.4284f, 0.1814f, 1};
    return (float*)a;
}

float* RenderUtil::bronzeSpecular() {
    static float a[4] = {0.3936f, 0.2719f, 0.1667f, 1};
    return (float*)a;
}

float RenderUtil::bronzeShininess() {
    return 25.6f;
}

float* RenderUtil::jadeAmbient() {
    static float a[4] = {0.1350f, 0.2225f, 0.1575f, 0.95f};
    return (float*)a;
}

float* RenderUtil::jadeDiffuse() {
    static float a[4] = {0.5400f, 0.8900f, 0.6300f, 0.95f};
    return (float*)a;
}

float* RenderUtil::jadeSpecular() {
    static float a[4] = {0.3162f, 0.3162f, 0.3162f, 0.95f};
    return (float*)a;
}

float RenderUtil::jadeShininess() {
    return 12.8f;
}

float* RenderUtil::pearlAmbient() {
    static float a[4] = {0.2500f, 0.2073f, 0.2073f, 0.922f};
    return (float*)a;
}

float* RenderUtil::pearlDiffuse() {
    static float a[4] = {1.000f, 0.8290f, 0.8290f, 0.922f};
    return (float*)a;
}

float* RenderUtil::pearlSpecular() {
    static float a[4] = {0.2966f, 0.2966f, 0.2966f, 0.922f};
    return (float*)a;
}

float RenderUtil::pearlShininess() {
    return 11.264f;
}