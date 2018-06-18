Texture2D AlbedoTex   : register(t0);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 WVP;
  float4x4 Bones[200];
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
	float2 Texcoord : TEXCOORD0;
  float  fDepth   : TEXCOORD1;
};

struct PS_OUTPUT {
	float4 Shadow : SV_TARGET0;
};

PS_OUTPUT
FS(PS_INPUT input) {
	PS_OUTPUT outRT;
	
	if (AlbedoTex.Sample(SS, input.Texcoord).w < 0.8f) discard;
	outRT.Shadow = input.fDepth.xxxx;

	return outRT;
}