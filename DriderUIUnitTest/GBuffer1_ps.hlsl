Texture2D AlbedoTex   : register(t0);
Texture2D NormalTex   : register(t1);
Texture2D EmissiveTex : register(t2);
Texture2D Metallic    : register(t3);
Texture2D Roughness   : register(t4);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 WVP;
  float4x4 WVPTrans;
	float4x4 WorldView;
  float4x4 Bones[200];
};

struct PS_INPUT {
  float4   Position : SV_POSITION;
  float4   RealPos  : TEXCOORD0;
  float2   Texcoord : TEXCOORD1;
  float3x3 TBN      : TEXCOORD2;
};

struct PS_OUTPUT {
  float4 PosNorm		: SV_TARGET0;
	float4 Albedo_M	  : SV_TARGET1;
	float4 Emissive_R : SV_TARGET2;
};

PS_OUTPUT
FS(PS_INPUT input) {
	PS_OUTPUT output;

	float2 uv = input.Texcoord;
	
	float4 albedo   = AlbedoTex.Sample(SS, uv);
  float4 emmisive = EmissiveTex.Sample(SS, uv);
  float3 normal   = normalize(2.0f * NormalTex.Sample(SS, uv).xyz - 1.0f);
  normal = normalize(mul(normal, input.TBN));
  
  float metalic   = Metallic.Sample(SS, uv).r;
  float roughness = Roughness.Sample(SS, uv).r;
	
  output.PosNorm    = float4(normal, input.RealPos.z / input.RealPos.w);
  output.Albedo_M   = float4(albedo.xyz, metalic);
  output.Emissive_R = float4(emmisive.xyz, roughness);
  
  return output;
}