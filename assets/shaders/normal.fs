#version 330
in vec3 fragNormal;
out vec4 finalColor;
void main()
{
    finalColor = vec4(abs(fragNormal.xyz), 1.0);
}