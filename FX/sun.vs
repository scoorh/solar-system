cbuffer mb
{
	matrix w;
	matrix v;
	matrix p;
};


cbuffer lmb 
{
	matrix vp;
};


struct vin
{
    float4 p : POSITION;    
	float2 tc : TEXCOORD0;
};


struct pin
{
	float4 p : SV_POSITION;
	float2 tc : TEXCOORD0;
	float4 l : TEXCOORD1;
	float4 nocl : TEXCOORD2;
};


pin vs(vin input)
{
    pin output;    

	input.p.w = 1.0f;	
    output.p = mul(input.p, w);
    output.p = mul(output.p, v);
    output.p = mul(output.p, p);  
	output.tc = input.tc;    

	float4 l = mul(float4(0,0,0,1),vp);
	output.nocl = l;

	l.xyz /= l.w;
	l.x = 0.5f + (l.x / 2.0f);
	l.y = 1.0f - (0.5f + (l.y / 2.0f));	
	output.l = l;

    return output;
}