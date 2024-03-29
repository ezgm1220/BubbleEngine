// Basic Texture Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 ViewProjection;
out vec3 WorldPos;
out vec2 Tcd;

void main()
{
    WorldPos = a_Position;
    Tcd = a_TexCoord;
    vec4 clipPos = ViewProjection* vec4(WorldPos, 1.0);
	//gl_Position = vec4(a_Position, 1.0);
	gl_Position = clipPos;
}

#type fragment
#version 450

layout(location = 0) out vec4 color;

in vec3 WorldPos;
in vec2 Tcd;

uniform sampler2D RenderMap;
uniform samplerCube Skybox;

void main()
{
	//color = texture(Skybox, WorldPos);
	color = texture(RenderMap, Tcd);
	//color = vec4(0.12,0.3,0.4,1.0);
}