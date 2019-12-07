#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform float light_source_x[20];
uniform float light_source_y[20];
uniform float radius[20];

uniform int ls_num;

// Output color
layout(location = 0) out vec4 color;

void main()
{
    float min_dist = 999999;
    float alpha = 99999;
    float index = -1;
    float r = 0;
    float alphas[20];
    
    for (int i = 0; i < 20; i++) {
        vec2 ls = vec2(light_source_x[i], light_source_y[i]);
        float dist = sqrt(
            (texcoord.x - ls.x)*(texcoord.x - ls.x) + 
            (texcoord.y - ls.y)*(texcoord.y - ls.y));

        float r = radius[i];
        alphas[i] = dist / r;

        if (dist < min_dist) {
            min_dist = dist;
            index = i;
            r = radius[i];
        }
    }

    for (int i = 0; i < 20; i++) {
        if (alphas[i] <= alpha) {
            alpha = alphas[i];
        }
    }

    color = vec4(0.0,0.0,0.0, alpha);
}
