#version 330 core

out vec4 FragColor;  
in vec3 ourColor;
in vec3 pColor;

void main()
{
    FragColor = vec4(pColor, 1.0);
}