// Basic Texture Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 projection;
uniform mat4 view;

out vec3 WorldPos;
out vec2 Tcd;

void main()
{
    WorldPos = a_Position;
    Tcd = a_TexCoord;

    mat4 rotView = mat4(mat3(view));
	vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);

	//gl_Position = vec4(a_Position, 1.0);
	gl_Position = clipPos.xyww;
}

#type fragment
#version 450

layout(location = 0) out vec4 outcolor;

in vec3 WorldPos;
in vec2 Tcd;

uniform sampler2D RenderMap;
uniform samplerCube Skybox;

void main()
{
    vec3 envColor = textureLod(Skybox, WorldPos, 0.0).rgb;
    outcolor = vec4(envColor,1.0);
	//color = texture(Skybox, WorldPos);
	//color = texture(RenderMap, Tcd);
	//color = vec4(1.0,0.0,0.0,1.0);
}