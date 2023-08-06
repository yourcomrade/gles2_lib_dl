# gles2_lib_gpgpu
This library is used for gpgpu by using OpenGL ES 2.0. This library only works on Linux, I am not sure whether it will work on Window Subsystem Linux or not. 
# How does it work
Basically, this library provides an old method to use GPU for parallel computing by using graphic API OpenGL ES2. We can use some other APIs like OpenCL, CUDA or even compute shader in OpenGL ES3.2 to do GPGPU, but not all GPUs support that.   
For GPUs do not support compute shader, OpenCL, and CUDA, we have to trick the GPU to use it for GPGPU. This requires us to understand OpenGL ES pipeline. Basically, we draw a rectangle, transfer data as image and the color of the rectangle is the result of we need. To do this, we have to set up both coordinate of rectangle and texture coordinate in vertex shader. After that, we transfer data as texture to fragment shader, we compute the data in fragment shader and retrieve them back.
# How to use this library
This library is currently provide a convenient way to accelerate the computation of the array of floating point number 32-bits. This library depends on GLESv2, EGL, and GBM libraries. Thus, user must install these libraries before using it.  

Step by step to use API functions in this library to compute:   
1. Init EGL and GBM
2. Create a surface to draw
3. Prepare data for gpu
4. Push the gpu data firstly which will contains future results of gpu computing first. It must be NULL. 
5. Create shader and build program
6. Push other input data to gpu memory
7. Create framebuffer binding to gpu data result
8. Set viewport or mapping texture coordinate to input data
9. Compute
10. Retrrieve the result back
11. Clean everything  
The example can be looked at test.c file for more understanding. To run the example, user can run this command:
```bash
make all
```
The result should look like this:
```
[non@fedora gles2_lib_dl]$ ./a.out 
EGL init with version 1.5
Max texture size support: 16384
Max texture size support: 16384
Max texture size support: 16384
push data
push data
Before insert: 
data: 178 157 175 62
data: 164 112 157 191
data: 123 20 22 192
data: 37 6 93 192
data: 77 243 131 192
data: 183 122 181 64
data: 123 131 205 64
data: 36 238 237 64
data: 169 60 7 65
data: 76 241 17 65
data: 125 249 33 65
data: 254 183 51 65
data: 233 183 67 65
data: 208 179 83 65
data: 55 26 98 65
data: 150 67 114 65
data: 55 252 128 65
data: 88 13 137 65
data: 156 33 145 65
data: 208 33 153 65
push data
Before insert: 
data: 178 157 175 62
data: 0 0 0 255
data: 123 20 22 64
data: 37 6 93 64
data: 77 243 131 64
data: 183 122 181 64
data: 123 131 205 64
data: 36 238 237 64
data: 169 60 7 65
data: 76 241 17 65
data: 125 249 33 65
data: 254 183 51 65
data: 233 183 67 65
data: 208 179 83 65
data: 55 26 98 65
data: 0 0 0 127
data: 55 252 128 65
data: 88 13 137 65
data: 156 33 145 65
data: 208 33 153 65
my_sz: 5
pull data
temp_arr: 178 157 47 63
gpu data: 0.686000
temp_arr: 0 0 128 255
gpu data: -inf
temp_arr: 0 0 0 0
gpu data: 0.000000
temp_arr: 0 0 0 0
gpu data: 0.000000
temp_arr: 0 0 0 0
gpu data: 0.000000
temp_arr: 183 122 53 65
gpu data: 11.342460
temp_arr: 124 131 77 65
gpu data: 12.844601
temp_arr: 37 238 109 65
gpu data: 14.870641
temp_arr: 169 60 135 65
gpu data: 16.904619
temp_arr: 76 241 145 65
gpu data: 18.242821
temp_arr: 125 249 161 65
gpu data: 20.246820
temp_arr: 254 183 179 65
gpu data: 22.464840
temp_arr: 234 183 195 65
gpu data: 24.464802
temp_arr: 209 179 211 65
gpu data: 26.462801
temp_arr: 56 26 226 65
gpu data: 28.262802
temp_arr: 0 0 128 127
gpu data: inf
temp_arr: 55 252 0 66
gpu data: 32.246304
temp_arr: 88 13 9 66
gpu data: 34.263031
temp_arr: 156 33 17 66
gpu data: 36.282822
temp_arr: 208 33 25 66
gpu data: 38.283020
Result of data + data1: 
data: 0.343000 + data1: 0.343000  res: 0.686000
data: -1.230000 + data1: -170141183460469231731687303715884105728.000000  res: -inf
data: -2.345000 + data1: 2.345000  res: 0.000000
data: -3.453500 + data1: 3.453500  res: 0.000000
data: -4.123450 + data1: 4.123450  res: 0.000000
data: 5.671230 + data1: 5.671230  res: 11.342460
data: 6.422300 + data1: 6.422300  res: 12.844601
data: 7.435320 + data1: 7.435320  res: 14.870641
data: 8.452310 + data1: 8.452310  res: 16.904619
data: 9.121410 + data1: 9.121410  res: 18.242821
data: 10.123410 + data1: 10.123410  res: 20.246820
data: 11.232420 + data1: 11.232420  res: 22.464840
data: 12.232400 + data1: 12.232400  res: 24.464802
data: 13.231400 + data1: 13.231400  res: 26.462801
data: 14.131400 + data1: 14.131400  res: 28.262802
data: 15.141500 + data1: 170141183460469231731687303715884105728.000000  res: inf
data: 16.123152 + data1: 16.123152  res: 32.246304
data: 17.131516 + data1: 17.131516  res: 34.263031
data: 18.141411 + data1: 18.141411  res: 36.282822
data: 19.141510 + data1: 19.141510  res: 38.283020
```
# Other projects
There are other projects about this topic. Here are some of them:
1. GPGPU with GLES: https://github.com/JDuchniewicz/GPGPU-with-GLES/tree/master
2. OpenGLES GPGPU: https://github.com/internaut/ogles_gpgpu
# Credit
The shader code to convert 4 unsigned bytes to float and convert float to 4 unsigned bytes is not from me. I copy shader converting float to 4 unsigned bytes from **mikolalysenko** in this repository: https://github.com/mikolalysenko/glsl-read-float/blob/master/index.glsl. 
For the shader code converting 4 unsigned bytes to float, I take it from this answer in StackOverflow: https://stackoverflow.com/questions/63827836/is-it-possible-to-do-a-rgba-to-float-and-back-round-trip-and-read-the-pixels-in?noredirect=1&lq=1. In case that the link to StackOverflow does not work, you can look at html file I download.
