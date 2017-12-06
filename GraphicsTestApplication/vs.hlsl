cbuffer ConstantBuffer {
	float4x4 WVP;
}

struct VS_INPUT{
    float3 position : POSITION;
	float3 normal   : NORMAL;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
};

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.hposition = mul( WVP, float4(input.position.xyz, 1.0));
	//OUT.hposition = input.position;
	OUT.hnormal = float4(input.normal.xyz, 0);
    return OUT;
}