#version 410

in vec4 vPosition;

out vec4 color;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 ColorUpdater;

void main()
{
    gl_Position =Projection * ModelView * vPosition;
    color = ColorUpdater;
}
