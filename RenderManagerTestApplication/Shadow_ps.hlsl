cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
  float4   DirectionalLight;
};

struct PS_INPUT {
  float4   Position : SV_POSITION;
  float4   RealPos  : POSITION;
};

struct PS_OUTPUT {
  float4 Shadow	  	: SV_TARGET0;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;

	float4 position	= input.RealPos;

	//outRT.Shadow	 = position;
	outRT.Shadow	 = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return outRT;
}