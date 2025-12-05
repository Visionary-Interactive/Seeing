#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;

uniform mat4 mvp;
uniform mat4 matModel;

out vec3 fragPos;
out vec3 fragNormal;
out vec4 fragClip;

void main() {
    vec4 worldPos = matModel * vec4(vertexPosition, 1.0);
    fragPos = worldPos.xyz;

    mat3 normalMatrix = mat3(transpose(inverse(matModel)));
    fragNormal = normalize(normalMatrix * vertexNormal);

    fragClip = mvp * vec4(vertexPosition, 1.0);
    gl_Position = fragClip;
}