cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
}

struct VS_INPUT{
    float4 position : POSITION;
	float4 normal   : NORMAL;	
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
};

VS_OUTPUT VS( VS_INPUT input ) {
    VS_OUTPUT OUT;

	OUT.hposition = mul(WVP, input.position);
	OUT.hnormal = normalize(float4(mul((float3x3)World, input.normal.xyz), 0.0));
    return OUT;
}