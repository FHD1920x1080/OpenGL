#include "objRead.h"

class MyBox :public objRead {
public:
	bool face[6] = { 1,1,1,1,1,1 };//���� �����ִ���
	int vertex_size = 0;
	
	glm::vec3 Rot = glm::vec3(0.0f, 0.0f, 0.0f); //--- �⺻ ���⿡�� �󸶳� ȸ���� �ִ���
	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f); //--- ��ü ��ġ
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f); //--- �����
	glm::vec3 Max_Scale = glm::vec3(1.0f, 3.0f, 1.0f); //--- �����
	glm::vec3 Min_Scale = glm::vec3(1.0f, 0.1f, 1.0f); //--- �����
	float velocity = 0.1;
	int dir = 1;
	bool low_state = false;
	void updown() {
		if (dir > 0) {
			up_scale();
		}
		else {
			down_scale();
		}
	}
	void up_scale() {
		Scale.y += velocity;
		if (Scale.y > Max_Scale.y) {
			Scale.y = Max_Scale.y;
			dir = -1;
		}
	}
	void down_scale() {
		Scale.y -= velocity;
		if (Scale.y < Min_Scale.y) {
			Scale.y = Min_Scale.y;
			dir = 1;
		}
	}
	void update() {
		vertex_size = outvertex.size();
	}
	void show() {
		if (vertex_size == 0)
			update();
		glDrawArrays(GL_TRIANGLES, 0, vertex_size);
	}
	void show_bottom() {
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	void show_top() {
		glDrawArrays(GL_TRIANGLES, 6, 6);
	}
	void show_front() {
		glDrawArrays(GL_TRIANGLES, 12, 6);
	}
	void show_right() {
		glDrawArrays(GL_TRIANGLES, 18, 6);
	}
	void show_back() {
		glDrawArrays(GL_TRIANGLES, 24, 6);
	}
	void show_left() {
		glDrawArrays(GL_TRIANGLES, 30, 6);
	}
	void show_face(int a) {
		glDrawArrays(GL_TRIANGLES, (a)*6, 6);
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