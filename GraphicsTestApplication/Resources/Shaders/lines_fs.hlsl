cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
}

float4 FS() : SV_TARGET {
    return float4(1.0, 0.0, 0.0, 1.0);
}