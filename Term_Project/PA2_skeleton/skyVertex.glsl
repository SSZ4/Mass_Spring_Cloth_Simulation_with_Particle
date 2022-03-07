#version 330 core
float PI = 3.141592653589793238;
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

mat4 projection = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 0.0, 1.0));
//mat4 projection = mat4(vec4(cos(15 * PI / 180), sin(15 * PI / 180), 0.0, 0.0), vec4(-sin(15 * PI / 180), cos(15 * PI / 180), 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 0.0, 1.0));

mat4 view =  mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 0.0, 1.0));
//mat4 view = mat4(vec4(cos(15 * PI / 180), sin(15 * PI / 180), 0.0, 0.0), vec4(-sin(15 * PI / 180), cos(15 * PI / 180), 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 0.0, 1.0));


void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos.xy, 1.0, 1.0);
}  
