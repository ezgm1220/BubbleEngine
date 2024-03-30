// Basic Texture Shader

#type vertex
#version 450

layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    WorldPos = aPos;  
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}

#type fragment
#version 450

layout(location = 0) out vec4 color;

in vec3 WorldPos;

uniform sampler2D OriginalMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(WorldPos));
    // vec3 color = texture(OriginalMap, uv).rgb;
    vec3 color = vec3(0.1,0.8,0.1);
}