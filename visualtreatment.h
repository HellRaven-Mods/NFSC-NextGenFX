#ifndef VISUALTREATMENT_H
#define VISUALTREATMENT_H

/********************************************************************

created:	2024/2/3
filename: 	visualtreatment.h
file base:	visualtreatment
file ext:	h
author:		HellRaven Mods

purpose:	Visualtreatment shader for NFS Carbon REDUX

*********************************************************************/

#define MOTIONBLUR_DONTPASSTHRUDEPTH
//#define RENDER_DEPTH
#define FLASH_SCALE		cvVisualTreatmentParams.x
#define COP_INTRO_SCALE     cvVisualTreatmentParams.z
#define BREAKER_INTENSITY       cvVisualTreatmentParams.y
#define VIGNETTE_SCALE		cvVisualTreatmentParams.w

shared float4 cvBlurParams				: cvBlurParams;

// CONSTANTS
// The per-color weighting to be used for luminance	calculations in	RGB	order.
static const float3	LUMINANCE_VECTOR  = float3(0.2125f, 0.7154f, 0.0721f);

// PARAMETERS
// Depth of Field variables
shared float4 cvDepthOfFieldParams : cvDepthOfFieldParams;
shared float cbDrawDepthOfField : cbDrawDepthOfField;
shared bool cbDepthOfFieldEnabled : cbDepthOfFieldEnabled;

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

/*** Forward Declarations *****************************************************************************************/


/*** Samplers *****************************************************************************************/

sampler PERLINNOISE_SAMPLER = sampler_state
{
    AddressU = WRAP;
    AddressV = WRAP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

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
    ASSIGN_TEXTURE(MOTIONVECTOR_TEXTURE)			   // needed for the PC
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

/*** Structures **************************************************************************************/

struct DepthSpriteOut
{
    float4 colour	: COLOR0;
    // Passing thru depth on the PC (1) isn't needed, (2) seems to cause visual errors on nvidia 7-series gpus
#ifndef MOTIONBLUR_DONTPASSTHRUDEPTH
    float  depth	: DEPTH0;
#endif
};

///////////////////////////////////////////////////////////////////////////////

// should pack these two, reduce input bandwidth.   may not be significant
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Motion Blur
///////////////////////////////////////////////////////////////////////////////////////////////////

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

float2 UpsampleMotionVector(float2 lowResMotionVector, float2 texCoord)
{
    // Define the upsampling factor
    float upsamplingFactor = 2.0f;

    // Calculate the high-res motion vector coordinates
    float2 highResTexCoord = texCoord * upsamplingFactor;

    // Interpolate the high-res motion vector
    float2 highResMotionVector = tex2D(MOTIONVECTOR_SAMPLER, highResTexCoord) * (1.0f - frac(highResTexCoord.x)) * (1.0f - frac(highResTexCoord.y));

    // Return the upsampled motion vector
    return highResMotionVector;
}

const float kWeights[8] = {10, 8, 6, 4, 2, 1, 0, 0};
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
    }

    // Sample the texture at the offset coordinates
    result += tex2D(MOTIONBLUR_SAMPLER, float4(offsetCoords[0], 0, 0)) * (kWeights[0] / 31.0f);
    result += tex2D(MOTIONBLUR_SAMPLER, float4(offsetCoords[1], 1, 0)) * (kWeights[1] / 31.0f);
    result += tex2D(MOTIONBLUR_SAMPLER, float4(offsetCoords[2], 2, 0)) * (kWeights[2] / 31.0f);
    result += tex2D(MOTIONBLUR_SAMPLER, float4(offsetCoords[3], 3, 0)) * (kWeights[3] / 31.0f);
    result += tex2D(MOTIONBLUR_SAMPLER, float4(offsetCoords[4], 4, 0)) * (kWeights[4] / 31.0f);
    result += tex2D(MOTIONBLUR_SAMPLER, float4(offsetCoords[5], 5, 0)) * (kWeights[5] / 31.0f);
    result += tex2D(MOTIONBLUR_SAMPLER, float4(offsetCoords[6], 6, 0)) * (kWeights[6] / 31.0f);
    result += tex2D(MOTIONBLUR_SAMPLER, float4(offsetCoords[7], 7, 0)) * (kWeights[7] / 31.0f);

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VtoP VS_CompositeBlur(const VS_INPUT_SCREEN IN)
{
    VtoP OUT;
    OUT.position = IN.position;
    OUT.tex0	= IN.tex0;
    return OUT;
}


DepthSpriteOut PS_CompositeBlur(const VtoP IN)
{
    DepthSpriteOut OUT;

    float4 screen = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);
    float4 blur = tex2D(MOTIONBLUR_SAMPLER, IN.tex0.xy);

    float senseOfSpeedScale = cvBlurParams.x;
    OUT.colour.xyz = lerp(screen.xyz, blur.xyz, blur.w * senseOfSpeedScale);
    OUT.colour.w = screen.w;

#ifndef MOTIONBLUR_DONTPASSTHRUDEPTH
    OUT.depth = tex2D(DEPTHBUFFER_SAMPLER, IN.tex0.xy).x;
#endif

    return OUT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Effect Shaders
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculate the texelsize
float2 texelSize = 1.0 / float2(SCREEN_WIDTH, SCREEN_HEIGHT);
float2 calcTexelSize(float2 textureSize)
{
    return 1.0 / textureSize;
}

// gererate a Noise texture
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Depth of Field shader
float4 applyDOF(const VtoP IN, in float4 result) : COLOR
{
    // Calculate the depth of the current pixel
    float depth = tex2D(DEPTHBUFFER_SAMPLER, IN.tex0.xy).r;

    // Calculate the zDist value
    float zDist = sqrt(1 / depth);

    // Calculate the circle of confusion (CoC) based on the zDist value
    float CoC = 0.02 * zDist; // increased CoC value to make DOF effect more pronounced

    // Calculate the blur amount based on the CoC value
    float blurAmount = CoC * 32.5; // increased blur amount to make DOF effect more visible

    // Define autofocus parameters
    float focusSpeed = 120.0; // adjust this value to control the autofocus speed
    float focusThreshold = 0.55; // adjust this value to control the autofocus sensitivity

    // Calculate the contrast of the image in the current region
    float contrast = 0;
    for (int i = -5; i <= 5; i++)
    {
        for (int j = -5; j <= 5; j++)
        {
            float2 offset = float2(i, j) * texelSize;
            float luminance = dot(tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + offset).rgb, float3(0.2126, 0.7152, 0.0722));
            contrast += abs(luminance - 0.5);
        }
    }
    contrast /= 121;

    // Adjust the focus based on the contrast
    if (contrast > focusThreshold)
    {
        // Increase the focus speed if the contrast is high
        focusSpeed *= 2;
    }
    else
    {
        // Decrease the focus speed if the contrast is low
        focusSpeed /= 2;
    }

    // Update the focus distance based on the autofocus
    zDist += focusSpeed * (1 - zDist);
    zDist = clamp(zDist, 0.0, 1.0);

    // Apply the blur effect to pixels outside of the focus area
    if (zDist > 0.9) // decreased focus area to make DOF effect more visible
    {
        float4 blurredColor = float4(0, 0, 0, 0);
        for (int i = -5; i <= 5; i++)
        {
            for (int j = -5; j <= 5; j++)
            {
                float2 offset = float2(i, j) * blurAmount * texelSize; // Calculate offset in texel units
                blurredColor += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + offset);
            }
        }
        blurredColor /= 121; // divide by the number of samples (11x11 kernel)
        result = blurredColor; // set result to blurred color
    }
    else
    {
        result = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy); // set result to original color
    }

    // Apply a hexagonal kernel to create a more realistic bokeh shape
    float2 kernelOffsets[6] = {
        float2(-0.5, -0.866),
        float2(0.5, -0.866),
        float2(1, 0),
        float2(0.5, 0.866),
        float2(-0.5,  0.866),
        float2(-1, 0)
    };
    float4 hexBlurredColor = float4(0, 0, 0, 0);
    for (int i = 0; i < 6; i++)
    {
        hexBlurredColor += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + kernelOffsets[i] * blurAmount * texelSize);
    }
    hexBlurredColor /= 6.0;
    result = hexBlurredColor; // set result to hex blurred color

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define the chromatic aberration function
float3 aberrate(float3 color, float2 uv)
{
    float3 aberratedColor = color;
    float2 aberrationOffset = float2(sin(uv.x * 3.14159), cos(uv.y * 3.14159)) * ABBERATION_AMOUNT;
    float focalLength = 110.0;
    float aperture = 2.8;
    float aberrationRadius = ABBERATION_AMOUNT * focalLength / aperture;
    aberratedColor.r = tex2D(DIFFUSE_SAMPLER, uv + aberrationOffset * aberrationRadius).r;
    aberratedColor.g = tex2D(DIFFUSE_SAMPLER, uv).g;
    aberratedColor.b = tex2D(DIFFUSE_SAMPLER, uv - aberrationOffset * aberrationRadius).b;
    return aberratedColor;
}

float4 applyChromaticAbberation(const VtoP IN, in float4 result) : COLOR0
{
    // Calculate the aberrated color
    float3 aberratedColor = aberrate(result.rgb, IN.tex0.xy);

    // Add the aberrated color to the result
    result.rgb = aberratedColor;

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sharpening Shader
float4 applySharpen(const VtoP IN, in float4 result)
{

    result = applyChromaticAbberation(IN, result);

    // Sample neighboring pixels
    float4 texelSize = float4(1.0f / 2560.0f, 1.0f / 1440.0f, 0.0f, 0.0f);
    float4 center = result;
    float4 left = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy - texelSize.xy);
    float4 right = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + texelSize.xy);
    float4 top = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy - texelSize.xy * float2(0.0f, 1.0f));
    float4 bottom = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + texelSize.xy * float2(0.0f, 1.0f));

    // Calculate sharpening
    float4 sharpen = center * (1.0f + SHARPEN_AMOUNT) -
        (left + right + top + bottom) * SHARPEN_AMOUNT / 4.0f;

    // Add sharpening effect to the original result
    result.rgb += sharpen.rgb * 0.1f; // Adjust the amount of sharpening to your taste
    result.rgb = saturate(result.rgb); // Ensure the result is within the valid color range

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fake HDR function -- ported from ReShade
// Not actual HDR - It just tries to mimic an HDR look
float4 applyHDR(const VtoP IN, in float4 result) {
    float HDRPower = FAKEHDR_POWER;
    float radius1 = FAKEHDR_RADIUS1;
    float radius2 = FAKEHDR_RADIUS2;
    result = applyDOF(IN, result);;
    float3 color = applySharpen(IN, result);

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ambient lighting properties
float ambientIntensity = Ambient_Intensity;
float3 ambientColor = Ambiennt_Color;

// Ambient lighting
float4 applyAmbientLighting(const VtoP IN, in float4 result)
{
    // Calculate ambient lighting
    float3 ambient = ambientColor * ambientIntensity;
    result.rgb += ambient;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vignette Constants
const float vignetteAmount = VIGNETTE_AMOUNT;
const float vignetteRadius = VIGNETTE_RADIUS;
const float2 VIGNETTE_CENTER = float2(0.5, 0.5);
const float vignetteCurve = VIGNETTE_CURVE;
uniform float vignetteTransparency = 0.4; // Control vignette transparency

// Screen Vignette function
float4 applyVignette(VtoP IN, float4 result) : SV_Target{
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

// Overlay blending mode
float Overlay(float LayerA, float LayerB)
{
    return 2.0 * (LayerA * LayerB + (LayerA + LayerB) * (1.0 - LayerA * LayerB)) - LayerA - LayerB;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bloom Shader
float4 applyBloom(const VtoP IN, in float4 result) : COLOR
{
    // Define the bloom threshold and intensity
    float threshold = 0.9;
    float intensity = 10.0;
    float bloomRadiusMin = 0.01;
    float bloomRadiusMax = 0.1;

    // Sample the texture at the current position
    float4 sample = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);

    // Calculate luminance
    float luminance = dot(sample.rgb, float3(0.2126, 0.7152, 0.0722));

    // Check if the luminance is above the threshold
    if (luminance > threshold)
    {
        // Calculate bloom amount based on luminance
        float bloomAmount = (luminance - threshold) * intensity;

        // Calculate bloom radius based on luminance
        float bloomRadius = lerp(bloomRadiusMin, bloomRadiusMax, bloomAmount);

        // Define bloom kernel
        float2 kernelOffsets[9] = {
            float2(-bloomRadius, -bloomRadius),
            float2(0, -bloomRadius),
            float2(bloomRadius, -bloomRadius),
            float2(-bloomRadius, 0),
            float2(0, 0),
            float2(bloomRadius, 0),
            float2(-bloomRadius, bloomRadius),
            float2(0, bloomRadius),
            float2(bloomRadius, bloomRadius)
        };

        // Simulate light scattering by adding a Gaussian blur
        float4 blurredSample = float4(0, 0, 0, 0);
        for (int i = 0; i < 9; i++)
        {
            blurredSample += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + kernelOffsets[i]);
        }
        blurredSample /= 9.0;

        // Add blurred sample to result
        result.rgb += blurredSample.rgb * bloomAmount;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filmgrain shader
float4 applyFilmGrain(const VtoP IN, in float4 result) : COLOR
{
    // Sample a noise texture to simulate film grain
    float2 noiseUV = IN.tex0.xy * 10.0f;
    float noise = generateNoise(noiseUV);

    // Apply the noise to the screen texture
    result.rgb += noise * 0.015f;

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tonemap Shaders

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
#ifdef USE_Filmic
float3 FilmicTonemap(float3 color)
{
    float luma = dot(color, Luma709);
    float3 ret = color * (luma * (1.0 + luma / 7.0)) / (luma + 0.1);
    return ret;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader Passes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 applyEffects(const VtoP IN, in float4 result) 
{   
    result = applyHDR(IN, result);

#ifdef USE_Filmic
    result.rgb = FilmicTonemap(result.rgb);
#endif

#ifdef USE_ACES2
    result.rgb = ACES2Tonemap(result.rgb);
#endif

    return result;
}

#ifdef USE_EFFECTS
float4 ShaderPass(const VtoP IN, in float4 result) 
{
    result = applyEffects(IN, result);
    result = applyAmbientLighting(IN, result);
    result = applyBloom(IN, result);
    result = applyFilmGrain(IN, result);
    result = applyVignette(IN, result);
    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// you shouldnt touch this

// Visualtreatment Function
float4 PS_VisualTreatment(const VtoP IN) : COLOR
{
    float4 result = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);

#ifdef USE_EFFECTS
    result = (ShaderPass(IN, result));
#else
    // Full screen image
    // Displays the image in fullscreen
    float4 screenTex = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);
    result = screenTex;
#endif

    // Convert from log space to linear space and clamp[0,1]
    result.xyz = DeCompressColourSpace(result.xyz);

#ifdef USE_LUT
    // LUT filter (enable LUT)
    float3 samplePosition = result.xyz;
    float3 sampledColor = tex3D(VOLUMEMAP_SAMPLER, samplePosition).xyz;
    result.xyz = sampledColor;
#endif

    // clamp anything above 1
    result.rgb = saturate(result.rgb);

    // Lens contamination gradient effect 
    result.xyz = AddGradient(result, IN.tex0.xy);

    // cop intro -- ported from Undercover
    float3 copTreatment = tex3D(BLENDVOLUMEMAP_SAMPLER, result.xyz).xyz;
    result.xyz = lerp(result.xyz, copTreatment, COP_INTRO_SCALE);

    // Speed-breaker VIGNETTE
    // Sample the vignette texture
    float4 vignette = tex2D(MISCMAP2_SAMPLER, IN.tex0.xy);
    float mask = saturate(vignette.y);
    result.xyz *= 1.1f - (mask * VIGNETTE_SCALE);
    float luminance = dot(LUMINANCE_VECTOR, result.xyz);

    // Pursuit / speed breaker
    result.xyz = lerp(result.xyz, luminance * 1.5, saturate(vignette.w) * BREAKER_INTENSITY);

#ifndef BLOOM_DISABLE
#ifdef BLOOM_DEBUG
    result.xyz = Bloom * 0.5; //ADD
#endif
#endif

    return result;
}
#endif

// Cliff-fall Function
float4 PS_UvesOverCliff(const VtoP IN) : COLOR
{
    float4 result;

    // Initialize result with screen texture
    float4 screenTex = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);
    result = screenTex;

    // Apply vignette effect
    result = applyVignette(IN, result) * 2;

    // Apply LUT filter with color grading
    float3 samplePosition = result.xyz;
    float3 sampledColor = tex3D(VOLUMEMAP_SAMPLER, samplePosition).xyz;
    float3 gradedColor = sampledColor * float3(1.2, 1.1, 1.0); // warm tone
    result.xyz = gradedColor;

    // VIGNETTE
    float4 vignette = tex2D(MISCMAP2_SAMPLER, IN.tex0.xy);
    float mask = saturate(vignette.y);
    result.xyz *= 1.1f - (mask * VIGNETTE_SCALE);

    // Calculate luminance and max channel
    float luminance = dot(LUMINANCE_VECTOR, result.xyz);
    float maxChannel = max(max(result.x, result.y), result.z);

    return result;
}
