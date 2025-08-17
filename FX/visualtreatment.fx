////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Visual Treatment
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
//-- 🔧 System / Global Settings --//
//////////////////////////////////////////
#define SCREEN_WIDTH 2560.0f    // X-Axis resolution (eg. 1920, 2560, 3840...)
#define SCREEN_HEIGHT 1440.0f   // Y-Axis resolution (eg. 1080, 1440, 2160...)
#define USE_LOG_TO_LINEAR 1     // 0 = OFF, 1 = ON; Convert from log space to linear space

//////////////////////////////////////////
//-- 🎛 Master Toggles / Pipeline --//
//////////////////////////////////////////
#define USE_POST_EFFECTS 1         	// 0 = OFF, 1 = ON; Makes Post-Effects usable
#define USE_LUT 0                  	// 0 = OFF, 1 = ON; Toggles In-Game LUT
#define MOTIONBLUR_QUALITY 1       	// 0 = Standard, 1 = High Quality
#define MOTIONBLUR_MASK_SCALE 0.75  // Global Scaling Multiplier for Motionblur Mask; < 1 for lesser, > 1 for bigger (scales inwards, means the higher the number the more gets blurred)

//////////////////////////////////////////
//-- 🌞 Basic Image Adjustments --//
//////////////////////////////////////////
#define BRIGHTNESS 1.00         // Adjusts Games Brightness
#define CONTRAST 1.05           // Adjusts Games Contrast

//////////////////////////////////////////
//-- 🎨 Tonemapping --//
//////////////////////////////////////////
#define USE_TONEMAPPING 1       // 0 = OFF, 1 = ON; Toggles Tonemapping
#define TONEMAP_VARIANT 8       // 1 = Reinhard, 2 = ACES2, 3 = ACES Legacy, 4 = Uncharted2, 5 = Unreal, 6 = Lottes, 7 = Gran Turismo, 8 = Narkowicz 2015, 9 = AgX, 10 = FilmicALU, 11 = NFS Heat Style, 12 = Reinhard2, 13 = Uchimura 2017
#define EXPOSURE_BIAS 0.25      // Defines Exposure-Bias for Tonemaps (needs adjustment on tonemap-change)

//////////////////////////////////////////
//-- 🌈 Color Grading & Look --//
//////////////////////////////////////////
#define COLOR_TEMPERATURE 0.20  // Color-Temperature > 0 for warmer, < 0 for cooler
#define USE_WHITEBALANCE 1      // 0 = OFF, 1 = ON; Toggles Adaptve Whitebalance
#define USE_PRESET 17           // 0 for User-Config (below), 1-19 to choose from preset.fx  (zu intensiv: 7,8,11,12,13)
#define SPLIT_TONE 1            // 0 for User-Config (below), 1 for standard split-tone (warm highlights, cold shadows)

#if USE_PRESET == 0
// User-Config can be set here!
    #define RED_CHANNEL   float3(1.0, 0.0, 0.0) 	// Sets Color in (r, g, b) for Red-Channel
    #define GREEN_CHANNEL float3(0.0, 1.0, 0.0) 	// Sets Color in (r, g, b) for Green-Channel
    #define BLUE_CHANNEL  float3(0.0, 0.0, 1.0) 	// Sets Color in (r, g, b) for Blue-Channel

    #define LUMA   1.00         // Sets Color-Luminance
    #define CHROMA 1.00         // Sets Color-Saturation
#else
    #include "MODULES/colorgrading_presets.fx"
#endif

#if SPLIT_TONE == 0
// User-Config can be set here!
    #define SHADOW_TINT      float3(0.2, 0.3, 0.7) 		// Sets Color in (r, g, b) for Shadows (dark areas)
    #define HIGHLIGHT_TINT   float3(1.0, 0.8, 0.6) 		// Sets Color in (r, g, b) for Highlights (brighter areas)
#endif

//////////////////////////////////////////
//-- ✨ Glow & Bloom --//
//////////////////////////////////////////
#define USE_BLOOM 1             	// 0 = OFF, 1 = ON; Toggles Bloom-Effect
#define BLOOM_INTENSITY 1.25    	// Sets Overall Bloom-Intensity

//////////////////////////////////////////
//-- 📈 Curves & Legacy Stuff --//
//////////////////////////////////////////
#define USE_LEGACY_CURVES 1     	// 0 = OFF, 1 = ON; Toggles Legacy-Curves from 360 Version (thx to Sh2dow)

//////////////////////////////////////////
//-- 📸 Lens FX --//
//////////////////////////////////////////
#define USE_LENSDIRT 1          	// 0 = OFF, 1 = ON; Toggles Lens-Dirt Effect
#define LENS_DIRT_INTENSITY 0.4 	// Sets Dirt-Intensity

//////////////////////////////////////////
//-- 🧠 Perception Tools --//
//////////////////////////////////////////
#define USE_HDR 1                   // 0 = OFF, 1 = ON; Toggles FakeHDR Effect
#define FAKEHDR_POWER 2.850         // Sets FakeHDR Intensity

#define USE_DESATURATION 0          // 0 = OFF, 1 = ON; Toggles Desaturation Effect
#define DESATURATION_AMOUNT 0.00    // Sets Desaturation Intensity

#define USE_SHARPEN 1               // 0 = OFF, 1 = ON; Toggles Sharpen Effect
#define SHARPEN_AMOUNT 0.75f        // Sets Sharpen Intensity

//////////////////////////////////////////
//-- 🖼️ Aesthetic FX --//
//////////////////////////////////////////
#define USE_VIGNETTE 1              // 0 = OFF, 1 = ON; Toggles Vignette Effect
#define VIGNETTE_AMOUNT 0.75        // Sets Vignette Intensity
#define VIGNETTE_RADIUS 0.85        // Sets Vignette Spread Radius
#define VIGNETTE_CURVE 1.35         // Sets Vignette Blend Curve

#define USE_ABERRATION 1            // 0 = OFF, 1 = ON; Toggles Aberration Effect
#define ABERRATION_RADIUS 0.008     // Sets Aberration Amount

#define USE_FILMGRAIN 0             // 0 = OFF, 1 = ON; Toggles Filmgrain Effect
#define FILM_GRAIN_STRENGTH 0.0150  // Sets Filmgrain Strenght

//////////////////////////////////////////
//-- 🎮 Ingame-Scenario Effects --//
//////////////////////////////////////////
#define USE_CUSTOM_CLIFF_EFFECT 1                           // 0 = OFF, 1 = ON; Toggles Custom Cliff Fall Effect
#define CLIFF_COLOR float3(0.8, 0.8, 0.8)                   // Sets Color in (r, g, b)
#define CLIFF_BLEND 1.00                                    // Sets Blend Amount (1 = max)

#define USE_CUSTOM_COP_ENGANGEMENT 0                        // 0 = OFF, 1 = ON; Toggles Custom Cop Engangement
#define INTRO_BLEND_COLOR  float3(0.4, 0.0, 0.0)            // Sets Color in (r, g, b)
#define INTRO_BLEND_AMOUNT 0.15                             // Sets Blend Amount (1 = max)

#define USE_CUSTOM_SPEEDBREAKER 1                           // 0 = OFF, 1 = ON; Toggles Custom Speedbreaker
#define SPEEDBREAKER_EFFECT_COLOR float3(0.8, 0.8, 0.8)     // Sets Color in (r, g, b)
#define SPEEDBREAKER_EFFECT_BLEND 1.00                      // Sets Blend Amount (1 = max)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 🧱 Internal Definitions (Dont Edit!!)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "visualtreatment.h"

#define DO_DOF(value)				value
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
        PixelShader = compile ps_3_0 PS_UvesOverCliff(DO_COLOUR_FADE(false));
    }
}

technique uvesovercliffdarken
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_UvesOverCliff(DO_COLOUR_FADE(true));
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
