// Basic Texture Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat3 NormalMatrix;
uniform mat4 ViewProjection;
uniform mat4 Model;
uniform vec4 BaseColor;
uniform int EntityID;

out vec4 v_Color;
out vec2 v_TexCoord;
out vec4 v_WorldPos;
out vec3 v_Normal;
out flat int v_EntityID;

void main()
{
	v_Color = BaseColor;
	v_TexCoord = a_TexCoord;
    v_WorldPos =  Model * vec4(a_Position, 1.0);
    //v_WorldPos =  Model * vec4(a_Position, 1.0);
    v_Normal = NormalMatrix * a_Normal;
	v_EntityID = EntityID;
	gl_Position = ViewProjection * v_WorldPos;
}

#type fragment
#version 450

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 out_Position;
layout(location = 2) out vec4 out_Normal;
layout(location = 3) out vec4 out_MRA;
layout(location = 4) out int out_ID;

in vec4 v_Color;
in vec2 v_TexCoord;
in vec4 v_WorldPos;
in vec3 v_Normal;
in flat int v_EntityID;

uniform sampler2D Albedo;
uniform sampler2D Normal;
uniform sampler2D Metallic;
uniform sampler2D Roughness;
uniform sampler2D AO;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(Normal, v_TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(v_WorldPos.xyz);
    vec3 Q2  = dFdy(v_WorldPos.xyz);
    vec2 st1 = dFdx(v_TexCoord);
    vec2 st2 = dFdy(v_TexCoord);

    vec3 N   = normalize(v_Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    out_Color = v_Color * texture(Albedo, v_TexCoord);
    out_Position = vec4(v_WorldPos.rgb,1.0);
    out_Normal = vec4(getNormalFromMap(),1.0);
    out_MRA = vec4(texture(Metallic, v_TexCoord).r,texture(Roughness, v_TexCoord).r,texture(AO, v_TexCoord).r,1.0);
	out_ID=v_EntityID;
}