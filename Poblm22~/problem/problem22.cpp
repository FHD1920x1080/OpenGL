// Reading Obj file
#include "stdafx.h"
#include "shader.h"
#include "myMaze.h"
#include <time.h>

using namespace std;

GLuint g_window_w = 800;
GLuint g_window_h = 800;
GLuint VAO[4];
GLuint VBO[4];

MyBox originBox;

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
void KeyUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void InitBuffer();
void inputColor(MyBox* Obj);
void enter();
void reset();
void FPS100(int value);

float* cheak_collision(float x, float y, float z, MyBox Rect);
bool check_collision_min_move(MyRobot* unit1, MyBox* unit2);
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
};

remote_control remocon;

int face_dir;
int North = 0;
int East = 1;
int South = 2;
int West = 3;
float Pos_x = 0.0, Pos_y = 0.0, Pos_z = 0.0;
float fall_speed = 0;
float gravity_force = 0.2;
MyCamera myCamera[2];

//GLint Object = objReader.loadObj_normalize_center("pyra.obj");
//int Poly = 0;

typedef struct MyMode {
}MyMode;

MyMode Anim;

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
	//glutTimerFunc(10, FPS100, 1);
	//glutTimerFunc(10, anim_1, 1);
	// freeglut 윈도우 이벤트 처리 시작. 윈도우가 닫힐때까지 후한루프 실행.
	glutMainLoop();

	return 0;
}

GLuint center_vao, center_vbo_pos, center_vbo_color;

GLfloat center_vPos[6][3] = {
	-1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, 1.0
};

GLfloat center_vColor[6][3] = {
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0
};

GLuint floor_vao, floor_vbo_pos, floor_vbo_color;

GLfloat floor_vPos[6][3] = {
	-200.0, -0.001, -200.0,
	-200.0, -0.001, 200.0,
	200.0, -0.001, -200.0,
	200.0, -0.001, -200.0,
	-200.0, -0.001, 200.0,
	200.0, -0.001, 200.0
};

GLfloat floor_vColor[6][3] = {
	0.3, 0.3, 0.3,
	0.3, 0.3, 0.3,
	0.3, 0.3, 0.3,
	0.3, 0.3, 0.3,
	0.3, 0.3, 0.3,
	0.3, 0.3, 0.3
};

GLuint stone_vao, stone_vbo_pos, stone_vbo_color;
MyBox stone;

GLuint room_vao, room_vbo_pos, room_vbo_color;
MyBox room;

void InitBuffer()
{

	//// 5.1. VAO 객체 생성 및 바인딩
	/*glGenVertexArrays(1, &center_vao);
	glGenBuffers(1, &center_vbo_pos);
	glGenBuffers(1, &center_vbo_color);
	glBindVertexArray(center_vao);

	glBindBuffer(GL_ARRAY_BUFFER, center_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), center_vPos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, center_vbo_color);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), center_vColor, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &floor_vao);
	glGenBuffers(1, &floor_vbo_pos);
	glGenBuffers(1, &floor_vbo_color);
	glBindVertexArray(floor_vao);

	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), floor_vPos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo_color);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), floor_vColor, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);
	*/

	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);

	GLint pAttribute;
	GLint nAttribute;
	glBindVertexArray(VAO[0]);

	//---위치속성
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, originBox.outvertex.size() * sizeof(glm::vec3), &originBox.outvertex[0], GL_STATIC_DRAW);

	pAttribute = glGetAttribLocation(s_program[0], "vPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	nAttribute = glGetAttribLocation(s_program[0], "vNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(nAttribute);

	glUseProgram(s_program[0]);
	unsigned int objColorLocation = glGetUniformLocation(s_program[0], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 0.3);
	unsigned int lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 0.5, -5.0);
	unsigned int lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);



}
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projectionLocation;

void drawWorld() {
	unsigned int viewPosLocation = glGetUniformLocation(s_program[0], "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, myCamera[0].Pos.x, myCamera[0].Pos.y, myCamera[0].Pos.z);
	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");//--- 모델 변환 설정
	glm::mat4 WC = glm::mat4(1.0f);//변환행렬 초기화

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(WC));
	//glBindVertexArray(center_vao);//중앙 좌표 표시
	//glDrawArrays(GL_LINES, 0, 2);
	//glDrawArrays(GL_LINES, 2, 2);
	//glDrawArrays(GL_LINES, 4, 2);
	//glBindVertexArray(floor_vao);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(VAO[0]);
	glm::mat4 Trans = glm::mat4(1.0f);//변환행렬 초기화
	Trans = glm::translate(Trans, glm::vec3(1.0f, 1.0f, 0.0));
	originBox.show(&modelLocation, Trans);

}

void MyViewport0() {
	glViewport(0, 0, g_window_w, g_window_h);

	//MyCamera[0].Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//카메라 보는 방향
	if (remocon.perspective_projection)
		projection = glm::perspective(glm::radians(45.0f), -1.0f, 0.1f, 200.0f);// 맨뒤 두개는 카메라로부터 근평면 원평면 거리
	else
		projection = glm::ortho(80.0f, -80.0f, -40.0f, 40.0f, 0.1f, 200.0f);

	view = glm::lookAt(glm::vec3(myCamera[0].Pos), glm::vec3(myCamera[0].Dir), glm::vec3(myCamera[0].Up));
	viewLocation = glGetUniformLocation(s_program[0], "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	drawWorld();
}

void drawScene()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program[0]);

	if (remocon.polygon_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//면
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//외곽선
	if (remocon.depth_test)
		glEnable(GL_DEPTH_TEST);//깊이테스트
	else
		glDisable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

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
		remocon.first_person = true;
		break;
	case '3':
		remocon.first_person = false;
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
			CTr = glm::mat4(1.0f);//변환행렬 초기화
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
			CT1 = glm::translate(CT1, glm::vec3(0.0f, 0.0f, 0.5f));
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
			CT1 = glm::translate(CT1, glm::vec3(0.0f, 0.0f, -0.5f));
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
			CT1 = glm::translate(CT1, glm::vec3(-0.5f, 0.0f, 0.0f));
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
			CT1 = glm::translate(CT1, glm::vec3(0.5f, 0.0f, 0.0f));
			CRr = glm::rotate(CRr, glm::radians(myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
			CC = CTr * CRr * CT1 * CR * CT;
			myCamera[i].Dir = CC * myCamera[i].Dir;
			myCamera[i].Pos = CC * myCamera[i].Pos;
		}
		break;
	}
	glutPostRedisplay();
}

void anim_play() {


}

void FPS100(int value) {
	anim_play();
	

	glutPostRedisplay();
	glutTimerFunc(10, FPS100, 1);
}

void enter() {
	srand((unsigned int)time(NULL));
	originBox.loadObj_normalize_center("1x1cube.obj");
	inputColor(&originBox);
}

void reset() {

}

void inputColor(MyBox* Obj) {
	glm::vec3 input;
	double max = 32767;
	input[0] = rand() / max;
	input[1] = rand() / max;
	input[2] = rand() / max;
	input[0] += 0.6;
	if (input[0] > 1.0)
		input[0] = 1.0;
	int size = Obj->vertexIndices.size();
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			Obj->outcolor.push_back(input);
		}
		input[0] -= 0.1;
	}

}