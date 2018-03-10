Texture2D AlbedoTex    : register(t0); //1, 0, 0
Texture2D PositionTex  : register(t1); //0, 1, 0
Texture2D NormalTex    : register(t2); //0, 0, 1
Texture2D EmissiveTex  : register(t3); //1, 1, 0
Texture2D MetallicTex  : register(t4); //1, 0, 1
Texture2D RoughnessTex : register(t5); //0, 1, 1
Texture2D SSAOTex      : register(t6); //1, 0, 1

SamplerState SS;

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

float4 FS(PS_INPUT input) : SV_TARGET {
  float2 uv = input.Texcoord;

	float4 color = AlbedoTex.Sample(SS, uv);
  
  return color;
}