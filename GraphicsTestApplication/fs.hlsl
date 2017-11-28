cbuffer ConstantBuffer{
    float4x4 World;
}

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
    float4 color =  float4(input.hnormal.rgb,1.0);	
    return color;
}