struct PS_INPUT {
  float4 Position     : POSITION;
  float4 normal       : NORMAL;
  float4 binormal     : BINORMAL;
  float4 tangent      : TANGENT;
  float4 bonesWeights : BONEWEIGHTS;
  int4 bonesIDs       : BONEIDS;
	float2 Texcoord     : TEXCOORD;
};

struct PS_OUTPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

PS_OUTPUT VS(PS_INPUT input){
  PS_OUTPUT psOut;
  
  psOut.Position  = input.Position;
  psOut.Position.x = -psOut.Position.x;

  psOut.Texcoord  = input.Texcoord;
  psOut.Texcoord.y = 1.0f - input.Texcoord.y;
  
  return psOut;
}