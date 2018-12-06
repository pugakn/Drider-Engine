struct PS_INPUT {
  float4 Position : POSITION;
};

struct PS_OUTPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

PS_OUTPUT
VS(PS_INPUT input) {
  PS_OUTPUT psOut;
  
  psOut.Position   =  input.Position;
  
  psOut.Texcoord  = (psOut.Position.xy + float2(1.0f, 1.0f)) * 0.5f;
  psOut.Texcoord.y  = 1.0f - psOut.Texcoord.y;
  
  return psOut;
}