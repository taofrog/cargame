#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;

uniform mat4 mvp;

out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;

void main()
{
    fragColor = vertexColor;
    
    fragNormal = vertexNormal;
    gl_Position = mvp*vec4(vertexPosition.xy, 0.0, 1.0);
}