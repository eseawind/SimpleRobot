#version 120
attribute highp vec4 vVertex;
attribute vec3 vNormal;

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat4 normalMatrix;
uniform vec3 vLightPosition;

varying vec3 vVaryingNormal;
varying vec3 vVaryingLightDir;

void main(void)
{
    vVaryingNormal = mat3(normalMatrix) * vNormal;
    vec4 vPosition4 = mvMatrix * vVertex;
    vec3 vPosition3 = vPosition4.xyz / vPosition4.w;

    vVaryingLightDir = normalize(vLightPosition-vPosition3);
    gl_Position = mvpMatrix * vVertex;
}

