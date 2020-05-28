#version 150 core

uniform float alpha;

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
    outColor = mix(texture(tex, Texcoord), texture(tex2, Texcoord), alpha);
    //outColor = texture(tex, Texcoord) * vec4(Color.xyz, alpha);
}