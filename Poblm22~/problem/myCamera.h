#include "stdafx.h"

class MyCamera {
public:
	static int total;
	glm::vec3 Rot = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �⺻ ���⿡�� �󸶳� ȸ���� �ִ���
	glm::vec4 Pos = glm::vec4(0.0f, 15.0f, -15.0f, 1.0f); //--- ī�޶� ��ġ
	glm::vec4 Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //--- ī�޶� �ٶ󺸴� ����
	glm::vec4 Up = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); //--- ī�޶� ���� ����

};
int	MyCamera::total = 2;