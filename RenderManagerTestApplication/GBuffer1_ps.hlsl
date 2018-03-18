Texture2D AlbedoTex   : register(t0);
Texture2D NormalTex   : register(t1);
Texture2D EmissiveTex : register(t2);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
};

struct PS_INPUT {
  float4   Position : SV_POSITION;
  float4   RealPos  : POSITION;
  float2   Texcoord : TEXCOORD0;
  float3x3 TBN      : TEXCOORD1;
};

struct PS_OUTPUT {
  float4 Albedo		: SV_TARGET0;
	float4 Position	: SV_TARGET1;
	float4 Normal	 	: SV_TARGET2;
	float4 Emissive	: SV_TARGET3;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;
  
	float2 uv = input.Texcoord;

	float4 albedo		= AlbedoTex.Sample(SS, uv);
	float4 position	= input.RealPos;

	float3 TexNormal = float3(0.0f, 0.0f, 1.0f);
	TexNormal = normalize(2.0f * NormalTex.Sample(SS, uv).xyz - 1.0f).xyz;
  float3 normal     = normalize(mul(TexNormal, input.TBN));

	float4 emissive	= EmissiveTex.Sample(SS, uv); 

	outRT.Albedo	 = albedo;
	outRT.Position = position;
	outRT.Normal	 = float4(normal, 1);
	outRT.Emissive = emissive;

	return outRT;
}