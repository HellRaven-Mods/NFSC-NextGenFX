#ifndef VISUALTREATMENT_H
#define VISUALTREATMENT_H

/********************************************************************

created:	2024/02/03
updated:    2025/08/17
filename: 	visualtreatment.h
file base:	visualtreatment
file ext:	h
author:		HellRaven Mods

purpose:	Visualtreatment-Shader for NFS Carbon

*********************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTANTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FLASH_SCALE		    cvVisualTreatmentParams.x
#define COP_INTRO_SCALE     cvVisualTreatmentParams.z
#define BREAKER_INTENSITY   cvVisualTreatmentParams.y
#define VIGNETTE_SCALE		cvVisualTreatmentParams.w

shared float4x4 cmWorldView                         : cmWorldView; //WORLDVIEW
//shared float4x4	cmWorldViewProj                 : WorldViewProj;
shared float4x4 cmWorldMat                          : cmWorldMat; //local to world 

shared float3 cvLocalEyePos                         : cvLocalEyePos; //LOCALEYEPOS;
shared float4 cvBlurParams                          : cvBlurParams;
shared float4 cvTextureOffset                       : cvTextureOffset;
shared float4 cvVisualEffectFadeColour              : cvVisualEffectFadeColour;

// The per-color weighting to be used for luminance	calculations in	RGB	order.
static const float3	LUMINANCE_VECTOR = float3(0.2126f, 0.7152f, 0.0722f);
static const int MAX_SAMPLES = 16;	// Maximum texture grabs

// Contains	sampling offsets used by the techniques 
shared float4 cavSampleOffsetWeights[MAX_SAMPLES]	: REG_cavSampleOffsetWeights;
shared float4 cavSampleOffsets[MAX_SAMPLES]         : REG_cavSampleOffsets;

// Tone	mapping	variables
shared float cfBloomScale                           : cfBloomScale;
shared float cfMiddleGray                           : REG_cfMiddleGray;
shared float cfBrightPassThreshold                  : REG_cfBrightPassThreshold;

uniform float gAverageLuminance;
uniform float gExposure;

shared float4 Coeffs0                               : CURVE_COEFFS_0;
shared float4 Coeffs1                               : CURVE_COEFFS_1;

shared float cfVignetteScale                        : cfVignetteScale;
shared float4 cvVisualTreatmentParams               : cvVisualTreatmentParams;
shared float4 cvVisualTreatmentParams2              : cvVisualTreatmentParams2;
shared float cfVisualEffectVignette                 : cfVisualEffectVignette;
shared float cfVisualEffectBrightness               : cfVisualEffectBrightness;

// Depth of Field variables
shared float4 cvDepthOfFieldParams : cvDepthOfFieldParams; //DEPTHOFFIELD_PARAMS;
shared bool cbDrawDepthOfField : cbDrawDepthOfField;
shared bool cbDepthOfFieldEnabled : cbDepthOfFieldEnabled;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Samplers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
texture custom_tex1;
sampler CUSTOM_SAMPLER = sampler_state
{
    Texture = <custom_tex1>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

texture custom_tex2;
sampler CUSTOM_SAMPLER1 = sampler_state
{
    Texture = <custom_tex2>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

texture custom_tex3;
sampler CUSTOM_SAMPLER2 = sampler_state
{
    Texture = <custom_tex3>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

sampler SSAO_SAMPLER = sampler_state
{
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(DIFFUSEMAP_TEXTURE)
sampler	DIFFUSE_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DIFFUSEMAP_TEXTURE)
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(MOTIONBLUR_TEXTURE)
sampler	MOTIONBLUR_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MOTIONBLUR_TEXTURE)
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(DEPTHBUFFER_TEXTURE)
sampler	DEPTHBUFFER_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DEPTHBUFFER_TEXTURE)
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(NORMALMAP_TEXTURE)
sampler NORMALMAP_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(NORMALMAP_TEXTURE)
    AddressU = WRAP;
    AddressV = WRAP;
    DECLARE_MIPFILTER(LINEAR)
    DECLARE_MINFILTER(LINEAR)
    DECLARE_MAGFILTER(LINEAR)
};

DECLARE_TEXTURE(VOLUMEMAP_TEXTURE)
sampler VOLUMEMAP_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(VOLUMEMAP_TEXTURE)
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

sampler2D MISCMAP1_SAMPLER = sampler_state
{
    AddressU = CLAMP;
    AddressV = CLAMP;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

DECLARE_TEXTURE(MISCMAP2_TEXTURE)
sampler	MISCMAP2_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP2_TEXTURE)
    AddressU = CLAMP;
    AddressV = WRAP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(MISCMAP3_TEXTURE)
sampler BLOOM_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP3_TEXTURE)
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(MISCMAP6_TEXTURE)			
sampler	MISCMAP6_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP6_TEXTURE)			
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(BLENDVOLUMEMAP_TEXTURE)
sampler BLENDVOLUMEMAP_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(BLENDVOLUMEMAP_TEXTURE)
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(HEIGHTMAP_TEXTURE)
sampler HEIGHTMAP_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(HEIGHTMAP_TEXTURE)
    AddressU = CLAMP;
    AddressV = CLAMP;
    DECLARE_MIPFILTER(LINEAR)
    DECLARE_MINFILTER(LINEAR)
    DECLARE_MAGFILTER(LINEAR)
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Depth Sprite
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct DepthSpriteOut
{
float4 colour	: COLOR0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_INPUT_SCREEN
{
    float4 position : POSITION;
    float4 tex0 : TEXCOORD0;
    float4 tex1 : TEXCOORD1;
};

struct VtoP
{
    float4 position : POSITION;
    float4 tex0 : TEXCOORD0;
    float4 tex1 : TEXCOORD1;
};

VtoP vertex_shader_passthru(const VS_INPUT_SCREEN IN)
{
    VtoP OUT;
    OUT.position = IN.position;
    OUT.tex0 = IN.tex0;
    OUT.tex1 = IN.tex1;

    return OUT;
}

float4 PS_PassThru(const VtoP IN) : COLOR
{
    float4 OUT;

    float4 diffuse = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);	// * IN;
    OUT.xyz = diffuse.xyz;
    OUT.w = diffuse.w;

    return OUT;
}

struct PS_INPUT
{
    float4 position : POSITION;
    float4 tex0 : TEXCOORD0;
    float4 tex1 : TEXCOORD1;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate Texelsize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float2 texelSizeL0 = float2(1.0 / SCREEN_WIDTH, 1.0 / SCREEN_HEIGHT);
float2 texelSizeL1 = float2(1.0 / (SCREEN_WIDTH / 2), 1.0 / (SCREEN_HEIGHT / 2));
float2 texelSizeL2 = float2(1.0 / (SCREEN_WIDTH / 4), 1.0 / (SCREEN_HEIGHT / 4));
float2 texelSizeL3 = float2(1.0 / (SCREEN_WIDTH / 8), 1.0 / (SCREEN_HEIGHT / 8));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bright-Pass
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 BrightPass(float2 texcoord : TEXCOORD0) : COLOR0
{
    float4 color = tex2D(DIFFUSE_SAMPLER, texcoord);

    // Prefer global average if provided
    float avgLum = max(gAverageLuminance, 1e-4);  // robust
    float exposure = cfMiddleGray / avgLum;       // classic auto-exposure

    float3 c = color.rgb * exposure;

    // keep only highlights above threshold
    float3 passed = max(c - cfBrightPassThreshold, 0.0);

    return float4(passed, color.a);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motionblur Quality Defines
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if MOTIONBLUR_QUALITY == 1
static const int   SAMPLES = 16;
static const float kWeights[SAMPLES] = { 2.6,1.5,1.4,1.3,1.2,1.1,1.0,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.05 };
static const float jitterAmount = 0.0035;
#else
static const int   SAMPLES = 8;
static const float kWeights[SAMPLES] = { 2.0,1.75,1.5,1.25,1.0,0.75,0.5,0.25 };
static const float jitterAmount = 0.0025;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Noise Texture
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float generateNoise(float2 uv)
{
    float total = 0.0;
    float amplitude = 1.0;
    float frequency = 2.0;
    float maxAmplitude = 0.0;
    const int OCTAVES = 4;

    [unroll]
        for (int i = 0; i < OCTAVES; i++)
        {
            float seed = dot(uv * frequency, float2(12.9898, 78.233));
            float noise = frac(sin(seed) * 43758.5453);
            noise = noise * 2.0 - 1.0;

            total += noise * amplitude;
            maxAmplitude += amplitude;

            amplitude *= 0.5;
            frequency *= 2.0;
        }

    return total / maxAmplitude;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate HQ Jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float2 generateHQJitter(int index, float2 baseTex)
{
    float angle = frac(sin(dot(baseTex * index, float2(12.9898, 78.233))) * 43758.5453) * 6.2831853;  // 2*PI
    float radius = sqrt(max(float(index), 0.0001) / float(SAMPLES));
    return float2(cos(angle), sin(angle)) * radius * jitterAmount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motion Blur
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Definitions
static const float baseRampCoeff = 0.75; // Base coefficient, dynamically modulated

// Motion Blur Vertex Shader
struct VS_INPUT_MOTIONBLUR
{
    float4 position : POSITION;
    float4 tex0 : TEXCOORD0;
    float4 tex1 : TEXCOORD1;
    float4 tex2 : TEXCOORD2;
    float4 tex3 : TEXCOORD3;
    float4 tex4 : TEXCOORD4;
    float4 tex5 : TEXCOORD5;
    float4 tex6 : TEXCOORD6;
    float4 tex7 : TEXCOORD7;
};

struct VtoP_MOTIONBLUR
{
    float4 position : POSITION;
    float2 tex[8] : TEXCOORD0;
};

// Vertex Shader
VtoP_MOTIONBLUR VS_MotionBlur(const VS_INPUT_MOTIONBLUR IN)
{
    VtoP_MOTIONBLUR OUT;
    OUT.position = IN.position;

    OUT.tex[0] = IN.tex0.xy;
    OUT.tex[1] = IN.tex1.xy;
    OUT.tex[2] = IN.tex2.xy;
    OUT.tex[3] = IN.tex3.xy;
    OUT.tex[4] = IN.tex4.xy;
    OUT.tex[5] = IN.tex5.xy;
    OUT.tex[6] = IN.tex6.xy;
    OUT.tex[7] = IN.tex7.xy;

    return OUT;
}

// Motion Blur Pixel Shader
float4 PS_MotionBlur(const VtoP_MOTIONBLUR IN) : COLOR
{
    float4 result = 0;

    float weightSum = 0.0;
    [unroll] for (int wi = 0; wi < SAMPLES; wi++) weightSum += kWeights[wi];

    [unroll]
    for (int i = 0; i < SAMPLES; i++)
    {
    float2 baseTex = IN.tex[i % 8];

    #if MOTIONBLUR_QUALITY == 1
        float2 jitteredTex = baseTex + generateHQJitter(i, IN.tex[0]);
    #else
        float jitterX = generateNoise(baseTex) * jitterAmount;
        float jitterY = generateNoise(baseTex.yx + float2(5.2, 1.3)) * jitterAmount;
        float2 jitteredTex = baseTex + float2(jitterX, jitterY);
    #endif

     // avoid sampling outside
     jitteredTex = saturate(jitteredTex);

     float4 sample = tex2D(MOTIONBLUR_SAMPLER, jitteredTex);
     result += sample * kWeights[i];
    }

    result /= max(weightSum, 1e-5);

    float4 firstSample = tex2D(MOTIONBLUR_SAMPLER, IN.tex[0]);
    float dynamicRampCoeff = saturate(baseRampCoeff + (firstSample.w * 0.25));
    result = lerp(firstSample, result, dynamicRampCoeff);

    float dither = generateNoise(IN.tex[0] * 123.456);
    result.rgb += dither * 0.008;

    return saturate(result);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Apply Motion Blur
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VtoP VS_CompositeBlur(const VS_INPUT_SCREEN IN)
{
    VtoP OUT;

    OUT.position = IN.position;
    OUT.tex0 = IN.tex0;
    OUT.tex1 = IN.tex1;

    return OUT;
}

DepthSpriteOut PS_CompositeBlur(const VtoP IN)
{
    DepthSpriteOut OUT;

    float4 blur = tex2D(MOTIONBLUR_SAMPLER, IN.tex0.xy);
    float4 screen = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);

    float2 scaledMaskUV = (IN.tex1.xy - 0.5) * MOTIONBLUR_MASK_SCALE + 0.5;

#if MOTIONBLUR_QUALITY == 1
    float4 vignette = tex2D(CUSTOM_SAMPLER2, scaledMaskUV);
    float motionBlurMask = (-vignette.x) * cvBlurParams.x;
    float radialBlurMask = (-vignette.y) + cvBlurParams.y;
#else
    float4 vignette = tex2D(CUSTOM_SAMPLER1, scaledMaskUV);
    float motionBlurMask = (-vignette.x) + cvBlurParams.x;
    float radialBlurMask = (-vignette.y) * cvBlurParams.y;
#endif

    float finalMask = saturate(motionBlurMask + radialBlurMask);

    float blurIntensity = blur.w;
    float finalBlend = saturate(blurIntensity * finalMask);

    OUT.colour.rgb = lerp(screen.rgb, blur.rgb, finalBlend);
    OUT.colour.a = screen.a;

    return OUT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fake HDR function - ported from ReShade but optimized by me
// Not actual HDR - It just tries to mimic an HDR look
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_HDR == 1
float4 applyHDR(const VtoP IN, in float4 result)
{
    // Use a simple fake HDR bloom by sampling along eight directions around the current pixel. 
    // Precomputed directions avoid expensive trig functions (cos/sin) inside the loop.
    float HDRPower = FAKEHDR_POWER;
    float radius1  = 1.500;
    float radius2  = 2.000;

    float3 color = result.rgb;

    float3 bloom_sum1 = float3(0.0, 0.0, 0.0);
    float3 bloom_sum2 = float3(0.0, 0.0, 0.0);

    // Predefined unit circle directions to avoid cos/sin in the loop
    static const float2 directions[8] = {
        float2( 1.0,  0.0),
        float2( 0.70710678,  0.70710678),
        float2( 0.0,  1.0),
        float2(-0.70710678,  0.70710678),
        float2(-1.0,  0.0),
        float2(-0.70710678, -0.70710678),
        float2( 0.0, -1.0),
        float2( 0.70710678, -0.70710678)
    };

    [loop]
    for (int i = 0; i < 8; ++i)
    {
        float2 dir = directions[i];
        bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + dir * radius1).rgb;
        bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + dir * radius2).rgb;
    }

    bloom_sum1 *= (1.0 / 8.0);
    bloom_sum2 *= (1.0 / 8.0);

    float dist = radius2 - radius1;
    float3 HDR  = (color + (bloom_sum2 - bloom_sum1)) * dist;
    float3 blend = HDR + color;

    // Protect against zero and NaN
    float3 safeBlend = max(abs(blend), 0.0001);
    float3 hdrColor  = blend * pow(safeBlend, HDRPower - 1.0) + HDR;

    return float4(saturate(hdrColor), result.w);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vanilla Depth Of Field
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 DoDepthOfField(const VtoP IN, in float4 result, float depth)
{
    float zDist = (1 / (1 - depth));
    float focalDist = cvDepthOfFieldParams.x;
    float depthOfField = cvDepthOfFieldParams.y;
    float falloff = cvDepthOfFieldParams.z;
    float maxBlur = cvDepthOfFieldParams.w;
    float blur = saturate((abs(zDist - focalDist) - depthOfField) * falloff / zDist);
    float mipLevel = blur * maxBlur;
    float3 blurredTex = tex2Dbias(MISCMAP6_SAMPLER, float4(IN.tex0.xy, 0, mipLevel)).rgb;
    result = float4(lerp(result.xyz, blurredTex, saturate(mipLevel)), result.w);

    //result.xyz = saturate((abs(zDist-50)-30)*falloff/zDist);
    if (cbDrawDepthOfField)
    {
        result.x += (1 - blur) * 0.3;						// draw the depth of field falloff
        result.xyz += (blur == 0.0f) ? 0.5 : 0.0f;	        // draw the crital focal point
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tonemap Shaders
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MODULES/tonemap_variants.fx"
float3 ApplyTonemap(float3 hdrColor)
{
    float3 color = hdrColor * EXPOSURE_BIAS;

#if TONEMAP_VARIANT == 1
    return Tonemap_Reinhard(color);
#elif TONEMAP_VARIANT == 2
    return Tonemap_ACES2(color);
#elif TONEMAP_VARIANT == 3
    return Tonemap_ACESLegacy(color);
#elif TONEMAP_VARIANT == 4
    return Tonemap_Uncharted2(color);
#elif TONEMAP_VARIANT == 5
    return Tonemap_Unreal(color);
#elif TONEMAP_VARIANT == 6
    return Tonemap_Lottes(color);
#elif TONEMAP_VARIANT == 7
    return Tonemap_GranTurismo(color);
#elif TONEMAP_VARIANT == 8
    return Tonemap_Narkowicz(color);
#elif TONEMAP_VARIANT == 9
    return Tonemap_AgX(color);
#elif TONEMAP_VARIANT == 10
    return Tonemap_FilmicALU(color);
#elif TONEMAP_VARIANT == 11
    return Tonemap_NFSHeatStyle(color);
#elif TONEMAP_VARIANT == 12
    return Tonemap_Reinhard2(color);
#elif TONEMAP_VARIANT == 13
    return Tonemap_Uchimura(color);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desaturation Shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_DESATURATION == 1
float4 applyDesaturation(float4 result, float amount)
{
    float luminance = dot(result.rgb, LUMINANCE_VECTOR); // Calculate Luminance
    float3 desaturatedColor = lerp(result.rgb, float3(luminance, luminance, luminance), amount);
    return float4(desaturatedColor, result.a); // Keep Alphachannel
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Subsurface-Like Diffusion
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float3 applySubsurfaceBlur(const VtoP IN, float3 color)
{
    float2 offset = float2(0.002, 0); // horizontal spread
    float3 blurred = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + offset).rgb;
    return lerp(color, blurred, 0.07); // super subtle
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Curve Shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_LEGACY_CURVES == 1
// Legacy Tone Mapping using Coeffs0-Coeffs3
float3 LegacyCurveTonemap(float3 color)
{
    float luminance = dot(color, LUMINANCE_VECTOR);

    // Ensure coefficients are valid, otherwise use fallback values
    float4 curve0 = Coeffs0;
    float4 curve1 = Coeffs1;

    if (all(curve0 == 0.0) || all(curve1 == 0.0)) {
        curve0 = float4(1.0, 1.0, 1.0, 0.0);  // Fallback to neutral curve
        curve1 = float4(0.5, 0.5, 0.5, 0.0);
    }

    // Compute curve normally
    float4 curve = curve1 * luminance + curve0;

    float3 adjustedColor = color * curve.xyz;

    // Debugging: Ensure colors never fully disappear
    // adjustedColor = max(adjustedColor, 0.15); // Minimum brightness level
    adjustedColor = lerp(color, adjustedColor, 0.6);

    return saturate(adjustedColor);
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Color Grading & Split Toning + Contrast Adjustment
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float3x3 colorMatrix = float3x3
(
    RED_CHANNEL,
    GREEN_CHANNEL,
    BLUE_CHANNEL
);

float3 applyColorGrading(float3 color)
{
    return saturate(mul(color, colorMatrix));
}

    float3 applySplitToning(float3 color)
    {
        // calculate luminance
        float luminance = dot(color, LUMINANCE_VECTOR);

        // Smoothstep for soft stepping
        float shadowAmount = saturate(1.0 - smoothstep(0.0, 0.5, luminance));
        float highlightAmount = saturate(smoothstep(0.5, 1.0, luminance));

    #if SPLIT_TONE == 0
        // mixing tones
        color = lerp(color, SHADOW_TINT, shadowAmount * 0.15);
        color = lerp(color, HIGHLIGHT_TINT, highlightAmount * 0.30);

    #elif SPLIT_TONE == 1
        // define colors
        float3 shadowTint = float3(0.2, 0.3, 0.7);      // blue-ish
        float3 highlightTint = float3(1.0, 0.8, 0.6);   // gold/warm

        // mixing tones
        color = lerp(color, shadowTint, shadowAmount * 0.15);
        color = lerp(color, highlightTint, highlightAmount * 0.30);
    #endif

        return saturate(color);
    }

float3 AdjustLumaChroma(float3 color, float lumaGain, float chromaGain)
{
    float luma = dot(color, LUMINANCE_VECTOR);
    float3 gray = float3(luma, luma, luma); // neutral-grey based on luminance
    float3 chroma = color - gray;           // chroma: difference between color and grey

    // modified seperately
    color = gray * lumaGain + chroma * chromaGain;

    return saturate(color);
}

float3 AdjustContrast(float3 color, float contrast)
{
    // Shift color around 0.5 (mid-gray), apply contrast, shift back
    return saturate((color - 0.5) * contrast + 0.5);
}

float3 processColor(float3 color)
{
    color = applyColorGrading(color);                // Grading (Matrix-based)
    color = applySplitToning(color);                 // Optional: Split Toning
    color = AdjustLumaChroma(color, LUMA, CHROMA);   // Luma/Chroma
    color = AdjustContrast(color, CONTRAST);         // Contrast
    return saturate(color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Perceptual Highlight Softening
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float3 softenHighlights(float3 c)
{
    float m = max(max(c.r, c.g), c.b);
    float t = smoothstep(1.0, 1.5, m);   // knee onset
    // compress instead of lerp-to-white
    return c / (1.0 + t * m * 0.6);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Color Temperatur
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float3 adjustTemperature(float3 color, float temperature)
{
    // temperature in [-1, +1] ≈ small shifts
    float rScale = 1.0 + 0.10 * temperature;
    float bScale = 1.0 - 0.10 * temperature;

    float3 wb = float3(rScale, 1.0, bScale); // keep energy in check
    return saturate(color * wb);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sharpening Shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_SHARPEN == 1

float luminance(float3 color) 
{
    return dot(color, float3(0.299, 0.587, 0.114));
}

float4 applySharpen(const VtoP IN, float4 result)
{
    float2 uv = IN.tex0.xy;

    // sample all from the same source (raw scene)
    float3 c = tex2D(DIFFUSE_SAMPLER, uv).rgb;
    float3 l = tex2D(DIFFUSE_SAMPLER, uv - float2(texelSizeL0.x, 0)).rgb;
    float3 r = tex2D(DIFFUSE_SAMPLER, uv + float2(texelSizeL0.x, 0)).rgb;
    float3 t = tex2D(DIFFUSE_SAMPLER, uv - float2(0, texelSizeL0.y)).rgb;
    float3 b = tex2D(DIFFUSE_SAMPLER, uv + float2(0, texelSizeL0.y)).rgb;

    float3 blur = (l + r + t + b + c) * (1.0 / 5.0);
    float3 highpass = c - blur;
    highpass = clamp(highpass, -0.25, 0.25); // keep symmetric edges

    result.rgb = saturate(result.rgb + highpass * SHARPEN_AMOUNT);
    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adaptive Whitebalance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float3 adaptiveWhiteBalance(float3 color)
{
    float3 sceneColor = color;

    // Temperature proxy: Balance red against blue dominance
    float tempShift = dot(sceneColor.rgb, float3(1.0, 0.0, -1.0));

    // Apply automatic correction towards neutral white
    float3 correction = float3(-tempShift, 0.0, tempShift) * 0.1;
    color += correction;

    // Optional green/magenta shift correction
    float greenShift = sceneColor.g - ((sceneColor.r + sceneColor.b) * 0.5);
    color.rg += float2(0.01, -0.01) * greenShift;

    return saturate(color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Chromatic Abberation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_ABERRATION == 1
// Tweakables
static const float3 RGB_OFFSET = float3(1.0, 0.0, -1.0);

float4 applyNosChromaticAberration(const VtoP IN, in float4 result) : COLOR
{
    float2 uv = IN.tex0.xy;
    float2 center = float2(0.5, 0.5);
    float2 fromCenter = uv - center;
    float dist = length(fromCenter);
    float2 dir = normalize(fromCenter + 1e-6);
    float senseOfSpeedScale = abs(cvBlurParams.x) / 7;

    // Radial distortion per channel
    float2 offsetR = uv + dir * senseOfSpeedScale * RGB_OFFSET.r * dist;
    float2 offsetG = uv + dir * senseOfSpeedScale * RGB_OFFSET.g * dist;
    float2 offsetB = uv + dir * senseOfSpeedScale * RGB_OFFSET.b * dist;

    // Sample shifted channels
    float r = tex2D(DIFFUSE_SAMPLER, offsetR).r;
    float g = tex2D(DIFFUSE_SAMPLER, offsetG).g;
    float b = tex2D(DIFFUSE_SAMPLER, offsetB).b;

    return float4(r, g, b, result.a);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LensDirt shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_LENSDIRT == 1
#define LENS_DIRT_THRESHOLD 0.125   // threshold for luminance
#define BLEND_FACTOR        0.750    // 0 = additive, 1 = screen
#define EDGE_FALLOFF_POWER  2.000    // falloff

float4 applyLensDirt(const VtoP IN, float4 baseColor) : COLOR
{
    // sample dirt texture
    float4 lensDirt = tex2D(CUSTOM_SAMPLER, IN.tex0.xy);

    // calculate scene luminance
    float sceneLuminance = dot(baseColor.rgb, LUMINANCE_VECTOR);

    // Luminance thresholding
    float adjustedLuminance = max(sceneLuminance - LENS_DIRT_THRESHOLD, 0.0);
    float dirtStrength = saturate(adjustedLuminance * LENS_DIRT_INTENSITY);

    // --------- EDGE MASK START ---------
    float2 screenCenter = float2(0.5, 0.5);
    float2 dir = IN.tex0.xy - screenCenter;
    float dist = length(dir) * 2.0;

    float edgeMask = saturate(pow(dist, EDGE_FALLOFF_POWER));
    dirtStrength *= edgeMask;
    // --------- EDGE MASK END ---------

    // adjusted blends:
    float3 additiveBlend = baseColor.rgb + lensDirt.rgb * dirtStrength;
    float3 screenBlend = 1.0 - ((1.0 - baseColor.rgb) * (1.0 - lensDirt.rgb * dirtStrength));

    // Lerp between blend modes
    baseColor.rgb = lerp(additiveBlend, screenBlend, BLEND_FACTOR);

    return baseColor;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filmgrain shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_FILMGRAIN == 1
float4 applyFilmGrain(const VtoP IN, in float4 result) : COLOR
{
    // Sample a noise texture to simulate film grain
    float2 noiseUV = IN.tex0.xy * 5.0f;

    float t = frac(cvTextureOffset.w * 0.123f);
    float noise = generateNoise(noiseUV + t);

    // Apply the noise to the screen texture
    result.rgb += noise * FILM_GRAIN_STRENGTH;

    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vignette Constants
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_VIGNETTE == 1
static const float2 VIGNETTE_CENTER = float2(0.5, 0.5);

// Screen Vignette function
float4 applyVignette(VtoP IN, float4 result) : COLOR
{
    float2 d = (IN.tex0.xy - VIGNETTE_CENTER) / max(VIGNETTE_RADIUS, 1e-3);
    float r2 = dot(d,d);

    // smooth falloff – no hard ring, only darken edges
    float v = exp(-VIGNETTE_CURVE * r2);
    float f = lerp(1.0, v, VIGNETTE_AMOUNT);

    result.rgb *= f;
    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader Pass
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uniform float BLOOM_BLEND_MODE = 0.2;

#ifdef USE_POST_EFFECTS
float4 ShaderPass(const VtoP IN, in float4 result)
{

#if USE_WHITEBALANCE == 1
    result.rgb = adaptiveWhiteBalance(result.rgb);
#endif

#if USE_TONEMAPPING == 1
    result.rgb = ApplyTonemap(result.rgb);
#endif

    result.rgb = adjustTemperature(result.rgb, COLOR_TEMPERATURE);

#if USE_DESATURATION == 1
    result = applyDesaturation(result, DESATURATION_AMOUNT);
#endif

    result.rgb = applySubsurfaceBlur(IN, result.rgb);

#if USE_LEGACY_CURVES == 1
    result.rgb = LegacyCurveTonemap(result.rgb);
#endif

    result.rgb = processColor(result.rgb);

#if USE_SHARPEN == 1
    result = applySharpen(IN, result);
#endif

#if USE_BLOOM == 1
    float4 bloomEffect = tex2D(BLOOM_SAMPLER, IN.tex0.xy);

    float3 bloomColor = bloomEffect.rgb * BLOOM_INTENSITY;

    float3 additiveBlend = result.rgb + bloomColor;
    float3 screenBlend = 1.0f - (1.0f - result.rgb) * (1.0f - bloomColor);

    result.rgb = lerp(screenBlend, additiveBlend, BLOOM_BLEND_MODE);
#endif

    result.rgb = softenHighlights(result.rgb);

#if USE_LENSDIRT == 1
    result = applyLensDirt(IN, result);
#endif

#if USE_FILMGRAIN == 1
    result = applyFilmGrain(IN, result);
#endif

#if USE_VIGNETTE == 1
    result = applyVignette(IN, result);
#endif

    return result;

}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// you shouldnt touch these
// Visualtreatment Function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 PS_VisualTreatment(const VtoP IN, uniform bool doDepthOfField, uniform bool doColourFade) : COLOR
{
    // Predefenitions
    float depth = tex2D(DEPTHBUFFER_SAMPLER, IN.tex0.xy).r;
    float4 result = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);
    float luminance = dot(result.xyz, LUMINANCE_VECTOR);

#if USE_ABERRATION == 1
    result = applyNosChromaticAberration(IN, result);
#endif

#if USE_HDR == 1
    // HDR
    float4 hdr = applyHDR(IN, result);
    result.rgb = hdr.rgb; // keep original alpha
#endif

#ifndef DONTDODEPTH
    if (doDepthOfField && cbDepthOfFieldEnabled)
    {
        result = DoDepthOfField(IN, result, depth);
}
#endif

#if USE_LOG_TO_LINEAR == 1
    // Convert from log space to linear space and clamp
    result.xyz = saturate(DeCompressColourSpace(result.xyz));
    result.xyz = saturate(result.xyz);
#endif

#if USE_POST_EFFECTS == 1
    // Effects
    result = ShaderPass(IN, result);
#endif

    // Apply Brightness adjustments
    result.rgb *= BRIGHTNESS;

#if USE_LUT == 1
    // LUT filter (enable LUT)
    result.rgb = saturate(result.rgb);
    result.rgb = tex3D(VOLUMEMAP_SAMPLER, result.rgb).rgb;
#endif

#if USE_CUSTOM_COP_ENGANGEMENT == 1
    // Cop Intro Effect (Conditional Blending)
    float3 copTreatment = tex3D(BLENDVOLUMEMAP_SAMPLER, result.xyz).xyz;
    if (COP_INTRO_SCALE > 0.25)
    {
        // Apply color blend only if COP_INTRO_SCALE is non-zero
        float3 copBlended = lerp(copTreatment, INTRO_BLEND_COLOR, INTRO_BLEND_AMOUNT); // Blending copTreatment with color
        result.xyz = lerp(result.xyz, copBlended, COP_INTRO_SCALE);                    // Applying cop intro effect
    }
#endif

#if USE_CUSTOM_SPEEDBREAKER == 1
    // Pursuit / speed breaker
    result.rgb = saturate(result.rgb);

    float3 blendColor = lerp(result.xyz, SPEEDBREAKER_EFFECT_COLOR, SPEEDBREAKER_EFFECT_BLEND);
    float3 goldenHighlight = float3(1.1, 1.0, 0.9);

    // scalar weight (kein vektorweises Lerp-Gewicht)
    float w = saturate(max(blendColor.r, max(blendColor.g, blendColor.b)) * BREAKER_INTENSITY);

    // wir haben oben schon: float luminance = dot(result.xyz, LUMINANCE_VECTOR);
    float lum = luminance;

    result.xyz = lerp(result.xyz, lum.xxx * 1.5, w);
    result.xyz *= goldenHighlight;
    result.xyz = saturate(result.xyz);
#else
    // Uses Vanilla Speedbreaker
    float4 vignette = tex2D(MISCMAP2_SAMPLER, IN.tex0.xy);
    result.xyz = lerp(result.xyz, luminance * 1.5, saturate(result) * BREAKER_INTENSITY);
#endif

    // NIS fade
    if (doColourFade)
    {
        result.xyz = lerp(result.xyz, cvVisualEffectFadeColour.xyz, cvVisualEffectFadeColour.w);
    }

    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cliff-fall Function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 PS_UvesOverCliff(const VtoP IN, uniform bool doColourFade) : COLOR
{
    float4 result;

// Initialize result with screen texture
float4 screenTex = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);
result = screenTex;

#if USE_VIGNETTE == 1
    // Apply vignette effect
    result = applyVignette(IN, result);
#endif

#if USE_LUT == 1
    // LUT filter (enable LUT)
    result.rgb = saturate(result.rgb);
    result.rgb = tex3D(VOLUMEMAP_SAMPLER, result.rgb).rgb;
#endif

    // VIGNETTE
    float4 vign = tex2D(MISCMAP2_SAMPLER, IN.tex0.xy);
    float edge = saturate(vign.y);                           // 0 center, 1 edges (angenommen)
    float edgeDarken = lerp(1.0, 1.0 - VIGNETTE_SCALE, edge);
    result.rgb *= edgeDarken;

    // Calculate luminance and max channel
    float luminance = dot(LUMINANCE_VECTOR, result.xyz);
    float maxChannel = max(max(result.x, result.y), result.z);

#if USE_CUSTOM_CLIFF_EFFECT == 1
    // Blend/fade to color for cliff effect
    float3 cliffBlendColor = CLIFF_COLOR; // example RGB color for the fade
    float cliffBlendAmount = saturate(maxChannel * CLIFF_BLEND); // control blend amount dynamically
    result.xyz = lerp(result.xyz, cliffBlendColor, cliffBlendAmount);
#endif

    // NIS fade
    if (doColourFade)
    {
        result.xyz = lerp(result.xyz, cvVisualEffectFadeColour.xyz, cvVisualEffectFadeColour.w);
    }

    return result;
}
