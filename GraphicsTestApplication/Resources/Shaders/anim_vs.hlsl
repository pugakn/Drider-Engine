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
	float2 uvs		: TEXCOORD;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
	float2 uvs		 : TEXCOOR0;
};

VS_OUTPUT VS( VS_INPUT input ) {
    VS_OUTPUT OUT;

    OUT.uvs = input.uvs;

    float4x4 BoneTransform;

	float4 Test = float4(0,0,0,0);

    for(int i = 0; i < 4; ++i)
	{
		int index = input.boneids[i];

		if( index == -1 )
		{
			break;
		}

		BoneTransform += Bones[index] * input.weights[i];

		
	}

	//BoneTransform = float4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

	float4 OutVertex = mul(BoneTransform, input.position);
	float4 OutNormal = float4(mul((float3x3)BoneTransform, input.normal.xyz),0);

    OUT.hposition = mul(WVP, OutVertex);
	//OUT.hposition = input.position;
	//OUT.hnormal = mul(float3x3(World), OutNormal.xyz);
	OUT.hnormal = OutNormal;
    return OUT;
}