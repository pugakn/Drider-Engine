Texture2D ColorTex    : register(t0);
Texture2D PositionTex : register(t1);
Texture2D NormalTex   : register(t2);

SamplerState SS;

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

float4 FS(PS_INPUT input) : SV_TARGET {
  float2 uv = input.Texcoord;

	float4 color = PositionTex.Sample(SS, uv);
  
  return color;
}