#include "PBR_Math.hlsl"

cbuffer ConstantBuffer : register(b0) {
  float4  fViewportDimensions;
  float4  kEyePosition;        //XYZ: EyePosition, W: Active Lights
  float4  kLightPosition[128]; //XYZ: Light Position, W: Range
  float4  kLightColor[128];    //XYZ: Light Color, W: Intensity
  float4  BloomThresholdLuminiscenceDelta;
};

SamplerState SS : register(s0);

Texture2D PositionDepthTex     : register(t0);
Texture2D NormalCoCTex         : register(t1);
Texture2D AlbedoMetallicTex    : register(t2);
Texture2D EmissiveRoughnessTex : register(t3);
Texture2D SSAOTex              : register(t4);
Texture2D ShadowTex            : register(t5);
TextureCube EnvironmentTex     : register(t6);
TextureCube IrradianceTex      : register(t7);

RWTexture2D<float4> Lightning  : register(u0);
RWTexture2D<float4> Brightness : register(u1);

float
luminescence(float3 Color) {
  static const float delta = BloomThresholdLuminiscenceDelta.w;
  static const float3 LuminanceFactor = float3(0.3f, 0.59f, 0.03f);
  //Case LDR
  return (dot(Color, LuminanceFactor) + delta);
  //Case HDR
  return log(dot(Color, LuminanceFactor) + delta);
}

float3
brightness(float3 LinearColor) {
  static const float3 BloomThreshold = BloomThresholdLuminiscenceDelta.xyz;
  return luminescence(LinearColor) * saturate(LinearColor - BloomThreshold.xxx);
}

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint  groupIndex			: SV_GroupIndex) {
  
  const float2 uvScale = float2(dispatchID.x, dispatchID.y);
	
	const float2 uv = float2(dispatchID.x / fViewportDimensions.x,
                           dispatchID.y / fViewportDimensions.y);
  
  const float4  position    = float4(PositionDepthTex.SampleLevel(SS, uv, 0).xyz, 1.0f);
  const float   SSAO        = SSAOTex.SampleLevel(SS, uv, 0).x;
  const float3  diffuse     = AlbedoMetallicTex.SampleLevel(SS, uv, 0).xyz * SSAO;
  const float3  normal      = NormalCoCTex.SampleLevel(SS, uv, 0).xyz;
  const float   metallic    = AlbedoMetallicTex.SampleLevel(SS, uv, 0).w;
  const float3  emissive    = EmissiveRoughnessTex.SampleLevel(SS, uv, 0).xyz;
  const float   roughness   = EmissiveRoughnessTex.SampleLevel(SS, uv, 0).w;
  const float3  diffusePBR  = (diffuse - (diffuse * metallic));
  const float3  specularPBR = lerp(float3(0.04f, 0.04f, 0.04f), diffuse, metallic) * SSAO;
  const float   alpha       = max(0.01f, roughness * roughness);
  const float   ShadowValue = ShadowTex.SampleLevel(SS, uv, 0).x;

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
  const float  NdotV = saturate(dot(normal, ViewDir));

  float3 DiffAcc, SpecAcc;

  const int activeLights = kEyePosition.w;
  
  [loop]
  for (int index = 0; index < activeLights; index += 2) {
    lightPosition  = kLightPosition[index].xyz;
    lightColor     = kLightColor[index].xyz;
    lightRange     = kLightPosition[index].w;
    lightIntensity = kLightColor[index].w;
    
    LightPower = saturate(1.0f - (length(lightPosition - position.xyz) / lightRange)) * lightIntensity;
    
    LightViewDir = normalize(lightPosition - position.xyz);

    H = normalize(LightViewDir + ViewDir);

    NdotL = saturate(dot(normal, LightViewDir));
    LdotV = saturate(dot(LightViewDir, ViewDir));
    
    NdotH = saturate(dot(normal, H));
    VdotH = saturate(dot(ViewDir, H));
    LdotH = saturate(dot(LightViewDir, H));

    DiffAcc = Diffuse_Burley(NdotL, NdotV, LdotH, roughness) * diffuse;
    SpecAcc = Specular_D(alpha, NdotH) *
              Specular_F(specularPBR * lightColor, LdotH) *
              Specular_G(alpha, LdotH);
    SpecAcc /= (4.0f * cos(NdotL) * cos(NdotV));
    
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
  //Lightning[uvScale] = float4(1.0f, 0.0f, 0.0f, 1.0f);

  //Brightness[uvScale] = float4(brightness(resultColor), 1.0f);
  //Brightness[uvScale] = float4(0.0f, 0.0f, 0.5f, 1.0f);
  Brightness[uvScale] = Lightning[uvScale];
 
  return;
}