#version 100

precision highp float;

attribute vec2 position;
uniform mat4 model;
uniform mat4 cam;
uniform vec3 color;

void main()
{
    gl_Position = cam * model * vec4(position, 0.0, 1.0);
}
