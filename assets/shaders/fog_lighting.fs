#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragPosition;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables
uniform vec3 viewPos;
uniform vec4 ambient;
uniform float fogDensity;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    if(fragColor != vec4(1.0, 1.0, 1.0, 1.0))
        texelColor = colDiffuse * fragColor;
    vec3 normal = normalize(fragNormal) * 0.9;
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);
    float NdotL = max(dot(normal, viewD), 0.0);
    vec3 lightDot = vec3(NdotL);
    float specCo = 0.0;
    if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(-viewD), normal))), 16.0); // Shine: 16.0
    specular += specCo;
    finalColor = (texelColor*((colDiffuse + vec4(specular,1))*vec4(lightDot, 1.0)));
    finalColor += texelColor*(ambient/10.0);
    finalColor = pow(finalColor, vec4(1.0/2.2));
    float dist = length(viewPos - fragPosition);
    const vec4 fogColor = vec4(0.5, 0.5, 0.5, 1.0);
    const float fogDensity = 0.01;
    float fogFactor = 1.0/exp((dist*fogDensity)*(dist*fogDensity));
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    finalColor = mix(fogColor, finalColor, fogFactor);
}