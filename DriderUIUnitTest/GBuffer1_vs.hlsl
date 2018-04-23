cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
};

struct VS_INPUT {
  float4 Position     : POSITION;
  float4 Normal       : NORMAL;
  float4 Binormal     : BINORMAL;
  float4 Tangent      : TANGENT;
  float4 BonesWeights : BONEWEIGHTS;
  int4   BonesIDs     : BONEIDS;
	float2 Texcoord     : TEXCOORD;
};

struct VS_OUTPUT {
  float4   Position : SV_POSITION;
  float4   RealPos  : POSITION;
  float2   Texcoord : TEXCOORD0;
  float3x3 TBN      : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input){
  VS_OUTPUT psOut;
  
  psOut.Position  = mul(WVP, float4(input.Position.xyz, 1.0f));
  psOut.RealPos   = mul(World, float4(input.Position.xyz, 1.0f));
  psOut.TBN[0]    = normalize(mul(World, input.Tangent)).xyz;
  psOut.TBN[1]    = normalize(mul(World, input.Binormal)).xyz;
  psOut.TBN[2]    = normalize(mul(World, input.Normal)).xyz;
  psOut.Texcoord  = input.Texcoord;
  
  return psOut;
}