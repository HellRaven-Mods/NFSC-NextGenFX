#if USE_PRESET == 1
// Neutral Base — reference look
#define RED_CHANNEL   float3(1.00, 0.00, 0.00)
#define GREEN_CHANNEL float3(0.00, 1.00, 0.00)
#define BLUE_CHANNEL  float3(0.00, 0.00, 1.00)
#define LUMA   1.00
#define CHROMA 1.00

#elif USE_PRESET == 2
// Tokyo Night — subtle teal/magenta tilt, slightly cool
#define RED_CHANNEL   float3(0.86, 0.05, 0.09)
#define GREEN_CHANNEL float3(0.04, 0.85, 0.11)
#define BLUE_CHANNEL  float3(0.05, 0.09, 0.86)
#define LUMA   0.98
#define CHROMA 1.06

#elif USE_PRESET == 3
// Tokyo Rain — cooler, a touch desaturated
#define RED_CHANNEL   float3(0.84, 0.05, 0.11)
#define GREEN_CHANNEL float3(0.03, 0.84, 0.13)
#define BLUE_CHANNEL  float3(0.04, 0.10, 0.86)
#define LUMA   0.96
#define CHROMA 1.03

#elif USE_PRESET == 4
// Underground 1 — Warm Streets (sodium vibe)
#define RED_CHANNEL   float3(0.92, 0.06, 0.02)
#define GREEN_CHANNEL float3(0.10, 0.86, 0.04)
#define BLUE_CHANNEL  float3(0.02, 0.06, 0.92)
#define LUMA   1.02
#define CHROMA 1.02

#elif USE_PRESET == 5
// Underground 1 — Garage (warmer mids, neutral blacks)
#define RED_CHANNEL   float3(0.93, 0.05, 0.02)
#define GREEN_CHANNEL float3(0.06, 0.90, 0.04)
#define BLUE_CHANNEL  float3(0.02, 0.06, 0.92)
#define LUMA   0.99
#define CHROMA 1.00

#elif USE_PRESET == 6
// Underground 2 — Cool Night (teal bias, clean whites)
#define RED_CHANNEL   float3(0.87, 0.06, 0.08)
#define GREEN_CHANNEL float3(0.06, 0.85, 0.09)
#define BLUE_CHANNEL  float3(0.05, 0.11, 0.84)
#define LUMA   0.98
#define CHROMA 1.04

#elif USE_PRESET == 7
// Underground 2 — Bayview (slightly brighter, mild teal/orange)
#define RED_CHANNEL   float3(0.88, 0.05, 0.08)
#define GREEN_CHANNEL float3(0.05, 0.86, 0.09)
#define BLUE_CHANNEL  float3(0.04, 0.11, 0.86)
#define LUMA   1.00
#define CHROMA 1.02

#elif USE_PRESET == 8
// Cyberpunk 2077 — V Default (cyan base, magenta accents)
#define RED_CHANNEL   float3(0.84, 0.05, 0.11)
#define GREEN_CHANNEL float3(0.06, 0.81, 0.13)
#define BLUE_CHANNEL  float3(0.05, 0.09, 0.86)
#define LUMA   0.97
#define CHROMA 1.08

#elif USE_PRESET == 9
// Cyberpunk 2077 — Afterlife (cooler green-teal club look)
#define RED_CHANNEL   float3(0.83, 0.06, 0.12)
#define GREEN_CHANNEL float3(0.06, 0.79, 0.15)
#define BLUE_CHANNEL  float3(0.05, 0.11, 0.84)
#define LUMA   0.97
#define CHROMA 1.03

#elif USE_PRESET == 10
// Filmic Soft — gentle, low-contrast, fewer saturated primaries
#define RED_CHANNEL   float3(0.96, 0.02, 0.02)
#define GREEN_CHANNEL float3(0.02, 0.96, 0.02)
#define BLUE_CHANNEL  float3(0.02, 0.02, 0.96)
#define LUMA   1.00
#define CHROMA 0.96

#elif USE_PRESET == 11
// Night Neutral+ — slightly richer color for night driving
#define RED_CHANNEL   float3(0.94, 0.03, 0.03)
#define GREEN_CHANNEL float3(0.03, 0.94, 0.03)
#define BLUE_CHANNEL  float3(0.03, 0.03, 0.94)
#define LUMA   1.02
#define CHROMA 1.02

#elif USE_PRESET == 12
// Desat City — modern documentary vibe
#define RED_CHANNEL   float3(0.95, 0.03, 0.02)
#define GREEN_CHANNEL float3(0.03, 0.95, 0.02)
#define BLUE_CHANNEL  float3(0.02, 0.03, 0.95)
#define LUMA   0.98
#define CHROMA 0.92

#elif USE_PRESET == 13
// Forza Horizon — Blue Hour (cool, airy)
#define RED_CHANNEL   float3(0.86, 0.08, 0.06)
#define GREEN_CHANNEL float3(0.04, 0.86, 0.10)
#define BLUE_CHANNEL  float3(0.03, 0.09, 0.88)
#define LUMA   0.99
#define CHROMA 1.04

#elif USE_PRESET == 14
// Gran Turismo 7 — Showroom Neutral (clean whites, near neutral)
#define RED_CHANNEL   float3(0.97, 0.02, 0.01)
#define GREEN_CHANNEL float3(0.01, 0.98, 0.01)
#define BLUE_CHANNEL  float3(0.02, 0.02, 0.96)
#define LUMA   1.00
#define CHROMA 0.98

#elif USE_PRESET == 15
// GTA V — Vinewood Nights (decent Magenta/Neon-like)
#define RED_CHANNEL   float3(0.87, 0.06, 0.07)
#define GREEN_CHANNEL float3(0.05, 0.86, 0.09)
#define BLUE_CHANNEL  float3(0.07, 0.08, 0.85)
#define LUMA   0.98
#define CHROMA 1.05

#elif USE_PRESET == 16
// Red Dead 2 — Cinematic Warm (warm mids, natural lights)
#define RED_CHANNEL   float3(0.93, 0.05, 0.02)
#define GREEN_CHANNEL float3(0.06, 0.90, 0.04)
#define BLUE_CHANNEL  float3(0.05, 0.06, 0.89)
#define LUMA   1.01
#define CHROMA 0.98

#elif USE_PRESET == 17
// The Witcher 3 — Skellige Cool (cool, slightly desaturated)
#define RED_CHANNEL   float3(0.85, 0.08, 0.07)
#define GREEN_CHANNEL float3(0.05, 0.84, 0.11)
#define BLUE_CHANNEL  float3(0.04, 0.12, 0.84)
#define LUMA   0.99
#define CHROMA 1.02

#elif USE_PRESET == 18
// ACC — Trackday Neutral+ (clear colors, slighty punchier)
#define RED_CHANNEL   float3(0.96, 0.03, 0.01)
#define GREEN_CHANNEL float3(0.02, 0.96, 0.02)
#define BLUE_CHANNEL  float3(0.01, 0.03, 0.96)
#define LUMA   1.02
#define CHROMA 1.00

#elif USE_PRESET == 19
// Midnight Club LA — LA Nights (mild cyan/amber, nightly)
#define RED_CHANNEL   float3(0.90, 0.06, 0.04)
#define GREEN_CHANNEL float3(0.05, 0.86, 0.09)
#define BLUE_CHANNEL  float3(0.04, 0.08, 0.88)
#define LUMA   1.00
#define CHROMA 1.04

#endif
