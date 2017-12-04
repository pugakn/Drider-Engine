Texture2D shaderTexture;
SamplerState SampleType;

cbuffer ConstantBuffer{
    //float4x4 World;
}

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
	float2 tex 	 : TEXCOORD0;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
	float4 textureColor;
    //textureColor = float4(input.hnormal.rgb,1.0);
    textureColor = float4(1,1,0,1);
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    return textureColor;
}