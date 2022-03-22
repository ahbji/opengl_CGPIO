#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include "Utils.h"

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

void init(GLFWwindow *window)
{
    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow *window, double currentTime)
{
    glUseProgram(renderingProgram);
    // 
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void)
{
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // I don't know what this does
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // and neither this
    GLFWwindow *window = glfwCreateWindow(600, 600, "Chapter2 - program5", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
