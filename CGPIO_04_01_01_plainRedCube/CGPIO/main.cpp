#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"
using namespace std;

#define numVAOs 1
#define numVBOs 2

Utils util = Utils();
float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

void setupVertices(void) {
    // 立方体的6个面由12个三角形组成，每个三角形3个顶点，每个顶点3个坐标值。
    float vertexPositions[108] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
    };

    // 初始化 1 个 vao
    glGenVertexArrays(1, vao);
    // 激活 vao
    glBindVertexArray(vao[0]);
    // 初始化 2 个 vbo
    glGenBuffers(numVBOs, vbo);
    // 激活 vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // 将 vertexPositions 数组复制到 vbo[0] 中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
    // 定义相机在世界空间中的坐标
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    // 定义立方体在世界空间中的坐标
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
    setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
    // 重置 z-buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    // 启用 shader
    glUseProgram(renderingProgram);

    // 获取 uniform 变量
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    
    // 设置透视投影矩阵
    // - 视角角度：60°
    // - 长宽比：width/height
    // - n 值：0.1f
    // - f 值：1000.0f
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    // 设置视图矩阵
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    // 设置模型矩阵
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
    // 预先构建 mv矩阵，以优化性能
    mvMat = vMat * mMat;

    // 将 mvMat 和 pMat 赋值给 uniform 变量
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // 激活 vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // 关联 vbo[0] 到识别号为 0 的顶点属性变量（3 代表三元组 vec3）
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    // 启用指定识别号的顶点属性变量
    glEnableVertexAttribArray(0);

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // 绘制三角形
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main(void) {
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // I don't know what this does
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // and neither this
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 4 - program 1", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
