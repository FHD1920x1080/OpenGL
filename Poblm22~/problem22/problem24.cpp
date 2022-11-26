// Reading Obj file
#include "stdafx.h"
#include "shader.h"
#include "myObj.h"
#include "myCamera.h"
#include "myLight.h"
#include <time.h>
#include <cmath>
#define botBox myBox[0]
#define midBox myBox[1]
#define topBox1 myBox[2]
#define topBox2 myBox[3]
using namespace std;

GLuint g_window_w = 800;
GLuint g_window_h = 800;
GLuint VAO[4];
GLuint VBO_pos[4];
GLuint VBO_norm[4];

MyObj originBox;
MyObj myBox[4];
MyObj pyramid;
MyObj light_ball;
MyLight light;

//bool mazeMap[max_h][max_w];
//MyBox maze3D.Map[max_h][max_w];
//int maze3D.h = 15;
//int maze3D.w = 15;
//GLuint VAO[25][25];
//GLuint VBO_position[25][25];
//GLuint VBO_color[25][25];

void drawScene();
void Reshape(int w, int h);
void KeyDown(unsigned char key, int x, int y);
void InitBuffer();
void randInitColor(MyObj* Obj);
void enter();
void reset();
void FPS100(int value);
//--- load obj related variabales
//objRead objReader;
//GLint Object = objReader.loadObj_normalize_center("mid_box.obj");

class UserInput {
public:
	bool left_down = false;
	float cursor_x = 0.0, cursor_y = 0.0;
	float cursor_x1 = 0.0, cursor_y1 = 0.0;
	float cursor_x2 = 0.0, cursor_y2 = 0.0;
};
UserInput userInput;

struct remote_control {
	bool depth_test = true;
	bool polygon_mode = true;
	bool perspective_projection = true;
	bool first_person = false;
	bool cube = true;
	bool light_on = true;
};

remote_control remocon;

float gravity_force = 0.2;
MyCamera myCamera[2];

//GLint Object = objReader.loadObj_normalize_center("pyra.obj");
//int Poly = 0;

typedef struct MyAnim {
	bool light_revolve = false;
};

MyAnim anim;

int main(int argc, char** argv)
{
	// create window using freeglut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_window_w, g_window_h);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Computer Graphics");
	//////////////////////////////////////////////////////////////////////////////////////
	//// initialize GLEW
	//////////////////////////////////////////////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//// Create shader program an register the shader
	//////////////////////////////////////////////////////////////////////////////////////

	GLuint vShader[4];
	GLuint fShader[4];

	vShader[0] = MakeVertexShader("vertex.glsl", 0);			// Sun
	fShader[0] = MakeFragmentShader("fragment.glsl", 0);

	// shader Program
	s_program[0] = glCreateProgram();
	glAttachShader(s_program[0], vShader[0]);
	glAttachShader(s_program[0], fShader[0]);
	glLinkProgram(s_program[0]);
	checkCompileErrors(s_program[0], "PROGRAM");

	enter();
	InitBuffer();

	// callback functions
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	//glutTimerFunc(10, gravity, 1);
	glutTimerFunc(10, FPS100, 1);
	//glutTimerFunc(10, anim_1, 1);
	// freeglut ������ �̺�Ʈ ó�� ����. �����찡 ���������� ���ѷ��� ����.
	glutMainLoop();

	return 0;
}

GLuint center_vao, center_vbo_pos;

GLfloat center_vPos[6][3] = {
	-1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, 1.0
};

GLuint floor_vao, floor_vbo_pos, floor_vbo_norm;

GLfloat floor_vPos[6][3] = {
	-2.0, -0.001, -2.0,
	-2.0, -0.001, 2.0,
	2.0, -0.001, -2.0,
	2.0, -0.001, -2.0,
	-2.0, -0.001, 2.0,
	2.0, -0.001, 2.0
};
GLfloat floor_vNomal[6][3] = {
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0
};

GLuint light_ball_vao, light_ball_vbo_pos;

unsigned int modelLocation;// �𵨺�ȯ ������
unsigned int modelRotateLocation;// �𵨺�ȯ ������
unsigned int viewLocation;// ī�޶� ��ȯ ������
unsigned int projectionLocation;// ���� ��ȯ ������

unsigned int objColorLocation; // ��ü ���� ������
unsigned int lightPosLocation; // ���� ��ġ ������
unsigned int lightColorLocation; // ���� ���� ������
unsigned int viewPosLocation; // ī�޶� ��ġ ������ - ���� �ݻ�, ���̶���Ʈ�� ǥ�� ���ֱ� ����.
void InitBuffer()
{
	GLint pAttribute = glGetAttribLocation(s_program[0], "vPos");
	GLint nAttribute = glGetAttribLocation(s_program[0], "vNormal");
	//// 5.1. VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &center_vao);
	glGenBuffers(1, &center_vbo_pos);
	glBindVertexArray(center_vao);

	glBindBuffer(GL_ARRAY_BUFFER, center_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), center_vPos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &floor_vao);
	glGenBuffers(1, &floor_vbo_pos);
	glGenBuffers(1, &floor_vbo_norm);
	glBindVertexArray(floor_vao);

	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), floor_vPos, GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo_norm);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), floor_vNomal, GL_STATIC_DRAW);
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glGenVertexArrays(4, VAO);
	glGenBuffers(4, VBO_pos);
	//glGenBuffers(4, VBO_normal);
	glGenBuffers(4, VBO_norm);
	// 2 triangles for quad floor
	for (int i = 0; i < 4; i++) {
		glBindVertexArray(VAO[i]);
		//---��ġ�Ӽ�
		glBindBuffer(GL_ARRAY_BUFFER, VBO_pos[i]);
		glBufferData(GL_ARRAY_BUFFER, myBox[i].outvertex.size() * sizeof(glm::vec3), &myBox[i].outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		//---����Ӽ�
		glBindBuffer(GL_ARRAY_BUFFER, VBO_norm[i]);
		glBufferData(GL_ARRAY_BUFFER, myBox[i].outnormal.size() * sizeof(glm::vec3), &myBox[i].outnormal[0], GL_STATIC_DRAW);

		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(nAttribute);
	}

	glGenVertexArrays(1, &light_ball_vao);
	glGenBuffers(1, &light_ball_vbo_pos);
	glBindVertexArray(light_ball_vao);
	glBindBuffer(GL_ARRAY_BUFFER, light_ball_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, light_ball.outvertex.size() * sizeof(glm::vec3), &light_ball.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glUseProgram(s_program[0]);
	objColorLocation = glGetUniformLocation(s_program[0], "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	viewPosLocation = glGetUniformLocation(s_program[0], "viewPos"); //--- viewPos �� ����: ī�޶� ��ġ

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");//--- �� ��ȯ ����
	modelRotateLocation = glGetUniformLocation(s_program[0], "modelRotation");
	viewLocation = glGetUniformLocation(s_program[0], "viewTransform"); //--- ���� ��ȯ ����
	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform"); //--- ���� ��ȯ �� ����
}
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
float Distance(MyObj ob1, MyObj ob2) {
	return sqrt(pow(ob2.Pos.x - ob1.Pos.x, 2) + pow(ob2.Pos.z - ob1.Pos.z, 2));
}
void drawWorld() {
	if (remocon.light_on)
		glUniform3f(lightColorLocation, light.Color.x, light.Color.y, light.Color.z); // ���� ��
	else
		glUniform3f(lightColorLocation, 0.0, 0.0, 0.0); // ���� ��
	glUniform3f(lightPosLocation, light.Pos.x, light.Pos.y, light.Pos.z); // ���� ��ġ
	glUniform3f(viewPosLocation, myCamera[0].Pos.x, myCamera[0].Pos.y, myCamera[0].Pos.z); // ī�޶� ��ġ

	glm::mat4 WorldConvert = glm::mat4(1.0f);//��ȯ��� �ʱ�ȭ
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(WorldConvert));
	glBindVertexArray(center_vao);//�߾� ��ǥ ǥ��
	glUniform3f(objColorLocation, 1.0, 0.0, 0.0);
	glDrawArrays(GL_LINES, 0, 2);
	glUniform3f(objColorLocation, 0.0, 1.0, 0.0);
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
	glDrawArrays(GL_LINES, 4, 2);

	glUniform3f(objColorLocation, 0.3, 0.2, 0.0);
	glBindVertexArray(floor_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(light_ball_vao);
	glUniform3f(objColorLocation, light_ball.Color.x, light_ball.Color.y, light_ball.Color.z); // ��ü ��
	glm::mat4 Light_Trans = glm::mat4(1.0f);//��ȯ��� �ʱ�ȭ
	Light_Trans = glm::translate(Light_Trans, glm::vec3(light.Pos.x, light.Pos.y, light.Pos.z));
	light_ball.show(&modelLocation, Light_Trans);

	glUniformMatrix4fv(modelRotateLocation, 1, GL_FALSE, glm::value_ptr(WorldConvert));
	for (int i = 0; i < 2; i++) {
		glBindVertexArray(VAO[i]);
		glUniform3f(objColorLocation, myBox[i].Color.x, myBox[i].Color.y, myBox[i].Color.z);
		glm::mat4 R0 = glm::mat4(1.0f);
		glm::mat4 T0 = glm::mat4(1.0f);
		glm::mat4 C0 = glm::mat4(1.0f);//��ȯ��� �ʱ�ȭ
		T0 = glm::translate(T0, glm::vec3(botBox.Pos.x, myBox[i].Pos.y, botBox.Pos.z));
		R0 = glm::rotate(R0, glm::radians(myBox[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
		C0 = T0 * R0;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(C0));

		myBox[i].show(&modelLocation, C0);
	}
	for (int i = 2; i < 4; i++) {
		glBindVertexArray(VAO[i]);
		glUniform3f(objColorLocation, myBox[i].Color.x, myBox[i].Color.y, myBox[i].Color.z);
		glm::mat4 R0 = glm::mat4(1.0f);
		glm::mat4 R1 = glm::mat4(1.0f);
		glm::mat4 T0 = glm::mat4(1.0f);
		glm::mat4 T1 = glm::mat4(1.0f);
		glm::mat4 C0 = glm::mat4(1.0f);//��ȯ��� �ʱ�ȭ
		T0 = glm::translate(T0, glm::vec3(myBox[i].Pos.x, myBox[i].Pos.y, myBox[i].Pos.z));
		T1 = glm::translate(T1, glm::vec3(botBox.Pos.x, 0.0f, botBox.Pos.z));
		R0 = glm::rotate(R0, glm::radians(myBox[i].Rot.x), glm::vec3(1.0, 0.0, 0.0));
		R0 = glm::rotate(R0, glm::radians(myBox[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
		R0 = glm::rotate(R0, glm::radians(myBox[i].Rot.z), glm::vec3(0.0, 0.0, 1.0));
		R1 = glm::rotate(R1, glm::radians(midBox.Rot.y), glm::vec3(0.0, 1.0, 0.0));
		C0 = T1 * R1 * T0 * R0;

		myBox[i].show(&modelLocation, C0);
	}
}

void MyViewport0() {
	glViewport(0, 0, g_window_w, g_window_h);

	//MyCamera[0].Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//ī�޶� ���� ����
	if (remocon.perspective_projection)
		projection = glm::perspective(glm::radians(45.0f), -1.0f, 0.1f, 200.0f);// �ǵ� �ΰ��� ī�޶�κ��� ����� ����� �Ÿ�
	else
		projection = glm::ortho(40.0f, -40.0f, -40.0f, 40.0f, 0.1f, 200.0f);

	view = glm::lookAt(glm::vec3(myCamera[0].Pos), glm::vec3(myCamera[0].Dir), glm::vec3(myCamera[0].Up));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	drawWorld();
}

void drawScene()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program[0]);

	if (remocon.polygon_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//��
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�ܰ���
	if (remocon.depth_test)
		glEnable(GL_DEPTH_TEST);//�����׽�Ʈ
	else
		glDisable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	MyViewport0();
	//MyViewport1();
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

void KeyDown(unsigned char key, int x, int y)
{
	switch (key) {
	case 'Q':
	case 'q':
		exit(0);
		break;
	case '1':
		light.Color = glm::vec3(1.0, 0.0, 0.0);
		break;
	case '2':
		light.Color = glm::vec3(0.0, 1.0, 0.0);
		break;
	case '3':
		light.Color = glm::vec3(0.0, 0.0, 1.0);
		break;
	case '`':
		light.Color = glm::vec3(1.0, 1.0, 1.0);
		break;
	case 'M':
	case 'm':
		if (remocon.light_on) {
			remocon.light_on = false;
			break;
		}
		remocon.light_on = true;
		break;
	case 'S':
	case 's':
		if (anim.light_revolve) {
			anim.light_revolve = false;
			break;
		}
		anim.light_revolve = true;
		break;
	case '\'':
		for (int i = 0; i < 1; i++) {
			glm::mat4 LR = glm::mat4(1.0f);
			glm::mat4 LT = glm::mat4(1.0f);
			glm::mat4 LRr = glm::mat4(1.0f);
			LR = glm::rotate(LR, glm::radians(-light.Rot.y), glm::vec3(0.0, 1.0, 0.0));
			LT = glm::translate(LT, glm::vec3(0.0, 0.0, 0.01));
			LRr = glm::rotate(LRr, glm::radians(light.Rot.y), glm::vec3(0.0, 1.0, 0.0));
			glm::mat4 LC = LRr * LT * LR;
			light.Pos = LC * light.Pos;
		}
		break;
	case '/':
		for (int i = 0; i < 1; i++) {
			glm::mat4 LR = glm::mat4(1.0f);
			glm::mat4 LT = glm::mat4(1.0f);
			glm::mat4 LRr = glm::mat4(1.0f);
			LR = glm::rotate(LR, glm::radians(-light.Rot.y), glm::vec3(0.0, 1.0, 0.0));
			LT = glm::translate(LT, glm::vec3(0.0, 0.0, -0.01));
			LRr = glm::rotate(LRr, glm::radians(light.Rot.y), glm::vec3(0.0, 1.0, 0.0));
			glm::mat4 LC = LRr * LT * LR;
			light.Pos = LC * light.Pos;
		}
		break;
	case 'Y':
	case 'y':
		originBox.Rot.y++;
		pyramid.Rot.y++;
		break;
	case 'R':
	case 'r':
		for (int i = 0; i < 1; i++) {
			glm::mat4 LT = glm::mat4(1.0f);
			glm::mat4 LR = glm::mat4(1.0f);
			glm::mat4 LTr = glm::mat4(1.0f);//��ȯ��� �ʱ�ȭ
			float Dx, Dy, Dz;
			Dx = originBox.Pos.x, Dy = originBox.Pos.y, Dz = originBox.Pos.z;
			LT = glm::translate(LT, glm::vec3(-Dx, -Dy, -Dz));
			LR = glm::rotate(LR, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
			light.Rot.y += 1.0;
			LTr = glm::translate(LTr, glm::vec3(Dx, Dy, Dz));
			glm::mat4 LC = LTr * LR * LT;
			light.Pos = LC * light.Pos;
		}
		break;
	case 'I':
	case 'i':
		if (remocon.polygon_mode == true)
			remocon.polygon_mode = false;
		else
			remocon.polygon_mode = true;
		break;
	case 'O':
	case 'o':
		if (remocon.depth_test == true)
			remocon.depth_test = false;
		else
			remocon.depth_test = true;
		break;
	case 'P':
	case 'p':
		if (remocon.perspective_projection == true)
			remocon.perspective_projection = false;
		else
			remocon.perspective_projection = true;
		break;
	case '[':
		glm::mat4 CR;
		glm::mat4 CT;
		glm::mat4 CC;
		glm::mat4 CTr;
		glm::mat4 CRr;
		float Px, Py, Pz;
		float Dx, Dy, Dz;
		for (int i = 0; i < 1; i++) {
			CR = glm::mat4(1.0f);
			CT = glm::mat4(1.0f);
			CC = glm::mat4(1.0f);
			CTr = glm::mat4(1.0f);//��ȯ��� �ʱ�ȭ
			Dx = myCamera[i].Dir.x, Dy = myCamera[i].Dir.y, Dz = myCamera[i].Dir.z;
			CT = glm::translate(CT, glm::vec3(-Dx, -Dy, -Dz));
			CR = glm::rotate(CR, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
			myCamera[i].Rot.y += 1.0;
			CTr = glm::translate(CTr, glm::vec3(Dx, Dy, Dz));
			CC = CTr * CR * CT;
			myCamera[i].Pos = CC * myCamera[i].Pos;
		}
		break;
	case ']':
		for (int i = 0; i < 1; i++) {
			CR = glm::mat4(1.0f);
			CT = glm::mat4(1.0f);
			CC = glm::mat4(1.0f);
			CTr = glm::mat4(1.0f);
			Dx = myCamera[i].Dir.x, Dy = myCamera[i].Dir.y, Dz = myCamera[i].Dir.z;
			CT = glm::translate(CT, glm::vec3(-Dx, -Dy, -Dz));
			CR = glm::rotate(CR, glm::radians(-1.0f), glm::vec3(0.0, 1.0, 0.0));
			myCamera[i].Rot.y -= 1.0;
			CTr = glm::translate(CTr, glm::vec3(Dx, Dy, Dz));
			CC = CTr * CR * CT;
			myCamera[i].Pos = CC * myCamera[i].Pos;
		}
		break;
	case '9':
		for (int i = 0; i < 1; i++) {
			CR = glm::mat4(1.0f);
			CT = glm::mat4(1.0f);
			CC = glm::mat4(1.0f);
			CTr = glm::mat4(1.0f);
			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
			CR = glm::rotate(CR, glm::radians(2.0f), glm::vec3(0.0, 1.0, 0.0));
			myCamera[i].Rot.y += 2.0;
			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
			CC = CTr * CR * CT;
			myCamera[i].Dir = CC * myCamera[i].Dir;
		}
		break;
	case '7':
		for (int i = 0; i < 1; i++) {
			CR = glm::mat4(1.0f);
			CT = glm::mat4(1.0f);
			CC = glm::mat4(1.0f);
			CTr = glm::mat4(1.0f);
			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
			CR = glm::rotate(CR, glm::radians(-2.0f), glm::vec3(0.0, 1.0, 0.0));
			myCamera[i].Rot.y -= 2.0;
			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
			CC = CTr * CR * CT;
			myCamera[i].Dir = CC * myCamera[i].Dir;
		}
		break;
	case '8':
		glm::mat4 CT1;
		for (int i = 0; i < 1; i++) {
			/*MyCamera[i].Pos.z += 0.1;
			MyCamera[i].Dir.z += 0.1;*/
			CT1 = glm::mat4(1.0f);
			CR = glm::mat4(1.0f);
			CT = glm::mat4(1.0f);
			CC = glm::mat4(1.0f);
			CTr = glm::mat4(1.0f);
			CRr = glm::mat4(1.0f);
			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
			CR = glm::rotate(CR, glm::radians(-myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
			CT1 = glm::translate(CT1, glm::vec3(0.0f, 0.0f, 0.1f));
			CRr = glm::rotate(CRr, glm::radians(myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
			CC = CTr * CRr * CT1 * CR * CT;
			myCamera[i].Dir = CC * myCamera[i].Dir;
			myCamera[i].Pos = CC * myCamera[i].Pos;
		}
		break;
	case '5':
		for (int i = 0; i < 1; i++) {
			CT1 = glm::mat4(1.0f);
			CR = glm::mat4(1.0f);
			CT = glm::mat4(1.0f);
			CC = glm::mat4(1.0f);
			CTr = glm::mat4(1.0f);
			CRr = glm::mat4(1.0f);
			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
			CR = glm::rotate(CR, glm::radians(-myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
			CT1 = glm::translate(CT1, glm::vec3(0.0f, 0.0f, -0.1f));
			CRr = glm::rotate(CRr, glm::radians(myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
			CC = CTr * CRr * CT1 * CR * CT;
			myCamera[i].Dir = CC * myCamera[i].Dir;
			myCamera[i].Pos = CC * myCamera[i].Pos;
		}
		break;
	case '4':
		for (int i = 0; i < 1; i++) {
			CT1 = glm::mat4(1.0f);
			CR = glm::mat4(1.0f);
			CT = glm::mat4(1.0f);
			CC = glm::mat4(1.0f);
			CTr = glm::mat4(1.0f);
			CRr = glm::mat4(1.0f);
			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
			CR = glm::rotate(CR, glm::radians(-myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
			CT1 = glm::translate(CT1, glm::vec3(-0.1f, 0.0f, 0.0f));
			CRr = glm::rotate(CRr, glm::radians(myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
			CC = CTr * CRr * CT1 * CR * CT;
			myCamera[i].Dir = CC * myCamera[i].Dir;
			myCamera[i].Pos = CC * myCamera[i].Pos;
		}
		break;
	case '6':
		for (int i = 0; i < 1; i++) {
			CT1 = glm::mat4(1.0f);
			CR = glm::mat4(1.0f);
			CT = glm::mat4(1.0f);
			CC = glm::mat4(1.0f);
			CTr = glm::mat4(1.0f);
			CRr = glm::mat4(1.0f);
			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
			CR = glm::rotate(CR, glm::radians(-myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
			CT1 = glm::translate(CT1, glm::vec3(0.1f, 0.0f, 0.0f));
			CRr = glm::rotate(CRr, glm::radians(myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
			CC = CTr * CRr * CT1 * CR * CT;
			myCamera[i].Dir = CC * myCamera[i].Dir;
			myCamera[i].Pos = CC * myCamera[i].Pos;
		}
		break;
	}
}

void anim_play() {
	if (anim.light_revolve) {
		glm::mat4 LT = glm::mat4(1.0f);
		glm::mat4 LR = glm::mat4(1.0f);
		glm::mat4 LTr = glm::mat4(1.0f);//��ȯ��� �ʱ�ȭ
		float Dx, Dy, Dz;
		Dx = originBox.Pos.x, Dy = originBox.Pos.y, Dz = originBox.Pos.z;
		LT = glm::translate(LT, glm::vec3(-Dx, -Dy, -Dz));
		LR = glm::rotate(LR, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
		light.Rot.y += 1.0;
		LTr = glm::translate(LTr, glm::vec3(Dx, Dy, Dz));
		glm::mat4 LC = LTr * LR * LT;
		light.Pos = LC * light.Pos;
	}

}

void FPS100(int value) {
	anim_play();


	glutPostRedisplay();
	glutTimerFunc(10, FPS100, 1);
}

void enter() {
	srand((unsigned int)time(NULL));
	light_ball.loadObj_normalize_center("sphere_center.obj");
	light_ball.Scale = glm::vec3(0.3, 0.3, 0.3);
	light_ball.Color = glm::vec3(1.0, 1.0, 1.0);
	light.Pos = glm::vec4(0.0, 1.0, -2.0, 1.0);
	originBox.loadObj_normalize_center("cube_ground.obj");
	for (int i = 0; i < 4; i++) {
		myBox[i] = originBox;
	}
	botBox.Scale = glm::vec3(0.6, 0.2, 0.6);
	midBox.Scale = glm::vec3(0.3, 0.1, 0.3);
	topBox1.Scale = glm::vec3(0.07, 0.2, 0.07);
	topBox2.Scale = glm::vec3(0.07, 0.2, 0.07);
	midBox.Pos.y = botBox.Scale.y;
	topBox1.Pos.y = midBox.Pos.y + midBox.Scale.y;
	topBox2.Pos.y = topBox1.Pos.y;
	topBox1.Pos.x -= 0.08;
	topBox2.Pos.x += 0.08;
	botBox.Color = glm::vec3(1.0, 0.0, 0.0);
	midBox.Color = glm::vec3(0.0, 1.0, 0.0);
	topBox1.Color = glm::vec3(0.0, 0.0, 1.0);
	topBox2.Color = glm::vec3(0.0, 0.0, 1.0);
	myCamera[0].Pos = glm::vec4(0.0, 2.0, -5.0, 1.0);
}
void reset() {

}

void randInitColor(MyObj* Obj) {
	glm::vec3 input;
	double max = 32767;
	input[0] = rand() / max;
	input[1] = rand() / max;
	input[2] = rand() / max;
	Obj->Color = input;
}