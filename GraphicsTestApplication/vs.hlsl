cbuffer ConstantBuffer{
	//float4x4 World;  
}

struct VS_INPUT{
    float4 position : POSITION;
	float4 normal   : NORMAL;
	float2 tex 	: TEXCOORD0;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
	float2 tex 	 : TEXCOORD0;
};

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    input.position.w = 1.0;
    //OUT.hposition = mul( World , float4(input.position,1.0) );
	OUT.hposition = input.position;
	OUT.hnormal = input.normal;
	OUT.tex = input.tex;
    return OUT;
}