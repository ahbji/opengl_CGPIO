#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"
#include "ImportedModel.h"
#include "Utils.h"
using namespace std;

#define numVAOs 1
#define numVBOs 6

float cameraX, cameraY, cameraZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

float rotAmt = 0.0f;

stack<glm::mat4> mvStack;

GLuint earthTexture;
GLuint sunTexture;
GLuint moonTexture;
GLuint shuttleTexture;

Sphere mySphere = Sphere(48);
ImportedModel myModel("shuttle.obj");

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void setupVertices(void) {
    std::vector<int> ind = mySphere.getIndices();
    std::vector<glm::vec3> sphereVert = mySphere.getVertices();
    std::vector<glm::vec2> sphereTex = mySphere.getTexCoords();
    std::vector<glm::vec3> sphereNorm = mySphere.getNormals();

    std::vector<float> spherePvalues;
    std::vector<float> sphereTvalues;
    std::vector<float> sphereNvalues;
    
    for (int i = 0; i < mySphere.getNumIndices(); i++) {
        spherePvalues.push_back((sphereVert[ind[i]]).x);
        spherePvalues.push_back((sphereVert[ind[i]]).y);
        spherePvalues.push_back((sphereVert[ind[i]]).z);
        sphereTvalues.push_back((sphereTex[ind[i]]).s);
        sphereTvalues.push_back((sphereTex[ind[i]]).t);
        sphereNvalues.push_back((sphereNorm[ind[i]]).x);
        sphereNvalues.push_back((sphereNorm[ind[i]]).y);
        sphereNvalues.push_back((sphereNorm[ind[i]]).z);
    }
    
    std::vector<glm::vec3> modelVert = myModel.getVertices();
    std::vector<glm::vec2> modelTex = myModel.getTextureCoords();
    std::vector<glm::vec3> modelNorm = myModel.getNormals();

    std::vector<float> modelPvalues;
    std::vector<float> modelTvalues;
    std::vector<float> modelNvalues;

    for (int i = 0; i < myModel.getNumVertices(); i++) {
        modelPvalues.push_back((modelVert[i]).x);
        modelPvalues.push_back((modelVert[i]).y);
        modelPvalues.push_back((modelVert[i]).z);
        modelTvalues.push_back((modelTex[i]).s);
        modelTvalues.push_back((modelTex[i]).t);
        modelNvalues.push_back((modelNorm[i]).x);
        modelNvalues.push_back((modelNorm[i]).y);
        modelNvalues.push_back((modelNorm[i]).z);
    }
    
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, spherePvalues.size()*4, &spherePvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sphereTvalues.size()*4, &sphereTvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sphereNvalues.size()*4, &sphereNvalues[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, modelPvalues.size()*4, &modelPvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glBufferData(GL_ARRAY_BUFFER, modelTvalues.size()*4, &modelTvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
    glBufferData(GL_ARRAY_BUFFER, modelNvalues.size()*4, &modelNvalues[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
    
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 12.0f;
    setupVertices();
    
    earthTexture = Utils::loadTexture("earth.jpg");
    sunTexture = Utils::loadTexture("sunmap.jpg");
    moonTexture = Utils::loadTexture("moon.jpg");
    shuttleTexture = Utils::loadTexture("spstob_1.jpg");
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(renderingProgram);

    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    // push view 矩阵
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mvStack.push(vMat);
    
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // ----------------------  sun
    // push model 矩阵
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    mvStack.push(mvStack.top());
    mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
        
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
    
    mvStack.pop();
    
    //-----------------------  shuttle
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime)*3.0, 0.0f, cos((float)currentTime)*3.0));
    mvStack.push(mvStack.top());
    mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime + toRadians(-90), glm::vec3(0.0, 1.0, 0.0));
    mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shuttleTexture);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());

    mvStack.pop();
    
    //----------------------- earth
    // push model 矩阵
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime)*1.0, 0.0f, cos((float)currentTime)*1.0));
    mvStack.push(mvStack.top());
    mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
    rotAmt += 0.0018f;
    mvStack.top() = rotate(mvStack.top(), rotAmt * 10, glm::vec3(0.0, 1.0, 0.0));
    mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.50f, 0.50f, 0.50f));

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
    
    mvStack.pop();
    
    //-----------------------  moon
    // push model 矩阵
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime), cos((float)currentTime)));
    mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));
    mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
    mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

    mvStack.pop(); mvStack.pop(); mvStack.pop();
    mvStack.pop();  // the final pop is for the view matrix
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
    aspect = (float)newWidth / (float)newHeight;
    glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // I don't know what this does
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // and neither this
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 6 - assignment 2", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
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
}
