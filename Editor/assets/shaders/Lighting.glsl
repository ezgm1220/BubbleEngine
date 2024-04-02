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

// PBR
uniform sampler2D ColorMap;
uniform sampler2D PositionMap;
uniform sampler2D NormalMap;
uniform sampler2D MRAMap;

// IBL
uniform samplerCube IrradianceMap;
uniform samplerCube PrefilterMap;
uniform sampler2D BrdfLUT;

uniform vec3 CameraPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   
// ----------------------------------------------------------------------------

void main()
{
    vec3 Albedo = texture(ColorMap,v_TexCoord).rgb;
    vec3 WorldPos = texture(PositionMap,v_TexCoord).rgb;
    vec3 Normal = texture(NormalMap,v_TexCoord).rgb;
    float Metallic = texture(MRAMap,v_TexCoord).r;
    float Roughness = texture(MRAMap,v_TexCoord).g;
    float AO = texture(MRAMap,v_TexCoord).b;

    // input lighting data
    vec3 N = normalize(Normal);
    vec3 V = normalize(CameraPos - WorldPos);
    vec3 R = reflect(-V, N); 

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, Albedo, Metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    // Lighting
    {

    }

    // 环境光照:

    // 这里面的F是为将kd提出时简化过的
    // float NoV = clamp(dot(N, V), 0.0, 1.0);
    // vec3 F = fresnelSchlickRoughness(F0, NoV,Roughness);
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;

    vec3 irradiance = texture(IrradianceMap, N).rgb;
    vec3 diffuse = irradiance * Albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    // vec3 prefilteredColor = textureLod(PrefilterMap, R,  Roughness * MAX_REFLECTION_LOD).rgb;     
    vec3 prefilteredColor = textureLod(PrefilterMap, WorldPos,  0).rgb;  



    //vec3 prefilteredColor = textureLod(PrefilterMap, WorldPos,  0).rgb;   



    vec2 brdf  = texture(BrdfLUT, vec2(max(dot(N, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * AO;

    vec3 color = ambient + Lo;
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

	outcolor = vec4(prefilteredColor, 1.0);
	//outcolor = vec4(texture(MRAMap,v_TexCoord).rgb, 1.0);
	//outcolor = vec4(0.1,0.3,0.4, 1.0);
}