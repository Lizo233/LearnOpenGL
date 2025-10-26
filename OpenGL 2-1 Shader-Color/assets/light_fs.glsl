#version 330 core
out vec4 FragColor;

//光的颜色
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    FragColor = vec4(1.0f,1.0f,1.0f,0.8f); // 将向量的四个分量全部设置为1.0
}