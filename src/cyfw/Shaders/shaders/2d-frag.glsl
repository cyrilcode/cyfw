#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)

// forward declaration of image function
void mainImage( out vec4 fragColor, in vec2 fragCoord );

void main()
{
    mainImage(fragColor, fragCoord);
}
