#shader vertex
#version 460 core

layout (location=0) in vec3 aPos;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0f);
}

#shader fragment
#version 460 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f);
}