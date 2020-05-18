#version 150 core

uniform float alpha;

in vec3 Color;

out vec4 outColor;

void main()
{
    outColor = vec4(Color.xyz, alpha);
}