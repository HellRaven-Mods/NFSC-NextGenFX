////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Visual Treatment
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Here you can put your screen resoluton
// Important for Motion-Blur and some other effects
#define SCREEN_WIDTH 2560.0f                                    // X-Axis resolution (eg. 1920, 2560, 3840...)
#define SCREEN_HEIGHT 1440.0f                                   // Y-Axis resolution (eg. 1080, 1440, 2160...)

// Here you can toggle custom shaders all at once
// put "//" infront of the "#define USE_EFFECTS" function to disable 
#define USE_EFFECTS

// Here you can toggle the Filter (Lut)
// put "//" infront of the "#define USE_LUT" function to disable 
#define USE_LUT

// DOF Shader
// put "//" infront of the "#define USE_DOF" function to disable 
#define USE_DOF

// Here you can toggle the Color decompression
// put "//" infront of the "#define" function to disable 
#define USE_LOG_to_LINIAR

// FOG
// put "//" infront of the "#define USE_FOG" function to disable 
//#define USE_FOG

// PAUSE BLUR
// delete "//" infront of the "#define DONTDOPAUSEBLUR" function to disable 
//#define DONTDOPAUSEBLUR

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Effect Config
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Brightness Adjustment
// if image maybe too dark, incraese slowly
#define BRIGHTNESS 1.00

// Cop Intro Color
#define INTRO_COLOR float3(0.4, 0.0, 0.0)                       // Sets the color
#define INTRO_BLEND_AMOUNT 0.15                                 // Sets the blend amount (0= no blend, 1= full overblend)

// Cliff Fall Color
#define CLIFF_COLOR float3(0.8, 0.8, 0.8)                       // Sets the color
#define CLIFF_BLEND 1.00                                        // Sets the blend amount (0= no blend, 1= full overblend)

// Speed-breaker Effect
#define SPEEDBREAKER_EFFECT_COLOR float3(0.8, 0.8, 0.8)         // Sets the color
#define SPEEDBREAKER_EFFECT_BLEND 1.00                          // Sets the blend amount (0= no blend, 1= full overblend)

// Comment the line you dont wanna use with "//"
//// The tonemap that isnt commented will get applied
#define USE_ACES2
//#define USE_Filmic

// Fake HDR
#define USE_HDR
#define FAKEHDR_POWER 1.200                                     // Intensity of the HDR effect
#define FAKEHDR_RADIUS1 0.800                                   // Sets inner fade radius
#define FAKEHDR_RADIUS2 0.800                                   // Sets outter fade radius

// Colorgrading for Fake HDR
// for best results in colorgrading, turn off "USE_LUT" first
#define RED_CHANNEL     float3(2.000f, 0.100f, 0.100f)          // red level
#define GREEN_CHANNEL   float3(0.100f, 2.000f, 0.100f)          // green level
#define BLUE_CHANNEL    float3(0.100f, 0.100f, 2.000f)          // blue level

// Desatuartion
// if colors too powerfull
#define USE_DESATURATION 
#define DESATURATION_AMOUNT 0.00                                 // 0.0 = no desaturation, 1.0 = full desaturation

// Sharpening
// put "//" infront of the "#define USE_SHARPEN" function to disable 
#define USE_SHARPEN
#define SHARPEN_AMOUNT 20.00f                                    // Intensity of the sharpening effect (too high values make game look bad) 

// Vignette
// put "//" infront of the "#define USE_VIGNETTE" function to disable 
//#define USE_VIGNETTE
#define VIGNETTE_AMOUNT 0.6                                      // Intensity of vignette
#define VIGNETTE_RADIUS 1.55                                     // Radius for vignette (determines if effects gets applied as circle, eclipse etc)
#define VIGNETTE_CURVE 2.6                                       // sets the fade length

// Ambient Lighting
// put "//" infront of the "#define USE_AMBIENTLIGHT" function to disable 
#define USE_AMBIENTLIGHT
#define Ambient_Intensity 0.150                                  // Intensity of ambient light                    
#define Ambiennt_Color  float3(0.4, 0.45, 0.5)                   // Color of ambient light

// BLOOM
// put "//" infront of the "#define USE_BLOOM" function to disable 
#define USE_BLOOM

// Chromatic Abberation
// put "//" infront of the "#define USE_ABBERATION" function to disable 
#define USE_ABBERATION

// Camera Distortion
// put "//" infront of the "#define USE_DISTORTIAN" function to disable
#define USE_DISTORTION

// Film-Grain
// put "//" infront of the "#define USE_FILMGRAIN" function to disable 
#define USE_FILMGRAIN

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dont edit these
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Luma709 float3(0.2126, 0.7152, 0.0722)

#include "global.h"
#include "gradient.h"
#include "visualtreatment.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dont edit these
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DO_DOF(value)               value
#define DO_COLOUR_FADE(value)		value

technique visualtreatment
{
	pass p0
	{
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_VisualTreatment(DO_DOF(false), DO_COLOUR_FADE(false));
    }
}

technique visualtreatment_enchanced
{
	pass p0
	{
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_VisualTreatment(DO_DOF(true), DO_COLOUR_FADE(true));
    }
}

technique motionblur
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS_MotionBlur();
        PixelShader = compile ps_3_0 PS_MotionBlur();
    }
}

technique composite_blur
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS_CompositeBlur();
        PixelShader = compile ps_3_0 PS_CompositeBlur();
    }
}

technique uvesovercliff
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_UvesOverCliff();
    }
}

technique uvesovercliffdarken
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_UvesOverCliff();
    }
}

technique screen_passthru
{
	pass p0
	{
        VertexShader = compile vs_3_0 vertex_shader_passthru();
		PixelShader	 = compile ps_3_0 PS_PassThru();
	}
}

