// Reading Obj file
#include "stdafx.h"
#include "shader.h"
#include "myMaze.h"
#include <time.h>

using namespace std;

GLuint g_window_w = 1600;
GLuint g_window_h = 800;
GLuint VAO[4];
GLuint VBO_position[4];
GLuint VBO_normal[4];
GLuint VBO_color[4];

MyBox originBox;
MyBox meteorBox;
#define max_h 25
#define max_w 25
//bool mazeMap[max_h][max_w];
//MyBox maze3D.Map[max_h][max_w];
MazeMap3D maze3D;
MazeMap maze2D;
MyRobot myRobot;
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
void MousePassiveDrag(int x, int y);
void MouseDrag(int x, int y);
void InitBuffer();
void inputColor(MyBox* Obj);
void enter();
void reset();
void FPS100(int value);
void gravity(int value);
void jump(int value);

void walk();
void open_back(int value);
void open_side(int value);
void updown_anim(int value);
void meteor(int value);
void make_maze(int value);
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
	bool updown_anim = 0;
	bool meteor = 0;
	bool jump = 0;
	bool make_maze = 0;
	bool low_height = 0;
}MyMode;

MyMode Anim;

int main(int argc, char** argv)
{	
	enter();
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

	
	InitBuffer();

	// callback functions
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseDrag);
	//glutTimerFunc(10, gravity, 1);
	glutTimerFunc(10, FPS100, 1);
	//glutTimerFunc(10, anim_1, 1);
	// freeglut ������ �̺�Ʈ ó�� ����. �����찡 ���������� ���ѷ��� ����.
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
	//0.0, 0.7, 0.0,
	//0.7, 0.0, 0.0,
	//0.0, 0.0, 0.7,
	//0.0, 0.0, 0.7,
	//0.7, 0.0, 0.0,
	//0.7, 0.7, 0.0
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
	//// 5.1. VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &center_vao);
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

	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO_position);
	glGenBuffers(1, VBO_color);
	GLint pAttribute;
	GLint cAttribute;
	glBindVertexArray(VAO[0]);
	//---��ġ�Ӽ�
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position[0]);
	glBufferData(GL_ARRAY_BUFFER, originBox.outvertex.size() * sizeof(glm::vec3), &originBox.outvertex[0], GL_STATIC_DRAW);

	pAttribute = glGetAttribLocation(s_program[0], "vPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	//---����Ӽ�
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color[0]);
	glBufferData(GL_ARRAY_BUFFER, originBox.outvertex.size() * sizeof(glm::vec3), &originBox.outcolor[0], GL_STATIC_DRAW);

	cAttribute = glGetAttribLocation(s_program[0], "in_Color");
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);

	myRobot.initBuffer(s_program[0]);

}
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projectionLocation;

void drawWorld() {

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");//--- �� ��ȯ ����
	glm::mat4 WC = glm::mat4(1.0f);//��ȯ��� �ʱ�ȭ

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(WC));
	glBindVertexArray(center_vao);//�߾� ��ǥ ǥ��

	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);
	glDrawArrays(GL_LINES, 4, 2);
	glBindVertexArray(floor_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//stone.draw();
	glm::mat4 Dir;
	glm::mat4 Pos;
	glm::mat4 Scale = glm::mat4(1.0f); //--- ���� ��ķ� �ʱ�ȭ
	glm::mat4 Init = glm::mat4(1.0f);
	glm::mat4 Convert = glm::mat4(1.0f);
	Scale = glm::scale(Scale, glm::vec3(meteorBox.Scale.x, meteorBox.Scale.y, meteorBox.Scale.z));
	Init = glm::translate(Init, glm::vec3(meteorBox.Pos.x, meteorBox.Pos.y, meteorBox.Pos.z));
	Convert = Init * Scale;
	glBindVertexArray(VAO[0]);
	meteorBox.show(&modelLocation, Init);
	for (int i = 0; i < maze3D.h; i++) {
		for (int j = 0; j < maze3D.w; j++) {
			Init = glm::mat4(1.0f);
			Scale = glm::mat4(1.0f);
			Convert = glm::mat4(1.0f);//��ȯ��� �ʱ�ȭ
			Scale = glm::scale(Scale, glm::vec3(maze3D.Map[i][j].Scale.x, maze3D.Map[i][j].Scale.y, maze3D.Map[i][j].Scale.z));
			Init = glm::translate(Init, glm::vec3(maze3D.Map[i][j].Pos.x, maze3D.Map[i][j].Pos.y, maze3D.Map[i][j].Pos.z));
			Convert = Init * Scale;
			maze3D.Map[i][j].show(&modelLocation, Init);
		}
	}
	myRobot.show(&modelLocation);
}

void MyViewport0() {
	glViewport(0, 0, g_window_w, g_window_h);

	//MyCamera[0].Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//ī�޶� ���� ����
	if (remocon.first_person) {
		projection = glm::perspective(glm::radians(45.0f), -2.0f, 0.1f, 200.0f);// �ǵ� �ΰ��� ī�޶�κ��� ����� ����� �Ÿ�
		view = glm::lookAt(glm::vec3(myRobot.Camera[0].Pos), glm::vec3(myRobot.Camera[0].Dir), glm::vec3(myRobot.Camera[0].Up));
	}
	else {
		if (remocon.perspective_projection)
			projection = glm::perspective(glm::radians(45.0f), -2.0f, 0.1f, 200.0f);// �ǵ� �ΰ��� ī�޶�κ��� ����� ����� �Ÿ�
		else
			projection = glm::ortho(80.0f, -80.0f, -40.0f, 40.0f, 0.1f, 200.0f);
		view = glm::lookAt(glm::vec3(myCamera[0].Pos), glm::vec3(myCamera[0].Dir), glm::vec3(myCamera[0].Up));
	}
	viewLocation = glGetUniformLocation(s_program[0], "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	drawWorld();
}
void MyViewport1() {
	glViewport(g_window_w * 3 / 4, g_window_h / 2, g_window_w / 4, g_window_h / 2);
	//if (remocon.perspective_projection) {//���� ����
	//	projection = glm::perspective(glm::radians(45.0f), -1.0f, 0.1f, 200.0f);// �ǵ� �ΰ��� ī�޶�κ��� ����� ����� �Ÿ�
	//	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));
	//}
	//else//����(����,����) ����
		projection = glm::ortho(22.0f, -22.0f, -22.0f, 22.0f, 0.1f, 200.0f);

	//MyCamera[0].Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//ī�޶� ���� ����
	view = glm::lookAt(glm::vec3(myCamera[1].Pos), glm::vec3(myCamera[1].Dir), glm::vec3(myCamera[1].Up));

	viewLocation = glGetUniformLocation(s_program[0], "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	drawWorld();
}
void drawScene()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
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

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	MyViewport0();
	MyViewport1();
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
		maze2D.delete_Map();
		maze3D.delete_Map();
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
	case 'M':
	case 'm':
		if (Anim.updown_anim == 1) {
			Anim.updown_anim = 0;
			break;
		}
		if (maze3D.low_state == 1) {
			maze3D.low_state = 0;
			for (int i = 0; i < maze3D.h; i++) {
				for (int j = 0; j < maze3D.w; j++) {
					maze3D.Map[i][j].dir = 1;
				}
			}
		}
		Anim.updown_anim = 1;
		maze3D.low_state = 0;
		break;
	case '=':
	case '+':
		for (int i = 0; i < maze3D.h; i++) {
			for (int j = 0; j < maze3D.w; j++) {
				maze3D.Map[i][j].velocity *= 1.1;
			}
		}
		break;
	case '-':
		for (int i = 0; i < maze3D.h; i++) {
			for (int j = 0; j < maze3D.w; j++) {
				maze3D.Map[i][j].velocity *= 0.9;
			}
		}
		break;
	case 'N':
	case 'n':
		if (Anim.low_height == 1) {
			Anim.low_height = 0;
			break;
		}
		if (Anim.updown_anim == 1)
			Anim.updown_anim = 0;
		Anim.low_height = 1;
		for (int i = 0; i < maze3D.h; i++) {
			for (int j = 0; j < maze3D.w; j++) {
				maze3D.Map[i][j].low_state = 0;
			}
		}
		maze3D.low_state = 1;
		break;
	case 'V':
	case 'v':
		if (Anim.meteor == 1) {
			Anim.meteor = 0;
			break;
		}
		meteorBox.Pos.y = 0;
		Anim.meteor = 1;
		//glutTimerFunc(10, meteor, 1);
		break;
	case 'B':
	case 'b':
		if (Anim.make_maze == 1) {
			Anim.make_maze = 0;
			break;
		}
		maze3D.down_maze_intensity = maze3D.max_height;
		for (int i = 0; i < maze3D.h; i++) {
			for (int j = 0; j < maze3D.w; j++) {
				maze3D.Map[i][j].Pos.y = 0.0;
			}
		}
		Anim.make_maze = 1;
		//glutTimerFunc(10, make_maze, 1);
		break;
	case 'L':
	case 'l':
		reset();
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
	//myRobot.Keyboard(key);
	switch (key) {
	case 'W':
	case 'w':
		myRobot.input.W_Down = true;
		break;
	case 'A':
	case 'a':
		myRobot.input.A_Down = true;
		break;
	case 'S':
	case 's':
		myRobot.input.S_Down = true;
		break;
	case 'D':
	case 'd':
		myRobot.input.D_Down = true;
		break;
	case 'Z':
	case 'z':
		myRobot.Rot.y--;
		break;
	case 'C':
	case 'c':
		myRobot.Rot.y++;
		break;
	case ' ':
		myRobot.input.Space_Down = true;
		break;
	}
	glutPostRedisplay();
}
void KeyUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'W':
	case 'w':
		myRobot.input.W_Down = false;
		break;
	case 'A':
	case 'a':
		myRobot.input.A_Down = false;
		break;
	case 'S':
	case 's':
		myRobot.input.S_Down = false;
		break;
	case 'D':
	case 'd':
		myRobot.input.D_Down = false;
		break;
	case ' ':
		myRobot.input.Space_Down = false;
		break;
	}
	glutPostRedisplay();
}
float rot_face_y = 0.0;
float rot_face_x = 0.0;
void Mouse(int button, int state, int x, int y) {
	userInput.cursor_x = x;
	userInput.cursor_y = y;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		userInput.left_down = true;
		userInput.cursor_x1 = userInput.cursor_x;
		userInput.cursor_y1 = userInput.cursor_y;
		rot_face_y = myRobot.Rot.y;
		rot_face_x = myRobot.Camera[0].Rot.x;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		userInput.left_down = false;
	}
}

void MousePassiveDrag(int x, int y) {

}
void MouseDrag(int x, int y) {
	if (userInput.left_down) {
		userInput.cursor_x2 = x;
		userInput.cursor_y2 = y;
		myRobot.Rot.y = rot_face_y + (userInput.cursor_x2 - userInput.cursor_x1) * 0.2;
		myRobot.Camera[0].Rot.x = rot_face_x + (userInput.cursor_y2 - userInput.cursor_y1) * 0.2;
		if (myRobot.Camera[0].Rot.x > 85)
			myRobot.Camera[0].Rot.x = 85;
		if (myRobot.Camera[0].Rot.x < -85)
			myRobot.Camera[0].Rot.x = -85;
		glutPostRedisplay();
	}
}

void anim_play() {
	if (Anim.updown_anim)
		for (int i = 0; i < maze3D.h; i++) {
			for (int j = 0; j < maze3D.w; j++) {
				maze3D.Map[i][j].updown();
			}
		}

	if (Anim.meteor) {
		meteorBox.Pos.y -= 0.05;
		if (meteorBox.top() < 0) {
			meteorBox.Pos.y = -meteorBox.Scale.y - 1.0;
			Anim.meteor = 0;
		}
	}
	if (Anim.make_maze) {
		maze3D.down_maze_intensity -= 0.05;
		for (int i = 0; i < maze2D.h; i++) {
			for (int j = 0; j < maze2D.w; j++) {
				if (maze2D.Map[i][j] == 0) {
					maze3D.Map[i][j].Pos.y -= 0.05;
				}
			}
		}
		if (maze3D.down_maze_intensity < 0) {
			Anim.make_maze = 0;
		}
	}

	if (Anim.low_height) {
		int n = 0;
		for (int i = 0; i < maze3D.h; i++) {
			for (int j = 0; j < maze3D.w; j++) {
				if (maze3D.Map[i][j].low_state == false) {
					n++;
					maze3D.Map[i][j].Scale.y -= maze3D.Map[i][j].velocity;
					if (maze3D.Map[i][j].Scale.y < maze3D.min_height) {
						maze3D.Map[i][j].Scale.y = maze3D.min_height;
						maze3D.Map[i][j].low_state = true;
					}
				}
			}
		}
		if (n == 0) {
			Anim.low_height = 0;
		}
	}
}

void FPS100(int value) {
	anim_play();
	
	myRobot.update(gravity_force);
	check_collision_min_move(&myRobot, &meteorBox);

	int start_z = abs(int((myRobot.Pos.z - 20) * 0.025 * maze3D.h)) - 1;//(myRobot.Pos.x + 20) / 40 * maze3D.w -1
	int end_z = start_z + 3; // ��ŸƮ�� ���� ��ǥ���� �ϳ� ������, ����� �ϳ� ū�� �� 3�� ��
	if (start_z < 0) {
		start_z = 0;
		end_z = 1;
	}
	else if (end_z > maze3D.h) {
		end_z = maze3D.h;
		start_z = end_z - 1;
	}
	for (int i = start_z; i < end_z; i++) {
		int start_x = int((myRobot.Pos.x + 20) * 0.025 * maze3D.w) - 1;//(myRobot.Pos.x + 20) / 40 * maze3D.w -1
		int end_x = start_x + 3; // ��ŸƮ�� ���� ��ǥ���� �ϳ� ������, ����� �ϳ� ū�� �� 3�� ��
		if (start_x < 0) {
			start_x = 0;
			end_x = 1;
		}
		else if (end_x > maze3D.w) {
			end_x = maze3D.w;
			start_x = end_x - 1;
		}
		for (int j = start_x; j < end_x; j++)
			check_collision_min_move(&myRobot, &maze3D.Map[i][j]);
	}

	if (myRobot.Pos.y < 0) {//�ٴڸ�
		myRobot.Pos.y = 0.0;
		myRobot.fall_speed = 0.08;
	}
	glutPostRedisplay();
	glutTimerFunc(10, FPS100, 1);
}

	int input_x, input_y;
void enter() {
	srand((unsigned int)time(NULL));
	originBox.loadObj_normalize_center("1x1cube.obj");
	inputColor(&originBox);
	meteorBox = originBox;
	meteorBox.Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	double max = 32767;

	printf("77���� ũ�� �̷ΰ� ������ �����ϳ� �׶��ؼ� �� ���� ����.\n");
	printf("���� ���Ǽ� : ");
	scanf_s("%d", &input_x);
	printf("���� ���Ǽ� : ");
	scanf_s("%d", &input_y); 
	printf("\nQ : ���α׷� ����\n");
	printf("1, 3 : ī�޶� ����\n");
	printf("W, A, S, D : �̵�\n");
	printf("Space_bar : ����\n");
	printf("���콺 ��Ŭ�� �巡�� : ��ü ȸ��, 1��Ī ī�޶� ȸ��\n\n");

	printf("[, ] : ī�޶� ����(3��Ī ī�޶�)\n");
	printf("8, 4, 5, 6 : ī�޶� �̵�(3��Ī ī�޶�, Ű���� ������ ��ȣ�� ��Ű ��� ����)\n");
	printf("7, 9 : ī�޶� ����(3��Ī ī�޶�)\n\n");

	printf("V : ��� ����\n");
	printf("B : �̷� ����\n");
	printf("N : ���� ���� �ִϸ��̼�\n");
	printf("M : ���Ʒ� �ִϸ��̼�\n");
	printf("+, - : �ִϸ��̼� �ӵ� ����\n\n");

	printf("I : ������ ���(��, ��)\n");
	printf("O : ���� �׽�Ʈ\n");
	printf("P : ����(����, ����)\n");
	printf("L : �ʱ�ȭ �� �̷� �����\n\n");
	myRobot.init();
	for (int i = 0; i < myRobot.parts; i++) {
		myRobot.box[i].update();
		inputColor(&myRobot.box[i]);
	}
	maze3D.init(input_y, input_x);
	maze2D.init(input_y, input_x);
	float max_height = 0.0;
	for (int i = 0; i < maze3D.h; i++) {
		for (int j = 0; j < maze3D.w; j++) {
			maze3D.Map[i][j] = originBox;
			maze3D.Map[i][j].Scale.x = 40.0 / maze3D.w;
			maze3D.Map[i][j].Pos.x = (-20 + maze3D.Map[i][j].Scale.x / 2.0) + j * maze3D.Map[i][j].Scale.x;
			
			maze3D.Map[i][j].Scale.z = 40.0 / maze3D.h;
			maze3D.Map[i][j].Pos.z = (20 - maze3D.Map[i][j].Scale.z / 2.0) - i * maze3D.Map[i][j].Scale.z;

			maze3D.Map[i][j].Max_Scale.y = (rand() / max) * 8 + 7; //0~8 7~15
			if (max_height < maze3D.Map[i][j].Max_Scale.y)
				max_height = maze3D.Map[i][j].Max_Scale.y;
			maze3D.Map[i][j].Min_Scale.y = (rand() / max) * 2 + 4; //0~2 4~6

			if (rand() % 2 == 0) {
				maze3D.Map[i][j].Scale.y = maze3D.Map[i][j].Max_Scale.y;
				maze3D.Map[i][j].dir = - 1;
			}
			else {
				maze3D.Map[i][j].Scale.y = maze3D.Map[i][j].Min_Scale.y;
				maze3D.Map[i][j].dir = 1;
			}
			maze3D.Map[i][j].velocity = (rand() / (max * 20)) + 0.025; //0.025~0.075
		}
	}

	maze3D.max_height = max_height;
	maze3D.min_height = 4.0;
	maze3D.down_maze_intensity = maze3D.max_height;
	meteorBox.Scale = glm::vec3(40.0, maze3D.max_height +1.0, 40.0);

	myCamera[0].Pos = glm::vec4(0.0f, 80.0f, -40.0f, 1.0f); //--- ī�޶� ��ġ
	myCamera[0].Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //--- ī�޶� �ٶ󺸴� ��
	myCamera[0].Rot = glm::vec3(0.0f, 0.0f, 0.0f); //--- ȸ�� ����

	myCamera[1].Pos = glm::vec4(0.0f, 60.0f, -0.001f, 1.0f); //--- ī�޶� ��ġ
	myCamera[1].Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //--- ī�޶� �ٶ󺸴� ��
	myCamera[1].Rot = glm::vec3(0.0f, 0.0f, 0.0f);
	remote_control remocon_sample;
	remocon = remocon_sample;
	MyMode Anim_sample;
	Anim = Anim_sample;
}

void reset() {
	double max = 32767;
	myRobot.reset();
	maze3D.delete_Map();
	maze2D.delete_Map();
	maze3D.init(input_y, input_x);
	maze2D.init(input_y, input_x);/*
	for (int i = 0; i < maze3D.h; i++) {
		for (int j = 0; j < maze3D.w; j++) {
			maze3D.Map[i][j].Pos.y = 0.0;
		}
	}*/
	float max_height = 0.0;
	for (int i = 0; i < maze3D.h; i++) {
		for (int j = 0; j < maze3D.w; j++) {
			maze3D.Map[i][j] = originBox;
			maze3D.Map[i][j].Scale.x = 40.0 / maze3D.w;
			maze3D.Map[i][j].Pos.x = (-20 + maze3D.Map[i][j].Scale.x / 2.0) + j * maze3D.Map[i][j].Scale.x;

			maze3D.Map[i][j].Scale.z = 40.0 / maze3D.h;
			maze3D.Map[i][j].Pos.z = (20 - maze3D.Map[i][j].Scale.z / 2.0) - i * maze3D.Map[i][j].Scale.z;

			maze3D.Map[i][j].Max_Scale.y = (rand() / max) * 8 + 7; //0~8 7~15
			if (max_height < maze3D.Map[i][j].Max_Scale.y)
				max_height = maze3D.Map[i][j].Max_Scale.y;
			maze3D.Map[i][j].Min_Scale.y = (rand() / max) * 2 + 4; //0~2 4~6

			if (rand() % 2 == 0) {
				maze3D.Map[i][j].Scale.y = maze3D.Map[i][j].Max_Scale.y;
				maze3D.Map[i][j].dir = -1;
			}
			else {
				maze3D.Map[i][j].Scale.y = maze3D.Map[i][j].Min_Scale.y;
				maze3D.Map[i][j].dir = 1;
			}
			maze3D.Map[i][j].velocity = (rand() / (max * 20)) + 0.025; //0.025~0.075
		}
	}

	maze3D.max_height = max_height;
	maze3D.min_height = 4.0;
	maze3D.down_maze_intensity = maze3D.max_height;
	meteorBox.Scale = glm::vec3(40.0, maze3D.max_height + 1.0, 40.0);
	maze3D.down_maze_intensity = maze3D.max_height;

	meteorBox.Pos.y = 0.0;
	myCamera[0].Pos = glm::vec4(0.0f, 80.0f, -40.0f, 1.0f); //--- ī�޶� ��ġ
	myCamera[0].Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //--- ī�޶� �ٶ󺸴� ��
	myCamera[0].Rot = glm::vec3(0.0f, 0.0f, 0.0f); //--- ȸ�� ����
	
	myCamera[1].Pos = glm::vec4(0.0f, 60.0f, -0.001f, 1.0f); //--- ī�޶� ��ġ
	myCamera[1].Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //--- ī�޶� �ٶ󺸴� ��
	myCamera[1].Rot = glm::vec3(0.0f, 0.0f, 0.0f);
	remote_control remocon_sample;
	remocon = remocon_sample;
	MyMode Anim_sample;
	Anim = Anim_sample;
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

float* cheak_collision(float x, float y, float z, MyBox Box) {
	float vec[3] = { 0.0, 0.0, 0.0 };
	//printf("%f %f %f %f %f %f\n", Box.left(), Box.right(), Box.back(), Box.front(), Box.top(), Box.bottom());
	if (x + 0.1 > Box.left() && x - 0.1 < Box.right() && z + 0.1 > Box.back() && z - 0.1 < Box.front() && y >= Box.bottom() && y < Box.top()) {
		//printf("collision!!\n");
		float left = Box.left() - x - 0.1;//���� ����
		float right = x - 0.1 - Box.right();
		//float bottom = Box.bottom() - y;
		float top = y - Box.top();
		float back = Box.back() - z - 0.1;
		float front = z - 0.1 - Box.front();
		float max = left;//���� ���� �и��� �� ã��
		if (max < right)
			max = right;
		if (max < top)
			max = top;
		if (max < front)
			max = front;
		if (max < back)
			max = back;
		//if (max < bottom)
		//	max = bottom;
		//printf("%f %f %f %f %f %f %f\n", left, right, front, back, top, bottom, max);

		if (max == right) {
			vec[0] = -(right - 0);
		}
		else if (max == left) {
			vec[0] = (left - 0);
		}
		else if (max == front) {
			vec[2] = -(front - 0);
		}
		else if (max == back) {
			vec[2] = (back - 0);
		}
		else if (max == top) {
			vec[1] = -(top - 0);
		}
		//else {
		//	vec[1] = bottom;
		//}
		//printf("%f\t%f\n", x, y);
		//printf("%f\t %f\t %f\t %f\n", Rect.left, Rect.right, Rect.top, Rect.bot);
		//printf("%f\t %f\t %f\t %f\n", left, right, top, bottom);
	}
	return vec;
}


bool check_collision_min_move(MyRobot* unit1, MyBox* unit2) { // unit1�� �����γ�
	float left = unit1->left() - unit2->right();
	if (left > 0)
		return false;
	float max = left;
	float right = unit2->left() - unit1->right();
	if (right > 0)
		return false;
	if (max < right)
		max = right;

	float back = unit1->back() - unit2->front();
	if (back > 0)
		return false;
	if (max < back)
		max = back;
	float front = unit2->back() - unit1->front();
	if (front > 0)
		return false;
	if (max < front)
		max = front;

	float bottom = unit1->bottom() - unit2->top();
	if (bottom > 0)
		return false;
	if (max < bottom)
		max = bottom;
	float top = unit2->bottom() - unit1->top();
	if (top > 0)
		return false;
	if (max < top)
		max = top;
	////printf("%f %f %f %f %f %f %f\n", left, right, front, back, top, bottom, max);
	// top, bottom, right, left�� ��� �����̸� �ִ밪�� ������ �о, (���� ���ݸ� �о �Ǵ� ������ �б� ����)
	if (max == right)
		unit1->Pos.x += right;
	else if (max == left)
		unit1->Pos.x -= left;
	else if (max == front)
		unit1->Pos.z += front;
	else if (max == back)
		unit1->Pos.z -= back;
	else if (max == top) {
		unit1->Pos.y += top;
	}
	else {
		unit1->Pos.y -= bottom;
		myRobot.fall_speed = 0.08;
	}
	return true;
}