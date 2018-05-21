cbuffer ConstantBuffer {
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
  float4 Position : SV_POSITION;
  float  fDepth   : TEXCOORD0;
};

VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT psOut;
  
  psOut.Position = mul(WVP, float4(input.Position.xyz, 1.0f));
  //psOut.fDepth   = psOut.Position.z / psOut.Position.w;
  psOut.fDepth   = psOut.Position.z;
  
  return psOut;
}