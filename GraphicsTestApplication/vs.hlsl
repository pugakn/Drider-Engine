cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
	float4x4 Bones[200];
}

struct VS_INPUT{
    float4 position : POSITION;
	float4 normal   : NORMAL;	
	float4 binormal : BINORMAL;
	float4 tangent  : TANGENT;
	float4 weights  : BONEWEIGHTS;
	int4 boneids  	: BONEIDS; 
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
};

VS_OUTPUT VS( VS_INPUT input ) {
    VS_OUTPUT OUT;

    float4x4 BoneTransform;

    for(int i = 0; i < 4; ++i)
	{
		int index = input.boneids[i];

		if( index == -1 )
		{
			break;
		}

		BoneTransform += Bones[index] * input.weights[i];
	}

	float4 OutVertex = mul(input.position, BoneTransform);
	float4 OutNormal = float4(mul(input.normal.xyz, (float3x3)BoneTransform), 0);

    OUT.hposition = mul(WVP, OutVertex);
	//OUT.hposition = input.position;
	//OUT.hnormal = mul(float3x3(World), OutNormal.xyz);
	OUT.hnormal = OutNormal;
    return OUT;
}