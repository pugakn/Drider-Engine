Texture2D PositionLDepthTex : register(t0);
Texture2D ColorTex          : register(t1);
Texture2D ColorBlurTex      : register(t2);
Texture2D NormCoC           : register(t3);
StructuredBuffer<float4> LuminescenceTex : register (t4);
//Texture2D LuminescenceTex   : register(tn);
//Texture2D GodRays           : register(tn);
//Texture2D BloomTex          : register(tn);
//Texture2D FilmLutTex        : register(tn);

SamplerState SS;

cbuffer ConstantBuffer {
  float4 CameraInfo;      //X: Aspect Ratio; Y: FOV; Z: Near Plane; W: Far Plane
  float4 CA_CoC_V;        //X: ChromaticAberrationStrenght, Y: FocusDistance, Z: FocusRange, W: VignetteScale
  float4 VignetteOptions; //XY: fVignetteConcentration, ZW: fVignetteRad;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

//#define CHROMATIC_ABERRATION
//#define DEPTH_OF_FIELD
//#define VIGNETTE
//#define BLOOM
//#define TONE_MAPPING

float4
BasicExposure(in float3 Color, in float exposure) {
  float4 retColor = float4(Color, 1.0f);
  retColor *= exposure;
  retColor = pow(retColor, 1.0f/2.2f);
  
  return retColor;
}

float4
Reinhard(in float3 Color, in float exposure) {
  float4 retColor = float4(Color, 1.0f);
  retColor *= exposure;
  retColor = retColor/(1.0f + retColor);
  retColor = pow(retColor, 1.0f/2.0f);
  
  return retColor;
}

float4
Haarm_Peter_Duiker(in float3 Color, in float exposure) {
  float3 texColor = Color;
  texColor *= exposure;
  
  float3 ld           = 0.002f;
  float  linReference = 0.18f;
  float  logReference = 444.0f;
  float  logGamma     = 0.45f;
  
  float3 LogColor;
  LogColor.rgb = (log10(0.4f*texColor.rgb/linReference)/ld*logGamma + logReference)/1023.0f;
  LogColor.rgb = saturate(LogColor.rgb);
  
  float FilmLutWidth = 256.0f;
  float Padding = 0.5f / FilmLutWidth;
  
  float3 retColor = float3(1.0f, 1.0f, 1.0f);
  //retColor.r = tex2D(FilmLutSampler, float2( lerp(Padding, 1 - Padding, LogColor.r), 0.5f)).r;
  //retColor.g = tex2D(FilmLutSampler, float2( lerp(Padding, 1 - Padding, LogColor.g), 0.5f)).r;
  //retColor.b = tex2D(FilmLutSampler, float2( lerp(Padding, 1 - Padding, LogColor.b), 0.5f)).r;

  //retColor.r = tex2D(FilmLutSampler, float2( lerp(Padding, 1 - Padding, LogColor.r), 0.5f)).r;
  //retColor.g = tex2D(FilmLutSampler, float2( lerp(Padding, 1 - Padding, LogColor.r), 0.5f)).g;
  //retColor.b = tex2D(FilmLutSampler, float2( lerp(Padding, 1 - Padding, LogColor.r), 0.5f)).b;
  return float4(retColor, 1.0f);
}

float4
Burgeos_Dawson(in float3 Color, float exposure) {
  float3 texColor = Color;
  texColor *= exposure;
  float3 x = max((0.0f).xxx, texColor - (0.004f).xxx);
  float3 retColor = (x * (6.2f * x + (0.5f).xxx)) / (x * (6.2f * x * 1.7f) + (0.06f).xxx);
  
  return float4(retColor, 1.0f);
}

float3
Uncharted2Tonemap(float3 r) {
  static const float A = 0.15f;
  static const float B = 0.50f;
  static const float C = 0.10f;
  static const float D = 0.20f;
  static const float E = 0.02f;
  static const float F = 0.30f;

  return ((r * (A * r + B * C) + D * E) / (r * (A * r + B) + D * F)) - E/F;
}

float4
Uncharted2(in float3 Color, in float exposure) {
  static const float W = 11.2f;

  float3 texColor = Color;
  texColor *= exposure;
  
  float ExposureBias = 2.0f;
  float3 curr = Uncharted2Tonemap(ExposureBias * texColor);
  
  float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
  float3 color = curr * whiteScale;
  
  float3 retColor = pow(color, 1.0f/2.2f);
  return float4(retColor, 1.0f);
}

float4
FS(PS_INPUT input) : SV_TARGET0 {
  const float2 uv = input.Texcoord;
  float4 Color;
  float4 ColorBlur;
  
  #ifdef CHROMATIC_ABERRATION
    static const float fChromaticAberrationStrength = CA_CoC_V[0];
    
    float2 CAOffset = float2(-abs((0.5f - uv.x) * fChromaticAberrationStrength), 0.0f);
    CAOffset *= CAOffset;
    
    float CR, CG;
    float2 CBA;
    
    CR = ColorTex.Sample(SS, uv + CAOffset).x;
    CG = ColorTex.Sample(SS, uv - CAOffset).y;
    CBA = ColorTex.Sample(SS, uv).zw;
    Color = float4(CR, CG, CBA);
    
    #ifdef DEPTH_OF_FIELD
      CR = ColorBlurTex.Sample(SS, uv + CAOffset).x;
      CG = ColorBlurTex.Sample(SS, uv - CAOffset).y;
      CBA = ColorBlurTex.Sample(SS, uv).zw;
      ColorBlur = float4(CR, CG, CBA);
    #endif //DEPTH_OF_FIELD
  #else
    Color = ColorTex.Sample(SS, uv);
    #ifdef DEPTH_OF_FIELD
      ColorBlur = ColorBlurTex.Sample(SS, uv);
    #endif //DEPTH_OF_FIELD
  #endif
  
  #ifdef DEPTH_OF_FIELD
    const float RealDepth = PositionLDepthTex.Sample(SS, uv).w * CameraInfo.w;
    static const float fFocusDistance = CA_CoC_V.y;
    //If focus Range is negative, everithing between the eye and the focus distance will be focus.
    static const float fFocusRange = -CA_CoC_V.z;
    float fCoC = (RealDepth - fFocusDistance) / abs(fFocusRange);
    
    //sign: Returns -1 if x is less than zero; 0 if x equals zero; and 1 if x is greater than zero.
    const float fA = saturate(sign(fFocusRange));
    
    fCoC = clamp(fCoC, -1.0f * fA, 1.0f);
    fCoC = abs(fCoC);
    //return float4(fCoC.xxx, 1.0f);
  #endif //DEPTH_OF_FIELD
  
  #ifdef VIGNETTE
    static const float2 fVignetteConcentration = VignetteOptions.xy;
    static const float2 fVignetteRad = VignetteOptions.zw;
    static const float  fVignetteScale = CA_CoC_V.w;
    
    const float x = pow(abs((uv.x * 2.0f) - 1.0f), fVignetteConcentration.x);
    const float y = pow(abs((uv.y * 2.0f) - 1.0f), fVignetteConcentration.y);
    
    const float vignette = 1.0f - (saturate(((x * x) / (fVignetteRad.x * fVignetteRad.x)) +
                                            ((y * y) / (fVignetteRad.y * fVignetteRad.y))) * fVignetteScale);
  #endif //VIGNETTE
  
  //CR = GodRays.Sample(SS, uv + CAOffset).x;
  //CG = GodRays.Sample(SS, uv - CAOffset).y;
  //CB = GodRays.Sample(SS, uv).z;
  //CA = GodRays.Sample(SS, uv).w;
  //float4 ColorGodray = float4(CR, CG, CB, CA);
  
  float4 finalColor;
  
  #ifdef DEPTH_OF_FIELD
  finalColor = lerp(Color, ColorBlur, fCoC);
  #else
  finalColor = Color;
  #endif //DEPTH_OF_FIELD
  
  #ifdef BLOOM
  #endif //BLOOM
  
  #ifdef TONE_MAPPING
  const float kExposure = LuminescenceTex[0].x;
  
  //finalColor = BasicExposure(finalColor, kExposure);
  finalColor = Reinhard(finalColor.xyz, kExposure);
  //finalColor = Burgeos_Dawson(finalColor, kExposure); //Caca
  //finalColor = Uncharted2(finalColor, kExposure);
  #endif //TONE_MAPPING
  
  #ifdef VIGNETTE
  finalColor *= vignette;
  #endif //VIGNETTE
  
  return float4(LuminescenceTex[0].xxx, 1.0f);
  return finalColor;
}