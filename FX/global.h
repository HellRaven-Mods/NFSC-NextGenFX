#ifndef GLOBAL_H
#define GLOBAL_H

#define DECLARE_TEXTURE(textured)	                 shared texture textured : textured;
#define ASSIGN_TEXTURE(textured)	                 Texture = <textured>;
#define DECLARE_MINFILTER(default_filter)			 MINFILTER = default_filter;
#define DECLARE_MAGFILTER(default_filter)			 MAGFILTER = default_filter;
#define DECLARE_MIPFILTER(default_filter)			 MIPFILTER = default_filter;
#define COMPILETIME_BOOL bool
#define cmWorldViewProj WorldViewProj

/////////////////////////////////////////////////////////////////////////////////////////
// Standard Calculations
/////////////////////////////////////////////////////////////////////////////////////////

float4x4	cmWorldViewProj			: WorldViewProj;
float4		cvScreenOffset			: cvScreenOffset;
float4		cvVertexPowerBrightness : cvVertexPowerBrightness;

float4 world_position(float4 screen_pos)
{
    float4 p = mul(screen_pos, cmWorldViewProj);
    p.xy += cvScreenOffset.xy * p.w;
    return p;
}

float4 screen_position(float4 screen_pos)
{
    screen_pos.xy += cvScreenOffset.xy;
    return screen_pos;
}

float4 CalcVertexColour(float4 colour)
{
    // Apply a power curve and brightness scale to the incoming colour.  Clamp
    // each component afterwards to prevent overbright values propagating into
    // later shader stages.  The alpha channel is preserved from the original
    // input.
    float4 result = pow(max(colour, 0), cvVertexPowerBrightness.x) * cvVertexPowerBrightness.y;
    result = saturate(result);
    result.w = colour.w;
    return result;
}

float3 ScaleHeadLightIntensity(float3 colour)
{
    // Scale the headlight intensity and clamp to [0,1] to avoid overbright values.
    float3 result = colour * cvVertexPowerBrightness.z;
    return saturate(result);
}

/////////////////////////////////////////////////////////////////////////////////////////
// HDR Colour Space compression
// Convert to a log or psudeo-log colour space to save high dynamic range data
/////////////////////////////////////////////////////////////////////////////////////////

float3 CompressColourSpace(float3 colour)
{
    return colour / (1.0f + colour);
}

float3 DeCompressColourSpace(float3 compressed)
{
    return compressed / max(1e-5f, 1.0f - compressed);
}

/////////////////////////////////////////////////////////////////////////////////////////
// RGBE8 Encoding/Decoding
// The RGBE8 format stores a mantissa per color channel and a shared exponent 
// stored in alpha. Since the exponent is shared, it's computed based on the
// highest intensity color component. The resulting color is RGB * 2^Alpha,
// which scales the data across a logarithmic scale.
/////////////////////////////////////////////////////////////////////////////////////////

float4 EncodeRGBE8(in float3 rgb)
{
    float4 vEncoded;

    // Determine the largest color component
    // Clamp the maximum component to a small positive value to avoid taking
    // log2(0) when all RGB channels are zero. 
    // Without this guard the exponent would become -Inf and contaminate
    // downstream calculations.
    float maxComponent = max(max(rgb.r, rgb.g), rgb.b);
    maxComponent = max(maxComponent, 1e-6);

    // Round to the nearest integer exponent
    float fExp = ceil(log2(maxComponent));

    // Divide the components by the shared exponent
    vEncoded.rgb = rgb / exp2(fExp);

    // Store the shared exponent in the alpha channel
    vEncoded.a = (fExp + 128) / 255;

    return vEncoded;
}

/////////////////////////////////////////////////////////////////////////////////////////

float3 DecodeRGBE8(in float4 rgbe)
{
    float3 vDecoded;

    // Retrieve the shared exponent
    float fExp = rgbe.a * 255 - 128;

    // Multiply through the color components
    vDecoded = rgbe.rgb * exp2(fExp);

    // Clamp the decoded HDR colour to [0, +inf).  Negative values can arise
    // from denormalised inputs or rounding errors and will cause issues in
    // subsequent operations.  Use saturate to clamp to [0,1] for safety.
    vDecoded = saturate(vDecoded);

    return vDecoded;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Colour space conversions (sRGB ↔ linear)
// These helpers convert between sRGB and linear colour spaces using a simple
// power approximation.  Use srgbToLinear when sampling sRGB textures and
// linearToSrgb when writing out colours for presentation.
/////////////////////////////////////////////////////////////////////////////////////////
float3 srgbToLinear(float3 c)
{
    return pow(max(c, 0), 2.2);
}

float3 linearToSrgb(float3 c)
{
    return pow(max(c, 0), 1.0 / 2.2);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Utility pseudo-random generator
// Generates a deterministic pseudo-random value in the range [0,1] from a 2D seed.
// Useful for adding variation to particle rotations or offsets without requiring
// additional textures.  See particles.fx for an example use.
/////////////////////////////////////////////////////////////////////////////////////////
float rand2d(float2 seed)
{
    return frac(sin(dot(seed, float2(12.9898, 78.233))) * 43758.5453);
}

/////////////////////////////////////////////////////////////////////////////////////////
#endif
