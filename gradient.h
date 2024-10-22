shared float4 cvGradientParams					: REG_cvGradientParams;  // SCREEN X Y  & R & intensity
shared float3 cvGradientColour					: REG_cvGradientColour;  // RGB
shared float3 cvGradientColourE					: REG_cvGradientColourE;  // RGB
shared float3 cvScreenRez						: REG_ScreenRez;  		// X & Y res


float3 AddGradient(float3 result, float2 texCoord)
{
    // Define constants
    const float maxRad = cvGradientParams.z; // X rad

    // Calculate adjusted texture coordinates
    float2 adjustedTexCoord = float2(texCoord.x, texCoord.y * (cvScreenRez.y / cvGradientParams.z) + cvGradientParams.y);

    // Calculate blend factor
    float blend = saturate(1 - (length(cvGradientParams.xy - adjustedTexCoord.xy) / maxRad));

    // Interpolate between the two colors based on the blend factor
    float3 finalColor = lerp(cvGradientColour, cvGradientColourE, blend);

    // Combine the final color with the original result color
    return lerp(result, finalColor, blend);
}
