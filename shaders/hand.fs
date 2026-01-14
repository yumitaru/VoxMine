#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D tex;
uniform int mode;

void main()
{
    if (mode == 1) {
        vec4 c = texture(tex, vUV);

        if (c.a < 0.1)
            discard;

        FragColor = c;
    } else {
        FragColor = vec4(0.85, 0.75, 0.6, 1.0);
    }
}
