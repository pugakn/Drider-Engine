Texture2D ShadowSt : register(t0);
Texture2D ShadowNd : register(t1);
Texture2D ShadowRd : register(t2);
Texture2D ShadowLt : register(t3);

SamplerState SS;

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

struct PS_OUTPUT {
	float4 Shadow : SV_TARGET0;
};

PS_OUTPUT
FS(PS_INPUT input) {
	PS_OUTPUT outRT;

	float2 uv = input.Texcoord;
	
	outRT.Shadow = float4(ShadowSt.Sample(SS, uv).x,
  										  ShadowNd.Sample(SS, uv).x,
  										  ShadowRd.Sample(SS, uv).x,
  										  ShadowLt.Sample(SS, uv).x);
	
	return outRT;
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
/*
//#define TXWIDTH 1920
//#define TXHEIGHT 1080

Texture2D ShadowSt : register(t0);
Texture2D ShadowNd : register(t1);
Texture2D ShadowRd : register(t2);
Texture2D ShadowLt : register(t3);

SamplerState SS : register(s0);

RWTexture2D<float> ShadowTex : register(t4);

[numthreads(1, TXHEIGHT, 1)]
void
CS(uint3 id : SV_DispatchThreadID) {

	float2 uv = input.Texcoord;
	
	for (int i = 0 i < TXWIDTH; ++i) {
		ShadowTex[i][id.y] = float4(ShadowSt.Sample(SS, uv).x,
																ShadowNd.Sample(SS, uv).x,
																ShadowRd.Sample(SS, uv).x,
																ShadowLt.Sample(SS, uv).x);
	}
}
*/