#include "gles2_helper.h"
#include <stdlib.h>
egl_gbm_controller*  my_test = NULL;
int main(){
    
    my_test = (egl_gbm_controller*)calloc(1, sizeof(egl_gbm_controller));
    if(my_test == NULL){
        perror("Create memory failed!\n");
        return 0;
    }
    const char * file_name = "/dev/dri/card1";
    gles2_init(my_test, file_name);
    gles2_destroy(my_test);
    free(my_test);
    return 0;
}