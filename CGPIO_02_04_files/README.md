glShaderSource() 方法的第四个参数指定了一个“长度数组”，其中包括给定着色器程序中每行代码的字符串的整数长度。

如果这个参数被设为null，像我们之前那样，OpenGL将会自动从以null结尾的字符串中构建这个数组。

因此特地确保所有传给 glShaderSource() 的字符串都是以 null 结尾的 [通过在createShaderProgram()中调用c_str()函数]。

实际中通常也会遇到手动构建这些数组而非传入 null 的应用程序。




