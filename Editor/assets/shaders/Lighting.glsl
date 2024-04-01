// Basic Texture Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 450

layout(location = 0) out vec4 outcolor;

in vec2 v_TexCoord;

uniform sampler2D Color;
uniform sampler2D Position;
uniform sampler2D NormMeatlic;
uniform sampler2D RouAOID;

void main()
{
	outcolor = texture(Color,v_TexCoord);
}