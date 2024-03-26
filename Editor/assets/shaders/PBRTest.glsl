// Basic Texture Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 Transform;
uniform vec4 BaseColor;
uniform int EntityID;

out vec4 v_Color;
out vec2 v_TexCoord;
out vec4 v_Position;
out flat int v_EntityID;

void main()
{
	v_Color = BaseColor;
	v_TexCoord = a_TexCoord;
	v_EntityID = EntityID;
    v_Position = u_ViewProjection * Transform * vec4(a_Position, 1.0);
	gl_Position = v_Position;
}

#type fragment
#version 450

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 Position;
layout(location = 2) out vec4 NormMeatlic;
layout(location = 3) out ivec4 RouAOID;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat int v_EntityID;
in vec4 v_Position;

uniform sampler2D Albedo;
uniform sampler2D Normal;
uniform sampler2D Metallic;
uniform sampler2D Roughness;
uniform sampler2D AO;

void main()
{
	vec4 texColor = v_Color;

    color = v_Color*texture(Albedo, v_TexCoord);
    Position = v_Position;
    NormMeatlic = vec4(texture(Normal, v_TexCoord).rgb,texture(Metallic, v_TexCoord).r);

    float roughness = texture(Roughness, v_TexCoord).r;
    float ao = texture(AO, v_TexCoord).r;
    roughness*=100000000;
    ao *= 100000000;

	RouAOID=ivec4(v_EntityID,roughness,ao,-1);
}