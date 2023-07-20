#ifndef GLES2_HELPER
#define GLES2_HELPER
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <EGL/egl.h>
#include <gbm.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdbool.h>
#define STRINGIFY(x) #x

struct egl_gbm_controller{
    EGLDisplay display;
	EGLConfig config;
	EGLContext context;
	EGLSurface surface;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLint numConfigs;
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
    int width, height, n;
    bool col_order;
    bool tf;
    float * fdata;
    int * idata;
};
typedef struct gles2_data gles2_data;
struct gles2_controller{
    char* ver_shader; // vertex shader
    char* frag_shader; // fragment shader
    int pro_id;
    int fb_id;
    int num_text;
    gles2_data my_data[3];
};
typedef struct gles2_controller gles2_controller;
static const EGLint configAttribs[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_BIND_TO_TEXTURE_RGBA, EGL_TRUE,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_NONE};
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
extern void gles2_make_pbo(egl_gbm_controller* my_controller, int w, int h);
extern void gles2_destroy_pbo(egl_gbm_controller* my_controller);
extern gles2_data* gles2_make_farr(float *data, size_t n);
extern gles2_data* gles2_make_iarr(int *data, size_t n);
extern void gles2_push_farr(float *data, size_t n, gles2_data* gpu_data);
extern void gles2_push_iarr(int *data, size_t n, gles2_data* gpu_data);
extern void gles2_pull_farr(float *data, size_t n, gles2_data* gpu_data);
extern void gles2_pull_iarr(int *data, size_t n, gles2_data* gpu_data);
extern void gles2_destroy_pbo(egl_gbm_controller* my_controller);
extern void gles2_free_dev_farr(gles2_data* gpu_data);
extern void gles2_free_dev_iarr(gles2_data* gpu_data);
extern void gles2_free_host_farr(float *data);
extern void gles2_free_host_iarr(int *data);
extern void gles2_build(gles2_controller* my_controller);
extern void gles2_compute(gles2_controller* my_controller, egl_gbm_controller* support);
#endif