cbuffer ConstantBuffer {
	float4x4 WVP;
  float4x4 Bones[200];
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float  fDepth   : TEXCOORD0;
};

struct PS_OUTPUT {
	float4 Shadow : SV_TARGET0;
};

PS_OUTPUT
FS(PS_INPUT input) {
	PS_OUTPUT outRT;
	
	outRT.Shadow = input.fDepth.xxxx;

	return outRT;
}