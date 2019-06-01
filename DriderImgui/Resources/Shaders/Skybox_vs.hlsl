#define PI 3.14152265f

cbuffer ConstantBuffer {
  float4   CameraInfo; //X: Aspect Ratio; Y: FOV; Z: Near Plane; W: Far Plane
  float4x4 ViewProjectionInverse;
  float4x4 SkyboxRotation;
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
  float4 Position   : SV_POSITION;
  float3 Direction  : TEXCOORD0;
};

VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT output = (VS_OUTPUT)0;
  
  output.Position.xy = input.Position.xy;
  output.Position.zw  = (1.0f).xx;

  output.Direction = normalize(mul(ViewProjectionInverse, output.Position).xyz);
  
  return output;
}