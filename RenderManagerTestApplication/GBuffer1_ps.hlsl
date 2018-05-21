Texture2D AlbedoTex   : register(t0);
Texture2D NormalTex   : register(t1);
Texture2D EmissiveTex : register(t2);
Texture2D Metallic    : register(t3);
Texture2D Roughness   : register(t4);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 World;
  float4x4 WorldView;
  float4x4 WVP;
  float4x4 Bones[200];
  float4   CameraInfo;
};

struct PS_INPUT {
  float4   Position : SV_POSITION;
  float2   Texcoord : TEXCOORD0;
  float4   RealPos  : TEXCOORD1;
  float3x3 TBN      : TEXCOORD2;
};

struct PS_OUTPUT {
  float4 PositionLDepth	: SV_TARGET0;
  float4 NormCoC	      : SV_TARGET1;
	float4 Albedo_M	      : SV_TARGET2;
	float4 Emissive_R     : SV_TARGET3;
};

PS_OUTPUT
FS(PS_INPUT input) {
	PS_OUTPUT output;

	float2 uv = input.Texcoord;
	
	float4 albedo   = AlbedoTex.Sample(SS, uv);
  float4 emmisive = EmissiveTex.Sample(SS, uv);
  float3 normal   = normalize((2.0f * NormalTex.Sample(SS, uv).xyz) - 1.0f);
  normal = normalize(mul(normal, input.TBN));
  
  float metalic   = Metallic.Sample(SS, uv).r;
  float roughness = Roughness.Sample(SS, uv).r;

  float RealDepth = input.RealPos.w;
  float fFocusDistance = 380.0f;
  float fFocusRange = 70.0f;
  float CoC = (RealDepth - fFocusDistance) / abs(fFocusRange);
  
  //sign: Returns -1 if x is less than zero; 0 if x equals zero; and 1 if x is greater than zero.
  float fA = saturate(sign(fFocusRange));
  
  CoC = clamp(CoC, -1.0f * fA, 1.0f);
  CoC = abs(CoC);
  
  output.PositionLDepth  = input.RealPos;
  output.NormCoC         = float4(normal, CoC);
  output.Albedo_M        = float4(albedo.xyz, metalic);
  output.Emissive_R      = float4(emmisive.xyz, roughness);
  
  return output;
}