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

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D Tex2D;

void main()
{
	color = texture(Tex2D,v_TexCoord);
}