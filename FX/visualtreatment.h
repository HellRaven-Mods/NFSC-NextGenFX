#ifndef VISUALTREATMENT_H
#define VISUALTREATMENT_H

/********************************************************************

created:	2024/02/03
updated:    2025/04/18
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

shared float4 Coeffs0                               : CURVE_COEFFS_0;
shared float4 Coeffs1                               : CURVE_COEFFS_1;

shared float cfVignetteScale                        : cfVignetteScale;
shared float4 cvVisualTreatmentParams               : cvVisualTreatmentParams;
shared float4 cvVisualTreatmentParams2              : cvVisualTreatmentParams2;
shared float cfVisualEffectVignette                 : cfVisualEffectVignette;
shared float cfVisualEffectBrightness               : cfVisualEffectBrightness;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Samplers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
texture CUSTOM_TEXTURE1;
sampler CUSTOM_SAMPLER = sampler_state
{
    Texture = <CUSTOM_TEXTURE1>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

texture CUSTOM_TEXTURE2;
sampler CUSTOM_SAMPLER1 = sampler_state
{
    Texture = <CUSTOM_TEXTURE2>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

DECLARE_TEXTURE(DIFFUSEMAP_TEXTURE)
sampler	DIFFUSE_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DIFFUSEMAP_TEXTURE)
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = NONE;
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
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
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
    OUT.xyz = diffuse;
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
       // Berechne einen "Seed" basierend auf der skalierten UV-Koordinate
       float seed = dot(uv * frequency, float2(12.9898, 78.233));
       // Erzeuge einen Pseudo-Zufallswert und mappe ihn von [0,1] nach [-1,1]
       float noise = frac(sin(seed) * 43758.5453);
       noise = noise * 2.0 - 1.0;

       total += noise * amplitude;
       maxAmplitude += amplitude;

       // Für die nächste Oktave: Verdopple die Frequenz und halbiere die Amplitude
       amplitude *= 0.5;
       frequency *= 2.0;
    }

    // Normiere das Ergebnis, sodass es stets im Bereich [-1,1] liegt
    return total / maxAmplitude;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Luminance-Sampling and Bright-Pass
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 SampleLuminance(in float2 texcoord : TEXCOORD0) : COLOR
{
    float fLogLumSum = 0.0f;

    for (int iSample = 0; iSample < 9; iSample++)
    {
        // Compute the sum of log(luminance) throughout	the	sample points
        float4 vColor = tex2D(DIFFUSE_SAMPLER, texcoord + cavSampleOffsets[iSample].xy);
        fLogLumSum += vColor.w;
    }

    // Divide the sum to complete the average
    fLogLumSum /= 9;

    return float4(fLogLumSum, fLogLumSum, fLogLumSum, 1.0f);
}

float4 BrightPass(float2 texcoord : TEXCOORD0) : COLOR0
{
    float4 centerLum = tex2D(MISCMAP1_SAMPLER, float2(0.5f, 0.5f)).b;
    float4 color = tex2D(DIFFUSE_SAMPLER, texcoord);

    float lumAdapt = max(centerLum.x, cfMiddleGray);
    lumAdapt += 0.001f;

    float invLumAdapt = 1.0f / lumAdapt;
    float exposure = cfMiddleGray * invLumAdapt;

    float3 scaledColor = color.rgb * exposure;

    float brightPass = max(max(scaledColor.r, scaledColor.g), scaledColor.b);
    float threshold = brightPass - cfBrightPassThreshold;
    threshold = max(threshold, 0.0f);
    float brightPassFactor = (threshold + cfBrightPassThreshold) * (1.0f / (threshold + cfBrightPassThreshold));

    scaledColor *= brightPassFactor * 0.1f;

    return float4(scaledColor, color.w);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate Texelsize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float2 texelSizeL0 = float2(1.0 / SCREEN_WIDTH, 1.0 / SCREEN_HEIGHT);
float2 texelSizeL1 = float2(1.0 / (SCREEN_WIDTH / 2), 1.0 / (SCREEN_HEIGHT / 2));
float2 texelSizeL2 = float2(1.0 / (SCREEN_WIDTH / 4), 1.0 / (SCREEN_HEIGHT / 4));
float2 texelSizeL3 = float2(1.0 / (SCREEN_WIDTH / 8), 1.0 / (SCREEN_HEIGHT / 8));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motion Blur
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Definitions
static const float totalWeight = 8.5f;
static const float kWeights[8] = { 2.0, 1.75, 1.5, 1.25, 1.0, 0.75, 0.5, 0.25 };
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

    // Preload first sample
    float4 firstSample = tex2D(MOTIONBLUR_SAMPLER, IN.tex[0]);
    result += firstSample * (kWeights[0] / totalWeight);

    // Jitter
    float jitterAmount = 0.002;

    // Optimized accumulation loop with jittering
    [unroll]
    for (int i = 1; i < 8; i++)
    {
        // noise-based Offsets for X and Y:
        float jitterX = generateNoise(IN.tex[i]);
        float jitterY = generateNoise(IN.tex[i].yx + float2(5.2, 1.3));
        float2 jitteredTex = IN.tex[i] + float2(jitterX, jitterY) * jitterAmount;

        float4 blurSample = tex2D(MOTIONBLUR_SAMPLER, jitteredTex);
        result += blurSample * (kWeights[i] / totalWeight);
    }

    // Adaptive motion ramping to avoid flickering
    float dynamicRampCoeff = saturate(baseRampCoeff + (firstSample.w * 0.25));
    result = lerp(firstSample, result, dynamicRampCoeff);

    // Dithering
    float dither = generateNoise(IN.tex[0] * 123.456);
    result.rgb += dither * 0.01;

    return result;
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

    float4 vignette = tex2D(CUSTOM_SAMPLER1, float2(IN.tex1.x, IN.tex1.y * 1.2));

    float motionBlurMask = -vignette.x + cvBlurParams.x;
    float radialBlurMask = -vignette.y * cvBlurParams.y;

    float finalMask = saturate(motionBlurMask + radialBlurMask);

    float blurIntensity = blur.w;
    float finalBlend = saturate(blurIntensity * finalMask);

    OUT.colour.rgb = lerp(screen.rgb, blur.rgb, finalBlend);
    OUT.colour.a = screen.a;

    return OUT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fake HDR function - ported from ReShade, optimized by me
// Not actual HDR - It just tries to mimic an HDR look
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_HDR == 1
float4 applyHDR(const VtoP IN, in float4 result)
{
    float HDRPower = FAKEHDR_POWER;
    float radius1 = 0.800;
    float radius2 = 0.800;

    float3 color = tex2Dbias(DIFFUSE_SAMPLER, IN.tex0);

    float3 bloom_sum1 = 0;
    float3 bloom_sum2 = 0;

    for (int i = 0; i < 8; i++)
    {
        float angle = i * 3.14159 / 4.0;
        float2 offset1 = float2(cos(angle), sin(angle)) * radius1;
        float2 offset2 = float2(cos(angle), sin(angle)) * radius2;

        bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + offset1).rgb;
        bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + offset2).rgb;
    }

    bloom_sum1 *= 0.125; // normalize (1 / 8 samples)
    bloom_sum2 *= 0.125;

    float dist = radius2 - radius1;
    float3 HDR = (color + (bloom_sum2 - bloom_sum1)) * dist;

    float3 blend = HDR + color;

    // Protect against zero and NaN
    float3 safeBlend = max(abs(blend), 0.0001);
    float3 hdrColor = blend * pow(safeBlend, HDRPower - 1.0) + HDR;

    return float4(saturate(hdrColor), result.w);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bloom Shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_BLOOM == 1
float4 applyRadialBloom(float2 texCoord)
{
    float4 bloomSum = 0.0;
    float weightSum = 0.0;

    for (int i = 0; i < 64; i++)
    {
        float angle = (float(i) / 64) * 6.283185; // 360 degrees in radians  
        float2 offset = float2(cos(angle), sin(angle)) * BLOOM_RADIUS * texelSizeL0;

        float4 sampleBloom = tex2D(BLOOM_SAMPLER, texCoord + offset);

        float weight = exp(-4.65 * length(offset) * 10.0); // Gaussian-style falloff  
        bloomSum += sampleBloom * weight;
        weightSum += weight;
    }

    bloomSum /= weightSum; // Normalize bloom effect  
    return saturate(bloomSum) * BLOOM_INTENSITY; // Apply intensity

}

float4 applyBloom(float2 texCoord)
{
    float4 diffuse = tex2D(DIFFUSE_SAMPLER, texCoord);
    float4 bloom = tex2D(BLOOM_SAMPLER, texCoord);

    float luminance = dot(diffuse.rgb, float3(0.2126, 0.7152, 0.0722));
    float bloomStrength = smoothstep(10.7, 0.9, luminance);

    return bloom * bloomStrength * 0.93;

}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Color Temperatur
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float3 adjustTemperature(float3 color, float temperature)
{
    // Adjust red and blue channels based on temperature parameter
    color.r += temperature * 0.1;  // Temperature > 0 for warmer, < 0 for cooler
    color.b -= temperature * 0.1;

    return saturate(color);  // Ensure the color values are clamped between 0 and 1
}

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

    // define colors
    float3 shadowTint = float3(0.2, 0.3, 0.7);      // blue-ish
    float3 highlightTint = float3(1.0, 0.8, 0.6);   // gold/warm

    // Smoothstep for soft stepping
    float shadowAmount = saturate(1.0 - smoothstep(0.0, 0.5, luminance));
    float highlightAmount = saturate(smoothstep(0.5, 1.0, luminance));

    // mixing tones
    color = lerp(color, shadowTint, shadowAmount * 0.15);
    color = lerp(color, highlightTint, highlightAmount * 0.30);

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
#endif
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

    // Debug output (optional, log somewhere in Reshade)
    // color = float3(curve.x, curve.y, curve.z); // Uncomment to visualize the curve

    float3 adjustedColor = color * curve.xyz;

    // Debugging: Ensure colors never fully disappear
    // adjustedColor = max(adjustedColor, 0.15); // Minimum brightness level
    adjustedColor = lerp(color, adjustedColor, 0.6);

    return saturate(adjustedColor);
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sharpening Shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_SHARPEN == 1
float4 applySharpen(const VtoP IN, float4 result)
{
    // Sample the neighboring pixels around the current pixel
    float4 center = result;
    float4 left = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy - float2(texelSizeL0.x, 0.0f));
    float4 right = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(texelSizeL0.x, 0.0f));
    float4 top = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy - float2(0.0f, texelSizeL0.y));
    float4 bottom = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(0.0f, texelSizeL0.y));

    // Calculate sharpening effect using a simple high-pass filter kernel
    float4 sharpen = center * (1.0f + SHARPEN_AMOUNT) - (left + right + top + bottom) * (SHARPEN_AMOUNT / 4.0f);

    // Blend the sharpen effect back into the original result
    result.rgb = lerp(result.rgb, result.rgb + sharpen.rgb * 0.1, 0.1); // Adjust the blend factor to control the intensity
    result.rgb = saturate(result.rgb); // Clamp values to maintain valid color ranges

    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Chromatic Abberation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_ABERRATION == 1
float4 applyChromaticAberration(const VtoP IN, float4 color) : COLOR
{
    // Constants
    static const float EDGE_THRESHOLD = 0.02;
    static const float EDGE_RANGE = 0.8;
    static const float EDGE_BLEND_START = 0.4;
    static const float EDGE_BLEND_END = 0.9;

    // Screen coordinates
    float2 screenCenter = float2(0.5, 0.5);
    float2 screenPos = IN.tex0.xy;
    float2 diff = screenPos - screenCenter;

    // Edge falloff
    float edgeFactor = saturate((length(diff) - EDGE_THRESHOLD) / EDGE_RANGE);

    // Aberration offsets
    float2 redOffset = screenPos + edgeFactor * ABERRATION_STRENGTH;
    float2 blueOffset = screenPos - edgeFactor * ABERRATION_STRENGTH;

    // Sample colors
    float redChannel = tex2D(DIFFUSE_SAMPLER, redOffset).r;
    float greenChannel = tex2D(DIFFUSE_SAMPLER, screenPos).g;
    float blueChannel = tex2D(DIFFUSE_SAMPLER, blueOffset).b;

    // Aberration effect
    float3 aberratedColor = pow(float3(redChannel, greenChannel, blueChannel), 1);

    // Smooth blending
    float blendFactor = smoothstep(EDGE_BLEND_START, EDGE_BLEND_END, edgeFactor);
    return lerp(color, float4(aberratedColor, color.a), blendFactor);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LensDirt shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_LENSDIRT == 1
#define LENS_DIRT_THRESHOLD 0.125   // threshold for luminance
#define BLEND_FACTOR        0.750    // 0 = additive, 1 = screen
#define EDGE_FALLOFF_POWER  0.000    // falloff

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
// Vignette Constants
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_VIGNETTE == 1
const float vignetteAmount = VIGNETTE_AMOUNT;
const float vignetteRadius = VIGNETTE_RADIUS;
const float2 VIGNETTE_CENTER = float2(0.5, 0.5);
const float vignetteCurve = VIGNETTE_CURVE;
const float vignetteTransparency = 0.4; // Control vignette transparency

// Screen Vignette function
float4 applyVignette(VtoP IN, float4 result) : SV_Target
{
    float2 uv = IN.tex0.xy;
    float distFromCenter = distance(uv, VIGNETTE_CENTER);
    float vignette = 1 - pow(distFromCenter / vignetteRadius, vignetteCurve);
    vignette = 1 - (1 - vignette) + vignetteAmount;

    // Apply a more advanced vignette formula
    float2 d = uv - VIGNETTE_CENTER;
    float r = sqrt(dot(d, d));
    float f = vignetteRadius * 0.5;
    float s = 1 - pow(r / f, vignetteCurve);
    vignette = s * vignette;

    // Apply vignette to color
    float3 vignetteColor = result.rgb * vignette;
    float vignetteAlpha = vignette * vignetteTransparency;

    // Blend vignette with background color (transparent black)
    float3 blendedColor = lerp(float3(0, 0, 0), vignetteColor, vignetteAlpha);
    float blendedAlpha = vignetteAlpha;

    return float4(blendedColor, blendedAlpha);
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
    float noise = generateNoise(noiseUV);

    // Apply the noise to the screen texture
    result.rgb += noise * FILM_GRAIN_STRENGTH;

    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader Pass
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_POST_EFFECTS
float4 ShaderPass(const VtoP IN, in float4 result)
{

#if USE_DESATURATION == 1
    result = applyDesaturation(result, DESATURATION_AMOUNT);
#endif

#if USE_SHARPEN == 1
    result = applySharpen(IN, result);
#endif

#if USE_TONEMAPPING == 1
    result.rgb = ApplyTonemap(result.rgb);
#endif

#if USE_BLOOM == 1
    float4 bloomEffect = applyRadialBloom(IN.tex0);

    const float radius = 0.008;  

    // Standard Bloom Pass with circular sampling  
    for (int i = 0; i < 16; i++) {
        float angle = (float)i / (float)16 * (2.0 * 3.14159265); // 2 * PI  
        float2 offset = float2(cos(angle), sin(angle)) * radius; // Circular offset  

        // Apply a Gaussian weight based on the angle  
        float weight = exp(-0.5 * (float(i) / float(16) - 0.03) * (float(i) / float(16) - 0.03) * 0.03); // Gaussian weight  
        bloomEffect += (applyBloom(IN.tex0.xy + offset) * weight * (0.01 / 16)) * 300;
    }

    // Blend the effect  
    result += bloomEffect;
#endif

#if USE_LEGACY_CURVES == 1
    result.rgb = LegacyCurveTonemap(result.rgb);
#endif

#if USE_ABERRATION == 1
    result = applyChromaticAberration(IN, result);
#endif

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

float4 PS_VisualTreatment(const VtoP IN, uniform bool doColourFade) : COLOR
{
    // Predefenitions
    float depth = tex2D(DEPTHBUFFER_SAMPLER, IN.tex0).r;
    float4 result = tex2D(DIFFUSE_SAMPLER, IN.tex0);
    float luminance = dot(result.xyz, LUMINANCE_VECTOR);

#if USE_HDR == 1
    // HDR
    result = applyHDR(IN, result);
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
    result.xyz = tex3D(VOLUMEMAP_SAMPLER, result.xyz).xyz;
#endif

    // Apply Color Grading + Temperature adjustment
    result.rgb = adjustTemperature(result.rgb, COLOR_TEMPERATURE);
    result.rgb = processColor(result.rgb);

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
    // Define the blend color and blend factor
    float3 blendColor = lerp(result.xyz, SPEEDBREAKER_EFFECT_COLOR, SPEEDBREAKER_EFFECT_BLEND);
    result.xyz = lerp(result.xyz, luminance * 1.5, blendColor * BREAKER_INTENSITY);
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
    result.xyz = tex3D(VOLUMEMAP_SAMPLER, result.xyz).xyz;
#endif

    // VIGNETTE
    float4 vignette = tex2D(MISCMAP2_SAMPLER, IN.tex0.xy);
    float mask = saturate(vignette.y);
    result.xyz *= 1.1f - (mask * VIGNETTE_SCALE);

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