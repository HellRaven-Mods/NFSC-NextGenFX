////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Standard Effect
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "global.h"

shared float4x4 cmWorldView         : cmWorldView; //WORLDVIEW
shared float4x4 cmWorldMat          : cmWorldMat; //local to world matrix
shared float4 cmPrevWorldViewProj   : cmPrevWorldViewProj; //BASEALPHAREF;

shared float4 cvLocalCenter         : cvLocalCenter; //LOCALCENTER;
shared float4 cvBaseAlphaRef        : cvBaseAlphaRef; //BASEALPHAREF;
shared float4 cvBlurParams          : cvBlurParams; //IS_MOTIONBLUR_VIGNETTED;
shared float4 cvFogColour           : cvFogColour;
shared float3 cvLocalEyePos         : cvLocalEyePos; //LOCALEYEPOS;
shared float4 cvTextureOffset       : cvTextureOffset; //TEXTUREANIMOFFSET;


shared float cfMipMapBias           : cfMipMapBias;
shared float cfBrightness           : cfBrightness; //STANDARD_BRIGHTNESS

float4 cavLightColours[1]           : cavLightColours;
float4 cavLightPositions[1]         : cavLightPositions;
float4 cavLightDirections[10]       : REG_cavLightDirections;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const float FuzzWidth = 0.15f;

static const float magnitude_debug = 13.5;
static const float MaxParticleSize = 2.5f;

static const float MaterialShininess = 10000000;

static const float3 MaterialSpecular = float3(1.2, 1.2, 1.2);
static const float ParticleBrightness = 0.6;

// these should be artist controlled	
static const float3 ambient_colour = float3(0.4, 0.45, 0.5);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Samplers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_TEXTURE(MISCMAP1_TEXTURE)
sampler MISCMAP1_SAMPLER = sampler_state	// backbuffer for screen distortion
{
	ASSIGN_TEXTURE(MISCMAP1_TEXTURE)
	AddressU = CLAMP;
	AddressV = CLAMP;
	DECLARE_MIPFILTER(LINEAR)
	DECLARE_MINFILTER(LINEAR)
	DECLARE_MAGFILTER(LINEAR)
};

DECLARE_TEXTURE(OPACITYMAP_TEXTURE)
sampler OPACITY_SAMPLER = sampler_state	// rain alpha texture
{
	ASSIGN_TEXTURE(OPACITYMAP_TEXTURE)
	AddressU = CLAMP;
	AddressV = CLAMP;
	DECLARE_MIPFILTER(LINEAR)
	DECLARE_MINFILTER(LINEAR)
	DECLARE_MAGFILTER(LINEAR)
};

DECLARE_TEXTURE(DIFFUSEMAP_TEXTURE) // PC edit - these NEED to be here for the shader to work!
sampler DIFFUSE_SAMPLER : register(ps_2_0, s0) = sampler_state
{
	ASSIGN_TEXTURE(DIFFUSEMAP_TEXTURE)
    AddressU = CLAMP;
    AddressV = CLAMP;
	DECLARE_MIPFILTER(LINEAR)
	DECLARE_MINFILTER(LINEAR)
	DECLARE_MAGFILTER(LINEAR)
};

DECLARE_TEXTURE(NORMALMAP_TEXTURE)
sampler NORMALMAP_SAMPLER = sampler_state
{
	ASSIGN_TEXTURE(NORMALMAP_TEXTURE)
    AddressU = WRAP;
    AddressV = WRAP;
	DECLARE_MIPFILTER(LINEAR)
	DECLARE_MINFILTER(LINEAR)
	DECLARE_MAGFILTER(LINEAR)
};

DECLARE_TEXTURE(HEIGHTMAP_TEXTURE)
sampler HEIGHTMAP_SAMPLER = sampler_state
{
	ASSIGN_TEXTURE(HEIGHTMAP_TEXTURE)
    AddressU = WRAP;
    AddressV = WRAP;
	DECLARE_MIPFILTER(LINEAR)
	DECLARE_MINFILTER(LINEAR)
	DECLARE_MAGFILTER(LINEAR)
};

DECLARE_TEXTURE(DEPTHBUFFER_TEXTURE) // needed for the PC
sampler DEPTHBUFFER_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DEPTHBUFFER_TEXTURE) // needed for the PC
    AddressU = CLAMP;
    AddressV = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_INPUT
{
	float4 position : POSITION;
	float4 color    : COLOR;
	float4 tex		: TEXCOORD;
	float4 size		: TEXCOORD1;
	int4   light_index	: TEXCOORD2;
};

struct VtoP_NormalMapped
{
	float4 position			: POSITION;
	float4 color			: COLOR0;
	float3 lightColor		: COLOR1;
	float4 tex				: TEXCOORD0;
	float4 tex1				: TEXCOORD1;
	float3 to_light_tan		: TEXCOORD2;
	float3 half_angle_tan	: TEXCOORD3;
	float4 position2		: TEXCOORD4;	
};

struct VtoP
{
	float4 position  : POSITION;
	float4 color     : COLOR0;
	float4 tex       : TEXCOORD0;
	float4 tex1      : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 color : COLOR0;
};

struct VtoP_Depth
{
	float4 position : POSITION;
	float dist		: COLOR0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Build Rotation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Build rotation matrix using Rodrigues' rotation formula
float3x3 BuildRotate(float angle, float3 rotAxis)
{
    float sinA, cosA;
    sincos(-angle, sinA, cosA);
    float3 axis = rotAxis;

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    float oneMinusCos = 1.0f - cosA;

    float3x3 m;
    m[0][0] = cosA + x * x * oneMinusCos;
    m[0][1] = x * y * oneMinusCos + z * sinA;
    m[0][2] = x * z * oneMinusCos - y * sinA;

    m[1][0] = y * x * oneMinusCos - z * sinA;
    m[1][1] = cosA + y * y * oneMinusCos;
    m[1][2] = y * z * oneMinusCos + x * sinA;

    m[2][0] = z * x * oneMinusCos + y * sinA;
    m[2][1] = z * y * oneMinusCos - x * sinA;
    m[2][2] = cosA + z * z * oneMinusCos;

    return m;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compute Fuzz Value
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float ComputeFuzzz(const VtoP_NormalMapped IN)
{
    float depth = tex2D(DEPTHBUFFER_SAMPLER, IN.tex1.xy).x;

	// NOTE! we are assuming that near is 0.5 and far is 10000!!!!
	
    const float zFar = 10000;
    const float zNear = 0.5;

    float Q = zFar / (zFar - zNear);
    float zDist = (-Q * zNear / (depth - Q));

    float depthBufferDistToParticle = IN.position2.z / IN.position2.w;
    float distanceToParticle = (-Q * zNear / (depthBufferDistToParticle - Q));

    float distanceBetweenParticleAndGround = abs(zDist - distanceToParticle);
    float fuzzz = saturate(distanceBetweenParticleAndGround * 1);
    return fuzzz;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tonemaps + Grading
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float3 Uncharted2ToneMapping(float3 x)
{
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;

    float exposure_bias = 7.0;
    x *= exposure_bias;

    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float3 ApplyColorGradeParticles(float3 color)
{
    float3 tint = float3(1.7, 1.5, 1.9); // slightly blue
    color *= tint;

    // simple cross processing
    color.r = pow(color.r, 1.1);
    color.g = pow(color.g, 0.95);
    color.b = pow(color.b, 1.05);

    return color;
}

float3 ApplyColorGradeRain(float3 color)
{
    float3 tint = float3(1.2, 1.0, 1.9); // blue-ish
    color *= tint;

    // simple cross processing
    color.r = pow(color.r, 1.1);
    color.g = pow(color.g, 0.95);
    color.b = pow(color.b, 1.05);

    return color;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Particles
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VtoP_NormalMapped vertex_shader_particles(const VS_INPUT IN)
{
    VtoP_NormalMapped OUT;
	// Offset the vertex by the particle size
    float3 right = cmWorldView._m00_m10_m20;
    float3 up = cmWorldView._m01_m11_m21;
    float3 facing = normalize(cvLocalEyePos - mul(IN.position, cmWorldMat).xyz);

	// Rotate the up and right around the facing
    float angle = IN.size.z;
    float3x3 rotation = BuildRotate(angle, facing);
    right = mul(right, rotation);
    up = mul(up, rotation);

	// Add offset from particle midpoint to the outside vertices
    float4 pv = IN.position;
    float3 offset = right * IN.size.x + up * IN.size.y;
    pv.xyz += offset;

	// Cap the screen size of any particle
    float4 worldCornerPos = pv;
    pv = world_position(pv);

    float3 pvn = pv.xyz / pv.w;
    float4 pc = world_position(IN.position);
    float3 pcn = pc.xyz / pc.w;
    float size = distance(pvn.xy, pcn.xy);
    float new_size = min(size, MaxParticleSize);
    float scale = new_size / size;
    pv = lerp(pc, pv, scale);

	// Each particle is affected by one light in the light array
	// Read which one it is:
    int lightIndex = IN.light_index.x;
    float3 lightPos = cavLightPositions[lightIndex].xyz; // PC edit - it uses arrays

    float3 worldPos = mul(IN.position, cmWorldMat).xyz;
    float3 toLightSource = normalize(lightPos - worldPos);

	// Create the matrix which transforms from world space to tangent space
    float3 tangent = right;
    float3 binormal = up;
    float3 normal = -facing;
    float3x3 matTSpace = transpose(float3x3(tangent, binormal, normal));

    OUT.to_light_tan.xyz = mul(toLightSource, matTSpace);
    OUT.to_light_tan = saturate(1.0 - IN.size.w);

    float3 toEyeWorld = normalize(cvLocalEyePos - worldPos);
    float3 toEyeTan = mul(toEyeWorld, matTSpace);

	// Calculate the half-angle vector for per-pixel specular
	OUT.half_angle_tan = (toEyeTan + OUT.to_light_tan) * 0.5;

    float3 lightColour = float3(1, 1, 1); //cavLightColours[lightIndex].xyz; // PC edit - it uses arrays
    OUT.lightColor = lightColour;

    OUT.position = OUT.position2 = pv;
    OUT.color = saturate(IN.color * 2);

    OUT.tex = IN.tex + cvTextureOffset;

	// Convert from screen space (-1 to 1) to texture coordinate space (0.0 to 1.0)
    float distance = pv.z / pv.w;
    OUT.tex1.x = (0.5 * pv.x / pv.w) + 0.5;
    OUT.tex1.y = (-0.5 * pv.y / pv.w) + 0.5;

	// Distance to the pixel in the depth buffer
    OUT.tex1.z = distance;

	// Calculate the distance from the particle to the camera in world coordinates
    OUT.tex1.w = length(cvLocalEyePos - IN.position);


    return OUT;
}

PS_OUTPUT pixel_shader_particles(const VtoP_NormalMapped IN)
{
    PS_OUTPUT OUT;

    // Sample and normalize the base color
    float4 baseColour = tex2D(DIFFUSE_SAMPLER, IN.tex) * IN.color;

    // Early exit for completely transparent particles
    if (baseColour.a < 0.01)
    {
        OUT.color = float4(0, 0, 0, 0);
        return OUT;
    }

    // Compute fuzziness with a stability bias to prevent jittering
    float fuzzz = saturate(ComputeFuzzz(IN) + 0.4);

    // Sample the normal map and normalize the result
    float3 normal = normalize(tex2D(NORMALMAP_SAMPLER, IN.tex).xyz * 2.0 - 1.0);

    // Lighting calculations
    float3 toLight = normalize(IN.to_light_tan);
    float nDotL = saturate(dot(normal, toLight));

    // Ambient light contribution
    float3 ambient = ambient_colour * baseColour.rgb;

    // Diffuse lighting
    float3 diffuseColour = nDotL * IN.lightColor;

    // Rim lighting for soft edges
    float rimAmount = pow(1.0 - nDotL, 2.0); // Smoother falloff
    float3 rimColour = rimAmount * IN.lightColor * 0.5; // Dampen rim intensity

    // Specular highlights
    float3 half_angle = normalize(IN.half_angle_tan);
    float nDotH = saturate(dot(normal, half_angle));
    float3 specular = MaterialSpecular * pow(nDotH, MaterialShininess);

    // Combine lighting components
    float3 lighting = ambient + diffuseColour + rimColour + specular;

    // Apply fuzziness and base color
    float3 finalColor = fuzzz * baseColour.rgb * lighting;

    // Tone mapping for better color range
    finalColor = Uncharted2ToneMapping(finalColor);

    // Apply brightness adjustment with clamping to avoid over-brightening
    finalColor *= saturate(ParticleBrightness);
    finalColor = ApplyColorGradeParticles(finalColor);

    // Output final color
    OUT.color.rgb = finalColor;
    OUT.color.a = baseColour.a; // Preserve alpha for blending

    return OUT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Raindrops
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PS_OUTPUT pixel_shader_raindrop(const VtoP_NormalMapped IN) : COLOR
{
    PS_OUTPUT OUT;

    // Calculate shadow
    float shadow = 0.5;

    // Sample diffuse texture
    float4 baseColour = tex2D(DIFFUSE_SAMPLER, IN.tex) * IN.color * 2;

    // Sample heightmap texture
    float depth = tex2D(HEIGHTMAP_SAMPLER, IN.tex1.xy).x;

    // Calculate distance to particle
    float zFar = 10000;
    float zNear = 0.5;
    float Q = zFar / (zFar - zNear);
    float zDist = (-Q * zNear / (depth - Q));
    float depthBufferDistToParticle = IN.position2.z / IN.position2.w;
    float distanceToParticle = (-Q * zNear / (depthBufferDistToParticle - Q));

    // Calculate distance between particle and ground
    float distanceBetweenParticleAndGround = abs(zDist - distanceToParticle);
    float fuzzz = ComputeFuzzz(IN);

    // Calculate normal map
    float3 normal = tex2D(NORMALMAP_SAMPLER, IN.tex) * 2 - 1;
    normal = normal * magnitude_debug;
	
	 // Calculate tangent-space eye direction
    float3 eyePos = float3(0, 0, -distanceToParticle); // assume eye position is at (0, 0, -distanceToParticle)
    float3 tangent = float3(1, 0, 0); // assume tangent vector is (1, 0, 0)
    float3 bitangent = float3(0, 1, 0); // assume bitangent vector is (0, 1, 0)
    float3 toEye = normalize(eyePos - IN.position2.xyz);
    float3 toEyeTan = float3(dot(toEye, tangent), dot(toEye, bitangent), dot(toEye, normal));
	
	// specular calculations
    float3 half_angle = normalize(IN.half_angle_tan);
    float nDotH = saturate(dot(normal, half_angle));
    float3 MaterialSpecular = float3(0.4, 0.4, 0.4);
    float3 specular = MaterialSpecular * pow(nDotH, MaterialShininess);

    // Apply diffuse lighting
    float3 toLight = normalize(IN.to_light_tan);
    float nDotL = saturate(dot(normal, toLight));
    float3 diffuseColour = nDotL * IN.lightColor;

    // Add specular lighting
    float3 reflectedLight = reflect(-toLight, normal);
    float specularAmount = pow(saturate(dot(reflectedLight, toEyeTan)), 16);
    float3 specularColour = specularAmount * IN.lightColor;

    // Add rim lighting
    float rimAmount = 1 - nDotL;
    rimAmount = pow(rimAmount, 2);
    float3 rimColour = rimAmount * IN.lightColor;

    // Combine colors
    OUT.color.rgb = fuzzz * shadow * baseColour * (ambient_colour + diffuseColour + specularColour + rimColour + specular);
    OUT.color.rgb = ApplyColorGradeRain(OUT.color.rgb);
    OUT.color.a = baseColour.a * shadow * fuzzz;
	
    return OUT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Flares
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_INPUT_FLARES
{
	float4 position : POSITION;
	float4 color    : COLOR;
	float4 tex		: TEXCOORD;
	float4 size		: TEXCOORD1;
};

struct VtoP_FLARES
{
	float4 position  : POSITION;
	float4 color     : COLOR0;
	float4 tex       : TEXCOORD0;
	float4 tex1      : TEXCOORD1;
};

VtoP_FLARES vertex_shader_flares(const VS_INPUT_FLARES IN)
{
    VtoP_FLARES OUT;

    // Offset the vertex by the particle size
    float3 right = cmWorldView._m00_m10_m20;
    float3 up = cmWorldView._m01_m11_m21;
    float3 facing = cmWorldView._m02_m12_m22;
    float isTrailFlare = IN.size.w;

    float angle = IN.size.z;
    // Rotate the up and right around the facing
    if (angle > 0)
    {
        float3x3 rotation = BuildRotate(angle, facing);
        right = mul(right, rotation);
        up = mul(up, rotation);
    }

    float4 pv = IN.position;
    pv.xyz += right * IN.size.x + up * IN.size.y;

    // Cap the screen size of any particle
    pv = world_position(pv);

    OUT.position = pv;
    OUT.color = IN.color;
    OUT.tex = IN.tex;
    OUT.tex.w = cfMipMapBias; // bias the mipmapping

    // Convert from screen space (-1 to 1) to texture coordinate space (0.0 to 1.0)
    float distance = pv.z / pv.w;
    OUT.tex1.x = (0.5 * pv.x / pv.w) + 0.5;
    OUT.tex1.y = (-0.5 * pv.y / pv.w) + 0.5;
    OUT.tex1.z = distance; // Distance to the pixel in the depth buffer

    // Scale FuzzWidth based on distance
    float scaledFuzzWidth = FuzzWidth * (1.0 - distance);
    OUT.tex1.w = saturate(scaledFuzzWidth);

    // fade out particles near the edge of the screen
    float screenFade = 1.0 - smoothstep(0.8, 1.0, abs(pv.x / pv.w));
    OUT.color.rgb *= screenFade;


    return OUT;
}

float4 pixel_shader_flares(const VtoP_FLARES IN) : COLOR
{
    // Sample the diffuse texture with bias to reduce aliasing
    float4 diffuse = tex2Dlod(DIFFUSE_SAMPLER, IN.tex);

    // Calculate the scaled fuzz width
    float scaled_fuzz_width = IN.tex1.w;

    // Sample the heightmap texture to get the depth value
    float depth = tex2D(HEIGHTMAP_SAMPLER, IN.tex1.xy).x;

    // Calculate the fuzz factor using the scaled fuzz width and depth difference
    float fuzzz = saturate((scaled_fuzz_width * 0.4 - (IN.tex1.z - depth)) / (scaled_fuzz_width));

    // Apply a simple glow effect to the diffuse texture
    float glow_amount = fuzzz * 0.4;
    diffuse.rgb += diffuse.rgb * glow_amount;
	
	// Apply the color and alpha values
    float4 result = diffuse * IN.color;
	
    // Define an overexposure threshold
    float overexposure_threshold = 0.90; // Adjust this value as needed
    float overexposure_intensity = 1.75; // How much to brighten overexposed areas

    // Check for overexposure and apply the effect
    if (result.r > overexposure_threshold || result.g > overexposure_threshold || result.b > overexposure_threshold)
    {
        // Brighten the color
        result.rgb *= overexposure_intensity;
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Streak Flares
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VtoP_SFLARES
{
    float4 position : POSITION;
    float4 color : COLOR0;
    float4 tex : TEXCOORD0;
};

VtoP_SFLARES vertex_shader_streak_flares(const VS_INPUT_FLARES IN)
{
    VtoP_SFLARES OUT;
	// Offset the vertex by the particle size
    float3 right = cmWorldView._m00_m10_m20;
    float3 up = cmWorldView._m01_m11_m21;
    float3 facing = cmWorldView._m02_m12_m22;
    float doStreakFlare = IN.size.w;
    float4 pv = IN.position;
    float4 vertexColour = IN.color;
    float4 tex = IN.tex;

    if (doStreakFlare > 0)
    {
		//float cameraSpeed = cvBaseAlphaRef.y * 75.0;
        float cameraSpeed = cvBaseAlphaRef.y;
        float nosAmount = cvBaseAlphaRef.w;

		// Add some speed for NOS
        cameraSpeed += nosAmount * 20;

		// Flare is stretch to create a flare trail for Sense of Speed
        float4 currFramePos = mul(IN.position, cmWorldViewProj);

		// Redice length if camera veleocity is off centre in the x direction
        cameraSpeed = lerp(cameraSpeed, 0, saturate(abs(cvBaseAlphaRef.x) / 0.05));

        float4 prevFramePos = currFramePos;
        prevFramePos.zw += 3 + saturate(cameraSpeed / 140) * 45; // Stretch flare back
        currFramePos += normalize(currFramePos - prevFramePos) * 10; // Stretch flare forward

		// Build the poly
        float3 prevFrameDir = prevFramePos.xyz / prevFramePos.w - currFramePos.xyz / currFramePos.w;
        float offsetWidth = min(abs(IN.size.x), abs(IN.size.y));
        offsetWidth *= 0.2;
        float4 currFrameOffset = mul(IN.position + float4(offsetWidth, offsetWidth, offsetWidth, 0), cmWorldViewProj);
        float currFrameSize = distance(currFrameOffset.xy, currFramePos.xy);
        float3 prevFrameDirTanget = normalize(prevFrameDir);
        
		// Rotate by 90 degrees to get tangent
        prevFrameDirTanget.xy = prevFrameDirTanget.yx;
        prevFrameDirTanget.x = -prevFrameDirTanget.x;
        prevFrameDirTanget *= currFrameSize;

		// Push the tex coord futher along the streak for longer streaks
        float texUlength = saturate(length(prevFrameDir.xyz) / 0.1);
        tex.x = IN.size.x > 0 ? 1 / 256 : texUlength;
        tex.y = IN.size.y > 0 ? cvBaseAlphaRef.z : cvBaseAlphaRef.z + 15.0f / 128.0f;
        tex.z = IN.size.x > 0 ? 0 : 1;

		// Cap the screen size of any particle
        pv = world_position(pv);
        pv = IN.size.x > 0 ? currFramePos : prevFramePos;
        
		// Adjust brightness for lower/higher speeds
        vertexColour *= saturate(cameraSpeed / 95);

		// Add the offset to create width for the streak along the tangent
        pv.xy += sign(IN.size.y) * prevFrameDirTanget;
    }
    else
    {
		// Push offscreen to clip
        pv = 1000;
    }

    OUT.position = pv;
    OUT.color = vertexColour;
    OUT.tex = tex;
    OUT.tex.w = -5; // bias the mipmapping

    return OUT;
}


float4 pixel_shader_streak_flares(const VtoP_SFLARES IN) : COLOR
{
    float4 result = tex2Dbias(DIFFUSE_SAMPLER, IN.tex);;
    result *= IN.color;
	
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Onscreen Rain Effect
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VtoP_RAIN
{
	float4 position  : POSITION;
	float4 tex       : TEXCOORD0;
};

VtoP_RAIN vertex_shader_passthru(const VS_INPUT IN)
{
	VtoP_RAIN OUT;
	OUT.position = screen_position(IN.position);
	OUT.position.w = 1.0f;
	OUT.tex = IN.tex;

	return OUT;
}

float4 pixel_shader_onscreen_distort(const VtoP_RAIN IN) : COLOR0
{
    // Sample the distortion texture
    float4 distortion = tex2D(DIFFUSE_SAMPLER, IN.tex);

    // Add a simple water ripple effect to the distortion texture
    float ripple = sin((IN.tex.x + IN.tex.y) * 10.0 + distortion.r * 0.1);
    ripple *= 0.1;
    distortion.gb += ripple;

    // Calculate the offset for the background texture
    float2 offset = distortion.gb * cvLocalCenter.ba + cvLocalCenter.rg;

    // Sample the background texture
    float4 background = tex2D(MISCMAP1_SAMPLER, offset);

    // Calculate the offset for the opacity texture
    offset = IN.tex;
    offset.x = cvBaseAlphaRef.y + IN.tex.x * 0.25;

    // Sample the opacity texture
    float4 opacity = tex2D(OPACITY_SAMPLER, offset);

    // Combine the background and opacity textures
    float4 result = background * opacity.y;
    result.w = opacity.r * cvBaseAlphaRef.x;

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Techniques
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

technique fuzzz <int shader = 1;>
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_particles();
        PixelShader = compile ps_3_0 pixel_shader_particles();
    }
}

technique no_fuzzz <int shader = 1; >
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_particles();
        PixelShader = compile ps_3_0 pixel_shader_particles();
    }
}


technique raindrop <int shader = 1;>
{
    pass p0
    {
        FogEnable = TRUE;

        VertexShader = compile vs_1_1 vertex_shader_particles();
        PixelShader = compile ps_3_0 pixel_shader_raindrop();
    }
}

technique flares <int shader = 1; >
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_flares();
        PixelShader = compile ps_3_0 pixel_shader_flares();
    }
}

technique streak_flares <int shader = 1;>
{
    pass p0
    {
        VertexShader = compile vs_3_0 vertex_shader_streak_flares();
        PixelShader = compile ps_3_0 pixel_shader_streak_flares();
    }
}

technique onscreen_distort <int shader = 1;>
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader  = compile ps_3_0 pixel_shader_onscreen_distort();
	}
}

