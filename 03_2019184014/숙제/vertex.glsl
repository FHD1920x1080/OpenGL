#version 330 core

layout (location = 0) in vec3 vPos;   // ��ġ ������ attribute position 0�� �����ϴ�.
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 in_Color;

uniform mat4 modelTransform; //--- �𵨸� ��ȯ ���: uniform ������ ����

uniform mat4 viewTransform; //--- ī�޶� ��ȯ ���: uniform ������ ����
uniform mat4 projectionTransform;
out vec3 out_Color;

void main()
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0); //--- ��ǥ���� modelTransform ��ȯ�� �����Ѵ�.
	out_Color = in_Color;
}  
