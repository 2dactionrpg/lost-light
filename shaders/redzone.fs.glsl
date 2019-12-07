#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform float cooldown;

// Output color
layout(location = 0) out vec4 color;

void main()
{
    float alpha = 1.0 - cooldown/2000.0;
    color = vec4(fcolor, alpha) * texture(sampler0, vec2(texcoord.x, texcoord.y));
}
