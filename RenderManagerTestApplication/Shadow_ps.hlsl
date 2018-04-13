Texture2D ShadowTex    : register(t0);

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
  int4      ShadowIndex;
};

struct PS_INPUT {
  float4   Position : SV_POSITION;
  float4   RealPos  : POSITION;
};

struct PS_OUTPUT {
  float4 Shadow1	: SV_TARGET0;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;
	
	outRT.Shadow1	 = float4(1.0f * ShadowIndex[0],
													1.0f * ShadowIndex[1],
													1.0f * ShadowIndex[2],
													1.0f * ShadowIndex[3]);

	return outRT;
}