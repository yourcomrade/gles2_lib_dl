#include "gles2_helper.h"

#include <stdlib.h>
#define STRINGIFY(x) #x
const char* vertex = STRINGIFY(
    attribute vec3 pos;
    attribute vec2 inputTexCoord;
    varying vec2 texco;
    void main() { gl_Position = vec4(pos, 1.0); 
    texco = inputTexCoord;}
);

const char* fragment = STRINGIFY(
    
    precision highp float; 
    uniform sampler2D data;
    uniform sampler2D data1;
    uniform float my;
    varying vec2 texco;
   const float max_float = 1.70141184e38;
    const float min_float = 1.17549435e-38;
lowp vec4 encode_float(highp float v) {
  highp float av = abs(v);

  

  highp vec4 c = vec4(0,0,0,0);

  //Compute exponent and mantissa
  highp float e = floor(log2(av));
  highp float m = av * pow(2.0, -e) - 1.0;
  
  //Unpack mantissa
  c[1] = floor(128.0 * m);
  m -= c[1] / 128.0;
  c[2] = floor(32768.0 * m);
  m -= c[2] / 32768.0;
  c[3] = floor(8388608.0 * m);
  
  //Unpack exponent
  highp float ebias = e + 127.0;
  c[0] = floor(ebias / 2.0);
  ebias -= c[0] * 2.0;
  c[1] += floor(ebias) * 128.0; 

  //Unpack sign bit
  c[0] += 128.0 * step(0.0, -v);

  //Check for 1st condition if(av < min_float) return vec4(0)
  highp vec4 res1 = vec4(step( min_float, av)*c);
  //Check for 2nd condition if(v > max_float) return vec4(127.0, 128.0, 0.0, 0.0)
  highp vec4 res2 = (1.0 - step(max_float, v))*res1 + step(max_float, v)*vec4(127.0, 128.0, 0.0, 0.0);
  //Check for 3rd condition if(v < -max_float) return  vec4(255.0, 128.0, 0.0, 0.0)
  highp vec4 res3 = (1.0 - step(v, -max_float))*res2 + step(v, -max_float)*vec4(255.0, 128.0, 0.0, 0.0);
  //Final result
  highp vec4 fin_res = res3.abgr;

  //Scale back to range
  return fin_res / 255.0;
}
float decode_float(vec4 v) {
     vec4 bits = v * 255.0;
     float sign = mix(-1.0, 1.0, step(bits[3], 128.0));
     float expo = floor(mod(bits[3] + 0.1, 128.0)) * 2.0 +
                  floor((bits[2] + 0.1) / 128.0) - 127.0;
     float sig = bits[0] +
                 bits[1] * 256.0 +
                 floor(mod(bits[2] + 0.1, 128.0)) * 256.0 * 256.0;
     return sign * (1.0 + sig / 8388607.0) * pow(2.0, expo);
   }


    void main() { 
        
        
        vec4 color1 = texture2D(data, texco);
        vec4 color2 = texture2D(data1, texco);
        float a = decode_float(color1);
        float b = decode_float(color2);
        float res = my*a + b;
        vec4 r = encode_float(res);
        gl_FragColor = r;
     }
);
egl_gbm_controller*  my_test = NULL;
gles2_controller* my_con = NULL;

void test_compute(egl_gbm_controller*my_sp, gles2_controller* my_controller){
    float arr[20]={0.343f, -1.23f,-2.345f, -3.4535f, -4.12345f, 5.67123f, 6.4223f, 7.43532f, 8.45231f, 9.12141f, 10.12341f,
    11.23242f, 12.2324f, 13.2314f, 14.1314f, 15.1415f, 16.123151f, 17.131515f, 18.14141f, 19.14151f};
    float arr1[20]={0.343f, -1.70141184e38f,2.345f, 3.4535f, 4.12345f, 5.67123f, 6.4223f, 7.43532f, 8.45231f, 9.12141f, 10.12341f,
    11.23242f, 12.2324f, 13.2314f, 14.1314f, 1.70141184e38f, 16.123151f, 17.131515f, 18.14141f, 19.14151f};
    int n = 20;
    int my_sz = (int)floor(sqrt(n)) + 1;
    float a = 0.923f;
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
    
    gles2_push_float(my_controller, a, "my");
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
        printf("Result of a*data + data1: \n");
        for(int i = 0; i < n; i++){
            printf("%f*data: %f + data1: %f  res: %f\n", a, arr[i],arr1[i] ,my_arr[i]);
        }
    }
   
    gles2_free_dev_farr(my_input);
    gles2_free_dev_farr(my_inp1);
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