attribute vec4 vertex;
attribute vec3 normale;
varying vec3 vNormale;
varying vec3 vCamera;

void main()
{
    vNormale     = gl_NormalMatrix*normale;
    vCamera   = -normalize(vec3(gl_ModelViewMatrix*vertex));
    gl_Position  =  gl_ModelViewProjectionMatrix*vertex;
}
