#define PI 3.14152265f

#include "Resources\\Shaders\\PBR_Math.hlsl"

cbuffer ConstantBuffer : register(b0) {
  float4 fViewportSzEnvIrr; //XY: Vierport dimension, Z: Enviroment lightning scale, W: Irradiance lightning scale
  float4 kEyePosition; //XYZ: EyePosition, W: Active Directional Lights
  float4 kPointLightPosition[RM_MAX_POINT_LIGHTS]; //XYZ: Light Position, W: Range
  float4 kPointLightColor[RM_MAX_POINT_LIGHTS]; //XYZ: Light Color, W: Intensity
  float4 kDirectionalLightDirection[RM_MAX_DIRECTIONAL_LIGHTS]; //XYZ: Light Direction, W: Shadow
  float4 kDirectionalLightColor[RM_MAX_DIRECTIONAL_LIGHTS]; //XYZ: Light Color, W: Intensity
  float4 threadsInfo; //X: Number of thread groups in x, Y: Number of thread groups in Y, Z: Number of Tiles in X, W: Number of Tiles in Y
  float4x4 SkyboxRotation;
};

SamplerState textureSS : register(s0);
SamplerState skyboxSS : register(s1);

Texture2D PositionDepthTex     : register(t0);
Texture2D NormalCoCTex         : register(t1);
Texture2D AlbedoMetallicTex    : register(t2);
Texture2D EmissiveRoughnessTex : register(t3);
Texture2D SSAO_SSShadowTex     : register(t4);
Texture2D SSReflectionTex      : register(t5);
Texture2D<int> LightsIndex     : register(t6);

Texture2D EnvironmentTex : register(t7);
Texture2D IrradianceTex  : register(t8);

RWTexture2D<float4> Lightning  : register(u0);


float3
SkyboxColor(Texture2D panoramicTexture, float3 direction, int mipLevel) {
  direction = normalize(direction);
  direction = normalize(mul(SkyboxRotation, float4(direction, 1.0f)).xyz);

  float2 longlat = float2(atan2(direction.x , direction.z), acos(direction.y));
  
  float2 uv = longlat / float2(2.0f * PI, PI);
  uv.x += 0.5f;
  
  return panoramicTexture.SampleLevel(skyboxSS, uv, mipLevel).xyz;
}

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint  groupIndex			: SV_GroupIndex) {
  
  const uint group = (groupID.y * threadsInfo.x) + groupID.x;

  const int2 TileGroupID = uint2(floor(dispatchID.x / ((float)RM_TILE_POINT_LIGHTS_SZ)),
                                 floor(dispatchID.y / ((float)RM_TILE_POINT_LIGHTS_SZ)));
  
  const int TileGroup = (TileGroupID.y * threadsInfo.z) + TileGroupID.x;
  
  const float2 uvScale = float2(dispatchID.x, dispatchID.y);
	
	const float2 uv = float2(dispatchID.x * rcp(fViewportSzEnvIrr.x),
                           dispatchID.y * rcp(fViewportSzEnvIrr.y));

  const float4  position        = float4(PositionDepthTex.SampleLevel(textureSS, uv, 0).xyz, 1.0f);
  const float   depth           = PositionDepthTex.SampleLevel(textureSS, uv, 0).w;
  const float   SSAO            = SSAO_SSShadowTex.SampleLevel(textureSS, uv, 0).r;
  const float3  diffuse         = AlbedoMetallicTex.SampleLevel(textureSS, uv, 0).xyz * SSAO;
  const float3  normal          = NormalCoCTex.SampleLevel(textureSS, uv, 0).xyz;
  const float   metallic        = AlbedoMetallicTex.SampleLevel(textureSS, uv, 0).w;
  const float3  emissive        = EmissiveRoughnessTex.SampleLevel(textureSS, uv, 0).xyz;
  const float   roughness       = EmissiveRoughnessTex.SampleLevel(textureSS, uv, 0).w;
  const float   roughnessAlpha  = max(0.01f, roughness * roughness);
  const float3  SSReflection    = SSReflectionTex.SampleLevel(textureSS, uv, 0).xyz;
  const float   SSRefProport    = SSReflectionTex.SampleLevel(textureSS, uv, 0).w;
  const float3  diffusekd       = (diffuse - (diffuse * metallic));
  const float3  specularF0      = lerp((0.04f).xxx, diffuse, metallic);
  const float   shadowValue     = SSAO_SSShadowTex.SampleLevel(textureSS, uv, 0).g;
  
  //////////Lightning//////////
  const float3 ViewDir = normalize(kEyePosition.xyz - position.xyz);
  const float  NdotV = saturate(dot(normal, ViewDir));

  float3 lightPosition,
         LightDirection,
         lightColor;
  float  lightRange,
         lightIntensity;

  float3 H;

  float NdotL,
        LdotV,
        NdotH,
        VdotH,
        LdotH;

  float3 diffAcc,
         specAcc;
  
  //static const int totalLights = LightsIndex[uint2(TileGroup, RM_MAX_POINT_LIGHTS_PER_BLOCK)]; //This doesn't work for some reason
  //const int totalLights = LightsIndex[uint2(TileGroup, 0)]; //This doesn't work for some reason
  static const int totalLights = RM_MAX_POINT_LIGHTS_PER_BLOCK;

  //Lightning[uvScale] = float4((totalLights / ((float)RM_MAX_POINT_LIGHTS_PER_BLOCK)).xxx, 1.0f); return;

  ////////////////////////////////////
  //////////Direct lightning//////////
  ////////////////////////////////////
  float3 directLighting = (0.0).xxx;

  float3 F, D, G;
  float3 kd;

  /////Point/////
  int actualLight;
  [loop]
  for (int PLIndex = 0; PLIndex < totalLights; ++PLIndex) {
    actualLight = LightsIndex[uint2(TileGroup, PLIndex + 1)];
    if (actualLight < 0) { break; }

    lightPosition  = kPointLightPosition[actualLight].xyz;
    lightColor     = kPointLightColor[actualLight].xyz;
    lightRange     = kPointLightPosition[actualLight].w;
    lightIntensity = kPointLightColor[actualLight].w;
    
    lightIntensity *= pow(saturate(1.0f - (length(lightPosition - position.xyz) * rcp(lightRange))), 2);
    
    LightDirection = normalize(lightPosition - position.xyz);

    H = normalize(LightDirection + ViewDir);

    NdotL = saturate(dot(normal, LightDirection));
    LdotV = saturate(dot(LightDirection, ViewDir));
    
    NdotH = saturate(dot(normal, H));
    VdotH = saturate(dot(ViewDir, H));
    LdotH = saturate(dot(LightDirection, H));

    //F = Specular_F(specularF0 * lightColor, LdotH); //Original
    //F = Specular_F(specularF0, LdotH); //Cool
    F = Specular_F(specularF0, VdotH); //Real
    D = Specular_D(roughnessAlpha, NdotH);
    G = Specular_G(roughnessAlpha, LdotH);

    kd = lerp((1.0f).xxx - F, (0.0f).xxx, metallic);

    diffAcc = kd * diffuse;
    specAcc = (F * D * G) * rcp(max(EPSILON, 4.0f * cos(NdotL) * cos(NdotV)));
    
		directLighting += (diffAcc + specAcc) * lightColor * NdotL * lightIntensity;
  };
  
  /////Directional/////
  const int activeDLights = kEyePosition.w;
  [loop]
  for (int DLIndex = 0; DLIndex < activeDLights; ++DLIndex) {
    LightDirection    = kDirectionalLightDirection[DLIndex].xyz;
    lightColor      = kDirectionalLightColor[DLIndex].xyz;
    lightIntensity  = kDirectionalLightColor[DLIndex].w;

    H = normalize(LightDirection + ViewDir);

    NdotL = saturate(dot(normal, LightDirection));
    LdotV = saturate(dot(LightDirection, ViewDir));
    
    NdotH = saturate(dot(normal, H));
    VdotH = saturate(dot(ViewDir, H));
    LdotH = saturate(dot(LightDirection, H));

    //F = Specular_F(specularF0 * lightColor, LdotH); //Original
    //F = Specular_F(specularF0, LdotH); //Cool
    F = Specular_F(specularF0, VdotH); //Real
    D = Specular_D(roughnessAlpha, NdotH);
    G = Specular_G(roughnessAlpha, LdotH);

    kd = lerp((1.0f).xxx - F, (0.0f).xxx, metallic);

    diffAcc = kd * diffuse;
    specAcc = (F * D * G) * rcp(max(EPSILON, 4.0f * cos(NdotL) * cos(NdotV)));
    
		directLighting += (diffAcc + specAcc) * lightColor * NdotL * lightIntensity;
  };

  //////////Image Based Lightning (IBL)//////////
  float3 ambientLighting;
  {
    //Diffuse
    const float irrScale = fViewportSzEnvIrr.w;
    //const float3 irradiance = IrradianceTex.SampleLevel(textureSS, normal, 0).xyz * irrScale;
    const float3 irradiance = SkyboxColor(IrradianceTex, normal, 0) * irrScale;

    F = Specular_F(specularF0, NdotV);
    
		kd = lerp((1.0f).xxx - F, 0.0f.xxx, metallic);
    
		float3 diffuseIBL = kd * diffuse * irradiance;
    
    //Specular
    const float mipIndex = roughnessAlpha * 0.8f;
    const float3 reflectVector = reflect(-ViewDir, normal);
    
    const float EnvScale = fViewportSzEnvIrr.z;
    //float3 envColor = EnvironmentTex.SampleLevel(textureSS, reflectVector, mipIndex).xyz;
    float3 envColor = SkyboxColor(EnvironmentTex, reflectVector, mipIndex);
    //envColor = lerp(envColor, SSReflection, SSRefProport);
    envColor *= EnvScale;
    
		float3 specularIBL = specularF0 * envColor;

    ambientLighting = diffuseIBL + specularIBL;
  }

  //////////Final//////////
  Lightning[uvScale] = float4(((directLighting + ambientLighting) * shadowValue) + emissive, 1.0f);
 
  return;
}