#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord_uv;
layout(location = 3) in vec3 normal;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform Ubo {
    mat4 projectionViewMatrix;
    vec3 lightDirection;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
} pc;


const float ambient_light = 0.1;

void main()
{
    gl_Position = ubo.projectionViewMatrix * pc.modelMatrix * vec4(position, 1.0);

    // TODO: need to check for non-uniform scaling
    vec3 normalWorld = normalize((pc.modelMatrix * vec4(normal, 0.0)).xyz);

    float diffuse = max(dot(normalWorld, ubo.lightDirection), 0.0) + ambient_light;
    
    fragColor = diffuse * color;
}