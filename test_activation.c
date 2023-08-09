#include "gles2_helper.h"
#include <assert.h>
#include <stdlib.h>
#define STRINGIFY(x) #x
const char* vertex = STRINGIFY(
    attribute vec3 pos;
    attribute vec2 inputTexCoord;
    varying vec2 texco;
    void main() { gl_Position = vec4(pos, 1.0); 
    texco = inputTexCoord;}
);
const char encode_decode_float_shader[] = STRINGIFY(
    precision highp float;
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
);
const char frag_leaky_activate_shader[] = STRINGIFY(
    uniform sampler2D data;
    varying vec2 texco;
    void main(){
        vec4 inp_data = texture2D(data, texco);
        float a = decode_float(inp_data);
        
        float res = max(0.1*a, a);
        gl_FragColor = encode_float(res);
    }
);
const char frag_logistic_activate_shader[] = STRINGIFY(
    uniform sampler2D data;
    varying vec2 texco;
    void main(){
        vec4 inp_data = texture2D(data, texco);
        float a = decode_float(inp_data);
        //Use step instead of if to boost performance
        float res = 1.0/(1.0 + exp(-a));
        gl_FragColor = encode_float(res);
    }
);

egl_gbm_controller*  my_test = NULL;
gles2_controller* my_con = NULL;
static inline float leaky_activate(float x){return (x>0) ? x : .1f*x;}
static inline float logistic_activate(float x){return 1.f/(1.f + expf(-x));}
void test_compute(egl_gbm_controller*my_sp, gles2_controller* my_controller){
    float arr[]={-0.123655, 0.533278, -0.082425, 0.085383, 0.360406, 0.276121, -0.249910, 0.010647, -0.292053, -0.094009, 0.050633, -0.728365, 0.045460, 0.106844, 0.475100, -0.181643, 0.072359, 0.864234, 0.342490, 0.100119, -0.451128, -0.198801, 0.747096, 0.464819, -1.155195, -0.303511, 0.223517, 0.491252, -0.238814, -0.315180, 0.485583, 0.050834, -0.305989, 0.068928, 0.640504, 1.809342, -2.524241, -1.301679, 0.224443, 0.192385, -0.019445, 0.083100, -0.126404, 0.189161, 0.529693, -0.386960, 0.064984, 1.047463, 1.742466, -2.633503, -1.069251, 0.944060, 0.124780, 0.093408, 0.382660, 0.532318, -0.620198, 0.484291, -0.623058, 0.182185, 
    0.596187, 0.274417, -1.200794, -0.307147, 0.584714, -0.195148, 0.000496, 0.989765, 0.229584, -0.968996, 0.916280, -0.673894, -0.513870, -0.019740, 0.496415, -1.320132, -0.219337, 0.680098, -0.425918, 0.004987, 0.896780, 0.465968, -0.077053, 0.893838, -0.631553, -0.345273, -0.744495, 0.305717, -0.750523, 0.007061, 0.414702, -0.754552, 0.147833, 0.432358, 0.376304, -0.664315, 0.514903, -0.025207, -0.360435, -1.190298, -0.137537, 0.179447, 0.126289, 0.293663, -0.684047, 0.344606, 0.238762, 0.639843, -1.134826, 0.123114, 0.153630, -0.587443, -0.898922, -0.039245, 0.509039, -0.150519, 0.669844, -1.113161, 0.248549, 0.766182, 0.529008, 
    -1.476342, -0.048927, -0.283949, -0.153570, -0.331945, -0.183395, 0.570587, -0.665018, 0.979866, -0.831387, 0.071961, 0.309893, 0.460147, -1.729719, 0.141076, -0.315610, -0.120831, -0.326742, -0.063020, 1.126565, -1.089649, 0.738107, -0.400927, -0.256063, 0.228736, -0.241761, -0.585645, -0.051945, -0.060133, -0.055287, -0.278840, 0.095685, 0.579973, -0.522352, -0.128538, 0.307560, 0.388706, 0.218800, -0.285189, 0.049138, -0.020880, 0.112467, -0.027898, -0.564284, -0.132473, -0.172358, -0.939438, -0.293592, 0.204834, 0.933982, 0.901564, 0.751517, 1.254550, 1.615005, 1.600761, 1.047475, 0.637500, 0.477055, 0.707457, 0.831063, 0.382670,
     0.277603, 0.551022, 0.576326, 0.140110, -0.287372, -0.422566, -0.238427, 0.063042, 0.277888, 0.997451, 0.817045, 0.427981, 0.010741, 0.792623, -0.216780, -0.069121, -0.060775, -0.085606, -0.030646, -0.240324, -0.461290, 0.584217, 1.837104, 1.939313, -0.602212, -0.043767, -0.441797, 0.007681, 0.202646, 0.927973, 0.661926, 0.315253, 0.222196, -0.392590, -1.453753, -3.179527, -3.079737, -1.134683, -0.619066, -0.570348, -0.454308, -0.010751, 0.740410, 0.473026, 0.621881, 0.404832, 0.325458, 0.232927, -0.023330, -0.080648, 0.219705, -0.003310, 0.088157, 0.174950, -0.114534, 0.512578, -0.107708, 0.236748, 0.433912, 0.489296, 0.112525, 0.072366,
      0.401072, -0.109265, 0.046045, -0.162682, 0.268410, 0.434584, 0.563617, 0.681888, 0.653762, 0.463123, 0.554173, 0.592548, -0.105602, 0.126355, 0.683793, 0.400360, 0.025066, -0.028201, -0.054082, 0.621733, -0.161909, -0.736499, -0.357951, -0.017385, -0.239490, -0.222102, -0.269496, 0.061411, -0.075093, -0.401478, -0.806593, -0.685419, -0.844947, -0.704978, -0.971064, -1.067050, -0.970155, -0.377552, -0.000541, -0.585546, -0.515019, -0.482230, 0.085028, 0.400723, -0.034774, -0.423466, -0.084381, -0.748764, -1.083550, -1.132414, -0.822726, -0.502388, -0.369514, 0.383438, -0.118170, 0.584600, 0.765521, 0.161844, -0.626965, -0.220565, 0.200685, 
      0.422511, 0.539109, 0.507928, 0.358242, 0.601025, 1.076676, -0.187966, -0.059838, -0.305276, 0.307980, 0.112539, 0.777924, 0.852599, 0.741060, 0.886395, 0.708189, 0.559705, 0.320733, 0.068881, 0.153527, -0.974075, -1.232133, -0.705986, -0.489367, -0.142673, 0.026656, 0.094383, 0.083549, -0.000417, -0.195541, -0.427266, -0.953776, -0.672079};
    int n = sizeof(arr)/sizeof(arr[0]);
    int my_sz = (int)floor(sqrt(n)) + 1;
    gles2_make_surface(my_sp, my_sz, my_sz);
    my_controller->num_text = 0;
    gles2_data* my_input = gles2_make_farr(arr, n);
    printf("n = %d, my_sz = %d", n, my_sz);
    gles2_data* res = gles2_make_farr(NULL, n);
    gles2_push_farr(my_controller,res, NULL,false);
    char* fragment = (char*)calloc(sizeof(encode_decode_float_shader) + sizeof(frag_logistic_activate_shader), sizeof(char));
    if(fragment == NULL){
        perror("Cannot allocate memory for fragment shader");
    }
    strcat(fragment, encode_decode_float_shader);
    strcat(fragment, frag_logistic_activate_shader);
    my_controller->frag_shader = &fragment[0];
    my_controller->ver_shader = vertex;
    gles2_build(my_controller);
    gles2_push_farr(my_controller,my_input,"data" ,true);
  
    
    
    gles2_make_fbo(my_controller, res);
    
    
    printf("my_sz: %d\n", my_sz);
    gles2_setViewport(my_sz, my_sz);
    gles2_compute(my_controller);
    
    
    float* my_arr = (float*)calloc(n, sizeof(float));
    if(my_arr == NULL){
        perror("Not enough memory for new array");
        
    }
    
    float *my_arr1 = (float*)calloc(n, sizeof(float));
    if(my_arr1 == NULL){
        perror("Not enough memory for my_arr1");
    }   
        
    gles2_pull_farr(my_arr, n, res);
    printf("Compare between cpu and gpu: \n");
    for(int i = 0; i < n; i++){
        my_arr1[i] = logistic_activate(arr[i]);
        printf("gpu[%d]: %f , cpu[%d]: %f\n", i, my_arr1[i], i, my_arr[i]);

    }
    
   
    gles2_free_dev_farr(my_input);
   
    gles2_free_dev_farr(res);
    gles2_free_host_farr(my_arr);
    gles2_free_host_farr(my_arr1);
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