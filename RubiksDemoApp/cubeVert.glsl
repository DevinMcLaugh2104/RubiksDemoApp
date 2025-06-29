#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vWorldNormal;
flat out vec3 vIdx;

uniform mat4 model, view, projection;
uniform vec3 cubieIndex;

void main()
{
    vWorldNormal = mat3(model) * aNormal;
    vIdx = cubieIndex;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
