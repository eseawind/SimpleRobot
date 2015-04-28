#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

varying vec3 vVaryingNormal;
varying vec3 vVaryingLightDir;

void main(void)
{
    float diff = max(0.0, dot(normalize(vVaryingNormal),
                              normalize(vVaryingLightDir)));
    gl_FragColor = diff * diffuseColor;
    gl_FragColor += ambientColor;

    if(diff>0.00001||diff<-0.00001){
        vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),
                                             normalize(vVaryingNormal)));
        float spec = max(0.0, dot(normalize(vVaryingNormal),
                                  vReflection));
        float fSpec = pow(spec, 128.0);
        gl_FragColor.rgb += vec3(fSpec, fSpec, fSpec);
    }
}

