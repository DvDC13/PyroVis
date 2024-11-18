#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord_uv;
layout(location = 3) in vec3 normal;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform Ubo {
    mat4 projectionViewMatrix;
    vec4 ambiantLightColor;
    vec3 lightPosition;
    vec4 lightColor;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
} pc;

void main()
{
    vec4 positionWorld = pc.modelMatrix * vec4(position, 1.0);
    gl_Position = ubo.projectionViewMatrix * positionWorld;

    // TODO: need to check for non-uniform scaling
    vec3 normalWorld = normalize((pc.modelMatrix * vec4(normal, 0.0)).xyz);

    vec3 directionToLight = ubo.lightPosition - positionWorld.xyz;
    float attenuation = 1.0 / dot(directionToLight, directionToLight);
    vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
    vec3 ambient_light = ubo.ambiantLightColor.xyz * ubo.ambiantLightColor.w;
    vec3 diffuse = max(dot(normalWorld, normalize(directionToLight)), 0.0) * lightColor;
    
    fragColor = color * (diffuse + ambient_light);
}