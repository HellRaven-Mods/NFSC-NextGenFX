#ifndef VISUALTREATMENT_H
#define VISUALTREATMENT_H

/********************************************************************

created:	2024/02/03
updated:    2024/11/03
filename: 	visualtreatment.h
file base:	visualtreatment
file ext:	h
author:		HellRaven Mods

purpose:	Visualtreatment-shader for NFS Carbon 

*********************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTANTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FLASH_SCALE		cvVisualTreatmentParams.x
#define COP_INTRO_SCALE     cvVisualTreatmentParams.z
#define BREAKER_INTENSITY       cvVisualTreatmentParams.y
#define VIGNETTE_SCALE		cvVisualTreatmentParams.w

shared float4x4 cmWorldView : cmWorldView; //WORLDVIEW
shared float3	cvLocalEyePos : cvLocalEyePos; //LOCALEYEPOS;

shared float4 cvBlurParams				: cvBlurParams;

// The per-color weighting to be used for luminance	calculations in	RGB	order.
static const float3	LUMINANCE_VECTOR  = float3(0.2125f, 0.7154f, 0.0721f);

// PARAMETERS
// Depth of Field variables
shared float4 cvDepthOfFieldParams : cvDepthOfFieldParams;
shared bool cbDrawDepthOfField : cbDrawDepthOfField;
shared bool cbDepthOfFieldEnabled : cbDepthOfFieldEnabled;

shared float4 cvVisualEffectFadeColour : cvVisualEffectFadeColour;

// Time Ticker
float cfTimeTicker : cfTimeTicker;

static const int MAX_SAMPLES = 16;	// Maximum texture grabs

// Contains	sampling offsets used by the techniques 
shared float4 cavSampleOffsetWeights[MAX_SAMPLES]	: REG_cavSampleOffsetWeights;

// Tone	mapping	variables
shared float cfBloomScale				: cfBloomScale;

shared float cfVignetteScale        : cfVignetteScale;
shared float4 cvVisualTreatmentParams : cvVisualTreatmentParams;
shared float4 cvVisualTreatmentParams2 : cvVisualTreatmentParams2;
shared float cfVisualEffectVignette : cfVisualEffectVignette;
shared float cfVisualEffectBrightness : cfVisualEffectBrightness;
shared float4 cvTextureOffset	: cvTextureOffset;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Samplers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_TEXTURE(DIFFUSEMAP_TEXTURE)				// needed for the PC
sampler	DIFFUSE_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DIFFUSEMAP_TEXTURE)			   // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER =	NONE;
    MINFILTER =	LINEAR;
    MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MOTIONBLUR_TEXTURE)				// needed for the PC
sampler	MOTIONBLUR_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MOTIONBLUR_TEXTURE)			   // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER =	NONE;
    MINFILTER =	LINEAR;
    MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MOTIONVECTOR_TEXTURE)
sampler MOTIONVECTOR_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DIFFUSEMAP_TEXTURE)			   // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(DEPTHBUFFER_TEXTURE)				// needed for the PC
sampler	DEPTHBUFFER_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DEPTHBUFFER_TEXTURE)			   // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER =	LINEAR;
    MINFILTER =	LINEAR;
    MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MISCMAP2_TEXTURE)				// needed for the PC
sampler	MISCMAP2_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP2_TEXTURE)			   // needed for the PC
    AddressU = CLAMP;
    AddressV = WRAP;		// Use mirror for split screen so the vignette works
    MIPFILTER =	NONE;
    MINFILTER =	LINEAR;
    MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(VOLUMEMAP_TEXTURE)				// needed for the PC
sampler VOLUMEMAP_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(VOLUMEMAP_TEXTURE)			   // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
    MIPFILTER =	NONE;
    MINFILTER =	LINEAR;
    MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MISCMAP3_TEXTURE)				// needed for the PC
sampler BLOOM_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP3_TEXTURE)			   // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(MISCMAP6_TEXTURE)				// needed for the PC
sampler	MISCMAP6_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP6_TEXTURE)			   // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(BLENDVOLUMEMAP_TEXTURE)				// needed for the PC
sampler BLENDVOLUMEMAP_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(BLENDVOLUMEMAP_TEXTURE)			   // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
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
};

struct VtoP
{
    float4 position : POSITION;
    float4 tex0 : TEXCOORD0;
};

VtoP vertex_shader_passthru(const VS_INPUT_SCREEN IN)
{
    VtoP OUT;
    OUT.position = IN.position;
    OUT.tex0	= IN.tex0;

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
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motion Blur
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    float2 tex[8]		: TEXCOORD0;
};

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

float2 UpsampleMotionVector(float2 lowResMotionVector, float2 texCoord) : SV_Target
{
    // Define the upsampling factor
    float upsamplingFactor = 2.0f;

    // Calculate the high-res motion vector coordinates
    float2 highResTexCoord = texCoord * upsamplingFactor;

    // Sample the high-res motion vector directly
    float2 highResMotionVector = tex2Dlod(MOTIONVECTOR_SAMPLER, float4(highResTexCoord, 0, 0));

    return highResMotionVector;
}

// Sum of all weights
const float totalWeight = 31.0f; 
const float kWeights[8] = { 10.0, 8.0, 6.0, 4.0, 2.0, 1.0, 0.5, 0.25};

float4 PS_MotionBlur(const VtoP_MOTIONBLUR IN) : COLOR
{
    float4 result = 0;

    // Calculate the scaling factor based on the screen resolution
    float scaleX = 1.0f / (float)SCREEN_WIDTH;
    float scaleY = 1.0f / (float)SCREEN_HEIGHT;

    // Sample the motion vector texture
    float2 lowResMotionVector = tex2D(MOTIONVECTOR_SAMPLER, IN.tex[0]).xy;

    // Upsample the motion vector
    float2 highResMotionVector = UpsampleMotionVector(lowResMotionVector, IN.tex[0]);

    // Calculate the offset coordinates using the motion vector
    float2 offsetCoords[8];
    for (int i = 0; i < 8; i++)
    {
        float t = (i - 3.5f) / 2.0f; // Shift and scale to center around 0
        float offset = t * highResMotionVector.y * scaleY;
        offsetCoords[i] = IN.tex[i] + float2(0, offset);
        offsetCoords[i] = clamp(offsetCoords[i], 0.0, 1.0);
    }

    // Sample the texture at the offset coordinates
    result += tex2D(MOTIONBLUR_SAMPLER, offsetCoords[0]) * (kWeights[0] / totalWeight);
    result += tex2D(MOTIONBLUR_SAMPLER, offsetCoords[1]) * (kWeights[1] / totalWeight);
    result += tex2D(MOTIONBLUR_SAMPLER, offsetCoords[2]) * (kWeights[2] / totalWeight);
    result += tex2D(MOTIONBLUR_SAMPLER, offsetCoords[3]) * (kWeights[3] / totalWeight);
    result += tex2D(MOTIONBLUR_SAMPLER, offsetCoords[4]) * (kWeights[4] / totalWeight);
    result += tex2D(MOTIONBLUR_SAMPLER, offsetCoords[5]) * (kWeights[5] / totalWeight);
    result += tex2D(MOTIONBLUR_SAMPLER, offsetCoords[6]) * (kWeights[6] / totalWeight);
    result += tex2D(MOTIONBLUR_SAMPLER, offsetCoords[7]) * (kWeights[7] / totalWeight);

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

    return OUT;
}

DepthSpriteOut PS_CompositeBlur(const VtoP IN)
{
    DepthSpriteOut OUT;

    float senseOfSpeedScale = cvBlurParams.x;
    float4 screen = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);
    float4 blur = tex2D(MOTIONBLUR_SAMPLER, IN.tex0.xy);

    if (blur.w * senseOfSpeedScale < 0.10) 
    {
        OUT.colour = screen;
    }
    else if (blur.w * senseOfSpeedScale > 0.90) {
        OUT.colour = blur;
    }
    else 
    {
        OUT.colour.xyz = lerp(screen.xyz, blur.xyz, blur.w * senseOfSpeedScale);
        OUT.colour.w = screen.w;
    }

    return OUT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate Texelsize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float2 texelSize = 1.0 / float2(SCREEN_WIDTH, SCREEN_HEIGHT);
float2 calcTexelSize(float2 textureSize)
{
    return 1.0 / textureSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Noise Texture
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_FILMGRAIN
float generateNoise(float2 uv)
{
    float seed = dot(uv, float2(12.9898, 78.233));
    float lrRandom = frac(sin(seed) * 43758.5453);
    lrRandom = lrRandom * 2.0f - 1.0f;

    // noise frequencies
    lrRandom += frac(sin(seed * 2.0f) * 34521.1234) * 0.5f; // second frequency
    lrRandom += frac(sin(seed * 4.0f) * 12345.6789) * 0.25f; // third frequency
    lrRandom += frac(sin(seed * 8.0f) * 67890.1234) * 0.125f; // fourth frequency
    lrRandom += frac(sin(seed * 8.0f) * 67890.1234) * 0.0625f; // fith frequency
    lrRandom += frac(sin(seed * 8.0f) * 67890.1234) * 0.03125f; // sixth frequency

    return lrRandom;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Depth of Field shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_DOF
float4 applyDOF(const VtoP IN, in float4 result)
{
    float2 screenPos = IN.tex0.xy;
    float depth = tex2D(DEPTHBUFFER_SAMPLER, screenPos).r;

    float focusDepth = 0.87;
    float nearFocusLimit = 0.9;
    float farFocusLimit = 0.6;

    float CoC = 0.0;
    if (depth < nearFocusLimit || depth > farFocusLimit)
    {
        CoC = clamp(abs(depth - focusDepth) * 4.0, 0.0, 1.0);
    }

    float4 blurredColor = float4(0, 0, 0, 0);
    int samples = 0;
    float2 texelSize = float2(1.0 / 2560.0, 1.0 / 1440.0);
    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            float2 offset = float2(i, j) * texelSize * CoC;
            blurredColor += tex2D(DIFFUSE_SAMPLER, screenPos + offset);
            samples++;
        }
    }
    blurredColor /= samples;

    // Improved blending
    result.rgb = lerp(result.rgb, blurredColor, CoC);

    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fake HDR function -- ported from ReShade
// Not actual HDR - It just tries to mimic an HDR look
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_HDR
float4 applyHDR(const VtoP IN, in float4 result) 
{
    float HDRPower = FAKEHDR_POWER;
    float radius1 = FAKEHDR_RADIUS1;
    float radius2 = FAKEHDR_RADIUS2;

#ifdef USE_DOF
    float3 color = applyDOF(IN, result);
#else
    float3 color = tex2Dbias(DIFFUSE_SAMPLER, IN.tex0);
#endif

    float3 bloom_sum1 = float3(0, 0, 0);
    float3 bloom_sum2 = float3(0, 0, 0);

    for (int i = 0; i < 8; i++)
    {
        float2 offset = float2(1.5, -1.5) * radius1 * float2(cos(i * 3.14159 / 4), sin(i * 3.14159 / 4));
        bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + offset).rgb;
    }

    for (int i = 0; i < 8; i++)
    {
        float2 offset = float2(1.5, -1.5) * radius2 * float2(cos(i * 3.14159 / 4), sin(i * 3.14159 / 4));
        bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + offset).rgb;
    }

    bloom_sum1 *= 0.32;
    bloom_sum2 *= 0.34;

    // Contrast Adjustment
    float contrast = 1.1;
    color = (color - 0.5) * contrast + 0.5;

    // Color Grading
    float3 r = RED_CHANNEL;
    float3 g = GREEN_CHANNEL;
    float3 b = BLUE_CHANNEL;

    float dist = radius2 - radius1;
    float3 HDR = (color + (bloom_sum2 - bloom_sum1)) * dist;
    float3 blend = HDR + color;
    color = blend * pow(abs(blend), abs(HDRPower) - 1) + HDR;
    return float4(((color / 2.2) * (r + g + b)), result.w);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desaturation Shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_DESATURATION
float4 applyDesaturation(float4 result, float amount) 
{
    float luminance = dot(result.rgb, Luma709); // Luminanz berechnen
    float3 desaturatedColor = lerp(result.rgb, float3(luminance, luminance, luminance), amount);
    return float4(desaturatedColor, result.a); // Alpha-Kanal beibehalten
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sharpening Shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_SHARPEN
float4 applySharpen(const VtoP IN, float4 result)
{
    // Define the texel size based on your texture resolution
    float2 texelSize = float2(1.0f / 2560.0f, 1.0f / 1440.0f);

    // Sample the neighboring pixels around the current pixel
    float4 center = result;
    float4 left = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy - float2(texelSize.x, 0.0f));
    float4 right = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(texelSize.x, 0.0f));
    float4 top = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy - float2(0.0f, texelSize.y));
    float4 bottom = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(0.0f, texelSize.y));

    // Calculate sharpening effect using a simple high-pass filter kernel
    float4 sharpen = center * (1.0f + SHARPEN_AMOUNT) - (left + right + top + bottom) * (SHARPEN_AMOUNT / 4.0f);

    // Blend the sharpen effect back into the original result
    result.rgb = lerp(result.rgb, result.rgb + sharpen.rgb * 0.1, 0.1); // Adjust the blend factor to control the intensity
    result.rgb = saturate(result.rgb); // Clamp values to maintain valid color ranges

    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Overlay blending mode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Overlay(float LayerA, float LayerB)
{
    return 2.0 * (LayerA * LayerB + (LayerA + LayerB) * (1.0 - LayerA * LayerB)) - LayerA - LayerB;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tonemap Shaders
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ACES2 Tonemap
#ifdef USE_ACES2
float3 ACES2Tonemap(float3 color)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;

    return clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0, 1.0);
}
#endif

// Filmic Tonemap
#ifdef USE_FILMIC
float3 FilmicTonemap(float3 color)
{
    float luma = dot(color, Luma709);
    float3 ret = color * (luma * (1.0 + luma / 7.0)) / (luma + 0.1);
    return ret;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ambient lighting
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_AMBIENTLIGHT
float ambientIntensity = Ambient_Intensity;
float3 ambientColor = Ambiennt_Color;


float4 applyAmbientLighting(const VtoP IN, in float4 result)
{
    // Calculate ambient lighting
    float3 ambient = ambientColor * ambientIntensity;
    result.rgb += ambient;
    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fog Shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_FOG
float4 PS_FogDepth(const VtoP IN, in float4 result) : COLOR
{
    // Retrieve the base color with all previous effects
    float4 color = result;

    // Sample depth from depth buffer
    float depth = tex2D(DEPTHBUFFER_SAMPLER, IN.tex0.xy).x;

    // Fog parameters
    float fogStart = 0.95;
    float fogEnd = 1.0;
    float fogDensity = 0.15;
    float fogHeightFalloff = 0.35;
    float3 fogColor = float3(0.7, 0.7, 0.8);
    float fogHeight = 0.70;

    // Calculate the distance-based fog factor
    float distanceFogFactor = saturate((depth - fogStart) / (fogEnd - fogStart));

    // Calculate height-based fog factor using world position (from TEXCOORD1)
    float heightFactor = exp(-fogHeightFalloff * abs(IN.position.y - fogHeight));

    // Combine the distance and height fog factors with adjustable density
    float combinedFogFactor = saturate(distanceFogFactor * fogDensity * heightFactor);

    // Blend the fog color with the scene's original color based on the combined fog factor
    color.rgb = lerp(color.rgb, fogColor, combinedFogFactor);

    // Update result with the blended color
    result = color;
    return result;
}
#endif 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bloom Shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_BLOOM
float4 applyBloom(float2 texCoord)
{
    float blurSize = 1.0 / 4096.0; // Adjust according to your texture size
    float BloomIntensity = 1.15;
    float4 sum = float4(0, 0, 0, 0);

    // Apply gaussian blur sampling using the BloomSampler
    sum += tex2D(BLOOM_SAMPLER, texCoord + float2(-4.0 * blurSize, 0)) * 0.05;
    sum += tex2D(BLOOM_SAMPLER, texCoord + float2(-3.0 * blurSize, 0)) * 0.09;
    sum += tex2D(BLOOM_SAMPLER, texCoord + float2(-2.0 * blurSize, 0)) * 0.12;
    sum += tex2D(BLOOM_SAMPLER, texCoord + float2(-1.0 * blurSize, 0)) * 0.15;
    sum += tex2D(BLOOM_SAMPLER, texCoord) * 0.16;
    sum += tex2D(BLOOM_SAMPLER, texCoord + float2(1.0 * blurSize, 0)) * 0.15;
    sum += tex2D(BLOOM_SAMPLER, texCoord + float2(2.0 * blurSize, 0)) * 0.12;
    sum += tex2D(BLOOM_SAMPLER, texCoord + float2(3.0 * blurSize, 0)) * 0.09;
    sum += tex2D(BLOOM_SAMPLER, texCoord + float2(4.0 * blurSize, 0)) * 0.05;

    // Apply threshold and intensity
    float4 bloom = max(float4(0, 0, 0, 0), sum - 0.15); // Adjust threshold based on your scene requirements
    bloom = saturate(bloom * BloomIntensity);

    return bloom;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Chromatic Abberation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_ABBERATION
float4 applyChromaticAberration(const VtoP IN, float4 color) : COLOR
{
    float aberrationStrength = 0.015;

    // Calculate edge factor using squared distance for optimized falloff
    float2 screenCenter = float2(0.5, 0.5);
    float2 screenPos = IN.tex0.xy;
    float2 diff = screenPos - screenCenter;
    float edgeFactor = saturate((dot(diff, diff) - 0.03) / 0.64);  // Adjusted range for squared distance

    // Aberration offsets per channel based on edgeFactor
    float2 redOffset = screenPos + edgeFactor * aberrationStrength * float2(cos(cfTimeTicker), sin(cfTimeTicker));
    float2 greenOffset = screenPos;
    float2 blueOffset = screenPos + edgeFactor * aberrationStrength * float2(-sin(cfTimeTicker), -cos(cfTimeTicker));

    // Brightness factor to enhance the aberration colors
    float brightnessFactor = 1.0;

    // Sample and brighten each color channel
    float redChannel = tex2D(DIFFUSE_SAMPLER, redOffset).r * brightnessFactor;
    float greenChannel = tex2D(DIFFUSE_SAMPLER, greenOffset).g * brightnessFactor;
    float blueChannel = tex2D(DIFFUSE_SAMPLER, blueOffset).b * brightnessFactor;

    // Apply gamma correction for a brighter, more saturated effect
    float3 aberratedColor = pow(float3(redChannel, greenChannel, blueChannel), float3(0.7, 0.7, 0.7));

    // Smooth blending of aberrated color towards edges
    float blendFactor = smoothstep(0.2, 0.8, edgeFactor);;
    return lerp(color, float4(aberratedColor, color.a), blendFactor);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Camera Distortion
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_DISTORTION
uniform float2 DISTORTION_CENTER = float2(0.5, 0.5);    // Center of distortion
uniform float DISTORTION_STRENGTH = 0.03;               // Amount of distortion

float4 applyDistortion(const VtoP IN, float4 result)
{
    // Calculate the distorted UV coordinates
    float2 diff = IN.tex0.xy - DISTORTION_CENTER;
    float dist = length(diff);
    float amount = DISTORTION_STRENGTH * dist * dist;   // Quadratic falloff

    // Apply radial distortion
    float2 distortedUV = IN.tex0.xy + diff * amount;

    // Assuming that you need to re-sample the texture with the distorted UVs
    float4 newResult = tex2D(DIFFUSE_SAMPLER, distortedUV); // Resample the texture with the new UVs

    // Blend the new result with the current result if necessary
    return lerp(result, newResult, 0.25); // Simple blend; adjust the factor as needed
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vignette Constants
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_VIGNETTE
const float vignetteAmount = VIGNETTE_AMOUNT;
const float vignetteRadius = VIGNETTE_RADIUS;
const float2 VIGNETTE_CENTER = float2(0.5, 0.5);
const float vignetteCurve = VIGNETTE_CURVE;
uniform float vignetteTransparency = 0.4; // Control vignette transparency

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

#ifdef USE_FILMGRAIN
float4 applyFilmGrain(const VtoP IN, in float4 result) : COLOR
{
    // Sample a noise texture to simulate film grain
    float2 noiseUV = IN.tex0.xy * 5.0f;
    float noise = generateNoise(noiseUV);

    // Apply the noise to the screen texture
    result.rgb += noise * 0.015f;

    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pause Blur shader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DONTDOPAUSEBLUR
float4 applyPauseBlur(const VtoP IN, in float4 result)
{
    // Depth-based blur calculations
    float depth         = tex2Dbias(DEPTHBUFFER_SAMPLER, IN.tex0).r;
    float zDist         = depth * (depth - 1);
    float focalDist     = cvDepthOfFieldParams.x;
    float depthOfField  = cvDepthOfFieldParams.y;
    float falloff       = cvDepthOfFieldParams.z;
    float maxBlur       = cvDepthOfFieldParams.w;
    float blurAmount    = saturate((abs(zDist - focalDist) - depthOfField) * falloff / zDist);
    float mipLevel      = (blurAmount * maxBlur) * 5;

    // Get blurred texture based on mipLevel
    float3 blurredTex = tex2Dbias(MISCMAP6_SAMPLER, float4(IN.tex0.xy, 1, mipLevel));

    // Conditional blend based on blur amount
    float blendStrength = saturate(blurAmount);
    float4 color = float4(lerp(result.xyz, blurredTex, blendStrength), result.w);

    return color;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader Pass
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_EFFECTS
float4 ShaderPass(const VtoP IN, in float4 result)
{
#ifdef USE_DESATURATION
    result = applyDesaturation(result, DESATURATION_AMOUNT);
#endif

#ifdef USE_SHARPEN
    result = applySharpen(IN, result);
#endif

#ifdef USE_Filmic
    result.rgb = FilmicTonemap(result.rgb);
#endif

#ifdef USE_ACES2
    result.rgb = ACES2Tonemap(result.rgb);
#endif

#ifdef USE_AMBIENTLIGHT
    result = applyAmbientLighting(IN, result);
#endif

#ifdef USE_FOG
    result = PS_FogDepth(IN, result);
#endif

#ifdef USE_BLOOM
    float4 bloomEffect = applyBloom(IN.tex0.xy);        // Bloom effect
    result = lerp(result, result + bloomEffect, 0.2);   // Adjust blend factor as needed
#endif

#ifdef USE_ABBERATION
    result = applyChromaticAberration(IN, result);
#endif

#ifdef USE_DISTORTION
    result = applyDistortion(IN, result);
#endif

#ifdef USE_VIGNETTE
    result = applyVignette(IN, result);
#endif

#ifdef USE_FILMGRAIN
    result = applyFilmGrain(IN, result);
#endif

    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// you shouldnt touch these
// Visualtreatment Function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 PS_VisualTreatment(const VtoP IN, uniform bool doDepthOfField) : COLOR
{
    float depth = tex2D(DEPTHBUFFER_SAMPLER, IN.tex0).r;
    float4 result = tex2D(DIFFUSE_SAMPLER, IN.tex0);

#ifdef USE_HDR
    // HDR + DOF
    result = applyHDR(IN, result);
#endif

#ifndef DONTDOPAUSEBLUR
    // Race Start DOF and Pause Blur
    if (doDepthOfField && cbDepthOfFieldEnabled)	// Compile time branch is optimized out
    {
        result = applyPauseBlur(IN, result);
}
#endif

#ifdef USE_EFFECTS
    // Effects
    result = ShaderPass(IN, result);
#else
    // Disabled Effects
    float4 screenTex = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);
    result = screenTex;
#endif

#ifdef USE_LUT
    // LUT filter (enable LUT)
    float3 samplePosition = result.xyz;
    float3 sampledColor = tex3D(VOLUMEMAP_SAMPLER, samplePosition).xyz;
    result.xyz = sampledColor;
#endif

#ifdef USE_LOG_to_LINIAR
    // Convert from log space to linear space and clamp[0,1]
    result.xyz = DeCompressColourSpace(result.xyz);
#endif

    // clamp anything above 1
    result.rgb = saturate(result.rgb);

    // Lens contamination gradient effect 
    result.xyz = AddGradient(result, IN.tex0.xy);

    // Calculate Luminance
    float luminance = dot(result.xyz, LUMINANCE_VECTOR);

    // NIS fade
    if (doColourFade)
    {
        result.xyz = lerp(result.xyz, cvVisualEffectFadeColour.xyz, cvVisualEffectFadeColour.w);
    }

    // Cop Intro Effect (Conditional Blending)
    // Define the blend color and blend factor
    float3 blendColor = INTRO_COLOR; // color
    float blendAmount = INTRO_BLEND_AMOUNT; // adjust blend amount
    float3 copTreatment = tex3D(BLENDVOLUMEMAP_SAMPLER, result.xyz).xyz;
    if (COP_INTRO_SCALE > 0.25) 
    {
        // Apply color blend only if COP_INTRO_SCALE is non-zero
        float3 copBlended = lerp(copTreatment, blendColor, blendAmount); // Blending copTreatment with color
        result.xyz = lerp(result.xyz, copBlended, COP_INTRO_SCALE);      // Applying cop intro effect
    }

    // Pursuit / speed breaker
    // Define the blend color and blend factor
    float3 blendColor1 = SPEEDBREAKER_EFFECT_COLOR; // color
    float blendAmount1 = SPEEDBREAKER_EFFECT_BLEND; // adjust blend amount
    blendColor = lerp(result.xyz, blendColor1, blendAmount1);
    result.xyz = lerp(result.xyz, luminance * 1.5, blendColor * BREAKER_INTENSITY); 

    // Apply Brightness Adjustment
    result.rgb *= BRIGHTNESS;

    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cliff-fall Function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 PS_UvesOverCliff(const VtoP IN) : COLOR
{
    float4 result;

    // Initialize result with screen texture
    float4 screenTex = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);
    result = screenTex;

    // Apply vignette effect
#ifdef USE_VIGNETTE
    result = applyVignette(IN, result);
#endif

    // Apply LUT filter with color grading
    float3 samplePosition = result.xyz;
    float3 sampledColor = tex3D(VOLUMEMAP_SAMPLER, samplePosition).xyz;
    float3 gradedColor = sampledColor * float3(1.2, 1.1, 1.0); // warm tone
    result.xyz = gradedColor;

    // VIGNETTE
    float4 vignette = tex2D(MISCMAP2_SAMPLER, IN.tex0.xy);
    float mask = saturate(vignette.y);
    result.xyz *= 1.1f - (mask * VIGNETTE_SCALE);

    // NIS fade
    if (doColourFade)
    {
        result.xyz = lerp(result.xyz, cvVisualEffectFadeColour.xyz, cvVisualEffectFadeColour.w);
    }

    // Calculate luminance and max channel
    float luminance = dot(LUMINANCE_VECTOR, result.xyz);
    float maxChannel = max(max(result.x, result.y), result.z);

    // Blend/fade to color for cliff effect
    float3 cliffBlendColor = CLIFF_COLOR; // example RGB color for the fade
    float cliffBlendAmount = saturate(maxChannel * CLIFF_BLEND); // control blend amount dynamically
    result.xyz = lerp(result.xyz, cliffBlendColor, cliffBlendAmount);

    return result;
}
