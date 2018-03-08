Texture2D Texture0 : register(t0);

SamplerState SS;

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

float4 FS(PS_INPUT input) : SV_TARGET  {
  float2 uv = input.Texcoord;
	float4 color = Texture0.Sample(SS, uv);

  return color;
}