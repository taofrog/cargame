#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

out vec4 finalColor;

uniform vec2 p1pos;
uniform vec2 p2pos;
uniform vec2 p3pos;
uniform vec2 p4pos;

void main()
{
    vec4 color = vec4(fragColor.rgb * dot(fragNormal, vec3(0.2672612419, -0.5345224838, 0.8017837257)), fragColor.a);
    vec2 ppos = vec2(gl_FragCoord.x, 1000 - gl_FragCoord.y);
    float distance = min(min(min(length(ppos - p1pos), length(ppos - p2pos)), min(length(ppos - p3pos), length(ppos - p4pos))),80);
    color.a = min(0.6-0.4*cos(3.1415*distance/81), color.a);
    gl_FragColor = color;
}