cbuffer ConstantBuffer {
	float4 config;
}

struct VS_INPUT{
    float4 position : POSITION;
	float4 normal   : NORMAL;	
	float2 uv  	: TEXCOORD; 
};

struct VS_OUTPUT{
  float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
  float2 htexcoord : TEXCOORD;
};

VS_OUTPUT VS( VS_INPUT input ) {
    VS_OUTPUT OUT;
	OUT.hposition = input.position;
	OUT.hnormal = input.normal;
    OUT.htexcoord = input.uv;
    return OUT;
}