#version 400

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelMatrix;
uniform vec3 viewPos;  // Added this uniform declaration

out vec3 normal;
out vec3 worldPos;     // Renamed from 'pos' for clarity
out vec3 viewDir;

void main() 
{
    gl_Position = modelViewProjectionMatrix * vec4(vPos, 1.0);
    
    // Transform normal using normal matrix
    normal = normalize(vec3(normalMatrix * vec4(vNormal, 0.0)));
    
    // Calculate world position
    worldPos = vec3(modelMatrix * vec4(vPos, 1.0));
    
    // Calculate view direction
    viewDir = normalize(viewPos - worldPos);
}