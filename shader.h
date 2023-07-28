#ifndef SHADER_H
#define SHADER_H
#define STRINGIFY(x) #x
const char* vertex = STRINGIFY(
attribute vec3 pos;
attribute vec2 inputTexCoord;
varying vec2 texCoord;
void main(void){
    gl_Position = vec4(pos, 1.0);
    texCoord = inputTexCoord;
}
);

const char* fragment = STRINGIFY(
precision highp float; 
uniform sampler2D data;
varying vec2 texCoord;

void main(void){
    gl_FragColor = texture2D(data, vec2(0.0, 0.0));
}\n
);
#endif