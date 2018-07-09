Texture2D ColorTex : register(t0);

SamplerState SS;

cbuffer ConstantBuffer {
  float4 Var;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

float4
 FS(PS_INPUT input) : SV_TARGET0 {
  float2 uv = input.Texcoord;
  
  return ColorTex.Sample(SS, uv);
}