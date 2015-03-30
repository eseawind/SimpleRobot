#version 120

attribute highp vec4 vVertex;
attribute highp vec3 vNormal;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform vec3 vLightPosition;
uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat4 normalMatrix;

varying highp vec4 vVaryingColor;

void main(void)
{
    vec3 vEyeNormal = mat3(normalMatrix) * vNormal;
    vec4 vPosition4 = mvMatrix * vVertex;
    vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
    vec3 vLightDir = normalize(vLightPosition - vPosition3);
    float diff = max(0.0, dot(vEyeNormal, vLightDir));
    vVaryingColor = diff * diffuseColor;
    vVaryingColor += ambientColor;
    vec3 vReflection = normalize(reflect(-vLightDir, vEyeNormal));
    float spec = max(0.0, dot(vEyeNormal, vReflection));
    if(diff!=0){
        float fSpec = pow(spec, 128.0);
        vVaryingColor.rgb += vec3(fSpec, fSpec, fSpec);
    }
    gl_Position = mvpMatrix * vVertex;
}

