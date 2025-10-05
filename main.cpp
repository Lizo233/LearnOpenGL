#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <shader.h>

#define WIDTH 800
#define HEIGHT 600

int mWidth = WIDTH, mHeight = HEIGHT;

//窗口大小被调整时调用的函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    mWidth = width;
    mHeight = height;
    glViewport(0, 0, width, height);
}

//处理用户输入
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char* argv[]) {
    
    //初始化glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //创建一个800x600的窗口
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL CN", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window|无法创建GLFW窗口" << std::endl;
        glfwTerminate();
        return -1;
    }
    //将我们窗口的上下文作为该线程的主上下文
    glfwMakeContextCurrent(window);

    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置视口的尺寸
    glViewport(0, 0, 800, 600);
    //注册窗口大小变化时调用的函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //准备顶点缓冲对象（VBO）

    //顶点数据
    float vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //将新建的VBO绑定到对应的缓冲类型上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //将数据复制到当前绑定的缓冲上，将数据复制到显存
    /*
    第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
    GL_STATIC_DRAW ：数据不会或几乎不会改变。
    GL_DYNAMIC_DRAW：数据会被改变很多。
    GL_STREAM_DRAW ：数据每次绘制时都会改变。
    */
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //告诉OpenGL该如何解析顶点数据
    //第一个参数指定我们要配置的顶点属性
    //第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3
    //第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)
    //对于第四个参数，如果我们设置为GL_TRUE，
    // 所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间
    //
    //第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
    // 由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)
    //最后一个参数，它表示位置数据在缓冲中起始位置的偏移量(Offset)。
    // 由于位置数据在数组的开头，所以这里是0
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；
    // 顶点属性默认是禁用的
    //glEnableVertexAttribArray(0);

    //准备着色器

    Shader ourShader("assets/vs.glsl","assets/fs.glsl");

    //准备顶点数组对象（VAO）

    //生成VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //解绑VAO
    glBindVertexArray(0);

    //线框模式，填充模式设置为线
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //切换回默认模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    //主循环
    while (!glfwWindowShouldClose(window))
    {
        //处理用户输入
        processInput(window);

        //设置清屏颜色
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清屏颜色变化
        glClearColor(0.2f * sin(glfwGetTime()), 0.3f, 0.3f * cos(glfwGetTime()), 1.0f);
        //清屏
        glClear(GL_COLOR_BUFFER_BIT);
        //注意清屏必须在渲染之前，不能在渲染之后，不然会覆盖掉渲染结果

        //渲染
        ourShader.use();
        ourShader.setFloat("xOffset", 0.5f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //交换双缓冲
        glfwSwapBuffers(window);
        //检查并调用事件
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //关闭并清理
    glfwTerminate();
    return 0;
}