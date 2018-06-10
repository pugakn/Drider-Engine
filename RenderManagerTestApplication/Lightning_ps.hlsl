Texture2D PositionLDepthTex    : register(t0);
Texture2D NormalCoCTex         : register(t1);
Texture2D AlbedoMetallicTex    : register(t2);
Texture2D EmissiveRoughnessTex : register(t3);
Texture2D SSAOTex              : register(t4);
Texture2D ShadowTex            : register(t5);

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

static const float M_PI = 3.14159265f;
static const float EPSILON = 1e-6f;

bool
insideBounds(float4 fromLightPos) {
  return (fromLightPos.x > -1.0f) *
         (fromLightPos.x <  1.0f) *
	       (fromLightPos.y > -1.0f) *
         (fromLightPos.y <  1.0f);
}

//#define INTERVAL_BASED_SELECTION
#define MAP_BASED_SELECTION
//#define DR_SH_PCF_ENABLED
#define CASCADE_BLUR
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

// Shlick's approximation of Fresnel
// https://en.wikipedia.org/wiki/Schlick%27s_approximation
float3
Fresnel_Shlick(float3 f0, float3 f90, float x) {
    return f0 + (f90 - f0) * pow(1.0f - x, 5.0f);
};

// Burley B. "Physically Based Shading at Disney"
// SIGGRAPH 2012 Course: Practical Physically Based Shading in Film and Game Production, 2012.
float
Diffuse_Burley(float NdotL,
               float NdotV,
               float LdotH,
               float roughness) {
    float fd90 = 0.5f + 2.f * roughness * LdotH * LdotH;
    return Fresnel_Shlick((1.0f).xxx, fd90.xxx, NdotL).x * Fresnel_Shlick((1.0f).xxx, fd90.xxx, NdotV).x;
};

// GGX specular D (normal distribution)
// https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf
float
Specular_D_GGX(float alpha, float NdotH) {
     const float alpha2 = alpha * alpha;
     const float lower = (NdotH * NdotH * (alpha2 - 1.0f)) + 1.0f;
     return alpha2 / max(EPSILON, M_PI * lower * lower);
};

// Schlick-Smith specular G (visibility) with Hable's LdotH optimization
// http://www.cs.virginia.edu/~jdl/bib/appearance/analytic%20models/schlick94b.pdf
// http://graphicrants.blogspot.se/2013/08/specular-brdf-reference.html
float
G_Shlick_Smith_Hable(float alpha, float LdotH) {
    return 1.0f / lerp(LdotH * LdotH, 1.0f, alpha * alpha * 0.25f);
};

float
Specular_D(float alpha, float NdotH) {
    return Specular_D_GGX(alpha, NdotH);
};

float3
Specular_F(float3 SpecularColor, float LdotH) {
    return Fresnel_Shlick(SpecularColor, (1.0f).xxx, LdotH);
};

float
Specular_G(float alpha, float LdotH) {
    return G_Shlick_Smith_Hable(alpha, LdotH);
};

PS_OUTPUT
FS(PS_INPUT input) {
  PS_OUTPUT psOut;

  float3 finalColor = float3(0.0f, 0.0f, 0.0f);

  const float2 uv = input.Texcoord;
  
  const float4 position  = float4(PositionLDepthTex.Sample(SS, uv).xyz, 1.0f);
  const float  depthCam  = PositionLDepthTex.Sample(SS, uv).w;
  const float3 normal    = NormalCoCTex.Sample(SS, uv).xyz;
  const float3 albedo    = AlbedoMetallicTex.Sample(SS, uv).xyz;
  const float  metallic  = AlbedoMetallicTex.Sample(SS, uv).w;
  const float3 emissive  = EmissiveRoughnessTex.Sample(SS, uv).xyz;
  const float  roughness = EmissiveRoughnessTex.Sample(SS, uv).w;
  const float3 specular  = lerp(float3(0.03f, 0.03f, 0.03f), albedo, metallic);
  const float  SSAO      = SSAOTex.Sample(SS, uv).x;
  const float  alpha     = max(0.01f, roughness * roughness);
  
  //Lightning Stuff
  const float3 ViewDir = normalize(kEyePosition.xyz - position.xyz);
  const float  NdotV = saturate(dot(normal, ViewDir));

  const int activeLights = kEyePosition.w;
  //[unroll]
  for (int index = 0; index < activeLights; index += 2) {
    float3 lightPosition  = kLightPosition[index].xyz;
    float3 lightColor     = kLightColor[index].xyz;
    float  lightRange     = kLightPosition[index].w;
    float  lightIntensity = kLightColor[index].w;
    
    float  LightPower = saturate(1.0f - (length(lightPosition - position.xyz) / lightRange)) * lightIntensity;

    float3 LightDir = normalize(lightPosition - position.xyz);

    float3 H     = normalize(LightDir + ViewDir);

    float  NdotL = saturate(dot(normal, LightDir));
    float  LdotV = saturate(dot(LightDir, ViewDir));

    float  NdotH = saturate(dot(normal, H));
    float  VdotH = saturate(dot(ViewDir, H));
    float  LdotH = saturate(dot(LightDir, H));

    float3 DiffAcc = Diffuse_Burley(NdotL, NdotV, LdotH, roughness) * albedo;
    float3 SpecAcc = Specular_D(alpha, NdotH) *
                     Specular_F(specular * lightColor, LdotH) *
                     Specular_G(alpha, LdotH);
    SpecAcc /= (4.0f * cos(NdotL) * cos(NdotV));
    
    finalColor += (SSAO * NdotL * LightPower) * (DiffAcc + SpecAcc);
  };
  
  //Shadow Stuff
  int iCurrentCascadeIndex = 0;
  
  static const float CascadeLerp = ShadowInfo[2];
  
  #if defined(INTERVAL_BASED_SELECTION)
    const float vCurrentPixelDepth = PositionLDepthTex.Sample(SS, uv).w;

    float4 fComparison;
    fComparison[0] = vCurrentPixelDepth > ShadowSplitDepth[0];
    fComparison[1] = vCurrentPixelDepth > ShadowSplitDepth[1];
    fComparison[2] = vCurrentPixelDepth > ShadowSplitDepth[2];
    fComparison[3] = vCurrentPixelDepth > ShadowSplitDepth[3];

    static const int activatedCacades = ShadowInfo[0];
    float fIndex = dot(float4(activatedCacades > 0,
                              activatedCacades > 1,
                              activatedCacades > 2,
                              activatedCacades > 3),
                       fComparison);
    fIndex = min(fIndex, activatedCacades);
    iCurrentCascadeIndex = (int)fIndex;

    #ifdef CASCADE_BLUR
      const float pxProportion = vCurrentPixelDepth / ShadowSplitDepth[iCurrentCascadeIndex];
      const float ShadowLerp = saturate((pxProportion - CascadeLerp) / (1.0f - CascadeLerp));
    #endif //CASCADE_BLUR
  #elif defined(MAP_BASED_SELECTION)
    float4 fComparison;
    fComparison[0] = insideBounds(mul(kShadowVP[0], position)) * 4;
    fComparison[1] = insideBounds(mul(kShadowVP[1], position)) * 3;
    fComparison[2] = insideBounds(mul(kShadowVP[2], position)) * 2;
    fComparison[3] = insideBounds(mul(kShadowVP[3], position)) * 1;

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

  float ShadowValue = 1.0f;

  #if defined(INTERVAL_BASED_SELECTION) || defined(MAP_BASED_SELECTION)
    #ifdef CASCADE_BLUR
      const float4 fromMaxLightPos = mul(kShadowVP[min(iCurrentCascadeIndex + 1, 3)], position);
      ShadowValue = lerp(GetShadowValue(fromMinLightPos, iCurrentCascadeIndex),
                         GetShadowValue(fromMaxLightPos, min(iCurrentCascadeIndex + 1, 3)),
                         ShadowLerp);
    #else
      ShadowValue = GetShadowValue(fromMinLightPos, iCurrentCascadeIndex);
    #endif //CASCADE_BLUR
  #endif //INTERVAL_BASED_SELECTION || MAP_BASED_SELECTION
  
  psOut.Lightning = float4((finalColor * ShadowValue) + emissive, 1.0f);
  psOut.Brightness = float4(brightness(psOut.Lightning.xyz), 1.0f);

  //psOut.Lightning = position;
  //psOut.Lightning = float4(depthCam.xxx, 1.0f);
  //psOut.Lightning = float4(normal, 1.0f);
  //psOut.Lightning = float4(albedo, 1.0f);
  //psOut.Lightning = float4(metallic.rrr, 1.0f);
  //psOut.Lightning = float4(emissive, 1.0f);
  //psOut.Lightning = float4(roughness.rrr, 1.0f);
  //psOut.Lightning = float4(specular, 1.0f);
  psOut.Lightning = float4(SSAO.rrr, 1.0f);
  //psOut.Lightning = float4(ShadowValue.xxx, 1.0f);
  //psOut.Lightning = float4(ShadowTex.Sample(SS, uv).xxx, 1.0f);
  //psOut.Lightning = float4(ShadowTex.Sample(SS, uv).yyy, 1.0f);
  //psOut.Lightning = float4(ShadowTex.Sample(SS, uv).zzz, 1.0f);
  //psOut.Lightning = float4(ShadowTex.Sample(SS, uv).www, 1.0f);
 
  return psOut;
}