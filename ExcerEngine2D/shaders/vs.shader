#version 330 core

in vec2 position;
in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec2 Texcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 MVP;

void main()
{
    Color = color;
    Texcoord = texcoord;
    gl_Position = MVP * vec4(position.xy, 0.0, 1.0);
    //gl_Position = model * view * projection * vec4(position, 0.0, 1.0);
}