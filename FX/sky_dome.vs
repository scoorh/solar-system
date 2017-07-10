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
	float t : PSIZE;   
};


struct pin
{
    float4 p : SV_POSITION;
	float2 tc : TEXCOORD0;
	float t : PSIZE;
};


pin vs(vin input)
{
    pin output;    

    input.p.w = 1.0f;	
    output.p = mul(input.p, w);
    output.p = mul(output.p, v);
    output.p = mul(output.p, p);    	
	output.tc = input.tc;
	 
    return output;
}