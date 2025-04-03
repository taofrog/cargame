#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;

uniform mat4 mvp;

uniform vec2 p1pos;
uniform vec2 p2pos;
uniform vec2 p3pos;
uniform vec2 p4pos;

out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;

void main()
{
    fragColor = vertexColor;
    
    //float distance = min(min(length(vertexPosition.xy - p1pos), length(vertexPosition.xy - p2pos)), min(length(vertexPosition.xy - p3pos), length(vertexPosition.xy - p4pos)));
    //fragColor.a = min(distance/80 - 0.1, fragColor.a);
    fragNormal = vertexNormal;
    gl_Position = mvp*vec4(vertexPosition.xy, 0.0, 1.0);
}