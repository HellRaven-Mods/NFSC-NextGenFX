///////////////////////////////////////////////////////////////////////////////
// HDR Effects
///////////////////////////////////////////////////////////////////////////////

#include "global.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////

shared float4 TextureOffset : TEXTUREANIMOFFSET;

///////////////////////////////////////////////////////////////////////////////

static const int MAX_SAMPLES = 32; // Maximum texture grabs
static const float BRIGHT_PASS_OFFSET = 10.0f; // Offset for BrightPass filter

// Contains	sampling offsets used by the techniques 
shared float4 cavSampleOffsets[MAX_SAMPLES] : cavSampleOffsets;
shared float4 cavSampleWeights[MAX_SAMPLES] : cavSampleWeights;
shared float4 cvBlurParams : cvBlurParams;
shared float4 cvBaseAlphaRef : cvBaseAlphaRef; //BASEALPHAREF;

shared float cfBrightPassThreshold : cfBrightPassThreshold;
shared float cfAdaptedLum : cfAdaptedLum;
shared float cfBloomScale : cfBloomScale;
shared float cfMiddleGray : cfMiddleGray;
   
static const float3 LUMINANCE_VECTOR = float3(0.2125f, 0.7154f, 0.0721f);

#ifndef SCREEN_WIDTH
    #define SCREEN_WIDTH 1920.0f
    #define SCREEN_HEIGHT 1080.0f
#endif

///////////////////////////////////////////////////////////////////////////////

sampler PERLINNOISE_SAMPLER = sampler_state
{
    AddressU = WRAP;
    AddressV = WRAP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(DIFFUSEMAP_TEXTURE)
sampler DIFFUSE_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DIFFUSEMAP_TEXTURE) // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(DEPTHBUFFER_TEXTURE)
sampler DEPTHBUFFER_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DEPTHBUFFER_TEXTURE) // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(MISCMAP1_TEXTURE)
sampler MISCMAP1_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP1_TEXTURE) // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(MISCMAP2_TEXTURE)
sampler MISCMAP2_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP2_TEXTURE) // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

///////////////////////////////////////////////////////////////////////////////

struct VS_INPUT_WORLD
{
    float4 position : POSITION;
    float4 color : COLOR;
    float4 texcoord : TEXCOORD;
};

struct VS_INPUT_SCREEN
{
    float4 position : POSITION;
    float4 texcoord : TEXCOORD;
};

struct VtoP
{
    float4 position : POSITION;
    float4 tex : TEXCOORD0;
};

struct DepthSpriteOut
{
    float4 colour : COLOR0;
    float depth : DEPTH0;
};

///////////////////////////////////////////////////////////////////////////////

VtoP vertex_shader_passthru(const VS_INPUT_SCREEN IN)
{
    VtoP OUT;
    OUT.position = IN.position;
    OUT.tex = IN.texcoord;

    return OUT;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS_ReflectionBlur(const VtoP IN) : COLOR
{
    // In Shader Model 2.0 the number of texture fetches is limited and loops 
    // are unrolled manually.
    // Shader Model 3.0 lifts these restrictions, so we can iterate over the
    // entire sample array in a simple for‐loop.
    // This reduces boilerplate code and makes it easy to change the number of samples.
    float4 sample = float4(0, 0, 0, 0);
    [loop]
    for (int i = 0; i < MAX_SAMPLES; ++i)
    {
        sample += cavSampleWeights[i] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[i].xy);
    }
    // Preserve the original behaviour by applying the 0.75 factor to the accumulated result.
    return sample * 0.75f;
}

///////////////////////////////////////////////////////////////////////////////
float4 GetBase(float2 texUV)
{
    float4 base = tex2D( DIFFUSE_SAMPLER, texUV );
    return base;
}

float4 PS_GaussBlur5x5(const VtoP IN) : COLOR
{
    // Gaussian blur filters traditionally unroll the sample loop because SM2.0
    // doesn’t support dynamic indexing.  Moving to SM3.0 we can simply loop
    // over the samples.  The SAMPLE_COUNT constant defines how many weights
    // and offsets to accumulate.
    static const int SAMPLE_COUNT = 20;
    float4 sample = float4(0, 0, 0, 0);
    [loop]
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        sample += cavSampleWeights[i] * GetBase(IN.tex.xy + cavSampleOffsets[i].xy);
    }
    return sample;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS_DownScale4x4(in float2 vScreenPosition : TEXCOORD0) : COLOR
{
    // Downsample a region of the screen by averaging multiple texels.  SM3.0
    // allows us to iterate over the offsets rather than explicitly summing
    // each term.  The result is divided by the number of samples to compute
    // the average color.
    static const int SAMPLE_COUNT = 16;
    float4 sample = float4(0, 0, 0, 0);
    [loop]
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        sample += GetBase(vScreenPosition + cavSampleOffsets[i].xy);
    }
    return sample / SAMPLE_COUNT;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS_DownScale2x2(in float2 vScreenPosition : TEXCOORD0) : COLOR
{
    // 2×2 downsample using four taps around the current pixel.  Shader Model
    // 3.0 removes the need to manually unroll the taps, so we loop over the
    // first four offsets and average the results.
    static const int SAMPLE_COUNT = 4;
    float4 sample = float4(0, 0, 0, 0);
    [loop]
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        sample += tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[i].xy);
    }
    return sample / SAMPLE_COUNT;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS_DownScale2x2ForMotionBlur(in float2 vScreenPosition : TEXCOORD0) : COLOR
{
    // Fetch base color and depth.  SM3.0 allows us to reorganise the depth
    // computations in a more readable form without worrying about instruction
    // limits.  Motion blur is reduced slightly to prevent overshoot.
    float4 sample = tex2D(DIFFUSE_SAMPLER, vScreenPosition);
    float depthValue = tex2D(DEPTHBUFFER_SAMPLER, vScreenPosition).x;

    // Reduce aggressive motion blur accumulation – scale by 0.75 and blend
    // half of the exaggerated result back into the original colour.
    float motionStrength = saturate(sample.w) * 0.75f;
    sample.xyz = lerp(sample.xyz, sample.xyz * (1.0f + motionStrength), 0.5f);

    // Distance falloff settings derived from blur parameters.  Multiply
    // cvBlurParams.y by 200 to map to scene units.
    float distStart = cvBlurParams.x;
    float distEnd   = cvBlurParams.y * 200.0f;

    // Convert depth buffer values (0–1) into a physical distance factor.  The
    // clamp prevents division by zero and extreme values.  A linear falloff
    // slope and intercept convert the reciprocal depth into a normalised
    // blur amount between 0 and 1.
    float depthFactor = 1.0f / max(0.0001f, 1.0f - depthValue);
    float slope       = -1.0f / (distEnd - distStart);
    float intercept   = -distEnd * slope;
    float scaledDepth = saturate(depthFactor * slope + intercept);

    // Return colour with scaled depth in the alpha channel.
    return float4(sample.xyz, scaledDepth);
}

///////////////////////////////////////////////////////////////////////////////
// Bloom
///////////////////////////////////////////////////////////////////////////////

#define BLOOM_EXPOSURE 2.5f
#define BLOOM_SPREAD_MULTIPLIER 2.25f 
#define BLOOM_BRIGHTPASS_THRESHOLD 0.675f

float3 tonemap_bloom(float3 col)
{
    const float A = 0.22f;
    const float B = 0.50f;
    const float C = 0.10f;
    const float D = 0.20f;
    const float E = 0.015f;
    const float F = 0.20f;
    const float W = 11.20f;

    col *= BLOOM_EXPOSURE;
    col = ((col * (A * col + C * B) + D * E) / (col * (A * col + B) + D * F)) - E / F;
    const float white = 1.0f / (((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F);
    col *= white;
    return col;
}

static const float weights[16] =
{
    0.19859f, 0.18306f, 0.15176f, 0.11306f, 0.07684f, 0.04834f,
    0.02812f, 0.01524f, 0.00776f, 0.00373f, 0.0017f, 0.00073f,
    0.0003f, 0.00012f, 0.00004f, 0.00001f
};

// PASS 1: Horizontal Blur + Brightpass
float4 PS_Bloom_Horizontal_HQ(const VtoP IN) : COLOR
{
    // Horizontal bloom pass.  Shader Model 3.0 allows us to use loops and
    // dynamic array indexing, so we compute the bright pass and blur in a
    // single loop.  The luminance is calculated using the standard
    // LUMINANCE_VECTOR rather than an RGB max; this yields a more perceptually
    // accurate brightness estimate.
    float2 texelSize = float2(1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT);

    // Bright-pass: compute local brightness and scale by a smooth knee around
    // the threshold.  The knee region softens the transition so bright areas
    // aren’t harshly clipped.
    float3 centerColor = tex2D(DIFFUSE_SAMPLER, IN.tex).rgb;
    float threshold    = BLOOM_BRIGHTPASS_THRESHOLD;
    float knee         = threshold * 0.5f;
    float brightness   = dot(centerColor, LUMINANCE_VECTOR);
    float contribution = smoothstep(threshold - knee, threshold + knee, brightness);
    float3 bloomSum    = centerColor * contribution * weights[0];

    // Accumulate samples symmetrically around the current pixel.  Looping is
    // possible in SM3.0 and helps keep the code concise and easy to adjust.
    [loop]
    for (int i = 1; i < 16; ++i)
    {
        float2 offset = float2(texelSize.x * i * BLOOM_SPREAD_MULTIPLIER, 0.0f);

        // Sample to the right
        float3 colorRight   = tex2D(DIFFUSE_SAMPLER, IN.tex + offset).rgb;
        float brightnessR   = dot(colorRight, LUMINANCE_VECTOR);
        float contribRight  = smoothstep(threshold - knee, threshold + knee, brightnessR);
        bloomSum           += colorRight * contribRight * weights[i];

        // Sample to the left
        float3 colorLeft    = tex2D(DIFFUSE_SAMPLER, IN.tex - offset).rgb;
        float brightnessL   = dot(colorLeft, LUMINANCE_VECTOR);
        float contribLeft   = smoothstep(threshold - knee, threshold + knee, brightnessL);
        bloomSum           += colorLeft * contribLeft * weights[i];
    }

    return float4(tonemap_bloom(bloomSum), 1.0f);
}

/*/ PASS 2: Vertical Blur
float4 PS_Bloom_Vertical_HQ(const VtoP IN) : COLOR
{
    float2 texelSize = float2(1.0f / 1280, 1.0f / 720);
    float3 bloomSum = tex2D(DIFFUSE_SAMPLER, IN.tex).rgb * weights[0];
    
    [unroll]
    for (int i = 1; i < 16; i++)
    {
        float2 offset = float2(0.0f, texelSize.y * i * BLOOM_SPREAD_MULTIPLIER);
        bloomSum += tex2D(DIFFUSE_SAMPLER, IN.tex + offset).rgb * weights[i];
        bloomSum += tex2D(DIFFUSE_SAMPLER, IN.tex - offset).rgb * weights[i];
    }
    
    return float4(tonemap_bloom(bloomSum), 1.0f);
}/*/

///////////////////////////////////////////////////////////////////////////////

float4 PS_BlendTextures(in float2 vScreenPosition :	TEXCOORD0 )	: COLOR
{
    float4 result;
    result  = tex2D( DIFFUSE_SAMPLER,vScreenPosition) * cavSampleWeights[0];
    result += tex2D( MISCMAP1_SAMPLER,	vScreenPosition) * cavSampleWeights[1];
    
    return result;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS_CombineReflectionColourAndHeadlightAlpha(in float2 vScreenPosition :	TEXCOORD0 )	: COLOR
{
    float4 result;
    // Flares
    result.xyz = tex2D( DIFFUSE_SAMPLER,vScreenPosition).xyz;
    
    // Headlight
    result.w   = dot(tex2D( MISCMAP1_SAMPLER,vScreenPosition).xyz, LUMINANCE_VECTOR);

    return result;
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// PREP BUFFER SHADER
///////////////////////////////////////////////////////////////////////////////

struct PrepBuffersOut
{
    float4 colour0 : COLOR0;
    float4 colour1 : COLOR1;
    float depth : DEPTH0;
};

struct VS_PassThru_OUT
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
};

VS_PassThru_OUT VS_PassThru(const VS_INPUT_SCREEN IN)
{
    VS_PassThru_OUT OUT;
    OUT.position = IN.position;
    OUT.texcoord = IN.texcoord;

    return OUT;
}

PrepBuffersOut PS_PrepSmokeBuffers(in float2 texCoord : TEXCOORD)
{
    PrepBuffersOut OUT;

    OUT.colour0 = float4(tex2D(DIFFUSE_SAMPLER, texCoord).xyz, 0);
    OUT.colour1 = float4(tex2D(PERLINNOISE_SAMPLER, texCoord).xyz, 0);
    
    float3 depthColor = tex2D(DEPTHBUFFER_SAMPLER, texCoord).rgb;
    depthColor *= float3(65536 * 255.0 / 16777215.0, 256 * 255.0 / 16777215.0, 255.0 / 16777215.0);
    depthColor += float3(1.0 / 4096.0, 1.0 / 1048576.0, 0);
    depthColor += float3(32768, 128, 0);
    depthColor -= float3(32768, 128, 0);
    OUT.depth = dot(depthColor, float3(1, 1, 1));

    return OUT;
}

struct CompositeOut
{
    float4 colour	: COLOR0;
    float  depth	: DEPTH0;
};

CompositeOut PS_CompositeSmoke(in float2 texCoord : TEXCOORD0)
{
    CompositeOut OUT;
    OUT.depth = tex2D(DEPTHBUFFER_SAMPLER, texCoord).x;

    float4 original_backbuffer = tex2D(DIFFUSE_SAMPLER, texCoord);
	
	// Sample into the blurred buffer using a perlin noise offset
    float2 perlin_noise = tex2D(PERLINNOISE_SAMPLER, texCoord).xy * 2 - 1;
    float4 solid_smoke = tex2D(MISCMAP1_SAMPLER, texCoord + perlin_noise / 30);

    OUT.colour.xyz = lerp(original_backbuffer.xyz, solid_smoke, saturate(solid_smoke.w * 1.1));
    OUT.colour.w = solid_smoke.w / 2;

    return OUT;
}

///////////////////////////////////////////////////////////////////////////////

technique CompositeSmoke
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS_PassThru();
        PixelShader	 = compile ps_3_0 PS_CompositeSmoke();
    }
}

technique PrepSmokeBuffers
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS_PassThru();
        PixelShader = compile ps_3_0 PS_PrepSmokeBuffers();
    }
}

technique TwoPassBlur
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_ReflectionBlur();
    }
}

technique GaussBlur5x5
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_GaussBlur5x5();
    }
}

technique DownScale4x4
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_DownScale4x4();
    }
}

technique DownScale2x2
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_DownScale2x2();
    }
}

technique DownScale2x2ForMotionBlur
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_DownScale2x2ForMotionBlur();
    }
}

technique DownScaleForBloom
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_Bloom_Horizontal_HQ();
    }
}

technique BlendTextures
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_BlendTextures();
    }
}

technique CombineReflectionColourAndHeadlightAlpha
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_CombineReflectionColourAndHeadlightAlpha();
    }
}
