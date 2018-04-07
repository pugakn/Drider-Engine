Texture2D Texture0 : register(t0);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float3 Normal   : NORMAL0;
  float3 Binormal : BINORMAL0;
  float3 Tangent  : TANGENT0;
  float2 Texcoord : TEXCOORD0;
};

struct PS_OUTPUT {
  float4 Diffuse;
	float4 Normal;
  float4 Metallic;
	float4 Roughness;
	float4 Emisivity;
	float4 Transparency;
	float4 ProjectShadow;
	float4 SSS;
	float4 Displacement;
	float4 SpecularColor;
	float4 Opacity;
	float4 Detail;
};

float4 FS(PS_INPUT input) : SV_TARGET  {
  
  float2 uv = input.Texcoord;
	float3 n = input.Normal.xyz;
	float4 color = Texture0.Sample(SS, uv);

  return color;
}
