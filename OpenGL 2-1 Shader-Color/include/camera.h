#pragma once


//将窗口变量外声明
extern GLFWwindow* window;
//将时间变化变量外声明
extern float deltaTime;
//窗口大小外声明
extern int mWidth, mHeight;

class Camera {
public:
    double yaw = -90.0;//初始方向
    double pitch = 0;
    
    //摄像机基础矩阵
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    //基本视张角
    float fov = 45.0f;
    
    bool firstMouse = true;
    double lastX, lastY;
    void camera_processInput() {
        
        //glm::normalize(glm::cross(-glm::normalize(glm::cross(cameraFront, cameraUp)), cameraUp))
        //以不改变y轴的方式向前后移动

        float cameraSpeed = 2.5f * deltaTime; // adjust accordingly|按需调整
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cameraPos -= cameraUp * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraPos += cameraUp * cameraSpeed;
        //cameraPos.y = 0;
    }
	void camera_scroll_callback(double xoffset, double yoffset) {
        if (fov >= 1.0f && fov <= 45.0f)
            fov -= yoffset;
        if (fov <= 1.0f)
            fov = 1.0f;
        if (fov >= 45.0f)
            fov = 45.0f;
	}
    void camera_mouse_callback(double xpos,double ypos) {
        if (firstMouse) // 这个bool变量初始时是设定为true的
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
        //printf("xpos:%llf ypos:%llf\t", xpos, ypos);
        //printf("xoffset:%f yoffset:%f \n",xoffset,yoffset);
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        //俯仰/偏航角计算
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraFront = glm::normalize(front);
    }
    glm::mat4 getView() {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    glm::mat4 getProjection() {
        return glm::perspective(glm::radians(fov), (float)mWidth / mHeight, 0.1f, 100.0f);
    }
};