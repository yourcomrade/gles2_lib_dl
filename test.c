#include "gles2_helper.h"
#include "shader.h"
#include <stdlib.h>

egl_gbm_controller*  my_test = NULL;
gles2_controller* my_con = NULL;

void test_compute(egl_gbm_controller*my_sp, gles2_controller* my_controller){
    float arr[20]={0.343f, -1.23f,-2.345f, -3.4535f, -4.12345f, 5.67123f, 6.4223f, 7.43532f, 8.45231f, 9.12141f, 10.12341f,
    11.23242f, 12.2324f, 13.2314f, 14.1314f, 15.1415f, 16.123151f, 17.131515f, 18.14141f, 19.14151f};
    float arr1[20]={0.343f, -1.70141184e38f,2.345f, 3.4535f, 4.12345f, 5.67123f, 6.4223f, 7.43532f, 8.45231f, 9.12141f, 10.12341f,
    11.23242f, 12.2324f, 13.2314f, 14.1314f, 1.70141184e38f, 16.123151f, 17.131515f, 18.14141f, 19.14151f};
    int n = 20;
    int my_sz = (int)floor(sqrt(n)) + 1;
    gles2_make_surface(my_sp, my_sz, my_sz);
    my_controller->num_text = 0;
    gles2_data* my_input = gles2_make_farr(arr, n);
    gles2_data* my_inp1 = gles2_make_farr(arr1, n);
    gles2_data* res = gles2_make_farr(NULL, n);
    gles2_push_farr(my_controller,res, NULL,false);
    my_controller->frag_shader = fragment;
    my_controller->ver_shader = vertex;
    gles2_build(my_controller);
    gles2_push_farr(my_controller,my_input,"data" ,true);
    gles2_push_farr(my_controller,my_inp1,"data1" ,true);
    
    
    gles2_make_fbo(my_controller, res);
    
    
    printf("my_sz: %d\n", my_sz);
    gles2_setViewport(my_sz, my_sz);
    gles2_compute(my_controller);
    
    
    float* my_arr = (float*)calloc(n, sizeof(float));
    if(my_arr == NULL){
        perror("Not enough memory for new array");
        
    }
    else{
        gles2_pull_farr(my_arr, n, res);
        printf("Compare between old and new: \n");
        for(int i = 0; i < n; i++){
            printf("old: %f   new: %f\n", arr[i], my_arr[i]);
        }
    }
   
    gles2_free_dev_farr(my_input);
    gles2_free_dev_farr(res);
    gles2_free_host_farr(my_arr);
    gles2_destroy_fbo(my_controller);
    gles2_destroy_surface(my_sp);
    


}
int main(){
    
    my_test = (egl_gbm_controller*)calloc(1, sizeof(egl_gbm_controller));
    if(my_test == NULL){
        perror("Create memory failed!\n");
        return 0;
    }
    my_con = (gles2_controller*)calloc(1, sizeof(gles2_controller));
    if(my_con == NULL){
        perror("Create memory failed!\n");
        return 0;
    }
    const char * file_name = "/dev/dri/renderD128";
    gles2_init(my_test, file_name);
    test_compute(my_test, my_con);
    gles2_destroy(my_test);
    free(my_test);
    free(my_con);
    return 0;
}