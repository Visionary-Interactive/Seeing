#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D screenTex;
uniform vec2 resolution;
uniform float intensity;
uniform float angle; //this is in radians
uniform float radiusMajor; //major axis radius in px
uniform float radiusMinor; //minor axis radius in px (obviously less than radiusMajor)
uniform int samples; //it's not that important but this should always be an odd number, e.g. 15 so that the center sample doesn't shift the image
uniform float chromaDispersion; //0-1 extra blur for blue channel

//gaussian helper (sigma derived from radius)
float gaussian(float x, float sigma) {
    return exp(-0.5 * (x*x) / (sigma*sigma));
}

void main()
{
    vec4 src = texture(screenTex, fragTexCoord) * fragColor;

    if (intensity <= 0.0001 || samples <= 1 || radiusMajor <= 0.001) {
        finalColor = src;
        return;
    }

    //normalized pixel step (texture coord per pixel)
    vec2 px = 1.0 / resolution;

    //direction vector for major axis in texture coordinate space
    vec2 dir = vec2(cos(angle), sin(angle));
    //convert radius in px -> texture coords along dir
    vec2 majorStep = dir * (radiusMajor * px);
    //perpendicular direction for minor axis (rotated 90 degrees)
    vec2 perp = vec2(-dir.y, dir.x);
    vec2 minorStep = perp * (radiusMinor * px);

    //We sample along the major axis and offset per sample by a scaled minor contribution to create an elliptical effect
    //iterate samples from -N..N along major axis. So for each sample t, compute ellipse offset along minor axis scaled 
    //by sqrt(1 - (t/majorRadius)^2) to stay on ellipse.
    int N = samples / 2;
    float sigmaMajor = max(1.0, radiusMajor * 0.5);
    float sigmaMinor = max(1.0, radiusMinor * 0.5);

    vec3 accum = vec3(0.0);
    float weightSum = 0.0;

    //this samples symmetric points
    for (int i = -20; i <= 20; ++i) {
        if (i < -N || i > N) continue;

        float t = float(i);
        float u = (N == 0) ? 0.0 : t / float(max(1, N));

        //position along major axis (texture coords)
        vec2 majorOffset = majorStep * t;

        float ellipseScale = sqrt(max(0.0, 1.0 - u*u));
        vec2 minorOffset = minorStep * ellipseScale;

        vec2 sampleCoord = fragTexCoord + majorOffset + minorOffset;

        //find the desired Gaussian weight using distance along major/minor separately
        float distMajor = abs(t);
        float weightMajor = gaussian(distMajor, sigmaMajor);
        float weightMinor = gaussian(ellipseScale * radiusMinor, sigmaMinor);
        float weight = weightMajor * weightMinor;

        ///this is completely optional but it looks nice. do not crank dispersion too high
        vec3 col = texture(screenTex, sampleCoord).rgb;
        
        if (chromaDispersion > 0.001) {
            vec2 blueShift = minorStep * 0.1 * chromaDispersion;
            float blue = texture(screenTex, sampleCoord + blueShift).b;
            col.b = blue;
        }

        accum += col * weight;
        weightSum += weight;

        //also sample symmetric negative minor offset to keep kernel symmetric..
        vec2 sampleCoord2 = fragTexCoord + majorOffset - minorOffset;
        vec3 col2 = texture(screenTex, sampleCoord2).rgb;
        if (chromaDispersion > 0.001) {
            vec2 blueShift2 = -minorStep * 0.1 * chromaDispersion;
            float blue2 = texture(screenTex, sampleCoord2 + blueShift2).b;
            col2.b = blue2;
        }
        float weight2 = weightMajor * weightMinor; // same
        accum += col2 * weight2;
        weightSum += weight2;
    }

    //normalize
    if (weightSum > 0.0) accum /= weightSum;

    //convert back... this is an inexpensive way to approximate it but it might be worth looking into other ways
    //since colour accuracy is the whole point
    vec3 srcLin = pow(src.rgb, vec3(2.2));  
    vec3 outLin = mix(srcLin, pow(accum, vec3(2.2)), intensity);
    vec3 outSRGB = pow(outLin, vec3(1.0/2.2));

    finalColor = vec4(outSRGB, src.a);
}
