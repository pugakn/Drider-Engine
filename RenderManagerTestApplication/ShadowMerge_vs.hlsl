struct PS_INPUT {
  float4 Position     : POSITION;
  float4 Normal       : NORMAL;
  float4 Binormal     : BINORMAL;
  float4 Tangent      : TANGENT;
  float4 BonesWeights : BONEWEIGHTS;
  int4   BonesIDs     : BONEIDS;
	float2 Texcoord     : TEXCOORD;
};

struct PS_OUTPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

PS_OUTPUT
VS(PS_INPUT input) {
  PS_OUTPUT psOut;
  
  psOut.Position   =  input.Position;
  psOut.Position.x = -psOut.Position.x;

  psOut.Texcoord  = 1.0f - input.Texcoord;
  
  return psOut;
}