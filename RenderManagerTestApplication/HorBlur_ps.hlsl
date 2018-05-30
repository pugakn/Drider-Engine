Texture2D inTex : register(t0);

SamplerState SS;

cbuffer ConstantBuffer {
  float4 fViewportDimensions;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

static const float kernel[30] = { 0.000001f, 0.000001f, 0.000002f, 0.000003f, 0.000006f, 0.000009f, 0.000015f, 0.000023f, 0.000036f, 0.000055f, 0.000082f, 0.00012f, 0.000171f, 0.000239f, 0.000328f, 0.00044f, 0.00058f, 0.000748f, 0.000945f, 0.001171f, 0.001421f, 0.001689f, 0.001968f, 0.002247f, 0.002514f, 0.002755f, 0.002958f, 0.003113f, 0.00321f, 0.003243f };
static const float BlurWeights[13] =  { 0.002216f, 0.008764f, 0.026995f, 0.064759f, 0.120985f, 0.176033f, 0.199471f, 0.176033f, 0.120985f, 0.064759f, 0.026995f, 0.008764f, 0.002216f };

float4
FS(PS_INPUT input) : SV_TARGET0 {
  float2 uv = input.Texcoord;

  static const float offset = 1.0f / fViewportDimensions.x;
  float4 shadowVal = float4(0.0f, 0.0f, 0.0f, 0.0f);
  
  float2 sampleCoords;
  
  [unroll]
  for (int i = -6; i <= 6; ++i) {
    sampleCoords = float2(uv.x + (i * offset), uv.y);
    shadowVal += inTex.Sample(SS, sampleCoords.xy) * BlurWeights[i + 6];
  }

  return shadowVal;
}