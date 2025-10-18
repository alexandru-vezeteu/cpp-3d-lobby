#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 viewPos;



out vec2 texCoord;
out vec3 worldPos;
out mat3 TBN;

void main()
{
    //mvp matrix pt pozitie
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    //coordonate tex pt interpolare pt frag shader
    texCoord = aTexCoord;

    //world pos pt interpolare pt frag shader
    worldPos = vec3(model * vec4(aPos, 1.0));

    //aplic model pe normal si tangent ca sa pot calcula bitangent
    vec3 normal = normalize(vec3(model * vec4(aNormal, 0.0)));
    vec3 tangent = normalize(vec3(model * vec4(aTangent, 0.0)));
    
    //calc bitangent = vector perpendicular pe normal si tangent
    vec3 bitangent = cross(normal, tangent); 

    //folosesc aTexCoord in textura de normalMap si inmultesc vectorul de acolo cu tbn 
    //ca sa pot aplica normal mapping cum trb
    TBN = mat3(tangent, bitangent, normal);
}
