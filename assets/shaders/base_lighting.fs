#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragPosition;
in vec3 fragNormal;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
out vec4 finalColor;
uniform vec3 viewPos;
void main()
{
    vec4 texelColor = colDiffuse * fragColor;
    vec3 normal = normalize(fragNormal) * 0.9;
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(1.0);
    float NdotL = max(dot(normal, viewD), 0.0);
    vec3 lightDot = vec3(NdotL);
    float specCo = 0.0;
    if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(-viewD), normal))), 16.0); // Shine: 16.0
    specular += specCo;
    finalColor = (texelColor*((colDiffuse + vec4(specular,1))*vec4(lightDot, 1.0)));
}
