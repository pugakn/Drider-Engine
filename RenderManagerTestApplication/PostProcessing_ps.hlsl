Texture2D ColorTex     : register(t0);
Texture2D ColorBlurTex : register(t1);
Texture2D NormCoC      : register(t2);
//Texture2D GodRays      : register(t3);

SamplerState SS;

cbuffer ConstantBuffer {
  float4 Var;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

#define CHROMATIC_ABERRATION
#define DEPTH_OF_FIELD
#define VIGNETTE

float4
 FS(PS_INPUT input) : SV_TARGET0 {
  float2 uv = input.Texcoord;
  float4 Color;
  float4 ColorBlur;

  #ifdef CHROMATIC_ABERRATION
    float  fChromaticAberrationStrength = 0.1f;
    float  fChromaticAberrationConcentration = 2.0f;
    
    float2 CAOffset = float2(pow(-abs((0.5f - uv.x) * fChromaticAberrationStrength), fChromaticAberrationConcentration), 0.0f);
    
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
    float fCoC = NormCoC.Sample(SS, uv).w;
    //return float4(fCoC.xxx, 1.0f);
  #endif //DEPTH_OF_FIELD

  #ifdef VIGNETTE
    float2 fVignetteConcentration = float2(4.0f, 4.0f);
    float2 fVignetteRad = float2(1.25f, 1.25f);
    float  fVignetteScale = 1.0f;
    
    float x = pow((uv.x * 2.0f) - 1.0f, fVignetteConcentration.x);
    float y = pow((uv.y * 2.0f) - 1.0f, fVignetteConcentration.y);
    float xR = fVignetteRad.x;
    float yR = fVignetteRad.y;
    
    float vignette = 1.0f - (saturate(((x*x)/(xR*xR)) + ((y*y)/(yR*yR))) * fVignetteScale);
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

  #ifdef VIGNETTE
  finalColor *= vignette;
  #endif //VIGNETTE
  
  return finalColor;
}