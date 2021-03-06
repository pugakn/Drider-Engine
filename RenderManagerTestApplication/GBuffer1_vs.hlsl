cbuffer ConstantBuffer {
	float4x4 World;
  float4x4 WorldView;
  float4x4 WVP;
  float4x4 Bones[200];
  float4   CameraInfo;
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
  float2   Texcoord : TEXCOORD0;
  float4   RealPos  : TEXCOORD1;
  float3x3 TBN      : TEXCOORD2;
};

VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT Output;
  
  Output.Position = mul(WVP, input.Position);
  Output.RealPos  = float4(mul(World, input.Position).xyz, Output.Position.z);
  Output.Texcoord = input.Texcoord;
  
  Output.TBN[0] = normalize(mul(WorldView, input.Tangent));
  Output.TBN[1] = normalize(mul(WorldView, input.Binormal));
  Output.TBN[2] = normalize(mul(WorldView, input.Normal));
  
  return Output;
}