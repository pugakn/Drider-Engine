cbuffer ConstantBuffer{
	float4x4 WVP;
}

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
	float3 n = input.hnormal.xyz;
    float4 color =  float4(n.x, n.y, n.z, 1.0);
    return color;
}