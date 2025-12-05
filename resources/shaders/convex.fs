#version 330

in vec2 fragTexCoord;
in vec3 fragPosition;

out vec4 finalColor;

uniform sampler2D sceneTex;   // scene rendered previously
uniform float radius;         // radius of lens in UV space
uniform float refractiveIndex; // e.g., 1.1–1.3
uniform float intensity;       // refraction strength

void main()
{
    // Convert UV to [-1, 1] lens space
    vec2 uv = fragTexCoord * 2.0 - 1.0;

    // Distance from center determines curvature
    float dist = length(uv);

    // If pixel is outside lens, show normal scene
    if (dist > radius) {
        finalColor = texture(sceneTex, fragTexCoord);
        return;
    }

    // Normal of a convex lens surface (sphere-like)
    float z = sqrt(max(0.0, radius * radius - dist * dist));
    vec3 normal = normalize(vec3(uv.x, uv.y, z));

    // Convert normal to refraction offset
    vec2 offset = normal.xy * (1.0 - refractiveIndex) * intensity;

    // Sample refracted scene
    vec2 refractedUV = fragTexCoord + offset;

    // Clamp to avoid sampling outside texture
    refractedUV = clamp(refractedUV, 0.001, 0.999);

    vec4 sceneColor = texture(sceneTex, refractedUV);

    // Slight tint to look like glass
    vec4 glassTint = vec4(0.85, 0.9, 1.0, 0.35);

    // Mix refracted scene with glass tint
    //finalColor = mix(sceneColor, glassTint, glassTint.a);
    finalColor = vec4(1,0,0,1);
}
