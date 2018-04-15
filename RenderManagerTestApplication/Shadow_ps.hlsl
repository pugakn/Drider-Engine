Texture2D ShadowTex    : register(t0);

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
  int4     ShadowIndex;
	float4   extraInfo;
};

struct PS_INPUT {
  float4   Position : SV_POSITION;
};

struct PS_OUTPUT {
  float4  Shadow1	: SV_TARGET0;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;
	
	float depth = (input.Position.z / input.Position.w);
	
	outRT.Shadow1[0] = depth;
	outRT.Shadow1[1] = 0.0f;
	outRT.Shadow1[2] = 0.0f;
	outRT.Shadow1[3] = 0.0f;

	return outRT;
}