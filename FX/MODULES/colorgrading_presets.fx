#if USE_PRESET == 1
// 🎬 Teal & Orange
#define RED_CHANNEL   float3(1.00, 0.20, 0.10)
#define GREEN_CHANNEL float3(0.10, 1.00, 0.15)
#define BLUE_CHANNEL  float3(0.05, 0.25, 0.95)
#define LUMA   1.00
#define CHROMA 1.10

#elif USE_PRESET == 2
// ☁️ Soft Desaturation
#define RED_CHANNEL   float3(0.85, 0.20, 0.20)
#define GREEN_CHANNEL float3(0.20, 0.85, 0.20)
#define BLUE_CHANNEL  float3(0.20, 0.20, 0.85)
#define LUMA   1.05
#define CHROMA 0.85

#elif USE_PRESET == 3
// 🌄 Warm Contrast
#define RED_CHANNEL   float3(1.00, 0.30, 0.10)
#define GREEN_CHANNEL float3(0.25, 1.00, 0.30)
#define BLUE_CHANNEL  float3(0.10, 0.15, 0.90)
#define LUMA   0.95
#define CHROMA 1.15

#elif USE_PRESET == 4
// 🖤 Fade to Grey
#define RED_CHANNEL   float3(0.60, 0.50, 0.45)
#define GREEN_CHANNEL float3(0.45, 0.65, 0.45)
#define BLUE_CHANNEL  float3(0.45, 0.45, 0.65)
#define LUMA   1.00
#define CHROMA 0.70

#elif USE_PRESET == 5
// 🌫️ Night City Enhanced
#define RED_CHANNEL   float3(1.00, 0.30, 0.20)
#define GREEN_CHANNEL float3(0.25, 1.00, 0.40)
#define BLUE_CHANNEL  float3(0.25, 0.40, 1.00)
#define LUMA   0.95
#define CHROMA 1.10

#elif USE_PRESET == 6
// 🌞 Golden Morning
#define RED_CHANNEL   float3(1.10, 0.40, 0.20)
#define GREEN_CHANNEL float3(0.45, 1.00, 0.40)
#define BLUE_CHANNEL  float3(0.20, 0.25, 0.85)
#define LUMA   1.10
#define CHROMA 0.95

#elif USE_PRESET == 7
// 🧃 Natural Pop
#define RED_CHANNEL   float3(1.00, 0.10, 0.10)
#define GREEN_CHANNEL float3(0.10, 1.00, 0.10)
#define BLUE_CHANNEL  float3(0.10, 0.10, 1.00)
#define LUMA   1.00
#define CHROMA 1.00

#elif USE_PRESET == 8
// 🎛 Neon Shadow
#define RED_CHANNEL   float3(0.95, 0.20, 0.35)
#define GREEN_CHANNEL float3(0.25, 0.85, 0.45)
#define BLUE_CHANNEL  float3(0.35, 0.45, 1.00)
#define LUMA   0.90
#define CHROMA 1.20

#elif USE_PRESET == 9
// 🌃 NFS Heat – "Midnight Glow"
#define RED_CHANNEL   float3(1.00, 0.30, 0.45)
#define GREEN_CHANNEL float3(0.30, 1.00, 0.45)
#define BLUE_CHANNEL  float3(0.40, 0.40, 1.00)
#define LUMA   0.95
#define CHROMA 1.10

#elif USE_PRESET == 10
// 🚗 NFS 2015 – "Urban Rain"
#define RED_CHANNEL   float3(0.80, 0.40, 0.50)
#define GREEN_CHANNEL float3(0.30, 0.90, 0.40)
#define BLUE_CHANNEL  float3(0.40, 0.50, 1.00)
#define LUMA   1.00
#define CHROMA 1.05

#elif USE_PRESET == 11
// 🧪 Stylized Realism
#define RED_CHANNEL   float3(1.00, 0.20, 0.10)
#define GREEN_CHANNEL float3(0.15, 1.00, 0.25)
#define BLUE_CHANNEL  float3(0.20, 0.25, 1.00)
#define LUMA   1.00
#define CHROMA 1.00

#elif USE_PRESET == 12
// 🟡 Horizon
#define RED_CHANNEL   float3(1.00, 0.40, 0.20)
#define GREEN_CHANNEL float3(0.40, 1.00, 0.30)
#define BLUE_CHANNEL  float3(0.20, 0.25, 0.90)
#define LUMA   1.10
#define CHROMA 1.05

#elif USE_PRESET == 13
// 🎮 Night-Runners
#define RED_CHANNEL   float3(1.00, 0.30, 0.20)
#define GREEN_CHANNEL float3(0.25, 1.00, 0.40)
#define BLUE_CHANNEL  float3(0.25, 0.40, 1.00)
#define LUMA   0.95
#define CHROMA 1.10

#elif USE_PRESET == 14
// 🔥 Color Boost
#define RED_CHANNEL   float3(1.00, 0.00, 0.05)
#define GREEN_CHANNEL float3(0.00, 1.00, 0.05)
#define BLUE_CHANNEL  float3(0.00, 0.00, 1.00)
#define LUMA   1.00
#define CHROMA 1.25

#elif USE_PRESET == 15
// 🌌 Carbon Drift – Noir Glow
#define RED_CHANNEL   float3(0.90, 0.20, 0.35)
#define GREEN_CHANNEL float3(0.25, 0.85, 0.40)
#define BLUE_CHANNEL  float3(0.40, 0.35, 1.10)
#define LUMA   0.90
#define CHROMA 1.15

#elif USE_PRESET == 16
// 🟣 Tokyo Drift – Purple Lights
#define RED_CHANNEL   float3(0.85, 0.25, 0.40)
#define GREEN_CHANNEL float3(0.20, 0.90, 0.35)
#define BLUE_CHANNEL  float3(0.50, 0.45, 1.10)
#define LUMA   0.95
#define CHROMA 1.20

#elif USE_PRESET == 17
// 🌫️ Misty Mountains – Touge Dawn
#define RED_CHANNEL   float3(0.95, 0.35, 0.25)
#define GREEN_CHANNEL float3(0.35, 0.95, 0.40)
#define BLUE_CHANNEL  float3(0.30, 0.35, 0.90)
#define LUMA   1.05
#define CHROMA 0.85

#elif USE_PRESET == 18
// 🌇 Sunset Overdrive – Miami Carbon
#define RED_CHANNEL   float3(1.10, 0.40, 0.30)
#define GREEN_CHANNEL float3(0.35, 1.00, 0.50)
#define BLUE_CHANNEL  float3(0.30, 0.35, 1.00)
#define LUMA   1.00
#define CHROMA 1.10

#elif USE_PRESET == 19
// 🎥 Cinematic Desat – Carbon Film
#define RED_CHANNEL   float3(0.80, 0.30, 0.30)
#define GREEN_CHANNEL float3(0.30, 0.80, 0.30)
#define BLUE_CHANNEL  float3(0.30, 0.30, 0.80)
#define LUMA   0.95
#define CHROMA 0.75
#endif