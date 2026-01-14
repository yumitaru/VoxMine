#version 330 core
layout (location = 0) in vec2 aPos;

out vec2 vUV;
uniform mat4 transform;

void main()
{
    vUV = aPos * 0.5 + 0.5;
    gl_Position = transform * vec4(aPos, 0.0, 1.0);
}
