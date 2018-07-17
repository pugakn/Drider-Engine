cbuffer ConstantBuffer {
	float4x4 WVP;
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
  float4 Position : SV_POSITION;
	float2 Texcoord : TEXCOORD0;
  float  fDepth   : TEXCOORD1;
};

VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT psOut;

  float4x4 BoneTransform = (float4x4)0;
  int activationScale = 0,
      activeWeights = 0,
      boneIndex = 0;
  
  [unroll]
  for (int i = 0; i < 4; ++i) {
    boneIndex = input.BonesIDs[i];
    activationScale = (boneIndex > -1);
    activeWeights += activationScale;
    BoneTransform += mul(Bones[boneIndex], input.BonesWeights[i] * activationScale);
  }
  
  BoneTransform[0][0] += 1.0f * (activeWeights <= 0);
  BoneTransform[1][1] += 1.0f * (activeWeights <= 0);
  BoneTransform[2][2] += 1.0f * (activeWeights <= 0);
  BoneTransform[3][3] += 1.0f * (activeWeights <= 0);
  
	float4 vertexTransformed = mul(BoneTransform, input.Position);
  
  psOut.Position = mul(WVP, vertexTransformed);
  psOut.Texcoord = input.Texcoord;
  psOut.fDepth   = psOut.Position.z;
  
  return psOut;
}