#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform float intensity;

float toLinear(float c) {
    if (c <= 0.04045) return c / 12.92;
    return pow((c + 0.055) / 1.055, 2.4);
}

float toSRGB(float c) {
    if (c <= 0.0031308) return c * 12.92;
    return 1.055 * pow(c, 1.0 / 2.4) - 0.055;
}

vec3 srgbToLinear(vec3 c) {
    return vec3(toLinear(c.r), toLinear(c.g), toLinear(c.b));
}

vec3 linearToSrgb(vec3 c) {
    return vec3(toSRGB(c.r), toSRGB(c.g), toSRGB(c.b));
}

void main()
{
    vec4 tex = texture(texture0, fragTexCoord) * fragColor;
    float a = tex.a;

    vec3 linear = srgbToLinear(tex.rgb);

    // Deuteranopia simulation transform
    mat3 deuteranopiaMat = mat3(
        0.625, 0.375, 0.000,
        0.700, 0.300, 0.000,
        0.000, 0.300, 0.700
    );

    vec3 simLinear = deuteranopiaMat * linear;
    simLinear = clamp(simLinear, 0.0, 2.0);

    vec3 simSRGB = linearToSrgb(simLinear);
    vec3 outColor = mix(tex.rgb, simSRGB, clamp(intensity, 0.0, 2.0));

    finalColor = vec4(outColor, a);
}