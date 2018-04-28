Texture2D inTex : register(t0);

SamplerState SS;

cbuffer ConstantBuffer {
  float4 fViewportDimensions;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};
static const float BlurWeights[13] =  { 0.002216f, 0.008764f, 0.026995f, 0.064759f, 0.120985f, 0.176033f, 0.199471f, 0.176033f, 0.120985f, 0.064759f, 0.026995f, 0.008764f, 0.002216f };

float4
FS(PS_INPUT input) : SV_TARGET0 {
  float2 uv = input.Texcoord;

  static const float offset = 1.0f / fViewportDimensions.x;
  float shadowVal = 0.0f;
  
  float2 sampleCoords;
  
  [unroll]
  for (int i = -6; i <= 6; i++) {
    sampleCoords = float2(uv.x + (i * offset), uv.y);
    shadowVal += inTex.Sample(SS, sampleCoords.xy).x * BlurWeights[i + 6];
  }

  return float4(shadowVal, shadowVal, shadowVal, 1.0f);
  return inTex.Sample(SS, uv);
}