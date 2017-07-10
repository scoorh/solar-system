cbuffer mb
{
	matrix w;
	matrix v;
	matrix p;
};


struct vin
{
    float4 p : POSITION;
	float2 tc : TEXCOORD0;
	float3 n : NORMAL;
    
};


struct pin
{
    float4 p : SV_POSITION;
	float3 pW : POSITION;
	float2 tc : TEXCOORD0;
	float3 n : NORMAL;
};


pin vs(vin input)
{
    pin output;    

    input.p.w = 1.0f;	
    output.p = mul(input.p, w);
    output.p = mul(output.p, v);
    output.p = mul(output.p, p);    	
	output.tc = input.tc;
	output.n = mul(input.n, (float3x3)w);
	output.n = normalize(output.n);
	output.pW = mul(input.p,w);
    
    return output;
}