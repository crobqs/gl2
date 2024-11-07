#shader vertex
#version 460 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexcoord;
out vec2 texCoord;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0f);
    texCoord = aTexcoord;
}

#shader fragment
#version 460 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, texCoord);
}