#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Render size values must be passed from code
const float renderWidth = 1500;
const float renderHeight = 1000;

void main()
{
    //vec2 fragCoord = vec2(fragTexCoord.x + sin(fragTexCoord.y*100)/150, fragTexCoord.y);
    vec4 texelColor = texture(texture0, fragTexCoord, 0.0);

    //texelColor.a *= texelColor.a - 0.01;

    texelColor.a -= 1/255;
    

    finalColor = vec4(texelColor);
}