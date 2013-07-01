varying vec3 vNormale;
varying vec3 vCamera;

uniform vec3 positionLumiere;
uniform bool estFocus;
uniform vec4 couleur;

void main ()
{
    vec4 speculaireCouleur=couleur*0.5;
    speculaireCouleur.a=couleur.a*3.0;
    if (estFocus)
        speculaireCouleur=speculaireCouleur*1.4;
    vec3 directionReflet = -reflect(normalize(vec3(gl_ModelViewMatrix * vec4(positionLumiere, 1.0))), vNormale);
    float speculaire = max(dot(vCamera, directionReflet), 0.0);
    gl_FragColor =couleur + speculaireCouleur*(speculaire+pow(speculaire, 10.0));
}
