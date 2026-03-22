#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// 分配在display()函数中使用的变量空间，这样它们就不必在渲染过程中分配
GLuint mvLoc, tfLoc, vLoc, projLoc;
int width, height;
float aspect, timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

void setupVertices(void) {
    float vertexPositions[108] = {
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
        -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,
    };
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void printShaderLog(GLuint shader) {
    GLint len{0};
    int chWrittn{0};
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

void printProgramLog(GLuint prog) {
    GLint len{0};
    int chWrittn{0};
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

bool checkOpenGLError() {
    bool foundError{false};
    GLenum glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        cout << "glError: " << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

GLuint createShaderProgram() {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    // 顶点着色器
    const char* vshaderSource = R"(
        #version 410

        layout (location=0) in vec3 position;

        uniform mat4 v_matrix;
        uniform mat4 proj_matrix;
        uniform float tf;

        out vec4 varyingColor;

        mat4 buildRotateX(float rad);
        mat4 buildRotateY(float rad);
        mat4 buildRotateZ(float rad);
        mat4 buildTranslate(float x, float y, float z);

        void main(void)
        {	
            float i = gl_InstanceID + tf;
	        float a = sin(203.0 * i/8000.0) * 403.0;;
	        float b = cos(301.0 * i/4001.0) * 401.0;
	        float c = sin(400.0 * i/6003.0) * 405.0;

	        mat4 localRotX = buildRotateX(1.75*i);
	        mat4 localRotY = buildRotateY(1.75*i);
	        mat4 localRotZ = buildRotateZ(1.75*i);
	        mat4 localTrans = buildTranslate(a,b,c);

	        mat4 newM_matrix = localTrans * localRotX * localRotY * localRotZ;
	        mat4 mv_matrix = v_matrix * newM_matrix;
	        gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	        varyingColor = vec4(position,1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
        }

        mat4 buildTranslate(float x, float y, float z)
        {	mat4 trans = mat4(	1.0, 0.0, 0.0, 0.0,
		        0.0, 1.0, 0.0, 0.0,
		        0.0, 0.0, 1.0, 0.0,
		        x, y, z, 1.0 );
	        return trans;
        }

        //  rotation around the X axis
        mat4 buildRotateX(float rad)
        {	mat4 xrot = mat4(	1.0, 0.0, 0.0, 0.0,
		        0.0, cos(rad), -sin(rad), 0.0,
		        0.0, sin(rad), cos(rad), 0.0,
		        0.0, 0.0, 0.0, 1.0 );
	        return xrot;
        }

        //  rotation around the Y axis
        mat4 buildRotateY(float rad)
        {	mat4 yrot = mat4(	cos(rad), 0.0, sin(rad), 0.0,
		        0.0, 1.0, 0.0, 0.0,
		        -sin(rad), 0.0, cos(rad), 0.0,
		        0.0, 0.0, 0.0, 1.0 );
	        return yrot;
        }

        //  rotation around the Z axis
        mat4 buildRotateZ(float rad)
        {	mat4 zrot = mat4(	cos(rad), sin(rad), 0.0, 0.0,
		        -sin(rad), cos(rad), 0.0, 0.0,
		        0.0, 0.0, 1.0, 0.0,
		        0.0, 0.0, 0.0, 1.0 );
	        return zrot;
        }
    )";

    // 片段着色器
    const char* fshaderSource = R"(
        #version 410

        in vec4 varyingColor;

        out vec4 color;

        uniform mat4 mv_matrix;
        uniform mat4 proj_matrix;

        void main(void){
            color = varyingColor;
        }
    )";
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    glCompileShader(vShader);

    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        cout << "linking failed" << endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 32.0f;
    cubeLocX = 0.0f;
    cubeLocY = -2.0f;
    cubeLocZ = 0.0f;
    setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    // 获取MV矩阵和投影矩阵的统一变量
    vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    // 构建视图矩阵、模型矩阵和MV矩阵
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    timeFactor = ((float)currentTime);
    tfLoc = glGetUniformLocation(renderingProgram, "tf");
    glUniform1f(tfLoc, (float)timeFactor);

    // 将VBO关联给顶点着色器中相应的顶点属性
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // 调整OpenGL设置，绘制模型
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
    aspect = (float)newWidth / (float)newHeight;
    glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(int argc, char const* argv[]) {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(600, 600, "Demo", NULL, NULL);
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
    exit(EXIT_SUCCESS);
    return 0;
}
