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
    MIPFILTER = LINEAR;
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
    float4 sample = 0;

    sample += cavSampleWeights[0] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[0].xy);
    sample += cavSampleWeights[1] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[1].xy);
    sample += cavSampleWeights[2] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[2].xy);
    sample += cavSampleWeights[3] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[3].xy);
    sample += cavSampleWeights[4] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[4].xy);
    sample += cavSampleWeights[5] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[5].xy);
    sample += cavSampleWeights[6] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[6].xy);
    sample += cavSampleWeights[7] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[7].xy);
    sample += cavSampleWeights[8] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[8].xy);
    sample += cavSampleWeights[9] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[9].xy);
    sample += cavSampleWeights[10] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[10].xy);
    sample += cavSampleWeights[11] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[11].xy);
    sample += cavSampleWeights[12] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[12].xy);
    sample += cavSampleWeights[13] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[13].xy);
    sample += cavSampleWeights[14] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[14].xy);
    sample += cavSampleWeights[15] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[15].xy);
    sample += cavSampleWeights[16] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[16].xy);
    sample += cavSampleWeights[17] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[17].xy);
    sample += cavSampleWeights[18] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[18].xy);
    sample += cavSampleWeights[19] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[19].xy);
    sample += cavSampleWeights[20] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[20].xy);
    sample += cavSampleWeights[21] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[21].xy);
    sample += cavSampleWeights[22] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[22].xy);
    sample += cavSampleWeights[23] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[23].xy);
    sample += cavSampleWeights[24] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[24].xy);
    sample += cavSampleWeights[25] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[25].xy);
    sample += cavSampleWeights[26] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[26].xy);
    sample += cavSampleWeights[27] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[27].xy);
    sample += cavSampleWeights[28] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[28].xy);
    sample += cavSampleWeights[29] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[29].xy);
    sample += cavSampleWeights[30] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[30].xy);
    sample += cavSampleWeights[31] * tex2D(DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[31].xy);

    return sample * 0.75;
}

///////////////////////////////////////////////////////////////////////////////
float4 GetBase(float2 texUV)
{
    float4 base = tex2D( DIFFUSE_SAMPLER, texUV );
    return base;
}

float4 PS_GaussBlur5x5(	const VtoP IN )	: COLOR
{
    const int SAMPLE_COUNT = 20; // Define the new number of samples
    float4	sample = 0.0f;
    
    // Manually unroll the loop for performance
    sample += cavSampleWeights[0] * GetBase(IN.tex.xy + cavSampleOffsets[0].xy);
    sample += cavSampleWeights[1] * GetBase(IN.tex.xy + cavSampleOffsets[1].xy);
    sample += cavSampleWeights[2] * GetBase(IN.tex.xy + cavSampleOffsets[2].xy);
    sample += cavSampleWeights[3] * GetBase(IN.tex.xy + cavSampleOffsets[3].xy);
    sample += cavSampleWeights[4] * GetBase(IN.tex.xy + cavSampleOffsets[4].xy);
    sample += cavSampleWeights[5] * GetBase(IN.tex.xy + cavSampleOffsets[5].xy);
    sample += cavSampleWeights[6] * GetBase(IN.tex.xy + cavSampleOffsets[6].xy);
    sample += cavSampleWeights[7] * GetBase(IN.tex.xy + cavSampleOffsets[7].xy);
    sample += cavSampleWeights[8] * GetBase(IN.tex.xy + cavSampleOffsets[8].xy);
    sample += cavSampleWeights[9] * GetBase(IN.tex.xy + cavSampleOffsets[9].xy);
    sample += cavSampleWeights[10] * GetBase(IN.tex.xy + cavSampleOffsets[10].xy);
    sample += cavSampleWeights[11] * GetBase(IN.tex.xy + cavSampleOffsets[11].xy);
    sample += cavSampleWeights[12] * GetBase(IN.tex.xy + cavSampleOffsets[12].xy);
    sample += cavSampleWeights[13] * GetBase(IN.tex.xy + cavSampleOffsets[13].xy);
    sample += cavSampleWeights[14] * GetBase(IN.tex.xy + cavSampleOffsets[14].xy);
    sample += cavSampleWeights[15] * GetBase(IN.tex.xy + cavSampleOffsets[15].xy);
    sample += cavSampleWeights[16] * GetBase(IN.tex.xy + cavSampleOffsets[16].xy);
    sample += cavSampleWeights[17] * GetBase(IN.tex.xy + cavSampleOffsets[17].xy);
    sample += cavSampleWeights[18] * GetBase(IN.tex.xy + cavSampleOffsets[18].xy);
    sample += cavSampleWeights[19] * GetBase(IN.tex.xy + cavSampleOffsets[19].xy);

    return sample; // Return the accumulated sample
}

///////////////////////////////////////////////////////////////////////////////

float4 PS_DownScale4x4(in float2 vScreenPosition : TEXCOORD0) : COLOR
{
    const int SAMPLE_COUNT = 16; // Define the number of samples
    float4 sample = 0;

    // Manually unroll the loop for performance
    sample += GetBase(vScreenPosition + cavSampleOffsets[0].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[1].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[2].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[3].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[4].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[5].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[6].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[7].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[8].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[9].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[10].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[11].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[12].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[13].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[14].xy);
    sample += GetBase(vScreenPosition + cavSampleOffsets[15].xy);

    // Calculate the average color
    return sample / SAMPLE_COUNT; // Return the averaged sample
}

///////////////////////////////////////////////////////////////////////////////

float4 PS_DownScale2x2(in float2 vScreenPosition : TEXCOORD0) : COLOR
{
    const int SAMPLE_COUNT = 4; // Define the number of samples
    float4 sample = 0;

    // Manually unroll the loop for performance
    sample += tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[0].xy);
    sample += tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[1].xy);
    sample += tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[2].xy);
    sample += tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[3].xy);

    // Calculate the average color
    return sample / SAMPLE_COUNT; // Return the averaged sample
}

///////////////////////////////////////////////////////////////////////////////

float4 PS_DownScale2x2ForMotionBlur(in float2 vScreenPosition : TEXCOORD0) : COLOR
{
    // Sample color and depth
    float4 sample = tex2D(DIFFUSE_SAMPLER, vScreenPosition);
    float depth = tex2D(DEPTHBUFFER_SAMPLER, vScreenPosition).x;

    // Fix: Ensure motion blur intensity is stable
    float motionStrength = sample.w * 0.75; // Reduce aggressive motion blur accumulation
    sample.xyz = lerp(sample.xyz, sample.xyz * (1.0 + motionStrength), 0.5);

    // Distance falloff settings
    const float DIST_FALLOFF_START = cvBlurParams.x; // meters
    const float DIST_FALLOFF_END = cvBlurParams.y * 200; // meters

    // Fix: More stable depth transformation
    float depthFactor = 1.0 / max(0.0001, (1.0 - depth)); // Prevent divide by zero
    float slope = -1.0 / (DIST_FALLOFF_END - DIST_FALLOFF_START);
    float intercept = -DIST_FALLOFF_END * slope;
    
    // Stabilized depth-based motion blur scaling
    depth = saturate(depthFactor * slope + intercept);

    // Output the corrected depth
    sample.w = saturate(depth);

    return sample;
}

///////////////////////////////////////////////////////////////////////////////
// Bloom
///////////////////////////////////////////////////////////////////////////////

#define BLOOM_EXPOSURE 0.40
#define BLOOM_BRIGHTPASS_THRESHOLD 0.375

float3 tonemap(float3 col, float exposure)
{
    const float A = 0.220;
    const float B = 0.500;
    const float C = 0.100;
    const float D = 0.200;
    const float E = 0.015;
    const float F = 0.200;
    const float W = 11.20;

    col *= exposure;

    col = ((col * (A * col + C * B) + D * E) / (col * (A * col + B) + D * F)) - E / F;
    const float white = 1.0 / (((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F);
    col *= white;
    return col;
}

float4 PS_DownScaleForBloom(in float2 vScreenPosition : TEXCOORD0) : COLOR
{
    float3 bloomSum = 0.0;
    
    // Bloom settings
    float blurStrength = 0.002; // You can increase for stronger blur
    float2 blurDirection = float2(1.0, 0.0); // Directional blur (horizontal). Use float2(1,1) for radial.

    // Gaussian weights (normalize as needed)
    const float weights[8] =
    {
        0.227027f, 0.1945946f, 0.1216216f, 0.054054f,
        0.016216f, 0.005405f, 0.002f, 0.001f
    };

    // Sample center pixel first
    float3 colorSample = tex2Dlod(DIFFUSE_SAMPLER, float4(vScreenPosition, 0, 1)).rgb;

    // Bright-pass thresholding (soft)
    float brightness = max(colorSample.r, max(colorSample.g, colorSample.b));
    float threshold = BLOOM_BRIGHTPASS_THRESHOLD;
    float knee = threshold * 0.75; // Controls softness of threshold falloff

    float soft = clamp((brightness - threshold + knee) / (2.0 * knee), 0.0, 1.0);
    float contribution = max(brightness - threshold, 0.0) + soft;

    bloomSum += weights[0] * colorSample * contribution;
    
    // Loop for additional samples in blurDirection
    for (int i = 1; i < 8; i++)
    {
        float offset = blurStrength * float(i);

        // Positive offset sample
        float2 samplePos1 = vScreenPosition + blurDirection * offset;
        float3 color1 = tex2Dlod(DIFFUSE_SAMPLER, float4(samplePos1, 0, 1)).rgb;

        float brightness1 = max(color1.r, max(color1.g, color1.b));
        float soft1 = clamp((brightness1 - threshold + knee) / (2.0 * knee), 0.0, 1.0);
        float contrib1 = max(brightness1 - threshold, 0.0) + soft1;

        bloomSum += weights[i] * color1 * contrib1;

        // Negative offset sample (symmetric blur)
        float2 samplePos2 = vScreenPosition - blurDirection * offset;
        float3 color2 = tex2Dlod(DIFFUSE_SAMPLER, float4(samplePos2, 0, 1)).rgb;

        float brightness2 = max(color2.r, max(color2.g, color2.b));
        float soft2 = clamp((brightness2 - threshold + knee) / (2.0 * knee), 0.0, 1.0);
        float contrib2 = max(brightness2 - threshold, 0.0) + soft2;

        bloomSum += weights[i] * color2 * contrib2;
    }
    
    // Optional: Tonemap the bloom result if you want it adjusted for LDR output.
    bloomSum = CompressColourSpace(bloomSum);
    bloomSum = tonemap(bloomSum, BLOOM_EXPOSURE);
    
    return float4(bloomSum, 1.0);
}

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
    
    float3 depthColor = tex2D(DEPTHBUFFER_SAMPLER, texCoord).arg;
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
        PixelShader = compile ps_3_0 PS_DownScaleForBloom();
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