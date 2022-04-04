#ifndef _EXTENSIONS_H_
#define _EXTENSIONS_H_

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#endif
#include <GL/gl.h>
#include "glext.h"
#include "config.h"


#ifdef DEBUG
	#ifdef POST_PROCESSING
		#define NUMFUNCTIONS 12
	#else
		#define NUMFUNCTIONS 8
	#endif
#else
	#ifdef POST_PROCESSING
		#define NUMFUNCTIONS 10
	#else
		#define NUMFUNCTIONS 6
		#endif
#endif

extern void *myglfunc[NUMFUNCTIONS];

#define glCreateShaderProgramv         ((PFNGLCREATESHADERPROGRAMVPROC)myglfunc[0])
#define glGenProgramPipelines          ((PFNGLGENPROGRAMPIPELINESPROC)myglfunc[1])
#define glBindProgramPipeline          ((PFNGLBINDPROGRAMPIPELINEPROC)myglfunc[2])
#define glUseProgramStages             ((PFNGLUSEPROGRAMSTAGESPROC)myglfunc[3])
#define glProgramUniform4fv            ((PFNGLPROGRAMUNIFORM4FVPROC)myglfunc[4])
#define glDrawBuffers                  ((PFNGLDRAWBUFFERSPROC)myglfunc[5])

#ifdef DEBUG
#ifdef POST_PROCESSING
#define glGenFramebuffers              ((PFNGLGENFRAMEBUFFERSPROC)myglfunc[6])
#define glBindFramebuffer              ((PFNGLBINDFRAMEBUFFERPROC)myglfunc[7])
#define glTexStorage2D                 ((PFNGLTEXSTORAGE2DPROC)myglfunc[8])
#define glFramebufferTexture           ((PFNGLFRAMEBUFFERTEXTUREPROC)myglfunc[9])
#define glGetProgramiv          ((PFNGLGETPROGRAMIVPROC)myglfunc[10])
#define glGetProgramInfoLog     ((PFNGLGETPROGRAMINFOLOGPROC)myglfunc[11])
#else
#define glGetProgramiv          ((PFNGLGETPROGRAMIVPROC)myglfunc[6])
#define glGetProgramInfoLog     ((PFNGLGETPROGRAMINFOLOGPROC)myglfunc[7])
#endif
#else
#ifdef POST_PROCESSING
#define glGenFramebuffers              ((PFNGLGENFRAMEBUFFERSPROC)myglfunc[6])
#define glBindFramebuffer              ((PFNGLBINDFRAMEBUFFERPROC)myglfunc[7])
#define glTexStorage2D                 ((PFNGLTEXSTORAGE2DPROC)myglfunc[8])
#define glFramebufferTexture           ((PFNGLFRAMEBUFFERTEXTUREPROC)myglfunc[9])
#endif
#endif

// init
int EXT_Init( void );

#endif
