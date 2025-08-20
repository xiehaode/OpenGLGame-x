#version 330 core
out vec4 FragColor;

uniform vec3 bulletColor; // 子弹颜色变量

void main()
{
    FragColor = vec4(bulletColor, 1.0); // 使用纯色渲染
}
    