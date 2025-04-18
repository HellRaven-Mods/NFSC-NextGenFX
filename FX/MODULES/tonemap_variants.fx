
// Tonemap Index Guide:
// 1 = Reinhard, 2 = ACES2, 3 = ACES Legacy, 4 = Uncharted2, 5 = Unreal, 6 = Lottes, 7 = Gran Turismo, 8 = Narkowicz 2015, 9 = AgX, 10 = FilmicALU, 11 = NFS Heat Style

float3 Tonemap_Reinhard(float3 color)
{
    return saturate(color / (1.0 + color));
}

float3 Tonemap_ACES2(float3 color)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0, 1.0);
}

float3 Tonemap_ACESLegacy(float3 color)
{
    const float3x3 ACESInputMat =
    {
        { 0.59719, 0.35458, 0.04823 },
        { 0.07600, 0.90834, 0.01566 },
        { 0.02840, 0.13383, 0.83777 }
    };
    const float3x3 ACESOutputMat =
    {
        { 1.60475, -0.53108, -0.07367 },
        { -0.10208, 1.10813, -0.00605 },
        { -0.00327, -0.07276, 1.07602 }
    };
    color = mul(ACESInputMat, color);
    color = (color * (color + 0.0245786)) / (color * (0.983729 * color + 0.4329510) + 0.238081);
    return saturate(mul(ACESOutputMat, color));
}

float3 Tonemap_Uncharted2(float3 color)
{
    float A = 0.15, B = 0.50, C = 0.10, D = 0.20, E = 0.02, F = 0.30, W = 11.2;
    float3 curr = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
    float whiteScale = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
    return saturate(curr / whiteScale);
}

float3 Tonemap_Unreal(float3 color)
{
    color = max(color - 0.004, 0.0);
    return saturate((color * (6.2 * color + 0.5)) / (color * (6.2 * color + 1.7) + 0.06));
}

float3 Tonemap_Lottes(float3 color)
{
    float a = 1.6, d = 0.977, hdrMax = 8.0;
    float midIn = 0.18, midOut = 0.267;
    float exposure = midOut / (pow(midIn, d) * (1.0 + pow(midIn / hdrMax, a - d)));
    float3 numerator = pow(color, d) * (1.0 + pow(color / hdrMax, a - d));
    return saturate(numerator * exposure);
}

float3 Tonemap_GranTurismo(float3 color)
{
    float ss = 0.22, ls = 0.3, la = 0.1, ts = 0.2, tn = 0.01, td = 0.3, lw = 11.2;
    float3 num = color * (ss * color + la * ls) + ts * tn;
    float3 denom = color * (ss * color + ls) + ts * td;
    float whiteScale = (lw * (ss * lw + la * ls) + ts * tn) / (lw * (ss * lw + ls) + ts * td);
    return saturate(num / denom / whiteScale);
}

float3 Tonemap_Narkowicz(float3 color)
{
    return saturate(color / (color + 0.155) * 1.019);
}

float3 Tonemap_AgX(float3 color)
{
    const float A = 0.22, B = 0.30, C = 0.10, D = 0.20, E = 0.01, F = 0.30;
    float3 x = max(0.0, color - E);
    return (x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F);
}

float3 Tonemap_FilmicALU(float3 color)
{
    float3 x = max(float3(0.0, 0.0, 0.0), color - 0.004);
    return saturate((x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06));
}

float3 Tonemap_NFSHeatStyle(float3 color)
{
    color = pow(color, 1.1);
    color.r *= 1.05;
    color.g *= 0.98;
    color.b *= 1.10;
    float3 mapped = color / (1.0 + color);
    return saturate(mapped);
}