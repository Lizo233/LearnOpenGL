#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

//非标准库
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//GLM数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//自创建库
#include <shader.h>
#include <camera.h>

#define WIDTH 1600
#define HEIGHT 1200

int mWidth = WIDTH, mHeight = HEIGHT;
glm::mat4 unitMat = glm::mat4(1.0f);

//窗口大小被调整时调用的函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//处理用户输入
void processInput(GLFWwindow* window);

//监听鼠标输入
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//监听鼠标滚轮
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

//创建相机
Camera camera;

//主窗口声明
GLFWwindow* window = nullptr;

int main(int argc, char* argv[]) {

    //初始化glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //创建一个800x600的窗口
    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL CN", NULL, NULL);
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
    glViewport(0, 0, WIDTH, HEIGHT);

    //注册窗口大小变化时调用的函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //注册鼠标移动时调用的函数
    glfwSetCursorPosCallback(window, mouse_callback);

    //注册鼠标滚轮移动时调用的函数
    glfwSetScrollCallback(window, scroll_callback);

    //设置FPS上限为显示器刷新率
    glfwSwapInterval(1);

    //生成VAO,VBO,EBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //大量的顶点数据
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };


    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    //将新建的VBO绑定到对应的缓冲类型上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //翻转y轴
    stbi_set_flip_vertically_on_load(true);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//缩小时用临近过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大时用线性过滤

    //使用stb_image.h加载图片，加载木箱图片
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/container2.png", &width, &height, &nrChannels, 0);

    //错误处理
    if (data == nullptr) {
        std::cout << "Failed to load texture\n";
        exit(1);
    }

    //生成纹理
    unsigned int diffuseMap;
    glGenTextures(1, &diffuseMap);
    //绑定纹理
    // 在绑定纹理之前先激活纹理单元，GL_TEXTURE0默认被激活
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    //加载纹理到显存中rgb
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //创建mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    //生成了纹理和相应的多级渐远纹理后，释放图像的内存是一个很好的习惯。
    stbi_image_free(data);


    //加载木箱镜面反射图片
    data = stbi_load("assets/container2_specular.png", &width, &height, &nrChannels, 0);

    //错误处理
    if (data == nullptr) {
        std::cout << "Failed to load texture\n";
        exit(1);
    }

    //生成纹理
    unsigned int specularMap;
    glGenTextures(1, &specularMap);
    //绑定纹理
    // 在绑定纹理之前先激活纹理单元，GL_TEXTURE0默认被激活
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    //加载纹理到显存中rgb
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //创建mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    //生成了纹理和相应的多级渐远纹理后，释放图像的内存是一个很好的习惯。
    stbi_image_free(data);


    //加载灯条图片
    data = stbi_load("assets/matrix.jpg", &width, &height, &nrChannels, 0);

    //错误处理
    if (data == nullptr) {
        std::cout << "Failed to load texture\n";
        exit(1);
    }

    //生成纹理
    unsigned int matrixLight;
    glGenTextures(1, &matrixLight);
    //绑定纹理
    // 在绑定纹理之前先激活纹理单元，GL_TEXTURE0默认被激活
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, matrixLight);
    //加载纹理到显存中rgb
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //创建mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    //生成了纹理和相应的多级渐远纹理后，释放图像的内存是一个很好的习惯。
    stbi_image_free(data);

    //投影矩阵 摄像机矩阵 模型矩阵 矩阵部分
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //鼠标将会被限制在窗口内且不显示，也不会离开窗口
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Shader lightingShader("assets/vs.glsl","assets/cube_fs.glsl");

    lightingShader.use();
    lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    glUseProgram(0);



    Shader lightCubeShader("assets/vs.glsl", "assets/light_fs.glsl");
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
        //lightPos = glm::vec3(1.2f * cos(glfwGetTime()), 1.0f, 1.0f * sin(-glfwGetTime()));

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //处理用户输入
        processInput(window);

        //设置清屏颜色
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清屏颜色变化
        glClearColor(0.2f * sin(glfwGetTime()), 0.3f, 0.3f * cos(glfwGetTime()), 1.0f);
        //清屏
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //注意清屏必须在渲染之前，不能在渲染之后，不然会覆盖掉渲染结果
        
        //矩阵变换部分
        //世界矩阵
        lightingShader.use();
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model", glm::mat4(1.0f));

        //摄像机矩阵
        view = camera.getView();
        lightingShader.setMat4("view", view);

        //投影矩阵
        projection = camera.getProjection();
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("viewPos", camera.cameraPos);

        //光的位置和光的颜色
        lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        //material结构体
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 1);
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setInt("matrix", 2);
        lightingShader.setFloat("time", glfwGetTime());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, matrixLight);

        glm::mat4 tiModel = glm::transpose(glm::inverse(model));
        //转置逆矩阵 
        lightingShader.setMat4("transposeInverseModel", tiModel);

        //绘画
        /*glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        lightingShader.setMat4("view", glm::translate(view,glm::vec3(1.0f,0.0f,0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        lightingShader.setMat4("view", glm::translate(view, glm::vec3(-1.0f, 0.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);*/

        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);
            glm::mat4 tiModel = glm::transpose(glm::inverse(model));
            //转置逆矩阵
            lightingShader.setMat4("transposeInverseModel", tiModel);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        /*lightCubeShader.use();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);*/




        //交换双缓冲
        glfwSwapBuffers(window);
        //检查并调用事件
        glfwPollEvents();
    }

    //删除不需要的资源
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    //关闭并清理
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    mWidth = width;
    mHeight = height;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    camera.camera_processInput();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.camera_mouse_callback(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.camera_scroll_callback(xoffset,yoffset);
}