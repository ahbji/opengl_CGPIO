/**
 一个简单的 C++/OpenGL 应用：创建一个 GLFWwindow 实例，并为其设置背景色。
 主要过程：
 - 初始化 GLFW 库
 - 实例化 GLFWwindow 对象
 - 初始化 GLEW 库
 - 初始化应用
 - 在渲染循环中调用 display 函数
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void init (GLFWwindow* window) {}

// 指定清除背景时使用的颜色值
void display(GLFWwindow* window, double currentTime) {
    // 指定颜色缓冲区 color buffer 清除后填充的值
    glClearColor(1.0, 0.0, 0.0, 1.0);
    
    // void glClear(GLbitfield mask)
    // GL_COLOR_BUFFERE_BIT : 引用包括渲染后像素的颜色缓冲区
    // glClear(GL_COLOR_BUFFERE_BIT) : 清理 OpenGL 包含的所有颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) {
    // 初始化 GLFW 库
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    
    // OpenGL version 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // 实例化 GLFWwindow 对象
    // monitor, share: 全屏和资源共享
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", nullptr, nullptr);
    // 关联 window 到当前 OpenGL 的 context
    glfwMakeContextCurrent(window);
    
    // 初始化 GLEW 库
    if (glewInit() != GLEW_OK) {exit(EXIT_FAILURE);}
    
    // 设置 Vsync 周期
    glfwSwapInterval(1);
    
    // includes a reference to the GLFW window object
    init(window);
    
    // 渲染循环
    // 当 GLFW 探测到应该关闭窗口的事件（如用户关闭了窗口），循环就会停止
    while (!glfwWindowShouldClose(window)) {
        
        // 传参 GLFW window object 的引用以便在特定环境下能访问到 GLFW window 对象
        // 传参 current time 以确保动画在不同的设备商以相同的速度播放
        display(window, glfwGetTime());
        
        // 以 Vsync 方式绘制屏幕
        glfwSwapBuffers(window);
        
        // 处理 window 相关 events
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
