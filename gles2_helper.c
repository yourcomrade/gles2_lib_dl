#include "gles2_helper.h"
const char* check_egl_error(){
    switch (eglGetError())
    {
    case EGL_SUCCESS:
        return "Success.";
    case EGL_NOT_INITIALIZED:
        return "EGL is not initialized, or could not be initialized, for the "
               "specified EGL display connection.";
    case EGL_BAD_ACCESS:
        return "EGL cannot access a requested resource. ";
    case EGL_BAD_ALLOC:
        return "EGL failed to allocate resources for the requested operation.";
    case EGL_BAD_ATTRIBUTE:
        return "An unrecognized attribute or attribute value was passed in the "
               "attribute list.";
    case EGL_BAD_CONTEXT:
        return "An EGLContext argument does not name a valid EGL rendering "
               "context.";
    case EGL_BAD_CONFIG:
        return "An EGLConfig argument does not name a valid EGL frame buffer "
               "configuration.";
    case EGL_BAD_CURRENT_SURFACE:
        return "The current surface of the calling thread is a window, pixel "
               "buffer or pixmap that is no longer valid.";
    case EGL_BAD_DISPLAY:
        return "An EGLDisplay argument does not name a valid EGL display "
               "connection.";
    case EGL_BAD_SURFACE:
        return "An EGLSurface argument does not name a valid surface (window, "
               "pixel buffer or pixmap) configured for GL rendering.";
    case EGL_BAD_MATCH:
        return "Arguments are inconsistent (for example, a valid context "
               "requires buffers not supplied by a valid surface).";
    case EGL_BAD_PARAMETER:
        return "One or more argument values are invalid.";
    case EGL_BAD_NATIVE_PIXMAP:
        return "A NativePixmapType argument does not refer to a valid native "
               "pixmap.";
    case EGL_BAD_NATIVE_WINDOW:
        return "A NativeWindowType argument does not refer to a valid native "
               "window.";
    case EGL_CONTEXT_LOST:
        return "A power management event has occurred. The application must "
               "destroy all contexts and reinitialise OpenGL ES state and "
               "objects to continue rendering.";
    default:
        break;
    }
    return "Unknown error!";
}
void gles2_init(egl_gbm_controller* my_controller, const char * file){
    //initialize 
    my_controller->fd = open(file,O_RDWR | FD_CLOEXEC);
    if(my_controller->fd < 0){
        perror("Cannot open device driver file of graphic");
        abort();
    }
    my_controller->gbm_dev = gbm_create_device(my_controller->fd);
    if(my_controller->gbm_dev == NULL){
        perror("Cannot create gbm device");
        abort();
    }
    //check for display
    my_controller->display = eglGetDisplay((EGLNativeDisplayType)(my_controller->gbm_dev));
    if(my_controller->display == EGL_NO_DISPLAY){
        perror("eglGetDisplay fails!");
        gbm_device_destroy(my_controller->gbm_dev);
        close(my_controller->fd);
        abort();
    }
    //initialize program
    if(eglInitialize(my_controller->display,&(my_controller->majorVersion),
    &(my_controller->minorVersion)) == EGL_FALSE){
       fprintf(stderr, "Failed to get EGL version! Error: %s\n",
                check_egl_error());
        eglTerminate(my_controller->display);
      
        gbm_device_destroy(my_controller->gbm_dev);
        close(my_controller->fd);
        abort();
    }
    printf("EGL init with version %d.%d\n", my_controller->majorVersion, my_controller->minorVersion);
    //Bind API
    if(eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE){
        fprintf(stderr,"eglBindApi failed! Error: %s\n",check_egl_error());
        eglTerminate(my_controller->display);
        gbm_device_destroy(my_controller->gbm_dev);
        close(my_controller->fd);
        abort();
    }
    //Initialize config
    if(eglChooseConfig(my_controller->display, configAttribs, &(my_controller->config),
    1 , &(my_controller->numConfigs)) == EGL_FALSE){
        fprintf(stderr, "eglChooseConfig failed! Error: %s\n", check_egl_error());
        eglTerminate(my_controller->display);
        gbm_device_destroy(my_controller->gbm_dev);
        close(my_controller->fd);
        abort();
    }

    //Create context
   my_controller->context = eglCreateContext(my_controller->display, my_controller->config, EGL_NO_CONTEXT, contextAttribs);
   if(my_controller->context == EGL_NO_CONTEXT){
        fprintf(stderr, "eglCreateContext failed! Error: %s\n", check_egl_error());
        eglTerminate(my_controller->display);
        gbm_device_destroy(my_controller->gbm_dev);
        close(my_controller->fd);
        abort();
   }
}

void gles2_destroy(egl_gbm_controller* my_controller){
    eglDestroyContext (my_controller->display, my_controller->context);
    eglTerminate (my_controller->display);
    gbm_device_destroy (my_controller->gbm_dev);
    close (my_controller->fd);
}

void gles2_make_pbo( egl_gbm_controller* my_controller, int width, int height){
  
    my_controller->gbm_surface = gbm_surface_create(my_controller->gbm_dev, width, height, GBM_FORMAT_RGBA8888, GBM_BO_USE_RENDERING);
    if(my_controller->gbm_surface == NULL){
        perror("Cannot create gbm surface!");
        gles2_destroy(my_controller);
        abort();
    }
    my_controller->surface = eglCreateWindowSurface(my_controller->display, my_controller->config,
    (EGLNativeWindowType)my_controller->gbm_surface, NULL);
    if(my_controller->surface == EGL_NO_SURFACE){
        fprintf(stderr, "Cannot create window surface! Error: %s\n",check_egl_error());
        gles2_destroy_pbo(my_controller);
        abort();
    }
    if(eglMakeCurrent(my_controller->display, my_controller->surface,
    my_controller->surface, my_controller->context) == EGL_FALSE){
        fprintf(stderr, "Cannot bind current thread to compute! Error: %s\n", check_egl_error());
        gles2_destroy_pbo(my_controller);
        abort();
    }
}

void gles2_destroy_pbo(egl_gbm_controller* my_controller){
     if(eglDestroySurface(my_controller->display, my_controller->surface) == EGL_FALSE){
        fprintf(stderr, "Cannot destroy surface! Error: %s\n", check_egl_error());
        gles2_destroy(my_controller);
        abort();
     }
}
