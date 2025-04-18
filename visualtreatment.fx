////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Visual Treatment
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
//-- 🔧 System / Global Settings --//
//////////////////////////////////////////
#define SCREEN_WIDTH 2560.0f    // X-Axis resolution (eg. 1920, 2560, 3840...)
#define SCREEN_HEIGHT 1440.0f   // Y-Axis resolution (eg. 1080, 1440, 2160...)
#define USE_LOG_TO_LINEAR 0     // 0 = OFF, 1 = ON; Convert from log space to linear space

//////////////////////////////////////////
//-- 🎛 Master Toggles / Pipeline --//
//////////////////////////////////////////
#define USE_POST_EFFECTS 1      // 0 = OFF, 1 = ON; Makes Post-Effects usable
#define USE_LUT 0               // 0 = OFF, 1 = ON; Toggles In-Game LUT

//////////////////////////////////////////
//-- 🌞 Basic Image Adjustments --//
//////////////////////////////////////////
#define BRIGHTNESS 0.95         // Adjusts Games Brightness
#define CONTRAST 1.05           // Adjusts Games Contrast

//////////////////////////////////////////
//-- 🎨 Tonemapping --//
//////////////////////////////////////////
#define USE_TONEMAPPING 1       // 0 = OFF, 1 = ON; Toggles Tonemapping
#define TONEMAP_VARIANT 5       // 1 = Reinhard, 2 = ACES2, 3 = ACES Legacy, 4 = Uncharted2, 5 = Unreal, 6 = Lottes, 7 = Gran Turismo, 8 = Narkowicz 2015, 9 = AgX, 10 = FilmicALU, 11 = NFS Heat Style
#define EXPOSURE_BIAS 0.30      // Defines Exposure-Bias for Tonemaps (needs adjustment on tonemap-change)

//////////////////////////////////////////
//-- 🌈 Color Grading & Look --//
//////////////////////////////////////////
#define COLOR_TEMPERATURE 0.05  // Color-Temperature > 0 for warmer, < 0 for cooler
#define USE_PRESET 14           // 0 for User-Config (below), 1-19 to choose from preset.fx

#if USE_PRESET == 0
// User-Config can be set here!
    #define RED_CHANNEL   float3(1.0, 0.0, 0.0) // Sets Color in (r, g, b) for Red-Channel
    #define GREEN_CHANNEL float3(0.0, 1.0, 0.0) // Sets Color in (r, g, b) for Green-Channel
    #define BLUE_CHANNEL  float3(0.0, 0.0, 1.0) // Sets Color in (r, g, b) for Blue-Channel

    #define LUMA   1.05         // Sets Color-Luminance
    #define CHROMA 1.25         // Sets Color-Saturation
#else
    #include "MODULES/colorgrading_presets.fx"
#endif

//////////////////////////////////////////
//-- ✨ Glow & Bloom --//
//////////////////////////////////////////
#define USE_BLOOM 1             // 0 = OFF, 1 = ON; Toggles Bloom-Effect
#define BLOOM_INTENSITY 0.35    // Sets Overall Bloom-Intensity
#define BLOOM_RADIUS 10.00      // Sets Bloom Spread-Radius

//////////////////////////////////////////
//-- 📈 Curves & Legacy Stuff --//
//////////////////////////////////////////
#define USE_LEGACY_CURVES 1     // 0 = OFF, 1 = ON; Toggles Legacy-Curves from 360 Version (thx to Sh2dow)

//////////////////////////////////////////
//-- 📸 Lens FX --//
//////////////////////////////////////////
#define USE_LENSDIRT 1           // 0 = OFF, 1 = ON; Toggles Lens-Dirt Effect
#define LENS_DIRT_INTENSITY 1.1  // Sets Dirt-Intensity

//////////////////////////////////////////
//-- 🧠 Perception Tools --//
//////////////////////////////////////////
#define USE_HDR 1                   // 0 = OFF, 1 = ON; Toggles FakeHDR Effect
#define FAKEHDR_POWER 1.275         // Sets FakeHDR Intensity

#define USE_DESATURATION 0          // 0 = OFF, 1 = ON; Toggles Desaturation Effect
#define DESATURATION_AMOUNT 0.00    // Sets Desaturation Intensity

#define USE_SHARPEN 1               // 0 = OFF, 1 = ON; Toggles Sharpen Effect
#define SHARPEN_AMOUNT 0.05f        // Sets Sharpen Intensity

//////////////////////////////////////////
//-- 🖼️ Aesthetic FX --//
//////////////////////////////////////////
#define USE_VIGNETTE 0              // 0 = OFF, 1 = ON; Toggles Vignette Effect
#define VIGNETTE_AMOUNT 0.60        // Sets Vignette Intensity
#define VIGNETTE_RADIUS 1.85        // Sets Vignette Spread Radius
#define VIGNETTE_CURVE 2.25         // Sets Vignette Blend Curve

#define USE_ABERRATION 0            // 0 = OFF, 1 = ON; Toggles Aberration Effect
#define ABERRATION_STRENGTH 0.005   // Sets Aberration Amount

#define USE_FILMGRAIN 1             // 0 = OFF, 1 = ON; Toggles Filmgrain Effect
#define FILM_GRAIN_STRENGTH 0.0125  // Sets Filmgrain Strenght

//////////////////////////////////////////
//-- 🎮 Ingame-Scenario Effects --//
//////////////////////////////////////////
#define USE_CUSTOM_CLIFF_EFFECT 1                           // 0 = OFF, 1 = ON; Toggles Custom Cliff Fall Effect
#define CLIFF_COLOR float3(0.8, 0.8, 0.8)                   // Sets Color in (r, g, b)
#define CLIFF_BLEND 1.00                                    // Sets Blend Amount (1 = max)

#define USE_CUSTOM_COP_ENGANGEMENT 1                        // 0 = OFF, 1 = ON; Toggles Custom Cop Engangement
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

#define DO_COLOUR_FADE(value)		value

technique visualtreatment
{
	pass p0
	{
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_VisualTreatment(DO_COLOUR_FADE(true));
    }
}

technique visualtreatment_enchanced
{
	pass p0
	{
        VertexShader = compile vs_3_0 vertex_shader_passthru();
        PixelShader = compile ps_3_0 PS_VisualTreatment(DO_COLOUR_FADE(true));
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
