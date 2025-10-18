#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

//非标准库
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//GLM数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WIDTH 800
#define HEIGHT 600

int mWidth = WIDTH, mHeight = HEIGHT;
glm::mat4 unitMat = glm::mat4(1.0f);

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
    
    glm::mat4 trans(1.0f);
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

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
    //glfwSwapInterval(0);

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



    //将数据复制到当前绑定的缓冲上，将数据复制到显存
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*
    第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
    GL_STATIC_DRAW ：数据不会或几乎不会改变。
    GL_DYNAMIC_DRAW：数据会被改变很多。
    GL_STREAM_DRAW ：数据每次绘制时都会改变。
    */

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

    //准备顶点数组对象（VAO）和顶点缓冲对象（VBO）和EBO

    //生成VAO,VBO,EBO
    unsigned int VAO,VBO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //顶点数据
    //float vertices[] = {
    //    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    //         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    //        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    //        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    //};

    float vertices[] = {//正方体的顶点和uv
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    //元素索引
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // 1. 绑定VAO
    glBindVertexArray(VAO);

    //绑定EBO，并将索引传输至EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    //将新建的VBO绑定到对应的缓冲类型上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    // 设置位置属性 并启用
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 设置颜色属性 并启用
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    // 设置纹理坐标属性 并启用
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //解绑VAO 防止被意外的修改
    glBindVertexArray(0);

    //线框模式，填充模式设置为线
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //切换回默认模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



    //纹理过滤设置

    /*
    纹理环绕方式
    GL_REPEAT 	对纹理的默认行为。重复纹理图像。
    GL_MIRRORED_REPEAT 	和GL_REPEAT一样，但每次重复图片是镜像放置的。
    GL_CLAMP_TO_EDGE 	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
    GL_CLAMP_TO_BORDER 	超出的坐标为用户指定的边缘颜色。

    第一个参数指定了纹理目标；我们使用的是2D纹理，因此纹理目标是GL_TEXTURE_2D。
    第二个参数需要我们指定设置的选项与应用的纹理轴。我们打算配置的是WRAP选项，并且指定S和T轴。
    最后一个参数需要我们传递一个环绕方式(Wrapping)，
    在这个例子中OpenGL会给当前激活的纹理设定纹理环绕方式为GL_MIRRORED_REPEAT。
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //选择GL_CLAMP_TO_BORDER时，还需要指定边缘颜色，使用glTexParameter函数的fv后缀形式
    //并传递一个float数组作为边缘的颜色值
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //设置纹理被放大/缩小时的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//缩小时用临近过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大时用线性过滤

    /*
    * 多级渐远纹理（MIPMAP）过滤方式
    GL_NEAREST_MIPMAP_NEAREST 	使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
    GL_LINEAR_MIPMAP_NEAREST 	使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
    GL_NEAREST_MIPMAP_LINEAR 	在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
    GL_LINEAR_MIPMAP_LINEAR 	在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
    */
    //放大不能使用mipmap
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    //加载与创建纹理
    
    //翻转y轴
    stbi_set_flip_vertically_on_load(true);
    
    //使用stb_image.h加载图片，加载木箱图片
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
    //这个函数首先接受一个图像文件的位置作为输入。接下来它需要三个int作为它的第二、第三和第四个参数，
    //stb_image.h将会用图像的宽度、高度和颜色通道的个数填充这三个变量。
    //我们之后生成纹理的时候会用到的图像的宽度和高度的。

    //错误处理
    if (data == nullptr) {
        std::cout << "Failed to load texture\n";
        exit(1);
    }

    //生成纹理
    unsigned int texture1;
    glGenTextures(1, &texture1);
    //绑定纹理
    // 在绑定纹理之前先激活纹理单元，GL_TEXTURE0默认被激活
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //加载纹理到显存中
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
    //第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
    //第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
    //第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
    //下个参数应该总是被设为0（历史遗留的问题）。
    //第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
    //最后一个参数是真正的图像数据。
    //创建mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    //生成了纹理和相应的多级渐远纹理后，释放图像的内存是一个很好的习惯。
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    //设置超出边缘时的处理方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    data = stbi_load("./assets/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    ourShader.use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
    ourShader.setInt("texture2", 1); // 或者使用着色器类设置
    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");

    //投影矩阵 摄像机矩阵 模型矩阵 矩阵部分
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    //最后我们需要做的是定义一个投影矩阵。我们希望在场景中使用透视投影，
    // 所以像这样声明一个投影矩阵：
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //很多立方体
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //处理用户输入
        processInput(window);

        //设置清屏颜色
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清屏颜色变化
        glClearColor(0.2f * sin(glfwGetTime()), 0.3f, 0.3f * cos(glfwGetTime()), 1.0f);
        //清屏
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //注意清屏必须在渲染之前，不能在渲染之后，不然会覆盖掉渲染结果

        //渲染
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        
        //矩阵变换部分
        int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            if (i % 3 == 0) {
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
                ourShader.setMat4("model", model);
            }

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        modelLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));
        modelLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        //绘画
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //交换双缓冲
        glfwSwapBuffers(window);
        //检查并调用事件
        glfwPollEvents();
    }

    //删除不需要的资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //关闭并清理
    glfwTerminate();
    return 0;
}