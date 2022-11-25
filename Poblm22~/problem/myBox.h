#include "objRead.h"
#include "stdafx.h"
class MyBox :public objRead {
public:
	bool face[6] = { 1,1,1,1,1,1 };//���� �����ִ���
	int vertex_size = 0;

	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f); //--- ��ü ��ġ
	glm::vec3 Rot = glm::vec3(0.0f, 0.0f, 0.0f); //--- �⺻ ���⿡�� �󸶳� ȸ���� �ִ���
	glm::vec3 Scale = glm::vec3(5.0f, 5.0f, 5.0f); //--- �����

	void update() {
		vertex_size = outvertex.size();
	}
	void show(unsigned int* modelLocation, glm::mat4 Convert = glm::mat4(1.0f)) {
		if (vertex_size == 0)
			update();
		glm::mat4 result = glm::scale(Convert, glm::vec3(Scale.x, Scale.y, Scale.z));
		glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(result));
		glDrawArrays(GL_TRIANGLES, 0, vertex_size);
	}

	float left() {
		return Pos.x - scaleX * Scale.x / 2;
	}
	float right() {
		return Pos.x + scaleX * Scale.x / 2;
	}
	float back() {
		return Pos.z - scaleZ * Scale.z / 2;
	}
	float front() {
		return Pos.z + scaleZ * Scale.z / 2;
	}
	float top() {
		return Pos.y + scaleY * Scale.y;
	}
	float bottom() {
		return Pos.y;
	}
};