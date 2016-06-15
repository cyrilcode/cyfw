#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 fragNormal;
in vec4 vertColor;

uniform vec3 lightPos = vec3(0.0f, 0.0f, -3.0f);
uniform vec3 viewPos = vec3(0.0f, 0.0f, -3.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Final colour calculation
    // TODO: support input alpha
    vec3 c = (ambient + diffuse) * vertColor.rgb;
    fragColor = vec4(c, 1.0);
}
