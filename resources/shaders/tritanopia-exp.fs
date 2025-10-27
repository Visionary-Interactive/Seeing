#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float intensity; //0 = normal vision, 1 = full tritanopia simulation

//SRGB linear conversions
float toLinear(float c) {
    return (c <= 0.04045) ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4);
}
float toSRGB(float c) {
    return (c <= 0.0031308) ? c * 12.92 : 1.055 * pow(c, 1.0 / 2.4) - 0.055;
}

vec3 srgbToLinear(vec3 c) {
    return vec3(toLinear(c.r), toLinear(c.g), toLinear(c.b));
}
vec3 linearToSrgb(vec3 c) {
    return vec3(toSRGB(c.r), toSRGB(c.g), toSRGB(c.b));
}

//RGB to LMS cone transform matrices
//these values are based on the 2009 Machado model, for the sRGB color space
const mat3 RGB_to_LMS = mat3(
    17.8824, 43.5161,  4.11935,
     3.45565,27.1554,  3.86714,
     0.0299566,0.184309,1.46709
);
const mat3 LMS_to_RGB = mat3(
     0.0809444479, -0.130504409,  0.116721066,
    -0.0102485335,  0.0540193266, -0.113614708,
    -0.000365296938,-0.00412161469, 0.693511405
);

//tritanopia projection matrix (Machado full deficiency)
const mat3 tritanopiaTransform = mat3(
    1.00000000, 0.00000000, 0.00000000,
    0.00000000, 0.25802959, 0.74197041,
    0.00000000, 0.25941189, 0.74058811
);

void main()
{
    vec4 tex = texture(texture0, fragTexCoord) * fragColor;
    float a = tex.a;

    vec3 rgbLin = srgbToLinear(tex.rgb);

    //convert to LMS cone space
    vec3 LMS = RGB_to_LMS * rgbLin;

    //apply tritanopia deficiency matrix
    vec3 LMS_deficient = tritanopiaTransform * LMS;

    vec3 rgbLinDef = LMS_to_RGB * LMS_deficient;

    //this is supposed to rebalance the luminance so the purples aren't so harsh but I don't notice a huge difference
    //I don't like this version as much as the other for that reason
    float gray = dot(rgbLinDef, vec3(0.299, 0.587, 0.114));
    rgbLinDef = mix(vec3(gray), rgbLinDef, 0.95);

    rgbLinDef = clamp(rgbLinDef, 0.0, 1.0);

    vec3 rgbDef = linearToSrgb(rgbLinDef);

    //blend based on intensity
    vec3 finalRGB = mix(tex.rgb, rgbDef, clamp(intensity, 0.0, 1.0));

    finalColor = vec4(finalRGB, a);
}
