cbuffer ConstantBuffer{
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

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.hposition = mul( World , float4(input.position,1.0) );
	OUT.hnormal = float4(input.normal,1.0);
    return OUT;
}