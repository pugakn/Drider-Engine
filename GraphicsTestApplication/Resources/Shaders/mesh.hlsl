cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
}

struct VS_INPUT{
    float4 position : POSITION;
	float4 normal   : NORMAL;	
	float4 binormal : BINORMAL;
	float4 tangent  : TANGENT;
	float4 weights  : BONEWEIGHTS;
	int4 boneids  	: BONEIDS; 
	float2 uvs		: TEXCOORD;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float4 hnormal   : NORMAL;
    float2 uvs	     : TEXCOORD0;
};

VS_OUTPUT VS( VS_INPUT input ) {
    VS_OUTPUT OUT;
    OUT.uvs = input.uvs;
	OUT.hposition = mul(WVP, input.position);
	OUT.hnormal = input.normal;//normalize(float4(mul((float3x3)World, input.normal.xyz), 0.0));
    return OUT;
}