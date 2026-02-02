#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D sceneTex;
uniform vec2 resolution;

uniform float intensity;
uniform float tunnelRadius; //keep this around 0.35
uniform float blurStrength;
uniform float contrastLoss;
uniform float edgeDarkening;

vec3 blurSample(vec2 uv, float strength)
{
    vec2 texel = 1.0 / resolution;
    vec3 color = vec3(0.0);

    color += texture(sceneTex, uv + texel * vec2(-strength, -strength)).rgb;
    color += texture(sceneTex, uv + texel * vec2( strength, -strength)).rgb;
    color += texture(sceneTex, uv + texel * vec2(-strength,  strength)).rgb;
    color += texture(sceneTex, uv + texel * vec2( strength,  strength)).rgb;

    return color * 0.25;
}

void main()
{
    vec2 uv = fragTexCoord;
    vec3 baseColor = texture(sceneTex, uv).rgb;

    vec2 centered = uv - 0.5;
    float dist = length(centered) * 2.0;

    float tunnel = smoothstep(tunnelRadius, 1.0, dist);

    vec3 blurred = blurSample(uv, blurStrength);
    vec3 mixedBlur = mix(baseColor, blurred, tunnel * intensity);
    vec3 gray = vec3(dot(mixedBlur, vec3(0.299, 0.587, 0.114)));
    vec3 contrastReduced = mix(mixedBlur, gray, contrastLoss * tunnel * intensity);

    float darkness = 1.0 - edgeDarkening * tunnel * intensity;
    vec3 finalRGB = contrastReduced * darkness;

    finalColor = vec4(finalRGB, 1.0);
}
