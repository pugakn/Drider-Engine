cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
}

struct VS_INPUT {
    float4 position : POSITION;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
};

VS_OUTPUT VS( VS_INPUT input ) {
    VS_OUTPUT OUT;
	OUT.position = mul(WVP, input.position);
    return OUT;
}