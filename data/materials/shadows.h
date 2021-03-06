

#if SHADOWS_DEPTH

float depthShadowPCF (shTexture2D shadowMap, float4 shadowMapPos, float2 offset, float bias)
{
	float4 smp = shadowMapPos / shadowMapPos.w;

	//float3 o = float3(offset.xy, -offset.x) * 0.3;
	float3 o = float3(0.0005, 0.0005, 0.0005);

	float c =   (smp.z <= bias + shSample(shadowMap, smp.xy - o.xy).r) ? 1 : 0;  // top left
	c +=        (smp.z <= bias + shSample(shadowMap, smp.xy + o.xy).r) ? 1 : 0;  // bottom right
	c +=        (smp.z <= bias + shSample(shadowMap, smp.xy + o.zy).r) ? 1 : 0;  // bottom left
	c +=        (smp.z <= bias + shSample(shadowMap, smp.xy - o.zy).r) ? 1 : 0;  // top right
	return c / 4;
}

#else

float depthShadowPCF (shTexture2D shadowMap, float4 shadowMapPos, float2 offset, float bias)
{
	float4 smp = shadowMapPos / shadowMapPos.w;

	//float3 o = float3(offset.xy, -offset.x) * 0.3;
	float3 o = float3(0.0005, 0.0005, 0.0005);

	float c =   shSample(shadowMap, smp.xy - o.xy).r;  // top left
	c +=        shSample(shadowMap, smp.xy + o.xy).r;  // bottom right
	c +=        shSample(shadowMap, smp.xy + o.zy).r;  // bottom left
	c +=        shSample(shadowMap, smp.xy - o.zy).r;  // top right
	return c / 4  + bias * 0.0000001;  // because of param bias not found
}

#endif


float pssmDepthShadow(
		float4 lightSpacePos0, float2 invShadowmapSize0, shTexture2D shadowMap0,
		float4 lightSpacePos1, float2 invShadowmapSize1, shTexture2D shadowMap1,
		float4 lightSpacePos2, float2 invShadowmapSize2, shTexture2D shadowMap2,
		float depth, float3 pssmSplitPoints, float bias)

{
	float shadow;

	float pcf1 = depthShadowPCF(shadowMap0, lightSpacePos0, invShadowmapSize0, bias);
	float pcf2 = depthShadowPCF(shadowMap1, lightSpacePos1, invShadowmapSize1, bias);
	float pcf3 = depthShadowPCF(shadowMap2, lightSpacePos2, invShadowmapSize2, bias);

	if (depth < pssmSplitPoints.x)
		shadow = pcf1;
	else if (depth < pssmSplitPoints.y)
		shadow = pcf2;
	else
		shadow = pcf3;

	return shadow;
}
