///////////////////////////////////////////////////////////////////////////////
// HDR Effects
///////////////////////////////////////////////////////////////////////////////

#include "global.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bloom
// you can tune the bloom parameters here, recommended to first enable BLOOM_DEBUG in visualtreatment.fx
// enter then exit video options to recompile shader

#define BLOOM_EXPOSURE 1.5
#define BLOOM_BRIGHTPASS_THRESHOLD 0.05

///////////////////////////////////////////////////////////////////////////////

static const int MAX_SAMPLES = 32; // Maximum texture grabs

// Contains	sampling offsets used by the techniques 
shared float4 cavSampleOffsets[MAX_SAMPLES] : cavSampleOffsets;
shared float4 cavSampleWeights[MAX_SAMPLES] : cavSampleWeights;
shared float4 cvBlurParams : cvBlurParams;
shared float4 cvBaseAlphaRef : cvBaseAlphaRef; //BASEALPHAREF;
   
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

float4 PS_Bloom( const VtoP	IN ) : COLOR
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

    return sample * 0.4; // * 1.5
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

float4 PS_DownScale2x2ForMotionBlur(in float2 vScreenPosition : TEXCOORD0) : COLOR
{
    // Sample the color and depth at the given screen position
    float4 sample = tex2D(DIFFUSE_SAMPLER, vScreenPosition);
    float depth = tex2D(DEPTHBUFFER_SAMPLER, vScreenPosition).x;

    // Apply motion blur based on the alpha channel
    sample.xyz += sample.w * sample.xyz; // Accumulate color based on alpha

    // Distance falloff parameters
    const float DIST_FALLOFF_START = cvBlurParams.x; // meters
    const float DIST_FALLOFF_END = cvBlurParams.y * 200; // meters
    const float SLOPE = -1.0 / (DIST_FALLOFF_END - DIST_FALLOFF_START); // slope: how rapid the fall off is
    const float INTERCEPT = -DIST_FALLOFF_END * SLOPE; // y-intercept for depth mapping

    // Transform depth to a more usable form
    depth = saturate((1.0 / (1.0 - depth)) * SLOPE + INTERCEPT); // Invert and scale depth

    // Set the alpha channel to the computed depth
    sample.w = depth;

    return sample; // Return the modified sample
}

float4 PS_DownScaleForBloom(in float2 vScreenPosition : TEXCOORD0) : COLOR
{
    const float BRIGHT_PASS_SCALE = 0.5; // Scale factor for the sample

    // Initialize the sample color
    float3 sample = 0;

    // Manually unroll the loop
    sample += cavSampleWeights[0] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[0].xy).xyz;
    sample += cavSampleWeights[1] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[1].xy).xyz;
    sample += cavSampleWeights[2] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[2].xy).xyz;
    sample += cavSampleWeights[3] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[3].xy).xyz;
    sample += cavSampleWeights[4] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[4].xy).xyz;
    sample += cavSampleWeights[5] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[5].xy).xyz;
    sample += cavSampleWeights[6] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[6].xy).xyz;
    sample += cavSampleWeights[7] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[7].xy).xyz;
    sample += cavSampleWeights[8] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[8].xy).xyz;
    sample += cavSampleWeights[9] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[9].xy).xyz;
    sample += cavSampleWeights[10] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[10].xy).xyz;
    sample += cavSampleWeights[11] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[11].xy).xyz;
    sample += cavSampleWeights[12] * tex2D(DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[12].xy).xyz;

    sample = DeCompressColourSpace(sample);

    // Bright pass threshold processing
    const float kBrightPassThreshold = BLOOM_BRIGHTPASS_THRESHOLD;
    float maxChannel = max(max(sample.x, sample.y), sample.z) - kBrightPassThreshold;

    // Early exit if below threshold
    if (maxChannel < 0)
    {
        return float4(sample * BRIGHT_PASS_SCALE, 0);
    }

    maxChannel = 2 * maxChannel / (kBrightPassThreshold + maxChannel);

    // Combine sample with maxChannel
    return float4(sample * BRIGHT_PASS_SCALE + sample * maxChannel, 0);
}

float4 PS_BlendTextures(in float2 vScreenPosition :	TEXCOORD0 )	: COLOR
{
    float4 result;
    result  = tex2D( DIFFUSE_SAMPLER,vScreenPosition) * cavSampleWeights[0];
    result += tex2D( MISCMAP1_SAMPLER,	vScreenPosition) * cavSampleWeights[1];
    
    return result;
}


DepthSpriteOut PS_DepthSprite(in float2 vScreenPosition : TEXCOORD0 )
{
    DepthSpriteOut OUT;

    OUT.colour = float4(tex2D( DIFFUSE_SAMPLER, vScreenPosition).xyz, 0);
    OUT.depth  = tex2D( DEPTHBUFFER_SAMPLER, vScreenPosition).x;

    return OUT;
}

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
        PixelShader = compile ps_3_0 PS_Bloom();
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

technique DepthSprite
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_DepthSprite();
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
