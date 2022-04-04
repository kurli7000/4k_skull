#version 430

layout (location=0) out vec4 fragColor;
uniform sampler2D inputTexture;

in vec2 p;

void main()
{
	vec2 fc = (p * 0.5 + 0.5);
    fragColor = texture(inputTexture, fc);
}