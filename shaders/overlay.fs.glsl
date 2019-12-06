#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;

// Output color
layout(location = 0) out vec4 color;

void main()
{
    vec2 centers[2];
    centers[0] = vec2(0.25, 0.5);
    centers[1] = vec2(0.75, 0.5);

    float min_dist = 9999;
    vec2 min_center;

    for (int i = 0; i < 2; i++) {
        vec2 center;
        float dist;
        center = centers[i];

        dist = sqrt((texcoord.x - center.x)*(texcoord.x - center.x) 
        + (texcoord.y - center.y)*(texcoord.y - center.y));
        if (dist < min_dist) {
            min_dist = dist;
            min_center = center;
        }
    }

    float distance_yo = sqrt((texcoord.x - min_center.x)*(texcoord.x - min_center.x) 
    + (texcoord.y - min_center.y)*(texcoord.y - min_center.y))*7.0;

    color = vec4(fcolor, distance_yo) * texture(sampler0, vec2(texcoord.x, texcoord.y));
}
