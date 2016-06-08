#version 330 core
in vec2 position;
out vec2 fragCoord;

uniform vec2      iResolution = vec2(0.0); // viewport resolution (in pixels)
uniform float     iGlobalTime = 0.0;       // shader playback time (in seconds)

void main() {
   gl_Position = vec4(position, 0, 1);
   fragCoord = position;
}
