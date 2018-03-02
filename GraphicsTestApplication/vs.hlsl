cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 Bones[200];
}

struct VS_INPUT{
    float4 position : POSITION;
	float4 normal   : NORMAL;	
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
	float4 OutNormal = mul(float4(input.normal.xyz,1), BoneTransform);

    OUT.hposition = mul(WVP, OutVertex);
	//OUT.hposition = input.position;
	//OUT.hnormal = mul( WVP,input.normal);
	OUT.hnormal = float4(input.normal.xyz, 0);
    return OUT;
}