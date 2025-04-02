#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

out vec4 finalColor;

void main()
{
    finalColor = vec4(fragColor.rgb * dot(fragNormal, vec3(0.2672612419, -0.5345224838, 0.8017837257)), fragColor.a);
}