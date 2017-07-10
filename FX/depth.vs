cbuffer mb
{
	matrix w;
	matrix v;
	matrix p;
};


struct vin
{
    float4 p : POSITION;    
};


struct pin
{
    float4 p : SV_POSITION;
	float4 dp : TEXTURE0;
};


pin vs(vin input)
{
    pin output;    

    input.p.w = 1.0f;	
    output.p = mul(input.p, w);
    output.p = mul(output.p, v);
    output.p = mul(output.p, p);  
	output.dp = output.p;
    
    return output;
}