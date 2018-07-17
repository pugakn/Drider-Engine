//#define INTERVAL_BASED_SELECTION
#define MAP_BASED_SELECTION
//#define DR_SH_PCF_ENABLED
#define CASCADE_BLUR

#include "PBR_Math.hlsl"

SamplerState SS;

Texture2D PositionDepthTex     : register(t0);
Texture2D NormalCoCTex         : register(t1);
Texture2D AlbedoMetallicTex    : register(t2);
Texture2D EmissiveRoughnessTex : register(t3);
Texture2D SSAOTex              : register(t4);
Texture2D ShadowTex            : register(t5);
TextureCube EnvironmentTex     : register(t6);
TextureCube IrradianceTex      : register(t7);

cbuffer ConstantBuffer {
  float4   kEyePosition;        //XYZ: EyePosition, W: Active Lights
  float4   kLightPosition[128]; //XYZ: Light Position, W: Range
  float4   kLightColor[128];    //XYZ: Light Color, W: Intensity
  float4   BloomThresholdLuminiscenceDelta;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

struct PS_OUTPUT {
	float4 Lightning  : SV_TARGET0;
	float4 Brightness : SV_TARGET1;
};

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

PS_OUTPUT
FS(PS_INPUT input) {
  PS_OUTPUT psOut;

  const float2 uv = input.Texcoord;
  
  const float4  position    = float4(PositionDepthTex.Sample(SS, uv).xyz, 1.0f);
  const float   depth       = PositionDepthTex.Sample(SS, uv).w;
  const float   SSAO        = SSAOTex.Sample(SS, uv).x;
  const float3  diffuse     = AlbedoMetallicTex.Sample(SS, uv).xyz * SSAO;
  const float3  normal      = NormalCoCTex.Sample(SS, uv).xyz;
  const float   metallic    = AlbedoMetallicTex.Sample(SS, uv).w;
  const float3  emissive    = EmissiveRoughnessTex.Sample(SS, uv).xyz;
  const float   roughness   = EmissiveRoughnessTex.Sample(SS, uv).w;
  const float3  diffusePBR  = (diffuse - (diffuse * metallic));
  const float3  specularPBR = lerp(float3(0.04f, 0.04f, 0.04f), diffuse, metallic) * SSAO;
  const float   alpha       = max(0.01f, roughness * roughness);
  const float   ShadowValue = ShadowTex.Sample(SS, uv).x;

  float3 finalColor = float3(0.0f, 0.0f, 0.0f);

  //psOut.Lightning = float4(ShadowValue.xxx, 1.0f);
  //psOut.Lightning = float4(diffuse * ShadowValue, 1.0f);
  //psOut.Brightness = float4(brightness(psOut.Lightning.xyz), 1.0f);
  //return psOut;
  
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
  const float3 Irradiance = IrradianceTex.Sample(SS, reflectVector).xyz;
  
  const float3 IBL = (specularPBR * envColor) + (diffusePBR * Irradiance);
  
  psOut.Lightning = float4(((finalColor + IBL) * ShadowValue) + emissive, 1.0f);
  psOut.Brightness = float4(brightness(psOut.Lightning.xyz), 1.0f);

  //psOut.Lightning = position;
  //psOut.Lightning = float4(normal, 1.0f);
  //psOut.Lightning = float4(diffuse, 1.0f);
  //psOut.Lightning = float4(metallic.rrr, 1.0f);
  //psOut.Lightning = float4(emissive, 1.0f);
  //psOut.Lightning = float4(roughness.rrr, 1.0f);
  //psOut.Lightning = float4(SSAO.rrr, 1.0f);
  //psOut.Lightning = float4(ShadowValue.xxx, 1.0f);
  //if (iCurrentCascadeIndex == 0) psOut.Lightning = float4(1, 0, 0, 1);
  //if (iCurrentCascadeIndex == 1) psOut.Lightning = float4(0, 1, 0, 1);
  //if (iCurrentCascadeIndex == 2) psOut.Lightning = float4(0, 0, 1, 1);
  //if (iCurrentCascadeIndex == 3) psOut.Lightning = float4(1, 1, 1, 1);
 
  return psOut;
}