#version 460 core

out vec4 FragColor ;
in vec2 out_tex_coord;

uniform sampler2D texture_0;
uniform sampler2D texture_1;

void main()
{
    FragColor  = texture(texture_0, out_tex_coord);
}
