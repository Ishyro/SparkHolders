// ENHANCED 2D liquid inside 'sphere' shader.

// Author: Mirza Beig

// https://twitter.com/TheMirzaBeig
// https://www.youtube.com/@MirzaBeig

// Feel free to use this however you want.
// Modify, learn from it, copy-paste, etc...

// This is the expanded version of:
// > https://www.shadertoy.com/view/Ds3BRN

// Variables use descriptive names- things should be self-explanatory for the most part.

shader_type canvas_item;

uniform vec3 back_fluid_color : source_color;
uniform vec3 front_fluid_color : source_color;
uniform vec3 fresnel_color : source_color;

uniform float level : hint_range(0.0, 1.0) = 1.0;

// https://iquilezles.org/articles/distfunctions2d/

float sdArc(in vec2 p, in vec2 sc, in float ra, float rb)
{
    // sc is the sin/cos of the arc's aperture
    p.x = abs(p.x);
    return ((sc.y*p.x>sc.x*p.y) ? length(p-sc*ra) : 
                                  abs(length(p)-ra)) - rb;
}

mat2 Get2DRotationMatrix(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return mat2(vec2(c, -s), vec2(s, c));
}

void fragment() {
    vec2 uv = (UV / -0.58) + vec2(0.86,0.86);
            
    float circleSDF = length(uv);
    
    // 1x circle used for the actual container/shell (as its mask).
    
    float fillMaskCircle = step(circleSDF, 0.85);
    
    // Progress/fill. Animated.
    
    const float fillAnimationFrequency = 4.0;
    const float fillAnimationAmplitude = 0.01;
        
    float fillAnimationPhase = TIME * fillAnimationFrequency;
    
    float fillAnimation = sin(fillAnimationPhase) * fillAnimationAmplitude;
    
    const float waveFrequency = 2.0;
    const float waveAmplitude = 0.01;
    
    const float waveAnimation = 2.0;
    
    // Waves as repeating sine/band offsets to the horizontal gradient.
    
    float frontWavePhase = (TIME * waveAnimation) + uv.x;
    float backWavePhase = (TIME * -waveAnimation) + uv.x;
        
    frontWavePhase *= waveFrequency;
    backWavePhase *= waveFrequency;
        
    const float backWavesPhaseOffset = PI;
    
    float frontWaves = sin(frontWavePhase) * waveAmplitude;
    float backWaves = sin(backWavePhase + backWavesPhaseOffset) * waveAmplitude;
            
    float verticalBand = sin(uv.x + (PI * 0.5)) - 0.3;
    verticalBand = smoothstep(0.1, 0.9, verticalBand);
   
    // Stretch waves up/down near center, synced as they bob up/down.
    
    const float animatedVerticalBandStrength = 0.0125;
    float animatedVerticalBand = verticalBand * animatedVerticalBandStrength;
            
    animatedVerticalBand *= sin(TIME * fillAnimationFrequency);
            
    frontWaves += animatedVerticalBand;
    backWaves -= animatedVerticalBand;
    
    // Pinch sides (mask by the vertical gradient band) so they don't move.
    
    fillAnimation *= verticalBand;
            
    // Centered fill progress.
    
    float fillProgressAnimationFrequency = 0.0;
    float fillProgressAnimationAmplitude = 0.0;
    
    float fillProgress = (level - 0.5) * 1.75;
    
    fillProgress += sin((TIME * fillProgressAnimationFrequency) * PI) * fillProgressAnimationAmplitude;
    //fillProgress = (fillProgress - 0.5) * 2.0; 
    
    float frontFill = step(uv.y, (fillAnimation + frontWaves) + fillProgress);
    float backFill = step(uv.y, (-fillAnimation + backWaves) + fillProgress);
    
    frontFill *= fillMaskCircle;
    backFill *= fillMaskCircle;
    
    // Mask back fill to only parts that would be visible separate from frontFill.
    
    backFill = clamp(backFill - frontFill, 0.0, 1.0);
    
    float fillMask = 1.0 - (frontFill + backFill);
    fillMask *= fillMaskCircle;
        
    float fill = frontFill + backFill;
    
    // Simple edge glow using radial gradient (circle SDF).
    
    const float fresnelOffset = 0.01;
    float fresnel = (circleSDF + fresnelOffset) * fillMask;    
    
    const float fresnelPower = 5.0;
    fresnel = clamp(pow(fresnel, fresnelPower), 0.0, 1.0);
    
    const float frontFillFresnelPower = 5.0;
    const float frontFillFresnelOffset = 0.02;
    
    float frontFillFresnel = (circleSDF + frontFillFresnelOffset) * (1.0 - fillMask);
    frontFillFresnel = clamp(pow(frontFillFresnel, frontFillFresnelPower), 0.0, 1.0);
    
    // Final mask. Can be used as alpha.
    
    float mask = fill + fresnel;
    
    // Pre-mask RGB colour.
    
    const vec3 frontFillOuterColour = vec3(0.0, 0.0, 0.0);

    vec3 frontFillColour = mix(front_fluid_color, frontFillOuterColour, frontFillFresnel);
    
    const vec3 specularColour = vec3(1.0, 1.0, 0.9);
             
    vec3 rgb =
        
        (frontFill * frontFillColour) +
        (backFill * back_fluid_color) +
        (fresnel * fresnel_color);
    
    // Output to screen.
    
    COLOR = vec4(rgb, mask);
}