// resources/shaders/lens_convex.fs
#version 330

in vec3 fragPos;
in vec3 fragNormal;
in vec4 fragClip;

out vec4 finalColor;

uniform vec3 cameraPosition;
uniform sampler2D sceneTex;
uniform vec2 resolution;

uniform float refractiveIndex;
uniform float lensStrength; //how much the UVs bend
uniform float tintStrength; //0..1 blend toward tintColor
uniform vec3 tintColor; //RGB tint to simulate glass absorption
uniform float alpha; //overall translucency

vec2 parallaxOffset(vec3 refracted, float strength) {
    float z = max(abs(refracted.z), 0.001);
    return (refracted.xy / z) * strength;
}

vec3 sampleScene(vec2 uv, vec2 offset) {
    vec2 uvR = clamp(uv + offset * 0.9, 0.001, 0.999);
    vec2 uvG = clamp(uv + offset,     0.001, 0.999);
    vec2 uvB = clamp(uv + offset * 1.1, 0.001, 0.999);

    float r = texture(sceneTex, uvR).r;
    float g = texture(sceneTex, uvG).g;
    float b = texture(sceneTex, uvB).b;
    return vec3(r, g, b);
}

void main() {
    vec3 N = normalize(fragNormal);
    vec3 I = normalize(fragPos - cameraPosition);

    float eta = 1.0 / refractiveIndex;
    vec3 refracted = refract(I, N, eta);

    vec2 ndc = fragClip.xy / fragClip.w;
    vec2 screenUV = ndc * 0.5 + 0.5;

    vec2 offset = parallaxOffset(refracted, lensStrength);
    vec3 sceneColor = sampleScene(screenUV, offset);

    vec3 tinted = mix(sceneColor, sceneColor * tintColor, clamp(tintStrength, 0.0, 1.0));

    float fresnel = pow(1.0 - max(dot(-I, N), 0.0), 3.0);
    float rimFade = smoothstep(0.0, 1.0, fresnel);

    finalColor = vec4(tinted, alpha * (1.0 - 0.35 * rimFade));
}