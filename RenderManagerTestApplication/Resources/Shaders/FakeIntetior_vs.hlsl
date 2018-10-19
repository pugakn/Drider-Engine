cbuffer ConstantBuffer {
	float4x4 World;
  float4x4 WVP;
  float4   CameraPosition;
  float4   vertice[8];
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
  VS_OUTPUT Output = (VS_OUTPUT)0;
  
  Output.Position  = mul(WVP, input.Position);
  Output.RealPos   = float4(mul(World, input.Position).xyz, 1.0f);
  Output.Texcoord  = input.Texcoord;
  
  Output.TBN[0] = normalize(mul(World, input.Tangent)).xyz;
  Output.TBN[1] = normalize(mul(World, input.Binormal)).xyz;
  Output.TBN[2] = normalize(mul(World, input.Normal)).xyz;
  
  return Output;
}