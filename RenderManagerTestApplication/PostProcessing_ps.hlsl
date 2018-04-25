Texture2D PositionNormalTex    : register(t0);
Texture2D AlbedoMetallicTex    : register(t1);
Texture2D EmissiveRoughnessTex : register(t2);
Texture2D SSAOTex              : register(t3);
Texture2D ShadowTex1           : register(t4);

SamplerState SS;

cbuffer ConstantBuffer {
  float4   kEyePosition;
  float4   kDirLight;
  float4   kLightPosition[128];
  float4   kLightColor[128];
  float4x4 View;
  float4x4 ViewInv;
  float4x4 Projection;
  float4x4 ProjectionInv;
  float4x4 VP;
  float4x4 VPInv;
  float4x4 kShadowVP[4];
  float4   ShadowSliptDepth;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

static const float M_PI = 3.14159265f;
static const float EPSILON = 1e-6f;

float3
getPosition(in float2 uv) {
  float x = (2.0f * uv.x) - 1.0f;
  float y = (2.0f * (1.0f - uv.y)) - 1.0;
  float z = PositionNormalTex.Sample(SS, uv).w;

  float4 posView = mul(ProjectionInv, float4(x, y, z, 1.0f));
  posView /= posView.w;
  
  return posView.xyz;
}

float3
getPosition(in float2 uv, in float z) {
  float x = (2.0f * uv.x) - 1.0f;
  float y = (2.0f * (1.0f - uv.y)) - 1.0f;

  float4 posView = mul(VPInv, float4(x, y, z, 1.0f));
  posView /= posView.w;
  
  return posView.xyz;
}

bool
insideBounds(float4 fromLightPos) {
  float2 fromLightCoords = (fromLightPos.xy * 0.5f) + 0.5f;
  return (fromLightCoords.x > 0.0f &&
          fromLightCoords.x < 1.0f &&
	        fromLightCoords.y > 0.0f &&
          fromLightCoords.y < 1.0f &&
	        fromLightPos.w > 0.0f &&
	        fromLightPos.z < 1.0f);
}

#define DR_SH_PCF_ENABLED
float
GetShadowValue(float4 fromLightPos, const int camIndex) {
  const float shadowBias = 0.0005f;
  float2 fromLightCoords = (0.5f * fromLightPos.xy) + 0.5f;

  float shadowValue = 1.0f;
  
  if (fromLightCoords.x > 0.0f &&
      fromLightCoords.x < 1.0f &&
	    fromLightCoords.y > 0.0f &&
      fromLightCoords.y < 1.0f &&
	    fromLightPos.w > 0.0f &&
	    fromLightPos.z < 1.0f) {
    fromLightCoords.y = 1.0 - fromLightCoords.y;
    //Pixel Depth seen from shadowCam
    float depthPos = fromLightPos.z;
    #ifdef DR_SH_PCF_ENABLED
    
    uint width, height; //TODO: Pass This in CBUFFER xdXDxDxDXDXD
    ShadowTex1.GetDimensions(width, height); //TODO: Pass This in CBUFFER xdXDxDxDXDXD
    float2 texelSize = float2(1.0f, 1.0f) / float2(width, height);
    const float sampleRadius = 3.0f;
    const float modifier = 0.25f / (sampleRadius * sampleRadius * 2.0f);
    [unroll]
    for (float y = -sampleRadius; y <= sampleRadius; y += 1.0f) {
      [unroll]
      for (float x = -sampleRadius; x <= sampleRadius; x += 1.0f) {
        //Projected depth
        float depthSample = ShadowTex1.Sample(SS, fromLightCoords.xy + texelSize * float2(x,y))[camIndex];
        
        if (depthPos > (depthSample + shadowBias)) {
          shadowValue -= modifier;
        }
      }
    }
    
    #else //DR_SH_PCF_ENABLED
    //Projected depth	
    float depthSample = ShadowTex1.Sample(SS, fromLightCoords.xy)[camIndex];
    
    if (depthPos > (depthSample + shadowBias)) {
      shadowValue = 0.25f;
    }
    #endif //DR_SH_PCF_ENABLED
  }
  
  return shadowValue;
}

float
luminescence(float3 Color) {
  float delta = 0.0f;
  static const float3 LuminanceFactor = float3(0.3f, 0.59f, 0.03f);
  //Case LDR
  return (dot(Color, LuminanceFactor) + delta);
  //Case HDR
  return log(dot(Color, LuminanceFactor) + delta);
}

float3
BiasX2(float3 x) {
    return 2.0f * x - 1.0f;
};

// Given a local normal, transform it into a tangent space given by surface normal and tangent
float3
PeturbNormal(float3 localNormal,
             float3 surfaceNormalWS,
             float3 surfaceTangentWS) {
    float3 normal = normalize(surfaceNormalWS);
    float3 tangent = normalize(surfaceTangentWS);
    float3 binormal = cross(normal, tangent);     // reconstructed from normal & tangent
    float3x3 TBN; // world "frame" for local normal 
    TBN[0] = tangent;
    TBN[1] = binormal;
    TBN[2] = normal;

    return mul(localNormal, TBN);                // transform to local to world (tangent space)
};

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
    return Fresnel_Shlick(float3(1, 1, 1), float3(fd90, fd90, fd90), NdotL).x * Fresnel_Shlick(float3(1, 1, 1), float3(fd90, fd90, fd90), NdotV).x;
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
    return Fresnel_Shlick(SpecularColor, float3(1.0f, 1.0f, 1.0f), LdotH);
};

float
Specular_G(float alpha, float LdotH) {
    return G_Shlick_Smith_Hable(alpha, LdotH);
};

float4 FS(PS_INPUT input) : SV_TARGET {
  float2 uv = input.Texcoord;
  
  float3 position  = getPosition(uv, PositionNormalTex.Sample(SS, uv).w);
  float3 normal    = PositionNormalTex.Sample(SS, uv).xyz;
  float3 albedo    = AlbedoMetallicTex.Sample(SS, uv).xyz;
  float  metallic  = AlbedoMetallicTex.Sample(SS, uv).w;
  float3 emissive  = EmissiveRoughnessTex.Sample(SS, uv).xyz;
  float  roughness = EmissiveRoughnessTex.Sample(SS, uv).w;
  float3 specular  = lerp(float3(0.03f, 0.03f, 0.03f), albedo, metallic);
  float  SSAO      = SSAOTex.Sample(SS, uv).x;

  float3 finalColor = float3(0.0f, 0.0f, 0.0f);

  float3 lightPosition  = float3(0.0f, 0.0f, 0.0f);
  float3 lightColor     = float3(0.0f, 0.0f, 0.0f);
  float  lightIntensity = 0.0f;
  float  LightPower = 0.0f;

  float3 ViewDir = normalize(kEyePosition.xyz - position.xyz);
  float  NdotV = saturate(dot(normal, ViewDir));

  float3 LightDir = normalize(lightPosition - position.xyz);
  float3 H;
  float  NdotL;
  float  LdotV;
  float  NdotH;
  float  VdotH;
  float  LdotH;

  float  alpha = max(0.01f, roughness * roughness);
  
  float3 DiffAcc = float3(0.0f, 0.0f, 0.0f);
  float3 SpecAcc = float3(0.0f, 0.0f, 0.0f);
  
  const int activeLights = kEyePosition.w;
  [unroll]
  for (int index = 0; index < activeLights; index += 4) {
    lightPosition  = kLightPosition[index].xyz;
    lightColor     = kLightColor[index].xyz;
    lightIntensity = kLightColor[index].w;
    
    LightPower = saturate( 1.0f - (length(lightPosition - position.xyz) / 130.0f) );

    LightDir = normalize(lightPosition - position.xyz);

    H     = normalize(LightDir + ViewDir);

    NdotL = saturate(dot(normal, LightDir));
    LdotV = saturate(dot(LightDir, ViewDir));

    NdotH = saturate(dot(normal, H));
    VdotH = saturate(dot(ViewDir, H));
    LdotH = saturate(dot(LightDir, H));

    DiffAcc = Diffuse_Burley(NdotL, NdotV, LdotH, roughness) * albedo;
    SpecAcc = Specular_D(alpha, NdotH) *
              Specular_F(specular * lightColor, LdotH) *
              Specular_G(alpha, LdotH);
    SpecAcc /= (4.0f * cos(NdotL) * cos(NdotV));
    
    finalColor += (DiffAcc + SpecAcc) * SSAO * NdotL * LightPower;
  };
  
  //return float4(position, 1.0f);
  //return float4(normal, 1.0f);
  //return float4(albedo, 1.0f);
  //return float4(metallic.rrr, 1.0f);
  //return float4(emissive, 1.0f);
  //return float4(roughness.rrr, 1.0f);
  //return float4(specular, 1.0f);
  //return float4(SSAO.rrr, 1.0f);
  //return float4(ShadowTex1.Sample(SS, uv).xxx, 1.0f);
  //return float4(ShadowTex1.Sample(SS, uv).yyy, 1.0f);
  //return float4(ShadowTex1.Sample(SS, uv).zzz, 1.0f);
  //return float4(ShadowTex1.Sample(SS, uv).www, 1.0f);

  float4 VPPos = mul(VP, float4(position, 1.0f));
  float vCurrentPixelDepth = VPPos.z / 10000.0f;
  int iCurrentCascadeIndex = 0;

//#define INTERVAL_BASED_SELECTION
//#define MAP_BASED_SELECTION

#ifdef INTERVAL_BASED_SELECTION
  float4 fComparison;
  fComparison[0] = (vCurrentPixelDepth > ShadowSliptDepth[0]);
  fComparison[1] = (vCurrentPixelDepth > ShadowSliptDepth[1]);
  fComparison[2] = (vCurrentPixelDepth > ShadowSliptDepth[2]);
  fComparison[3] = (vCurrentPixelDepth > ShadowSliptDepth[3]);

  //TODO: change the 4 with a activated cascades int const
  float fIndex = dot(float4(4 > 0, 4 > 1, 4 > 2, 4 > 3), fComparison);
  fIndex = min(fIndex, 4);
  iCurrentCascadeIndex = (int)fIndex;

  //if (iCurrentCascadeIndex == 0)
  //  return float4(1, 0, 0, 1);
  //if (iCurrentCascadeIndex == 1)
  //  return float4(0, 1, 0, 1);
  //if (iCurrentCascadeIndex == 2)
  //  return float4(0, 0, 1, 1);
  //if (iCurrentCascadeIndex == 3)
  //  return float4(1, 1, 1, 1);
#else //MAP_BASED_SELECTION
  iCurrentCascadeIndex = 3;
  iCurrentCascadeIndex = insideBounds(mul(kShadowVP[3], float4(position, 1.0f))) ? 3 : iCurrentCascadeIndex;
  iCurrentCascadeIndex = insideBounds(mul(kShadowVP[2], float4(position, 1.0f))) ? 2 : iCurrentCascadeIndex;
  iCurrentCascadeIndex = insideBounds(mul(kShadowVP[1], float4(position, 1.0f))) ? 1 : iCurrentCascadeIndex;
  iCurrentCascadeIndex = insideBounds(mul(kShadowVP[0], float4(position, 1.0f))) ? 0 : iCurrentCascadeIndex;
  
  //if (iCurrentCascadeIndex == 0)
  //  return float4(1, 0, 0, 1);
  //if (iCurrentCascadeIndex == 1)
  //  return float4(0, 1, 0, 1);
  //if (iCurrentCascadeIndex == 2)
  //  return float4(0, 0, 1, 1);
  //if (iCurrentCascadeIndex == 3)
  //  return float4(1, 1, 1, 1);
#endif
  //Projects the position from the mainCam to what shadowCam sees
  float4 fromLightPos = mul(kShadowVP[iCurrentCascadeIndex], float4(position, 1.0f));
  //fromLightPos.xyz /= fromLightPos.w;
  float ShadowValue = GetShadowValue(fromLightPos, iCurrentCascadeIndex);
  
  return float4((finalColor * ShadowValue) + emissive, 1.0f);
  return float4(finalColor + emissive, 1.0f);
  return float4(luminescence((finalColor * ShadowValue) + emissive).xxx, 1.0f);
  return float4(albedo * ShadowValue, 1.0f);
}