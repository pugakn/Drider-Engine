Texture2D AlbedoTex    : register(t0);
Texture2D PositionTex  : register(t1);
Texture2D NormalTex    : register(t2);
Texture2D EmissiveTex  : register(t3);
Texture2D MetallicTex  : register(t4);
Texture2D RoughnessTex : register(t5);
Texture2D SSAOTex      : register(t6);
Texture2D ShadowTex1   : register(t7);

SamplerState SS;

cbuffer ConstantBuffer {
  float4 kEyePosition;
  float4 kDirLight;
  float4 kLightPosition[128];
  float4 kLightColor[128];
  float4x4 VP;
  float4x4 VPInv;
  float4x4 W;
  float4x4 WInv;
  float4x4 V;
  float4x4 VInv;
  float4x4 P;
  float4x4 PInv;
  float4x4 kShadowVP[4];
  float4x4 kShadowVPInv[4];
  float4x4 kShadowW[4];
  float4x4 kShadowWInv[4];
  float4x4 kShadowV[4];
  float4x4 kShadowVInv[4];
  float4x4 kShadowP[4];
  float4x4 kShadowPInv[4];
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

static const float M_PI = 3.14159265f;
static const float EPSILON = 1e-6f;

float
LinearShadowDepth(const int camIndex, float4 pos) {
  float4 newPos = mul(kShadowVP[0], pos);
  float depth = (newPos.z / newPos.w);

  return depth;
}

float2
UVCoordFromCam(const int camIndex, float3 Pos) {
  float4 detransformedPos = float4(Pos, 1.0f);
  float2 uv = mul(kShadowVP[0], detransformedPos).xy;
  uv *= 0.5f;
  uv += 0.5f;
  uv.y = 1.0f - uv.y;
  //return saturate(uv * 0.5f);
  return uv;
}

float2
UVCoordFromMainCam(const int camIndex, float3 Pos) {
  float2 uv = mul(VP, float4(Pos, 1.0f)).xy;
  uv *= 0.5f;
  uv += 0.5f;
  uv.y = 1.0f - uv.y;
  //return saturate(uv * 0.5f);
  return uv;
}

#define DR_SH_PCF_ENABLED
float GetShadowValue(float4 fromLightPos, const int camIndex ) {
  const float shadowBias = 0.0005;
  float2 fromLightCoords = fromLightPos.xy * 0.5 + 0.5;
  float shadowValue = 1.0;
  if(fromLightCoords.x < 1.0 && 
     fromLightCoords.y < 1.0 && 
     fromLightCoords.x  > 0.0 && 
	 fromLightCoords.y > 0.0 && 
	 fromLightPos.w > 0.0 && 
	 fromLightPos.z < 1.0 )
  {
  fromLightCoords.y = 1.0 - fromLightCoords.y;
  //Pixel Depth seen from shadowCam
  float depthPos = fromLightPos.z;
  #ifdef DR_SH_PCF_ENABLED

  uint width, height;   //TODO: Pass This in CBUFFER xdXDxDxDXDXD
  ShadowTex1.GetDimensions(width, height);  //TODO: Pass This in CBUFFER xdXDxDxDXDXD
  float2 texelSize = float2(1.0 ,1.0) / float2(width,height);
  const float sampleRadius = 3.0;
  const float modifier = 0.25 / (sampleRadius*sampleRadius*2.0);
  [unroll]
  for (float y = -sampleRadius; y <= sampleRadius; y += 1.0){
    [unroll]
	for (float x = -sampleRadius; x <= sampleRadius; x += 1.0){
	      //Projected depth	
	      float depthSample = ShadowTex1.Sample( SS, fromLightCoords.xy + texelSize * float2(x,y) )[camIndex];
	   
	      if( depthPos > (depthSample + shadowBias))
	      	shadowValue -= modifier;	  
	}
  }
  
  #else //DR_SH_PCF_ENABLED
  //Projected depth	
  float depthSample = ShadowTex1.Sample( SS, fromLightCoords.xy )[camIndex];
  
  if( depthPos > (depthSample + shadowBias))
  	shadowValue = 0.25;	  
  #endif //DR_SH_PCF_ENABLED
  }
  
  return shadowValue;
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
  
  float3 albedo    = AlbedoTex.Sample(SS, uv).xyz;
  float4 position  = PositionTex.Sample(SS, uv).xyzw;
  //if (PositionTex.Sample(SS, uv).w == 0.0f) discard;
  float3 normal    = NormalTex.Sample(SS, uv).xyz;
  float3 emissive  = EmissiveTex.Sample(SS, uv).xyz;
  float  metallic  = MetallicTex.Sample(SS, uv).x;
  float3 specular  = lerp(float3(0.03f, 0.03f, 0.03f), albedo, metallic);
  float  roughness = RoughnessTex.Sample(SS, uv).x;
  float  SSAO      = SSAOTex.Sample(SS, uv).x;
  //float3 shadow    = ShadowTex.Sample(SS, uv).xyz;

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
  for (int index = 0; index < activeLights; index += 8) {
    lightPosition  = kLightPosition[index].xyz;
    lightColor     = kLightColor[index].xyz;
    lightIntensity = kLightColor[index].w;
    
    LightPower = saturate( 1.0f - (length(lightPosition - position.xyz) / 150.0f) );

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

  
  //return AlbedoTex.Sample(SS, uv);
  //return PositionTex.Sample(SS, uv);
  //return NormalTex.Sample(SS, uv);
  //return EmissiveTex.Sample(SS, uv);
  //return MetallicTex.Sample(SS, uv);
  //return RoughnessTex.Sample(SS, uv);
  //return SSAOTex.Sample(SS, uv);
  //return float4(ShadowTex1.Sample(SS, uv).xyz, 1);
  //return float4(ShadowTex1.Sample(SS, uv).xxx, 1); //ShadowCam1
  //return float4(ShadowTex1.Sample(SS, uv).yyy, 1); //ShadowCam2
  //return float4(ShadowTex1.Sample(SS, uv).zzz, 1); //ShadowCam3
  //return float4(ShadowTex1.Sample(SS, uv).www, 1); //ShadowCam4
  
  //return float4(uv, 0.0f, 1); //ShadowCam1

  //Projects the position from the mainCam to what shadowCam sees
  float4 fromLightPos = mul(kShadowVP[0],position);
  fromLightPos.xyz /= fromLightPos.w;

  float4 ShadowValue = GetShadowValue(fromLightPos,0).xxxx;
  //return ShadowValue;
  return float4(finalColor * ShadowValue  + emissive, 1.0f);
}