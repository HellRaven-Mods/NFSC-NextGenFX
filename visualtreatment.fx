////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Visual Treatment
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Here you can toggle custom shaders all at once
// put "//" infront of the "#define" function to disable 
#define USE_EFFECTS

// Here you can toggle the Filter (Lut)
// put "//" infront of the "#define" function to disable 
#define USE_LUT

// Here you can toggle the Color decompression
// put "//" infront of the "#define" function to disable 
//#define USE_LOG_to_LINIAR

// Here you can put your screen resoluton
// Important for Motion-Blur and some other effects
#define SCREEN_WIDTH 2560.0f                                    // X-Axis resolution (eg. 1920, 2560, 3840...)
#define SCREEN_HEIGHT 1440.0f                                   // Y-Axis resolution (eg. 1080, 1440, 2160...)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Effect Config
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Comment the line you dont wanna use with "//"
// The tonemap that isnt commented will get applied
//#define USE_ACES2
#define USE_Filmic

// Fake HDR
#define FAKEHDR_POWER 1.320                                     // Intensity of the HDR effect
#define FAKEHDR_RADIUS1 1.395                                   // Sets outter fade radius
#define FAKEHDR_RADIUS2 1.380                                   // Sets inner fade radius

// Colorgrading for Fake HDR
// for best results in colorgrading, turn off "USE_LUT" first
#define RED_CHANNEL     float3(2.100f, 0.150f, 0.150f)
#define GREEN_CHANNEL   float3(0.150f, 1.850f, 0.150f)
#define BLUE_CHANNEL    float3(0.150f, 0.150f, 1.975f)

// Chromatic Abberation (implemented in HDR function)
#define ABBERATION_AMOUNT 0.0035                                 // Intensity of chromatic abberation (too high values make game look bad)

// Sharpening (implemented in HDR function)
#define SHARPEN_AMOUNT 25.00f                                     // Intensity of the sharpening effect (too high values make game look bad)

// Ambient Lighting
// put "//" infront of the "#define USE_AMBIENTLIGHT" function to disable 
#define USE_AMBIENTLIGHT

#define Ambient_Intensity 0.155                                  // Intensity of ambient light                    
#define Ambiennt_Color  float3(0.4, 0.45, 0.5)                   // Color of ambient light 

// Vignette
// put "//" infront of the "#define USE_VIGNETTE" function to disable 
#define USE_VIGNETTE

#define VIGNETTE_AMOUNT 0.4                                      // Intensity of vignette
#define VIGNETTE_RADIUS 1.55                                     // Radius for vignette (determines if effects gets applied as circle, eclipse etc)
#define VIGNETTE_CURVE 2.6                                       // sets the fade length

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

technique visualtreatment
{
	pass p0
	{
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_VisualTreatment();
    }
}

technique visualtreatment_enchanced
{
	pass p0
	{
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_VisualTreatment();
    }
}

technique motionblur
{
	pass p0
	{
        VertexShader = compile vs_3_0 VS_MotionBlur();
		PixelShader	 = compile ps_3_0 PS_MotionBlur();
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

