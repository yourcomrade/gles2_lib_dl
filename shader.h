#ifndef SHADER_H
#define SHADER_H
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
    varying vec2 texco;
    const vec4 bitEnc = vec4(1.,255.,65025.,16581375.);
    const vec4 bitDec = 1./bitEnc;
    vec4 floatToRGBA(float value) {
    // Map the float value back to the range [0, 255] for each channel
    float red = value * 255.0;
    float green = fract(value * 256.0) * 255.0;  // Example: Using fractional part for the green channel
    float blue = fract(value * 65536.0) * 255.0; // Example: Using fractional part for the blue channel
    float alpha = fract(value * 16777216.0) * 255.0; // Example: Using fractional part for the alpha channel

    // Return the resulting RGBA color as a vec4
    return vec4(red, green, blue, alpha) / 255.0;
}
    float DecodeFloatRGBA (vec4 v) {
        float result = v.r + v.g / 256.0 + v.b / 65536.0 + v.a / 16777216.0;

        return result;
    }   
    
    void main() { 
        float inputValue = 0.75; // Replace this with your desired float value
        vec4 color = floatToRGBA(inputValue);

        gl_FragColor = color;
     }
);
#endif