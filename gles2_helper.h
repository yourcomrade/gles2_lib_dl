#ifndef GLES2_HELPER
#define GLES2_HELPER
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <EGL/egl.h>
#include <gbm.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

struct egl_gbm_helper{
    EGLDisplay display;
	EGLConfig config;
	EGLContext context;
	EGLSurface surface;
    struct gbm_device *gbm_dev;
	struct gbm_surface *gbm_surface;
    int fd;

};
typedef struct egl_gbm_helper egl_gbm_helper;
static const EGLint configAttribs[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_NONE};
static const EGLint contextAttribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE};

/*
* @brief function to check error when initialize egl
*/
extern const char* check_egl_error();
/*
* @brief function to initialize gles2
* @details 
*/
extern void gles2_init(egl_gbm_helper* my_helper);
extern void gles2_destroy(egl_gbm_helper* my_helper);
extern void gles2_make_farr(float *x, size_t n);
extern void gles2_make_iarr(int *x, size_t n);
extern void gles2_push_farr(float *x, size_t n);
extern void gles2_push_iarr(int *x, size_t n);
extern void gles2_pull_farr(float *x, size_t n);
extern void gles2_pull_iarr(int *x, size_t n);
extern void gles2_free_dev_farr(float *x);
extern void gles2_free_dev_iarr(int *x);
extern void gles2_free_host_farr(float *x);
extern void gles2_free_host_iarr(int *x);

#endif