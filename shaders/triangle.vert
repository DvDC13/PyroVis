#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord_uv;
layout(location = 3) in vec3 normal;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform PushConstants {
    mat4 transform;
    mat4 modelMatrix;
} pc;

const vec3 dir_light = normalize(vec3(1.0, -3.0, -1.0));
const float ambient_light = 0.1;

void main()
{
    vec4 posWorld = pc.modelMatrix * vec4(position, 1.0);
    gl_Position = pc.transform * vec4(position, 1.0);

    // TODO: need to check for non-uniform scaling
    vec3 normalWorld = normalize((pc.modelMatrix * vec4(normal, 0.0)).xyz);

    float diffuse = max(dot(normalWorld, dir_light), 0.0) + ambient_light;
    
    fragColor = diffuse * color;
}