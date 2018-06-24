//#define INTERVAL_BASED_SELECTION
#define MAP_BASED_SELECTION
//#define DR_SH_PCF_ENABLED
#define CASCADE_BLUR

#include "PBR_Math.hlsl"

Texture2D PositionDepthTex     : register(t0);
Texture2D NormalCoCTex         : register(t1);
Texture2D AlbedoMetallicTex    : register(t2);
Texture2D EmissiveRoughnessTex : register(t3);
Texture2D SSAOTex              : register(t4);
Texture2D ShadowTex            : register(t5);
TextureCube EnvironmentTex     : register(t6);
TextureCube IrradianceTex      : register(t7);

SamplerState SS;

cbuffer ConstantBuffer {
  float4   kEyePosition;        //XYZ: EyePosition, W: Active Lights
  float4   kLightPosition[128]; //XYZ: Light Position, W: Range
  float4   kLightColor[128];    //XYZ: Light Color, W: Intensity
  float4x4 kShadowVP[4];
  float4   ShadowSplitDepth;
  float4   ShadowSizesProportion;
  float4   ShadowInfo; //X: Activated cascades, Y: TextureSize, Z: CascadeLerp
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

bool
insideBounds(float4 fromLightPos) {
  return (fromLightPos.x > -1.0f) *
         (fromLightPos.x <  1.0f) *
	       (fromLightPos.y > -1.0f) *
         (fromLightPos.y <  1.0f);
}

float
GetShadowValue(float4 fromLightPos, const int camIndex) {
  float shadowValue = 1.0f;

  const float CascadeBiasModifier = ShadowSizesProportion[camIndex] - (camIndex * 1.5f); //Dunno LOL
  
  float2 uv = 0.5f + (0.5f * fromLightPos.xy);
  uv.y = 1.0 - uv.y;

  const float depthValue = fromLightPos.z;

  #ifdef DR_SH_PCF_ENABLED
  static const float shadowBias = 0.0005f;

  const float texelSize = 1.0f / ShadowInfo[1];
  const float sampleRadius = 3.0f;
  const float modifier = 0.25f / (sampleRadius * sampleRadius * 2.0f);

  //[unroll]
  for (float y = -sampleRadius; y <= sampleRadius; y += 1.0f) {
    //[unroll]
    for (float x = -sampleRadius; x <= sampleRadius; x += 1.0f) {
      //Projected depth
      float depthSample = ShadowTex.Sample(SS, uv + (texelSize * float2(x, y)))[camIndex];
                                            //Dunno LOL
      shadowValue -= (modifier + (camIndex * 0.0004f)) * (depthValue > (depthSample + (shadowBias * CascadeBiasModifier)));
    }
  }
  
  #else //DR_SH_PCF_ENABLED
  static const float shadowBias = 0.00005f;
  float depthSample = ShadowTex.Sample(SS, uv)[camIndex];
  shadowValue -= (0.25f * (depthValue > (depthSample + (shadowBias * CascadeBiasModifier))));
  #endif //DR_SH_PCF_ENABLED

  return shadowValue;
}

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
  
  const float4 position    = float4(PositionDepthTex.Sample(SS, uv).xyz, 1.0f);
  const float  depth       = PositionDepthTex.Sample(SS, uv).w;
  const float  SSAO        = SSAOTex.Sample(SS, uv).x;
  const float3 diffuse     = AlbedoMetallicTex.Sample(SS, uv).xyz * SSAO;
  const float3 normal      = NormalCoCTex.Sample(SS, uv).xyz;
  const float  metallic    = AlbedoMetallicTex.Sample(SS, uv).w;
  const float3 emissive    = EmissiveRoughnessTex.Sample(SS, uv).xyz;
  const float  roughness   = EmissiveRoughnessTex.Sample(SS, uv).w;
  const float3 diffusePBR  = (diffuse - (diffuse * metallic));
  const float3 specularPBR = lerp(float3(0.04f, 0.04f, 0.04f), diffuse, metallic) * SSAO;
  const float  alpha       = max(0.01f, roughness * roughness);

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
  const float3 Irradiance = IrradianceTex.Sample(SS, reflectVector).xyz;
  
  const float3 IBL = (specularPBR * envColor) + (diffusePBR * Irradiance);
  
  //////////Shadow//////////
  float ShadowValue = 1.0f;
  static const float CascadeLerp = ShadowInfo[2];
  
  int iCurrentCascadeIndex = 0;
  
  #if defined(INTERVAL_BASED_SELECTION)
    float4 fComparison;
  
    fComparison[0] = depth > ShadowSplitDepth[0];
    fComparison[1] = depth > ShadowSplitDepth[1];
    fComparison[2] = depth > ShadowSplitDepth[2];
    fComparison[3] = depth > ShadowSplitDepth[3];

    static const int activatedCacades = ShadowInfo[0];
    float fIndex = dot(float4(activatedCacades > 0,
                              activatedCacades > 1,
                              activatedCacades > 2,
                              activatedCacades > 3),
                       fComparison);
    iCurrentCascadeIndex = (int)min(fIndex, activatedCacades);

    #ifdef CASCADE_BLUR
      const float pxProportion = depth / ShadowSplitDepth[iCurrentCascadeIndex];
      const float ShadowLerp = saturate((pxProportion - CascadeLerp) / (1.0f - CascadeLerp));
    #endif //CASCADE_BLUR
  #elif defined(MAP_BASED_SELECTION)
    float4 fComparison;
    
    static const int activatedCacades = ShadowInfo[0];
    fComparison[0] = insideBounds(mul(kShadowVP[0], position)) * 4 * (0 < activatedCacades);
    fComparison[1] = insideBounds(mul(kShadowVP[1], position)) * 3 * (1 < activatedCacades);
    fComparison[2] = insideBounds(mul(kShadowVP[2], position)) * 2 * (2 < activatedCacades);
    fComparison[3] = insideBounds(mul(kShadowVP[3], position)) * 1 * (3 < activatedCacades);

    iCurrentCascadeIndex = fComparison[0];
    iCurrentCascadeIndex = max(fComparison[1], iCurrentCascadeIndex);
    iCurrentCascadeIndex = max(fComparison[2], iCurrentCascadeIndex);
    iCurrentCascadeIndex = max(fComparison[3], iCurrentCascadeIndex);

    iCurrentCascadeIndex = min(4 - iCurrentCascadeIndex, 3);

    #ifdef CASCADE_BLUR
      const float2 cascadeUV = abs(mul(kShadowVP[iCurrentCascadeIndex], position).xy);
      const float ShadowLerp = saturate((max(cascadeUV.x, cascadeUV.y) - CascadeLerp) / (1.0f - CascadeLerp));
    #endif //CASCADE_BLUR
  #endif //INTERVAL_BASED_SELECTION || MAP_BASED_SELECTION

  //Projects the position from the mainCam to what shadowCam sees
  const float4 fromMinLightPos = mul(kShadowVP[iCurrentCascadeIndex], position);

  #if (defined(INTERVAL_BASED_SELECTION) || defined(MAP_BASED_SELECTION))
    #ifdef CASCADE_BLUR
      const float4 fromMaxLightPos = mul(kShadowVP[min(iCurrentCascadeIndex + 1, 3)], position);
      ShadowValue = lerp(GetShadowValue(fromMinLightPos, iCurrentCascadeIndex),
                         GetShadowValue(fromMaxLightPos, min(iCurrentCascadeIndex + 1, 3)),
                         ShadowLerp);
    #else
      ShadowValue = GetShadowValue(fromMinLightPos, iCurrentCascadeIndex);
    #endif //CASCADE_BLUR
  #endif //INTERVAL_BASED_SELECTION || MAP_BASED_SELECTION
  
  psOut.Lightning = float4(((finalColor + IBL) * ShadowValue) + emissive, 1.0f);
  psOut.Brightness = float4(brightness(psOut.Lightning.xyz), 1.0f);
  
  //psOut.Lightning = position;
  //psOut.Lightning = float4(depthCam.xxx, 1.0f);
  //psOut.Lightning = float4(normal, 1.0f);
  //psOut.Lightning = float4(diffuse, 1.0f);
  //psOut.Lightning = float4(metallic.rrr, 1.0f);
  //psOut.Lightning = float4(emissive, 1.0f);
  //psOut.Lightning = float4(roughness.rrr, 1.0f);
  //psOut.Lightning = float4(specular, 1.0f);
  //psOut.Lightning = float4(SSAO.rrr, 1.0f);
  //psOut.Lightning = float4(ShadowValue.xxx, 1.0f);
  //psOut.Lightning = float4(ShadowTex.Sample(SS, uv).xxx, 1.0f);
  //psOut.Lightning = float4(ShadowTex.Sample(SS, uv).yyy, 1.0f);
  //psOut.Lightning = float4(ShadowTex.Sample(SS, uv).zzz, 1.0f);
  //psOut.Lightning = float4(ShadowTex.Sample(SS, uv).www, 1.0f);
  //if (iCurrentCascadeIndex == 0) psOut.Lightning = float4(1, 0, 0, 1);
  //if (iCurrentCascadeIndex == 1) psOut.Lightning = float4(0, 1, 0, 1);
  //if (iCurrentCascadeIndex == 2) psOut.Lightning = float4(0, 0, 1, 1);
  //if (iCurrentCascadeIndex == 3) psOut.Lightning = float4(1, 1, 1, 1);
 
  return psOut;
}