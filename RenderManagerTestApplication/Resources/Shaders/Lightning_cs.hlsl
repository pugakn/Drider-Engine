#include "Resources\\Shaders\\PBR_Math.hlsl"

cbuffer ConstantBuffer : register(b0) {
  float4 fViewportDimensions;
  float4 kEyePosition;        //XYZ: EyePosition, W: Active Lights
  float4 kLightPosition[RM_MAX_LIGHTS]; //XYZ: Light Position, W: Range
  float4 kLightColor[RM_MAX_LIGHTS];    //XYZ: Light Color, W: Intensity
  float4 threadsInfo; //X: Number of thread groups in x, Y: Number of thread groups in Y, Z: Number of Tiles in X, W: Number of Tiles in Y
};

SamplerState SS : register(s0);

Texture2D PositionDepthTex     : register(t0);
Texture2D NormalCoCTex         : register(t1);
Texture2D AlbedoMetallicTex    : register(t2);
Texture2D EmissiveRoughnessTex : register(t3);
Texture2D SSAO_SSShadowTex     : register(t4);
Texture2D SSReflectionTex      : register(t5);
Texture2D<int> LightsIndex     : register(t6);
TextureCube EnvironmentTex     : register(t7);
TextureCube IrradianceTex      : register(t8);

RWTexture2D<float4> Lightning  : register(u0);

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint  groupIndex			: SV_GroupIndex) {
  
  //Este indica en que index esta este group thread (como si fuese un array).
  const uint group = (groupID.y * threadsInfo.x) + groupID.x;

  const int2 TileGroupID = uint2(floor(dispatchID.x / ((float)RM_TILE_LIGHTS_SZ)),
                                 floor(dispatchID.y / ((float)RM_TILE_LIGHTS_SZ)));
  
  const int TileGroup = (TileGroupID.y * threadsInfo.z) + TileGroupID.x;
  
  const float2 uvScale = float2(dispatchID.x, dispatchID.y);
	
	const float2 uv = float2(dispatchID.x * rcp(fViewportDimensions.x),
                           dispatchID.y * rcp(fViewportDimensions.y));

  const float4  position     = float4(PositionDepthTex.SampleLevel(SS, uv, 0).xyz, 1.0f);
  const float   depth        = PositionDepthTex.SampleLevel(SS, uv, 0).w;
  const float   SSAO         = SSAO_SSShadowTex.SampleLevel(SS, uv, 0).r;
  const float3  diffuse      = AlbedoMetallicTex.SampleLevel(SS, uv, 0).xyz * SSAO;
  const float3  normal       = NormalCoCTex.SampleLevel(SS, uv, 0).xyz;
  const float   metallic     = AlbedoMetallicTex.SampleLevel(SS, uv, 0).w;
  const float3  emissive     = EmissiveRoughnessTex.SampleLevel(SS, uv, 0).xyz;
  const float   roughness    = EmissiveRoughnessTex.SampleLevel(SS, uv, 0).w;
  const float3  SSReflection = SSReflectionTex.SampleLevel(SS, uv, 0).xyz;
  const float3  diffusePBR   = (diffuse - (diffuse * metallic));
  //const float3  specularPBR  = lerp(float3(0.04f, 0.04f, 0.04f), diffuse, metallic) * SSAO;
  const float3  specularPBR  = lerp(float3(0.04f, 0.04f, 0.04f), SSReflection, metallic) * SSAO;
  const float   alpha        = max(0.01f, roughness * roughness);
  const float   ShadowValue  = SSAO_SSShadowTex.SampleLevel(SS, uv, 0).g;


  float3 finalColor = float3(0.0f, 0.0f, 0.0f);
  
  //////////Lightning//////////
  float3 lightPosition,
         lightColor;
  float  lightRange,
         lightIntensity,
         LightPower;

  float3 LightViewDir;

  float3 H;

  float  NdotL,
         LdotV,
         NdotH,
         VdotH,
         LdotH;

  const float3 ViewDir = normalize(kEyePosition.xyz - position.xyz);
  const float3 InvViewDir = normalize(position.xyz - kEyePosition.xyz);
  const float  NdotV = saturate(dot(normal, -ViewDir));

  float3 DiffAcc, SpecAcc;

  const int activeLights = kEyePosition.w;
  
  int actualLight;
  //static const int totalLights = LightsIndex[uint2(TileGroup, RM_MAX_LIGHTS_PER_BLOCK)]; //This doesn't work for some reason
  //const int totalLights = LightsIndex[uint2(TileGroup, 0)]; //This doesn't work for some reason
  static const int totalLights = RM_MAX_LIGHTS_PER_BLOCK;

  //Lightning[uvScale] = float4((totalLights / ((float)RM_MAX_LIGHTS_PER_BLOCK)).xxx, 1.0f); return;

  [loop]
  for (int index = 0; index < totalLights; ++index) {
    actualLight = LightsIndex[uint2(TileGroup, index + 1)];
    if (actualLight < 0) { break; }

    lightPosition  = kLightPosition[actualLight].xyz;
    lightColor     = kLightColor[actualLight].xyz;
    lightRange     = kLightPosition[actualLight].w;
    lightIntensity = kLightColor[actualLight].w;
    
    LightPower = pow(saturate(1.0f - (length(lightPosition - position.xyz) / lightRange)), 2) * lightIntensity;
    
    LightViewDir = normalize(lightPosition - position.xyz);

    H = normalize(LightViewDir + ViewDir);

    NdotL = saturate(dot(normal, LightViewDir));
    //LdotV = saturate(dot(LightViewDir, ViewDir));
    
    NdotH = saturate(dot(normal, H));
    //VdotH = saturate(dot(ViewDir, H));
    LdotH = saturate(dot(LightViewDir, H));

    DiffAcc = Diffuse_Burley(NdotL, NdotV, LdotH, roughness) * diffuse;
    SpecAcc = Specular_D(alpha, NdotH) *
              Specular_F(specularPBR * lightColor, LdotH) *
              Specular_G(alpha, LdotH);
    SpecAcc *= rcp(4.0f * cos(NdotL) * cos(NdotV));
    
    finalColor += (DiffAcc + SpecAcc) * (NdotL * LightPower);
  };
  
  //////////IBL//////////
  const float3 reflectVector = reflect(-ViewDir, normal);
  const float mipIndex = alpha * 0.8f;

  const float3 envColor = EnvironmentTex.SampleLevel(SS, reflectVector, mipIndex).xyz;
  const float3 Irradiance = IrradianceTex.SampleLevel(SS, reflectVector, 0).xyz;
  
  const float3 IBL = (specularPBR * envColor) + (diffusePBR * Irradiance);

  //////////Final//////////
  const float3 resultColor = ((finalColor + IBL) * ShadowValue) + emissive;

  Lightning[uvScale] = float4(resultColor, 1.0f);
 
  return;
}