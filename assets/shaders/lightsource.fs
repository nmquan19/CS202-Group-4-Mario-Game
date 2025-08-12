#version 330

#define MAX_LIGHTS 128
    
in vec2 fragTexCoord;
out vec4 finalColor;
uniform sampler2D sceneTexture;
uniform int numLights;
uniform vec2 lightPositions[MAX_LIGHTS];  // In screen pixel coordinates
uniform float lightRadii[MAX_LIGHTS];     // In UV space (0–1)
uniform float lightBrightness[MAX_LIGHTS];
uniform vec3 innerColors[MAX_LIGHTS];
uniform vec3 outerColors[MAX_LIGHTS];
uniform vec2 screenSize;
uniform vec3 ambientColor;
void applyShockWave()
{
     
}
void main() {
    vec4 sceneColor = texture(sceneTexture, fragTexCoord);
    vec3 lighting = ambientColor; // Ambient base
    for (int i = 0; i < numLights; i++) {
        // Convert light position from pixels to UV space
        vec2 lightUV = lightPositions[i] / screenSize;
        // Calculate distance in UV space
        float dist = distance(fragTexCoord, lightUV);
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
