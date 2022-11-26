#include "stdafx.h"

class MyLight {
public:
	static int total;
	glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 Pos = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec4 Rot = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);//공전을 위한 회전 벡터
};