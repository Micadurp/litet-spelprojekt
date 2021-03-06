#define LIGHT_COUNT 40

Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txWorldP : register(t2);

SamplerState sampAni
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct PointLight
{
	float4 position;
	float4 diffuse;
	float4 ambient;
	float3 attenuation;
	float range;
	float intensity;
};

struct DirectionalLight
{
	float3 direction;
	float4 diffuse;
	float4 ambient;
};


cbuffer enviromentLight : register(b0)
{
	DirectionalLight dirLight;
}

cbuffer LightArray : register(b1)
{
	PointLight lights[LIGHT_COUNT];
}

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
};


float3 calculateLight(int index, float4 txDiff, float4 txNormal, float4 txWorldPos)
{
	float3 color = { 0.0f, 0.0f, 0.0f };
	float3 lightToPixel = lights[index].position.xyz - txWorldPos.xyz;
	float distance = length(lightToPixel);

	lightToPixel /= distance;

	float lightAmount = dot(lightToPixel, txNormal.xyz);

	if (lightAmount > 0.0f)
	{
		color += lightAmount * txDiff.xyz * lights[index].diffuse.xyz * lights[index].intensity;

		color /= lights[index].attenuation[0] + (lights[index].attenuation[1] * distance) + (lights[index].attenuation[2] * (distance * distance));
	}

	return color;
}


float4 PS_main(VS_OUT input) : SV_TARGET
{
	float4 diffuse = txDiffuse.Sample(sampAni, input.tex);
	float4 normal = txNormal.Sample(sampAni, input.tex);
	float4 worldPos = txWorldP.Sample(sampAni, input.tex);


	float3 finalColor = saturate(dot(dirLight.direction, normal.xyz) * dirLight.diffuse.xyz * diffuse.xyz);
	float3 finalAmbient = diffuse.xyz * dirLight.ambient.xyz;

	
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		// Don't calculate black light
		if (lights[i].diffuse.x > 0.0f && lights[i].diffuse.y > 0.0f && lights[i].diffuse.z > 0.0f)
		{
			finalColor += calculateLight(i, diffuse, normal, worldPos);
		}
	}
	

	finalColor = saturate(finalColor + finalAmbient);

	return float4(finalColor, diffuse.a);
}