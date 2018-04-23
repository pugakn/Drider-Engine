Texture2D ShadowTex    : register(t0);

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
  int4     ShadowIndex;
	float4   extraInfo;
	float4x4 WV;
};

struct PS_INPUT {
  float4   Position : SV_POSITION;
  float4   RealPos  : TEXCOORD0;
};

struct PS_OUTPUT {
  float4  Shadow1	: SV_TARGET0;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;
	
	//float depth = (input.RealPos.z / (extraInfo.y - extraInfo.x)); //Lineal
	float depth = (input.RealPos.z / input.RealPos.w); //No Lineal
	
	outRT.Shadow1[0] = depth;
	outRT.Shadow1[1] = depth;
	outRT.Shadow1[2] = depth;
	outRT.Shadow1[3] = depth;

	return outRT;
}