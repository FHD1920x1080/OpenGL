#include "objRead.h"

class MyObj :public objRead {
public:
	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f); //--- ��ü ��ġ
	glm::vec3 Rot = glm::vec3(0.0f, 0.0f, 0.0f); //--- �⺻ ���⿡�� �󸶳� ȸ���� �ִ���
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f); //--- �����
	glm::vec3 Color = glm::vec3(0.0f, 0.0f, 0.0f); // �÷�

	void show(unsigned int* modelLocation, glm::mat4 Convert = glm::mat4(1.0f)) {
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