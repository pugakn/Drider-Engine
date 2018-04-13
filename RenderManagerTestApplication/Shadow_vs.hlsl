cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
  int4      ShadowIndex;
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
};

VS_OUTPUT VS(VS_INPUT input){
  VS_OUTPUT psOut;
  
  psOut.Position  = mul(WVP, float4(input.Position.xyz, 1.0f));
  psOut.RealPos   = mul(World, float4(input.Position.xyz, 1.0f));
  
  return psOut;
}