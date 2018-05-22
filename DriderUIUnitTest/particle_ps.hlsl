cbuffer ConstantBuffer {
  float4x4 WVP;
  float4 color;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};


float4
FS(PS_INPUT input) : SV_TARGET0 {
  float2 uv = input.Texcoord;
  return float4(color.xyz, 1.0f);
}