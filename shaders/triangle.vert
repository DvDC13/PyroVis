#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout(push_constant) uniform PushConstants {
    mat2 transform;
    vec2 offset;
    vec3 color;
} pc;

void main()
{
    gl_Position = vec4(pc.transform * position + pc.offset, 0.0, 1.0);
}