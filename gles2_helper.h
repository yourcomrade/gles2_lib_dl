#ifndef GLES2_HELPER
#define GLES2_HELPER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <EGL/egl.h>
#include <gbm.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>



struct egl_gbm_controller{
    EGLDisplay display;
	EGLConfig *configs;
	EGLContext context;
	EGLSurface surface;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLint numConfigs;
    EGLint count;
    struct gbm_device *gbm_dev;
	struct gbm_surface *gbm_surface;
    int fd;

};
typedef struct egl_gbm_controller egl_gbm_controller;
/*
* 
*/
struct gles2_data{
    int text_id;
    size_t textSize, n;
    bool col_order;
    bool tf;
    float * fdata;
    int * idata;
};
typedef struct gles2_data gles2_data;
struct gles2_controller{
    const char*  ver_shader; // vertex shader
    const char*  frag_shader; // fragment shader
    int pro_id;
    int fb_id;
    int num_text;
    GLint ver_id, frag_id;
   
};
typedef struct gles2_controller gles2_controller;
static const EGLint configAttribs[] = {
    EGL_BUFFER_SIZE, 32,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_NONE
    };
static const EGLint contextAttribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE};

/*
* @brief function to check error when initialize egl, 
* this function I takes from rpi_without_x code
*/
extern const char* check_egl_error();
/*
* @brief function to initialize gles2
* @details 
*/
extern void gles2_init(egl_gbm_controller* my_controller, const char * file);
extern void gles2_destroy(egl_gbm_controller* my_controller);
extern int gles2_matchconfig_egl(egl_gbm_controller* my_controller, uint32_t gbm_format);
extern void gles2_make_surface(egl_gbm_controller* my_controller, int w, int h);
extern void gles2_destroy_surface(egl_gbm_controller* my_controller);
extern gles2_data* gles2_make_farr(float *data, size_t n);
extern gles2_data* gles2_make_iarr(int *data, size_t n);
extern void gles2_make_fbo(gles2_controller* my_controller, gles2_data* gpu_data);
extern void gles2_push_farr( gles2_controller* my_controller, gles2_data* gpu_data, const char*name , bool is_input);
extern void gles2_push_iarr(gles2_data* gpu_data);
extern void gles2_pull_farr( float* data, size_t n,gles2_data* gpu_data);
//extern void gles2_pull_iarr( float* data, size_t n,gles2_data* gpu_data);
extern void gles2_destroy_fbo(gles2_controller* my_controller);
extern void gles2_free_dev_farr(gles2_data* gpu_data);
//extern void gles2_free_dev_iarr(gles2_data* gpu_data);
extern void gles2_free_host_farr(float *data);
extern void gles2_free_host_iarr(int *data);
extern void gles2_check_compile_error(unsigned int shader_id, const char* type);
extern void gles2_build(gles2_controller* my_controller);
extern void gles2_compute(gles2_controller* my_controller);
extern void gles2_setViewport(int width, int height);
extern int gles2_checkerror();

#endif