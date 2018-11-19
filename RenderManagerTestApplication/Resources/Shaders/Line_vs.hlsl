cbuffer ConstantBuffer {
  float4x4 VP;
  float4x4 W;
  float4   LineColor;
  float4   CameraInfo; //X: Aspect Ratio; Y: FOV; Z: Near Plane; W: Far Plane
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
};

VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT Output;
  
  Output.Position  = mul(mul(W, VP), input.Position);

  return Output;
}