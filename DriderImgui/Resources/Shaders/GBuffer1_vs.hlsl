cbuffer ConstantBuffer {
	float4x4 World;
  float4x4 WorldView;
  float4x4 WVP;
  float4x4 Bones[200];
  float4   CameraInfo; //X: Aspect Ratio; Y: FOV; Z: Near Plane; W: Far Plane
  bool     ConstantFlags[4]; //0 To check if we are using Bones
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
  
  float4x4 BoneTransform = (float4x4)0;
  
  if (ConstantFlags[0]) {
    int activationScale = 0;
    int boneIndex = 0;
    
    [unroll]
    for (int i = 0; i < 4; ++i) {
      boneIndex = input.BonesIDs[i];
      activationScale = (boneIndex > -1);
      BoneTransform += mul(Bones[boneIndex], input.BonesWeights[i] * activationScale);
    }
  }
  else {
    BoneTransform[0][0] = 1.0f;
    BoneTransform[1][1] = 1.0f;
    BoneTransform[2][2] = 1.0f;
    BoneTransform[3][3] = 1.0f;
  }
  
	float4 vertexTransformed = mul(BoneTransform, input.Position);
  float3 TransformedT = mul(BoneTransform, input.Tangent).xyz;
  float3 TransformedB = mul(BoneTransform, input.Binormal).xyz;
  float3 TransformedN = mul(BoneTransform, input.Normal).xyz;
  
  Output.Position  = mul(WVP, vertexTransformed);
  Output.RealPos   = float4(mul(World, vertexTransformed).xyz, 1.0f);
  Output.RealPos.w = Output.Position.z * rcp(CameraInfo[3]);
  Output.Texcoord  = input.Texcoord;
  
  Output.TBN[0] = normalize(mul((float3x3)World, TransformedT));
  Output.TBN[1] = normalize(mul((float3x3)World, TransformedB));
  Output.TBN[2] = normalize(mul((float3x3)World, TransformedN));

  return Output;
}