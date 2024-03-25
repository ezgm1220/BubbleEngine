// Basic Texture Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out flat int v_EntityID;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450

layout(location = 0) out vec4 color;
layout(location = 1) out ivec4 color2;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat int v_EntityID;

uniform sampler2D Albedo;
uniform sampler2D Normal;
uniform sampler2D Metallic;
uniform sampler2D Roughness;
uniform sampler2D AO;

void main()
{
	vec4 texColor = v_Color;

    color = v_Color*texture(Albedo, v_TexCoord);
    color *= texture(Normal, v_TexCoord);
    color *= texture(Metallic, v_TexCoord);
    color *= texture(Roughness, v_TexCoord);
    color *= texture(AO, v_TexCoord);

	color2=ivec4(v_EntityID,2,3,4);
}