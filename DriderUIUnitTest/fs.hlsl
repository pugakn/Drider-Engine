cbuffer ConstantBuffer{
	float4x4 WVP;
}
Texture2D t : t0;
SamplerState SS;

struct VS_OUTPUT {
  float4 hposition : SV_POSITION;
  float4 hnormal   : NORMAL;
  float2 htexcoord : TEXCOORD;
};
float4 FS( VS_OUTPUT input ) : SV_TARGET  {
    float4 color =  t.Sample(SS, input.htexcoord);
    return color;
}