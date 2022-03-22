#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define numVAOs 1

// GLuint : unsigned int
GLuint shaderProgram;
GLuint vao[numVAOs];

using namespace std;

void createShaderProgram() {
    
    // 在 String 中定义 GLSL（GL Shader Language）code
    const char* vertexShaderSource =
    "#version 410 \n"
    "void main(void) \n"
    // 设置顶点在 3D 控件的坐标位置
    // 硬编码原点位置值 : (0,0,0)
    // gl_Position 是预定义的输出变量，使用 vec4 存储四元组值
    // gl_Position 的值最终会输出到光栅化阶段
    "{gl_Position = vec4(0.0, 0.0, 0.0, 1.0);}";
    
    const char* fragmentShaderSource =
    "#version 410 \n"
    // 输出颜色值
    // out 表明 color 变量是个输出变量
    "out vec4 color; \n"
    "void main(void) \n"
    "{"
    // 使用预定义变量 gl_FragCoord 访问当前片段的坐标
    "   if (gl_FragCoord.x < 200)\n"
    "       color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "   else\n"
    "       color = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "}";
    
    // glCreateShader : 生成两个 shaderObject ，类型分别是 GL_VERTEX_SHADER and GL_FRAGMENT_SHADER
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // 从 String 中加载 GLSL code 到 shaderObject
    // glShaderSource(ShaderObject, NumberOfStrings : 1, ArrayOfPointers, -)
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    
    // 编译 shader
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    
    // the integer ID of a ShaderProgram object
    shaderProgram = glCreateProgram();
    
    // 将 shader 载入 ShaderProgram object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    // 请求 GLSL 编译期确保 shader 的兼容性
    glLinkProgram(shaderProgram);
}


void init (GLFWwindow* window) {
    createShaderProgram();
    
    // 初始化 VAO 对象
    // 以缓冲区的形式将数据集发送给管线
    // 这些缓冲区最后都会被存入 VAO 对象
    // VAO : Vertex Array Objects, OpenGL requires at least one VAO
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
    
    // loads the program containing the two compiled shaders into the OpenGL pipeline stages (onto the GPU)
    glUseProgram(shaderProgram);
    
    // 定义一个 Point 对应的像素数量。
    glPointSize(400.0f);
    
    // 通知 OpenGL 绘制图元
    // initiates pipeline processing
    // mode: GL_POINTS, from 0, one (point)
    glDrawArrays(GL_POINTS, 0, 1);
}

int main(void) {
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // I don't know what this does
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // and neither this
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program2", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {exit(EXIT_FAILURE);}                // without the line 80 or 81, this makes an error
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
