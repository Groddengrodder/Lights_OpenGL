#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vertex_color;
out vec4 input_color;

void main(){
    gl_Position = position;
    input_color = vertex_color;
}
