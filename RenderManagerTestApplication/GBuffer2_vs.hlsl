cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
};

struct PS_INPUT {
  float4 Position : POSITION;
  float4 Normal   : NORMAL0;
  float4 Binormal : BINORMAL0;
  float4 Tangent  : TANGENT0;
  float4 Weights  : BONEWEIGHTS;
	int4 BoneIDs  	: BONEIDS; 
	float2 Texcoord : TEXCOORD;
};

struct PS_OUTPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

PS_OUTPUT VS(PS_INPUT input){
  PS_OUTPUT psOut;
  
  psOut.Position  = mul(WVP, input.Position);
  psOut.Texcoord  = input.Texcoord;
  
  return psOut;
}