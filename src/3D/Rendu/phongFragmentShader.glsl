varying vec3 vNormale;
varying vec3 vPosition;

uniform bool speculaireActivee;
uniform bool estFocus;
uniform vec4 couleur;

void main()
{
    vec3 normale;
    vec4 couleurBis;
   // if (dot(normalize(vPosition), vNormale)>0.0)
    if (!gl_FrontFacing)
    {
        normale=-vNormale;
        couleurBis=vec4(0.811, 0.709, 0.231, 1.0);
    }
    else
    {
        normale=vNormale;
        couleurBis=couleur;
    }
    float brillance=50.0;
    vec3 vecL=normalize(-vPosition);
    float dotLN=dot(normale, vecL);
    vec4 ambiante= couleurBis*0.5;
    if (estFocus)
        ambiante = ambiante*1.2;
    vec4 diffuse= couleurBis*0.5;
    vec4 speculaire= couleurBis*0.8;
    diffuse=diffuse*max(0.0, dotLN);
    vec3 vecR=normalize(2.0*dotLN*normale-vecL);
    float dotRV=dot(vecR, normalize(-vPosition));
    speculaire=speculaire*max(0.0, pow(dotRV, brillance));
    if (speculaireActivee)
        gl_FragColor = ambiante + diffuse + speculaire;
    else
        gl_FragColor = ambiante + diffuse;
}
