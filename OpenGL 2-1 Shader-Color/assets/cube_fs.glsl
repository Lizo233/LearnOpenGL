#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

//光的颜色
struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //手电筒
    float cutOff;
    float outerCutOff;

    //点光源
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

//材质
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
uniform Material material;

in vec2 TexCoords;

uniform sampler2D matrix;

uniform float time;

void main()
{
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));
    
    //计算环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    //计算光源的漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords)); 

    //镜面反射
    float specularStrength = 0.5;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;
    
    vec3 result = ambient + diffuse + specular;


    if(theta > light.outerCutOff) FragColor = vec4(result, 1.0);
    else FragColor = vec4(ambient, 1.0);
}