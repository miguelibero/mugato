#version 100

precision highp float;

attribute vec2 position;

uniform vec3 color;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}
