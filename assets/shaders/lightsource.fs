#version 330

#define MAX_LIGHTS 128
#define MAX_DISTORTIONS 8

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D sceneTexture;

// Lights
uniform int numLights;
uniform vec2 lightPositions[MAX_LIGHTS];
uniform float lightRadii[MAX_LIGHTS];
uniform float lightBrightness[MAX_LIGHTS];
uniform vec3 innerColors[MAX_LIGHTS];
uniform vec3 outerColors[MAX_LIGHTS];

// Distortions
uniform int numDistortions;
uniform vec2 distortionCenters[MAX_DISTORTIONS];   // In UV space [0,1]
uniform float distortionTimers[MAX_DISTORTIONS];   // seconds since start
uniform float distortionTimes[MAX_DISTORTIONS];// total duration
uniform float distortionStrengths[MAX_DISTORTIONS];// max strength
uniform float distortionRadius[MAX_DISTORTIONS];   // effect radius
uniform int distortionTypes[MAX_DISTORTIONS];      // 0 = shockwave

// Screen
uniform vec2 screenSize;

// Ambient
uniform vec3 ambientColor;

// Debug
uniform int debugMode = 0; // 0 = normal, 1 = show distortion centers, 2 = show UV distortion

vec2 applyShockwave(vec2 uv, vec2 center, float timer, float lifetime, float strength, float maxRadius) {
    // Normalized progress [0,1]
    float progress = clamp(timer / lifetime, 0.0, 1.0);
    
    float waveRadius = progress * maxRadius;
    vec2 dir = uv - center;
    float dist = length(dir);
    
    if (dist > waveRadius + 0.05) return uv;
    
    // Normalized direction
    dir = normalize(dir);
    
    float waveWidth = 0.004 * maxRadius; // Width of the ripple
    float wavePosition = dist - waveRadius;
    
    float ripple = exp(-wavePosition*wavePosition/(waveWidth*waveWidth)) * 
                  (1.0 - abs(wavePosition)/waveWidth);
    
    // Animation easing
    float intro = smoothstep(0.0, 0.2, progress);       // Gentle start
    float outro = 1.0 - smoothstep(0.7, 1.0, progress); // Smooth fade
    
    // Combined displacement
    float displacement = strength * ripple * intro * outro * 0.1;
    
    // Apply displacement with chromatic aberration
    vec2 offset = dir * displacement;
    
    // Return distorted UV
    return uv + offset;
}
void main() {
    vec2 uv = fragTexCoord;

    // Apply distortions
    for (int i = 0; i < numDistortions; i++) {
        if (distortionTypes[i] == 0) { 
             vec2 distortionUV = distortionCenters[i] / screenSize;
            uv = applyShockwave(
                uv,
                distortionUV,
                distortionTimers[i],
                distortionTimes[i],
                distortionStrengths[i],
                distortionRadius[i]
            );
        }
    }
    
    // Normal rendering
    vec4 sceneColor = texture(sceneTexture, uv);
    vec3 lighting = ambientColor; // Ambient base
    for (int i = 0; i < numLights; i++) {
        // Convert light position from pixels to UV space
        vec2 lightUV = lightPositions[i] / screenSize;
        // Calculate distance in UV space
        float dist = distance(uv, lightUV);
        // Convert radius from pixels to UV space (Y axis used for consistency)
        float radiusUV = lightRadii[i];
        if (dist < radiusUV) {
            float attenuation = 1.0 - smoothstep(0.0, radiusUV, dist);
            vec3 lightColor = mix(outerColors[i], innerColors[i], 1.0 - dist/radiusUV);
            lighting += lightColor * lightBrightness[i] * attenuation;
        }
    }
    finalColor = vec4(sceneColor.rgb * lighting, sceneColor.a);
}