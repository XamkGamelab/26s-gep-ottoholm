#version 460 core

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec2 in_tex_coord;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out vec2 out_tex_coord ;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(in_pos, 0.0, 1.0);
    out_tex_coord  = in_tex_coord;
}
