cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
};

struct VS_INPUT {
  float4 Position : POSITION;
  float4 Normal   : NORMAL0;
  float4 Binormal : BINORMAL0;
  float4 Tangent  : TANGENT0;
  float4 Weights  : BONEWEIGHTS;
	int4 BoneIDs  	: BONEIDS;
	float2 Texcoord : TEXCOORD;
};

struct VS_OUTPUT {
  float4 Position : SV_POSITION;
  float4 Normal   : NORMAL0;
  float4 Binormal : BINORMAL0;
  float4 Tangent  : TANGENT0;
  float2 Texcoord : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input){
  VS_OUTPUT psOut;
  
  psOut.Position  = mul(WVP, float4(input.Position.xyz, 1.0f));
  psOut.Normal    = float4(normalize(mul((float3x3)World, input.Normal.xyz)), 0);
  psOut.Binormal  = float4(normalize(mul((float3x3)World, input.Binormal.xyz)), 0);
  psOut.Tangent   = float4(normalize(mul((float3x3)World, input.Tangent.xyz)), 0);
  psOut.Texcoord  = input.Texcoord;
  psOut.Texcoord.x  = 1.0f - psOut.Texcoord.x;
  psOut.Texcoord.y  = 1.0f - psOut.Texcoord.y;
  
  return psOut;
}