#version 330 core
in vec3 position;
in vec3 normal;
in vec3 color;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 model = mat4(1.0);

out vec3 fragNormal;
out vec3 fragPos;
out vec3 vertColor;

void main() {
   gl_Position = projection * view * model * vec4(position, 1.0f);
   fragPos = vec3(model * vec4(position, 1.0f));
   fragNormal = mat3(transpose(inverse(model))) * normal;
   vertColor = color;
}
