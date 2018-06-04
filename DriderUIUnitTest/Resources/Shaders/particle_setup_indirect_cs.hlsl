cbuffer ListCount1 : register(b0)
{
  uint	numAliveParticles;
  uint3	pad1;
};
RWBuffer<uint>	DrawArgs: register(u0);
[numthreads(1, 1, 1)]
void CS( uint3 id : SV_DispatchThreadID )
{
    DrawArgs[0] = 6;	//IndexCount
    DrawArgs[1] = numAliveParticles;	//Instances
    DrawArgs[2] = 0; 
    DrawArgs[3] = 0;
    DrawArgs[4] = 0;
}

