cbuffer ConstantBuffer {
	float4x4 World;
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
  float  fDepth   : TEXCOORD0;
};

VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT psOut;

  float4x4 BoneTransform = (float4x4)0;
  BoneTransform[0][0] = 1.0f;
  BoneTransform[1][1] = 1.0f;
  BoneTransform[2][2] = 1.0f;
  BoneTransform[3][3] = 1.0f;
  
  [unroll]
  for (int i = 0; i < 4; ++i) {
    int index = input.BonesIDs[i];
    if (index != -1) {
      BoneTransform += mul(Bones[index], input.BonesWeights[i]);
    }
  }
  
	float4 vertexTransformed = mul(BoneTransform, input.Position);
  
  //psOut.Position = mul(WVP, input.Position);
  psOut.Position  = mul(WVP, vertexTransformed);
  //psOut.fDepth   = psOut.Position.z / psOut.Position.w;
  psOut.fDepth   = psOut.Position.z;
  psOut.fDepth   = mul(WVP, input.Position).z;
  
  return psOut;
}