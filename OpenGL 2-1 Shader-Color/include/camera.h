#pragma once


//�����ڱ���������
extern GLFWwindow* window;
//��ʱ��仯����������
extern float deltaTime;
//���ڴ�С������
extern int mWidth, mHeight;

class Camera {
public:
    double yaw = -90.0;//��ʼ����
    double pitch = 0;
    
    //�������������
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    //�������Ž�
    float fov = 45.0f;
    
    bool firstMouse = true;
    double lastX, lastY;
    void camera_processInput() {
        
        //glm::normalize(glm::cross(-glm::normalize(glm::cross(cameraFront, cameraUp)), cameraUp))
        //�Բ��ı�y��ķ�ʽ��ǰ���ƶ�

        float cameraSpeed = 2.5f * deltaTime; // adjust accordingly|�������
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
        if (firstMouse) // ���bool������ʼʱ���趨Ϊtrue��
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
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

        //����/ƫ���Ǽ���
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