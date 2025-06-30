#version 330 core
in vec3 vWorldNormal;
flat in vec3 vIdx;
out vec4 FragColor;

uniform vec3 faceColors[6];

void main()
{
    vec3 n = normalize(vWorldNormal);
    vec3 col = vec3(0.05);

    if (n.y > 0.9)       col = faceColors[0];
    else if (n.y < -0.9) col = faceColors[1];
    else if (n.x < -0.9) col = faceColors[2];
    else if (n.x > 0.9)  col = faceColors[3];
    else if (n.z > 0.9)  col = faceColors[4];
    else if (n.z < -0.9) col = faceColors[5];

    FragColor = vec4(col, 1.0);
}
