#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <shader.h>

#define WIDTH 800
#define HEIGHT 600

int mWidth = WIDTH, mHeight = HEIGHT;

//���ڴ�С������ʱ���õĺ���
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    mWidth = width;
    mHeight = height;
    glViewport(0, 0, width, height);
}

//�����û�����
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
    
    //��ʼ��glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //����һ��800x600�Ĵ���
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL CN", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window|�޷�����GLFW����" << std::endl;
        glfwTerminate();
        return -1;
    }
    //�����Ǵ��ڵ���������Ϊ���̵߳���������
    glfwMakeContextCurrent(window);

    //��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //�����ӿڵĳߴ�
    glViewport(0, 0, 800, 600);
    //ע�ᴰ�ڴ�С�仯ʱ���õĺ���
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //׼�����㻺�����VBO��

    //��������
    float vertices[] = {
        // λ��              // ��ɫ
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //���½���VBO�󶨵���Ӧ�Ļ���������
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //�����ݸ��Ƶ���ǰ�󶨵Ļ����ϣ������ݸ��Ƶ��Դ�
    /*
    ���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
    GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
    GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
    GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
    */
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //����OpenGL����ν�����������
    //��һ������ָ������Ҫ���õĶ�������
    //�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3
    //����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)
    //���ڵ��ĸ������������������ΪGL_TRUE��
    // �������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮��
    //
    //�����������������(Stride)�������������������Ķ���������֮��ļ����
    // �����¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)
    //���һ������������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)��
    // ����λ������������Ŀ�ͷ������������0
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�
    // ��������Ĭ���ǽ��õ�
    //glEnableVertexAttribArray(0);

    //׼����ɫ��

    Shader ourShader("assets/vs.glsl","assets/fs.glsl");

    //׼�������������VAO��

    //����VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. ��VAO
    glBindVertexArray(VAO);
    // 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. ���ö�������ָ��
    // λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ��ɫ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //���VAO
    glBindVertexArray(0);

    //�߿�ģʽ�����ģʽ����Ϊ��
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //�л���Ĭ��ģʽ
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    //��ѭ��
    while (!glfwWindowShouldClose(window))
    {
        //�����û�����
        processInput(window);

        //����������ɫ
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //������ɫ�仯
        glClearColor(0.2f * sin(glfwGetTime()), 0.3f, 0.3f * cos(glfwGetTime()), 1.0f);
        //����
        glClear(GL_COLOR_BUFFER_BIT);
        //ע��������������Ⱦ֮ǰ����������Ⱦ֮�󣬲�Ȼ�Ḳ�ǵ���Ⱦ���

        //��Ⱦ
        ourShader.use();
        ourShader.setFloat("xOffset", 0.5f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //����˫����
        glfwSwapBuffers(window);
        //��鲢�����¼�
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //�رղ�����
    glfwTerminate();
    return 0;
}