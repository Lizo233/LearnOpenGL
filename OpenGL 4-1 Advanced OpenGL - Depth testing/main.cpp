#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include <map>

//非标准库
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//GLM数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//assimp库（用于加载模型）
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//自创建库
#include <shader.h>
#include <camera.h>

#include <mesh.h>
#include <model.h>

constexpr int WIDTH = 1600;
constexpr int HEIGHT = 1200;

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

void loadTextureRGB(unsigned int* textureID, std::string path, int glTexture);
void loadTextureRGBA(unsigned int* textureID, std::string path, int glTexture);

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
    //glfwSwapInterval(1);

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

    //大量的顶点数据

    /*
    Remember: to specify vertices in a counter-clockwise winding order you need to visualize the triangle
    as if you're in front of the triangle and from that point of view, is where you set their order.

    To define the order of a triangle on the right side of the cube for example, you'd imagine yourself looking
    straight at the right side of the cube, and then visualize the triangle and make sure their order is specified
    in a counter-clockwise order. This takes some practice, but try visualizing this yourself and see that this
    is correct.
    */

    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
    };

    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    };

    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // 草的VAO
    unsigned int transparentVAO, transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // 草的位置
    std::vector<glm::vec3> windows;
    windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    //翻转y轴
    //stbi_set_flip_vertically_on_load(true);

    //mipmap设置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//缩小时用临近过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大时用线性过滤

    unsigned char* data = nullptr;
    int width{}, height{}, nrChannels{};

    //加载材质资源

    unsigned int cubeTexture{};
    unsigned int floorTexture{};
    unsigned int windowsTexture{};

    loadTextureRGB(&cubeTexture, "./assets/marble.jpg", GL_TEXTURE0);
    loadTextureRGB(&floorTexture, "./assets/metal.png", GL_TEXTURE1);
    loadTextureRGBA(&windowsTexture, "./assets/blending_transparent_window.png", GL_TEXTURE2);

    //投影矩阵 摄像机矩阵 模型矩阵 矩阵部分
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    //鼠标将会被限制在窗口内且不显示，也不会离开窗口
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    Shader shader("./assets/depth-test-vs.glsl", "./assets/depth-test-fs.glsl");
    
    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //设置深度测试函数
    glDepthFunc(GL_LESS);

    //开启模板测试
    glEnable(GL_STENCIL_TEST);
    //glStencilMask(0xFF); // 每一位写入模板缓冲时都保持原样
    //glStencilMask(0x00); // 每一位在写入模板缓冲时都会变成0（禁用写入）

    //Shader shaderSingleColor("./assets/depth-test-vs.glsl", "./assets/single-color-fs.glsl");

    // 启用混合
    glEnable(GL_BLEND);
    // 设置混合因子，使用源颜色的alpha作为源因子，使用1-alpha作为目标因子，最终颜色等于: 源颜色*alpha + 目标颜色*(1-alpha)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    
    // 开启面剔除
    glEnable(GL_CULL_FACE);
    // 由逆时针的顶点组成的三角形是正面
    glFrontFace(GL_CCW);
    // 剔除背向面
    glCullFace(GL_BACK);

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
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //注意清屏必须在渲染之前，不能在渲染之后，不然会覆盖掉渲染结果
        
        //矩阵变换部分
        
        //世界矩阵

        model = glm::mat4(1.0f);

        //摄像机矩阵
        view = camera.getView();
        //投影矩阵
        projection = camera.getProjection();
        //转置逆矩阵 
        glm::mat4 tiModel = glm::transpose(glm::inverse(model));

        //绘画

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);

        //将矩阵设置到shader中
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        
        //画地板
        //glStencilMask(0x00); // 记得保证我们在绘制地板的时候不会更新模板缓冲
        shader.use();
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        


        //glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
        glStencilMask(0xFF); // 启用模板缓冲写入

        // 画那两个容器
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.01f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.01f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 给玻璃按照距离排序
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windows.size(); i++)
        {
            float distance = glm::length(camera.cameraPos - windows[i]);
            sorted[distance] = windows[i];
        }

        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, windowsTexture);
        
        // 画半透明的玻璃

        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        /*
        //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //glStencilMask(0x00); // 禁止模板缓冲的写入
        //glDisable(GL_DEPTH_TEST);

        //shaderSingleColor.use();

        //// 画更大一点的两个容器
        //shaderSingleColor.setMat4("view", view);
        //shaderSingleColor.setMat4("projection", projection);

        //float scale = 1.1f;
        //glBindVertexArray(cubeVAO);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, cubeTexture);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-1.0f, 0.01f, -1.0f));
        //model = glm::scale(model, glm::vec3(scale));
        //shaderSingleColor.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(2.0f, 0.01f, 0.0f));
        //model = glm::scale(model, glm::vec3(scale));
        //shaderSingleColor.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //glStencilMask(0xFF);
        //glStencilFunc(GL_ALWAYS, 0, 0xFF);
        //glEnable(GL_DEPTH_TEST);
        */

            

        //交换双缓冲
        glfwSwapBuffers(window);
        //检查并调用事件
        glfwPollEvents();
    }

    //删除不需要的资源
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);

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

void loadTextureRGBA(unsigned int* textureID,std::string path, int glTexture)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    //错误处理
    if (data == nullptr) {
        std::cout << "Failed to load texture\n";
        exit(1);
    }

    //生成纹理
    glGenTextures(1, textureID);
    //绑定纹理
    // 在绑定纹理之前先激活纹理单元，GL_TEXTURE0默认被激活
    glActiveTexture(glTexture);
    glBindTexture(GL_TEXTURE_2D, *textureID);
    //加载纹理到显存中rgb
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //创建mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    //设置环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //设置mipmap方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    //生成了纹理和相应的多级渐远纹理后，释放图像的内存是一个很好的习惯。
    stbi_image_free(data);
}

void loadTextureRGB(unsigned int* textureID, std::string path, int glTexture)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    //错误处理
    if (data == nullptr) {
        std::cout << "Failed to load texture\n";
        exit(1);
    }

    //生成纹理
    glGenTextures(1, textureID);
    //绑定纹理
    // 在绑定纹理之前先激活纹理单元，GL_TEXTURE0默认被激活
    glActiveTexture(glTexture);
    glBindTexture(GL_TEXTURE_2D, *textureID);
    //加载纹理到显存中rgb
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //创建mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    //生成了纹理和相应的多级渐远纹理后，释放图像的内存是一个很好的习惯。
    stbi_image_free(data);
}


