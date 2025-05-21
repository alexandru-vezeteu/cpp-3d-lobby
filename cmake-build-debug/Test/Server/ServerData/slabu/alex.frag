#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 worldPos;
in vec3 viewDir;

uniform vec3 lightPos;
uniform vec3 viewPos;  // Still needed for some calculations

vec3 lighting(vec3 objectColor, vec3 pos, vec3 normal, vec3 lightPos, 
              vec3 ambient, vec3 lightColor, vec3 specular, float specPower)
{
    // Light direction
    vec3 L = normalize(lightPos - pos);
    
    // Normalized normal
    vec3 N = normalize(normal);
    
    // View direction (from vertex shader)
    vec3 V = normalize(viewDir);
    
    // Reflection vector (Blinn-Phong half vector)
    vec3 H = normalize(L + V);
    
    // Diffuse coefficient
    float diffCoef = max(dot(N, L), 0.0);
    
    // Specular coefficient
    float specCoef = pow(max(dot(N, H), 0.0), specPower);
    
    // Combine lighting components
    vec3 ambientColor = ambient * lightColor;
    vec3 diffuseColor = diffCoef * lightColor;
    vec3 specularColor = specCoef * specular * lightColor;
    
    return clamp((ambientColor + diffuseColor + specularColor) * objectColor, 0.0, 1.0);
}

void main() 
{
    vec3 objectColor = vec3(0.8, 0.7, 0.6); // Light brown
    vec3 lightColor = vec3(1.0, 1.0, 1.0);  // White light
    vec3 ambient = vec3(0.1);
    vec3 specular = vec3(0.5);
    float specPower = 32.0;
    
    vec3 color = lighting(objectColor, worldPos, normal, lightPos,
                         ambient, lightColor, specular, specPower);
    
    // Gamma correction
    color = pow(color, vec3(1.0/2.2));
    
    fragColor = vec4(color, 1.0);
}