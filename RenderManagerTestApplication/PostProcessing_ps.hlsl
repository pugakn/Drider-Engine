Texture2D AlbedoTex    : register(t0); //1, 0, 0
Texture2D PositionTex  : register(t1); //0, 1, 0
Texture2D NormalTex    : register(t2); //0, 0, 1
Texture2D EmissiveTex  : register(t3); //1, 1, 0
Texture2D MetallicTex  : register(t4); //1, 0, 1
Texture2D RoughnessTex : register(t5); //0, 1, 1
Texture2D SSAOTex      : register(t6); //1, 0, 1
TextureCube Enviroment : register(t7);

SamplerState SS;

cbuffer ConstantBuffer {
	float4 EyePosition;
  float4 LightPositions[128];
  float LightIntensities[128];
  float4 LightColors[128];
  int activeLights;
  int shit1;
  int shit2;
  int shit3;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

static const float M_PI = 3.14159265f;
static const float EPSILON = 1e-6f;

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
  
  float3 albedo    = AlbedoTex.Sample(SS, uv).xyz;
  float3 position  = PositionTex.Sample(SS, uv).xyz;
  if (PositionTex.Sample(SS, uv).w == 0.0f){
    discard;
  }
  float3 normal    = NormalTex.Sample(SS, uv).xyz;
  float3 emissive  = EmissiveTex.Sample(SS, uv).xyz;
  float  metallic  = MetallicTex.Sample(SS, uv).x;
  float  roughness = RoughnessTex.Sample(SS, uv).x;
  float  SSAO      = SSAOTex.Sample(SS, uv).x;

  float3 finalColor = float3(0.0f, 0.0f, 0.0f);
	

  float3 LightPosition = float3(0.0f, 50.0f, -100.0f);

  float3 ViewDir = normalize(EyePosition.xyz - position);
  //float3 LightDir = normalize(LightPosition - position);
  float3 LightDir = normalize(LightPosition - position);

  float  NdotV = saturate(dot(normal, ViewDir));
  float3 H;
  float  NdotL;
  float  LdotV;
  float  NdotH;
  float  VdotH;
  float  LdotH;

  ///////////////////////////////////////
  roughness = 1.0f;
  float alpha = max(0.01f, roughness * roughness);
  
  //Diffuse light acumulation
  float3 DiffAcc;
  float3 SpecAcc;;
  
  SSAO = 1.0f;
  emissive = float3(0.0f, 0.0f, 0.0f);
  ///////////////////////////////////////

  int maxLights = min(activeLights, 128);
  for (int i = 0; i < 128; i++) {
    LightPosition = LightPositions[i].xyz;

    LightDir = normalize(LightPosition - position);

    H     = normalize(LightDir + ViewDir);

    NdotL = saturate(dot(normal, LightDir));
    LdotV = saturate(dot(LightDir, ViewDir));

    NdotH = saturate(dot(normal, H));
    VdotH = saturate(dot(ViewDir, H));
    LdotH = saturate(dot(LightDir, H));

    DiffAcc = Diffuse_Burley(NdotL, NdotV, LdotH, roughness) * albedo;
    SpecAcc = Specular_D(alpha, NdotH) *
              Specular_F(LightColors[i].xyz, LdotH) *
              Specular_G(alpha, LdotH);
    SpecAcc /= (4.0f * cos(NdotL) * cos(NdotV));

    finalColor += ((DiffAcc + SpecAcc) * SSAO * NdotL);
  };
  
  return float4(finalColor, 1.0f);
}