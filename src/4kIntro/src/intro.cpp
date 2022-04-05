#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <GL/gl.h>
#include <math.h>
#include <time.h>
#include "config.h"
#include "ext.h"
#include "vertex_shader.inl"
#include "fragment_shader.inl"
#ifdef POST_PROCESSING
#include "post_processing_shader.inl"
#endif
#include "fp.h"
#include <stdio.h>
//#include <malloc.h>

static GLuint fragmentShader;
static GLuint fragmentShaderPP;
static GLuint fragmentShaderPP2;
static GLuint renderingPipeline;
#ifdef POST_PROCESSING
static GLuint framebuffer;
static GLuint texture;
static GLuint depthTexture;
static GLuint postProcessingPipeline;
#endif

static GLuint tex1;

const int skull_cnt = 600;
float skull[skull_cnt] = {
	-0.385f,0.415f,0.450f,0.045f,
	-0.193f,0.200f,0.609f,0.025f,
	-0.048f,0.313f,0.780f,0.040f,
	-0.338f,0.318f,0.194f,0.025f,
	-0.115f,0.317f,0.594f,0.100f,
	-0.050f,0.587f,-0.187f,0.450f,
	-0.048f,0.589f,0.710f,0.020f,
	0.001f,0.313f,0.803f,0.040f,
	-0.329f,0.280f,0.314f,0.025f,
	-0.023f,0.110f,0.865f,0.050f,
	-0.296f,0.631f,0.396f,0.045f,
	-0.125f,0.769f,0.528f,0.035f,
	-0.041f,0.377f,0.772f,0.025f,
	-0.100f,0.100f,0.818f,0.050f,
	-0.249f,0.623f,0.399f,0.045f,
	-0.030f,0.610f,0.711f,0.025f,
	-0.244f,0.051f,0.554f,0.040f,
	-0.328f,0.581f,0.472f,0.030f,
	-0.192f,0.551f,0.405f,0.045f,
	-0.080f,0.528f,0.687f,0.030f,
	-0.150f,0.495f,0.576f,0.060f,
	-0.041f,0.403f,0.772f,0.025f,
	-0.395f,0.389f,0.349f,0.025f,
	-0.021f,0.461f,-0.231f,0.450f,
	-0.051f,0.760f,0.553f,0.035f,
	-0.021f,0.409f,0.515f,0.150f,
	-0.338f,0.309f,0.225f,0.025f,
	0.006f,0.407f,0.718f,0.025f,
	-0.202f,0.065f,0.667f,0.045f,
	-0.094f,0.495f,0.692f,0.030f,
	-0.077f,0.770f,0.544f,0.035f,
	-0.126f,0.409f,0.415f,0.175f,
	-0.153f,0.437f,0.546f,0.085f,
	-0.095f,0.438f,0.731f,0.025f,
	-0.296f,0.700f,0.449f,0.030f,
	-0.301f,0.405f,0.542f,0.045f,
	-0.168f,0.795f,0.412f,0.100f,
	-0.330f,0.613f,0.462f,0.025f,
	-0.291f,0.328f,0.489f,0.050f,
	-0.127f,0.221f,0.752f,0.025f,
	-0.063f,0.333f,0.773f,0.040f,
	-0.107f,0.275f,0.777f,0.025f,
	0.006f,0.539f,0.565f,0.110f,
	-0.341f,0.338f,0.153f,0.035f,
	-0.004f,0.630f,0.510f,0.085f,
	-0.026f,0.685f,-0.067f,0.450f,
	-0.226f,0.527f,0.551f,0.025f,
	-0.292f,0.268f,0.502f,0.050f,
	-0.391f,0.408f,0.418f,0.035f,
	-0.353f,0.447f,0.480f,0.045f,
	-0.090f,0.422f,0.630f,0.065f,
	-0.228f,0.058f,0.609f,0.040f,
	-0.311f,0.631f,0.422f,0.045f,
	-0.194f,0.534f,0.563f,0.025f,
	-0.067f,0.326f,0.675f,0.100f,
	-0.060f,0.289f,0.822f,0.025f,
	-0.179f,0.775f,0.510f,0.035f,
	-0.333f,0.291f,0.282f,0.025f,
	-0.314f,0.257f,0.387f,0.038f,
	-0.241f,0.743f,0.378f,0.100f,
	-0.178f,0.210f,0.648f,0.025f,
	-0.208f,0.244f,0.569f,0.025f,
	-0.338f,0.302f,0.251f,0.025f,
	-0.249f,0.551f,0.417f,0.045f,
	-0.164f,0.549f,0.578f,0.025f,
	-0.078f,0.495f,0.634f,0.060f,
	-0.168f,0.075f,0.721f,0.050f,
	-0.017f,0.386f,0.772f,0.025f,
	-0.250f,0.523f,0.533f,0.025f,
	-0.293f,0.358f,0.549f,0.045f,
	-0.376f,0.361f,0.238f,0.025f,
	-0.094f,0.403f,0.704f,0.030f,
	-0.276f,0.524f,0.513f,0.025f,
	-0.070f,0.603f,0.611f,0.025f,
	-0.160f,0.213f,0.685f,0.025f,
	-0.320f,0.536f,0.435f,0.045f,
	-0.104f,0.573f,0.603f,0.025f,
	-0.009f,0.244f,0.839f,0.025f,
	-0.065f,0.561f,0.687f,0.030f,
	-0.322f,0.644f,0.450f,0.025f,
	-0.020f,0.789f,0.128f,0.400f,
	-0.208f,0.198f,0.569f,0.025f,
	-0.358f,0.346f,0.198f,0.025f,
	-0.397f,0.397f,0.382f,0.030f,
	-0.073f,0.535f,0.716f,0.020f,
	-0.313f,0.518f,0.478f,0.045f,
	-0.256f,0.405f,0.565f,0.045f,
	-0.127f,0.266f,0.752f,0.025f,
	-0.088f,0.236f,0.801f,0.025f,
	-0.310f,0.580f,0.434f,0.045f,
	0.004f,0.722f,0.541f,0.060f,
	-0.028f,0.591f,0.633f,0.050f,
	-0.356f,0.405f,0.528f,0.045f,
	-0.089f,0.144f,0.613f,0.100f,
	-0.081f,0.504f,0.713f,0.020f,
	-0.012f,0.419f,0.772f,0.025f,
	-0.033f,0.289f,0.836f,0.025f,
	-0.005f,0.640f,0.728f,0.025f,
	-0.383f,0.370f,0.274f,0.025f,
	-0.391f,0.380f,0.311f,0.025f,
	-0.305f,0.550f,0.492f,0.025f,
	-0.033f,0.243f,0.836f,0.025f,
	-0.060f,0.243f,0.822f,0.025f,
	-0.231f,0.757f,0.485f,0.035f,
	-0.026f,0.421f,0.660f,0.025f,
	-0.242f,0.426f,0.500f,0.100f,
	-0.061f,0.552f,0.607f,0.050f,
	-0.325f,0.269f,0.347f,0.025f,
	-0.128f,0.795f,0.426f,0.100f,
	0.000f,0.642f,0.672f,0.050f,
	-0.276f,0.502f,0.480f,0.045f,
	0.009f,0.412f,0.679f,0.025f,
	-0.101f,0.340f,0.665f,0.085f,
	0.006f,0.468f,0.646f,0.025f,
	-0.129f,0.565f,0.593f,0.025f,
	-0.160f,0.259f,0.685f,0.025f,
	-0.381f,0.386f,0.476f,0.045f,
	-0.308f,0.674f,0.450f,0.030f,
	-0.013f,0.178f,0.687f,0.150f,
	-0.028f,0.604f,0.665f,0.040f,
	-0.023f,0.156f,0.833f,0.050f,
	-0.009f,0.289f,0.839f,0.025f,
	0.006f,0.435f,0.669f,0.025f,
	-0.316f,0.346f,0.060f,0.100f,
	-0.001f,0.346f,0.764f,0.040f,
	-0.107f,0.230f,0.777f,0.025f,
	-0.053f,0.636f,0.612f,0.025f,
	-0.088f,0.474f,0.722f,0.025f,
	-0.082f,0.281f,0.801f,0.025f,
	-0.141f,0.266f,0.724f,0.025f,
	-0.263f,0.101f,0.531f,0.040f,
	-0.178f,0.256f,0.648f,0.025f,
	-0.279f,0.204f,0.512f,0.050f,
	-0.141f,0.217f,0.724f,0.025f,
	-0.060f,0.563f,0.713f,0.020f,
	-0.137f,0.092f,0.776f,0.050f,
	-0.090f,0.417f,0.742f,0.025f,
	-0.020f,0.789f,0.128f,0.400f,
	-0.059f,0.864f,0.322f,0.200f,
	-0.064f,0.110f,0.847f,0.050f,
	-0.301f,0.234f,0.442f,0.050f,
	-0.125f,0.262f,0.537f,0.085f,
	-0.193f,0.244f,0.609f,0.025f,
	-0.069f,0.409f,0.761f,0.025f,
	0.000f,0.682f,0.625f,0.050f,
	-0.272f,0.737f,0.464f,0.035f,
	-0.268f,0.143f,0.520f,0.045f,
	-0.021f,0.789f,0.383f,0.165f,
	-0.094f,0.452f,0.692f,0.030f,
	-0.030f,0.158f,0.764f,0.100f,
};


const int stuff_cnt = 68;
float stuff[stuff_cnt] = {
	0.775f,0.100f,1.051f,0.100f,
	-3.510f,0.350f,-2.060f,0.350f,
	-3.948f,0.200f,-1.960f,0.200f,
	-1.651f,0.100f,0.430f,0.100f,
	-3.510f,0.350f,-0.680f,0.350f,
	-2.030f,0.100f,0.757f,0.100f,
	-3.050f,0.100f,-1.210f,0.100f,
	0.810f,0.200f,-2.620f,0.200f,
	-2.680f,0.100f,-0.283f,0.100f,
	1.400f,0.350f,-2.840f,0.350f,
	0.824f,0.100f,0.313f,0.100f,
	-3.410f,0.200f,-0.150f,0.200f,
	1.095f,0.150f,0.330f,0.150f,
	-1.736f,0.100f,0.257f,0.100f,
	-2.980f,0.150f,-0.640f,0.150f,
	-2.110f,0.100f,-0.290f,0.100f,
	-2.300f,0.750f,-2.460f,0.750f,
};

float buf[skull_cnt * 2];

int init(void)
{
	if (!EXT_Init())
		return 0;

	GLuint vertexShader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertex_shader_glsl);
	fragmentShader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragment_shader_glsl);

	glGenProgramPipelines(1, &renderingPipeline);
	glBindProgramPipeline(renderingPipeline);
	glUseProgramStages(renderingPipeline, GL_VERTEX_SHADER_BIT, vertexShader);
	glUseProgramStages(renderingPipeline, GL_FRAGMENT_SHADER_BIT, fragmentShader);
	
#ifdef POST_PROCESSING
	fragmentShaderPP = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &post_processing_shader_glsl);
	glGenProgramPipelines(1, &postProcessingPipeline);
	glBindProgramPipeline(postProcessingPipeline);
	glUseProgramStages(postProcessingPipeline, GL_VERTEX_SHADER_BIT, vertexShader);
	glUseProgramStages(postProcessingPipeline, GL_FRAGMENT_SHADER_BIT, fragmentShaderPP);
#endif

#ifdef DEBUG
	int result;
	char info[1536];
	glGetProgramiv(vertexShader, GL_LINK_STATUS, &result);
	glGetProgramInfoLog(vertexShader, 1024, NULL, (char *)info);
	if (!result) {
		MessageBox(0, info, "Error!", MB_OK | MB_ICONEXCLAMATION);
	}
	glGetProgramiv(fragmentShader, GL_LINK_STATUS, &result);
	glGetProgramInfoLog(fragmentShader, 1024, NULL, (char *)info);
	if (!result) {
		MessageBox(0, info, "Error!", MB_OK | MB_ICONEXCLAMATION);
	}
#ifdef POST_PROCESSING
	glGetProgramiv(fragmentShaderPP, GL_LINK_STATUS, &result);
	glGetProgramInfoLog(fragmentShaderPP, 1024, NULL, (char *)info);
	if (!result) {
		MessageBox(0, info, "Error!", MB_OK | MB_ICONEXCLAMATION);
	}
#endif
#endif

#ifdef POST_PROCESSING
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, XRES, YRES);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, XRES, YRES);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	static const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
#endif

	memcpy(buf, skull, sizeof(float) * skull_cnt);
	memcpy(buf + skull_cnt, stuff, sizeof(float) * stuff_cnt);
	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, skull_cnt / 4, 2, 0, GL_RGBA, GL_FLOAT, buf);

	return 1;
}

static float fparams[4 * 4];
static int x = 0;
static int y = 0;

bool render(bool final)
{
#ifdef POST_PROCESSING

	if (final)
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindProgramPipeline(postProcessingPipeline);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glRects(-1, -1, 1, 1);
		return true;
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		
		//glClearColor(1.0f, 0, 0, 0);
		//glClear(GL_COLOR_BUFFER_BIT);

		glBindProgramPipeline(renderingPipeline);
		fparams[0] = (float)(skull_cnt / 4) + 1.0;
		fparams[1] = 1.0f / (float)(skull_cnt / 4);
		fparams[2] = (float)(stuff_cnt / 4) + 1.0;
		glBindTexture(GL_TEXTURE_2D, tex1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glProgramUniform4fv(fragmentShader, 0, 4, fparams);

		float x0 = -1.0 + (float)x * 0.2f;
		float x1 = -1.0 + (float)(x + 1) * 0.2f;
		float y0 = -1.0 + (float)y * 0.2f;
		float y1 = -1.0 + (float)(y + 1) * 0.2f;

		glBegin(GL_POLYGON);
		glVertex2f(x0, y0);
		glVertex2f(x1, y0);
		glVertex2f(x1, y1);
		glVertex2f(x0, y1);
		glEnd();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glClear(GL_COLOR_BUFFER_BIT);
		glBindProgramPipeline(0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glRects(-1.0 + x * 0.2, -1.0 + y * 0.2, -1.0 + (x + 1) * 0.2, -1.0 + (y + 1) * 0.2);

		x++;

		if (x == 10)
		{
			if (y == 9) return true;
			x = 0;
			y++;
		}

		return false;
	}

#else
	glBindProgramPipeline(renderingPipeline);

	float t = clock() / 1000.0f;

	glBindProgramPipeline(renderingPipeline);

	fparams[0] = (float)(skull_cnt / 4) + 1.0;
	fparams[1] = 1.0f / (float)(skull_cnt / 4);
	fparams[2] = (float)(stuff_cnt / 4) + 1.0;
	fparams[3] = sin(t * 0.3f);

	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glProgramUniform4fv(fragmentShader, 0, 4, fparams);

	glRects(-1, -1, 1, 1); 
	return true;
#endif
}