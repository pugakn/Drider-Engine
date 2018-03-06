struct PS_INPUT {
  float4 Position : POSITION;
	float2 Texcoord : TEXCOORD;
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
  psOut.Texcoord.x = 1.0f - psOut.Texcoord.x;
  
  return psOut;
}
