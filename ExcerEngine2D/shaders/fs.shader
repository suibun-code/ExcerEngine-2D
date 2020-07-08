#version 330 core

//layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;
//uniform sampler2D tex2;

uniform float alpha;
uniform float time;

void main()
{

    /***REVERSED LOWER HALF***/
    //if (Texcoord.y < 0.5)
    //    outColor = texture(tex, Texcoord);
    //else
    //    outColor = texture(tex, vec2(1.0 - Texcoord.x, 1.0 - Texcoord.y));

    /***SWITCH BETWEEN TWO IMAGES IN AN OPACITY METHOD OVER TIME***/
    //float factor = (sin(time * 3.0) + 1.0) / 2.0;
    //outColor = mix(texture(tex, Texcoord), texture(tex2, Texcoord), factor);

    /***WATER-LIKE OUTPUT OF BOTTOM HALF***/
    //if (Texcoord.y < 0.5)
    //    outColor = texture(tex, Texcoord);
    //else
    //    outColor = texture(tex, vec2(Texcoord.x + sin(Texcoord.y * 60.0 + time * 2.0) / 30.0, 1.0 - Texcoord.y)) * vec4(0.7, 0.7, 1.0, 1.0);

    /***MIX TWO TEXTURES***/
    //outColor = mix(texture(tex, Texcoord), texture(tex2, Texcoord), alpha);

    /***NORMAL OUTPUT***/
    outColor = texture(tex, Texcoord) * vec4(Color.xyz, alpha);
}