//// Reading Obj file
//#include "stdafx.h"
//#include "shader.h"
//#include "myObj.h"
//#include "myCamera.h"
//#include "myLight.h"
//#include <time.h>
//#include <cmath>
//using namespace std;
//
//GLuint g_window_w = 800;
//GLuint g_window_h = 800;
//GLuint VAO[4];
//GLuint VBO_pos[4];
//GLuint VBO_norm[4];
//
//MyObj originBox;
//MyObj pyramid;
//MyObj light_ball;
//MyLight light;
//
////bool mazeMap[max_h][max_w];
////MyBox maze3D.Map[max_h][max_w];
////int maze3D.h = 15;
////int maze3D.w = 15;
////GLuint VAO[25][25];
////GLuint VBO_position[25][25];
////GLuint VBO_color[25][25];
//
//void drawScene();
//void Reshape(int w, int h);
//void KeyDown(unsigned char key, int x, int y);
//void InitBuffer();
//void randInitColor(MyObj* Obj);
//void enter();
//void reset();
//void FPS100(int value);
////--- load obj related variabales
////objRead objReader;
////GLint Object = objReader.loadObj_normalize_center("mid_box.obj");
//
//class UserInput {
//public:
//	bool left_down = false;
//	float cursor_x = 0.0, cursor_y = 0.0;
//	float cursor_x1 = 0.0, cursor_y1 = 0.0;
//	float cursor_x2 = 0.0, cursor_y2 = 0.0;
//};
//UserInput userInput;
//
//struct remote_control {
//	bool depth_test = true;
//	bool polygon_mode = true;
//	bool perspective_projection = true;
//	bool first_person = false;
//	bool cube = true;
//	bool light_on = true;
//};
//
//remote_control remocon;
//
//float gravity_force = 0.2;
//MyCamera myCamera[2];
//
////GLint Object = objReader.loadObj_normalize_center("pyra.obj");
////int Poly = 0;
//
//typedef struct MyAnim {
//};
//
//MyAnim anim;
//
//int main(int argc, char** argv)
//{	
//	// create window using freeglut
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(g_window_w, g_window_h);
//	glutInitWindowPosition(0, 0);
//
//	glutCreateWindow("Computer Graphics");
//	//////////////////////////////////////////////////////////////////////////////////////
//	//// initialize GLEW
//	//////////////////////////////////////////////////////////////////////////////////////
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)
//	{
//		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	else
//	{
//		std::cout << "GLEW OK\n";
//	}
//
//	//////////////////////////////////////////////////////////////////////////////////////
//	//// Create shader program an register the shader
//	//////////////////////////////////////////////////////////////////////////////////////
//
//	GLuint vShader[4];
//	GLuint fShader[4];
//
//	vShader[0] = MakeVertexShader("vertex.glsl", 0);			// Sun
//	fShader[0] = MakeFragmentShader("fragment.glsl", 0);
//
//	// shader Program
//	s_program[0] = glCreateProgram();
//	glAttachShader(s_program[0], vShader[0]);
//	glAttachShader(s_program[0], fShader[0]);
//	glLinkProgram(s_program[0]);
//	checkCompileErrors(s_program[0], "PROGRAM");
//
//	enter();
//	InitBuffer();
//
//	// callback functions
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(Reshape);
//	glutKeyboardFunc(KeyDown);
//	//glutTimerFunc(10, gravity, 1);
//	glutTimerFunc(10, FPS100, 1);
//	//glutTimerFunc(10, anim_1, 1);
//	// freeglut 윈도우 이벤트 처리 시작. 윈도우가 닫힐때까지 후한루프 실행.
//	glutMainLoop();
//
//	return 0;
//}
//
//GLuint center_vao, center_vbo_pos;
//
//GLfloat center_vPos[6][3] = {
//	-5.0, 0.0, 0.0,
//	5.0, 0.0, 0.0,
//	0.0, -5.0, 0.0,
//	0.0, 5.0, 0.0,
//	0.0, 0.0, -5.0,
//	0.0, 0.0, 5.0
//};
//
//GLuint floor_vao, floor_vbo_pos;
//
//GLfloat floor_vPos[6][3] = {
//	-200.0, -0.001, -200.0,
//	-200.0, -0.001, 200.0,
//	200.0, -0.001, -200.0,
//	200.0, -0.001, -200.0,
//	-200.0, -0.001, 200.0,
//	200.0, -0.001, 200.0
//};
//GLuint light_ball_vao, light_ball_vbo_pos;
//
//unsigned int modelLocation;// 모델변환 유니폼
//unsigned int modelRotateLocation;// 모델변환 유니폼
//unsigned int viewLocation;// 카메라 변환 유니폼
//unsigned int projectionLocation;// 투영 변환 유니폼
//
//unsigned int objColorLocation; // 물체 색상 유니폼
//unsigned int lightPosLocation; // 빛의 위치 유니폼
//unsigned int lightColorLocation; // 빛의 색상 유니폼
//unsigned int viewPosLocation; // 카메라 위치 유니폼 - 빛의 반사, 하이라이트를 표현 해주기 위함.
//void InitBuffer()
//{
//	GLint pAttribute = glGetAttribLocation(s_program[0], "vPos");
//	GLint nAttribute = glGetAttribLocation(s_program[0], "vNormal");
//	//// 5.1. VAO 객체 생성 및 바인딩
//	glGenVertexArrays(1, &center_vao);
//	glGenBuffers(1, &center_vbo_pos);
//	glBindVertexArray(center_vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, center_vbo_pos);
//	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), center_vPos, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(0);
//
//	glGenVertexArrays(1, &floor_vao);
//	glGenBuffers(1, &floor_vbo_pos);
//	glBindVertexArray(floor_vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo_pos);
//	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), floor_vPos, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(0);
//
//	glGenVertexArrays(2, VAO);
//	glGenBuffers(2, VBO_pos);
//	glGenBuffers(2, VBO_norm);
//
//	glBindVertexArray(VAO[0]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos[0]);
//	glBufferData(GL_ARRAY_BUFFER, originBox.outvertex.size() * sizeof(glm::vec3), &originBox.outvertex[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(pAttribute);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_norm[0]);
//	glBufferData(GL_ARRAY_BUFFER, originBox.outnormal.size() * sizeof(glm::vec3), &originBox.outnormal[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(nAttribute);
//
//	glBindVertexArray(VAO[1]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos[1]);
//	glBufferData(GL_ARRAY_BUFFER, pyramid.outvertex.size() * sizeof(glm::vec3), &pyramid.outvertex[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(pAttribute);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_norm[1]);
//	glBufferData(GL_ARRAY_BUFFER, pyramid.outnormal.size() * sizeof(glm::vec3), &pyramid.outnormal[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(nAttribute);
//
//	glGenVertexArrays(1, &light_ball_vao);
//	glGenBuffers(1, &light_ball_vbo_pos);
//	glBindVertexArray(light_ball_vao);
//	glBindBuffer(GL_ARRAY_BUFFER, light_ball_vbo_pos);
//	glBufferData(GL_ARRAY_BUFFER, light_ball.outvertex.size() * sizeof(glm::vec3), &light_ball.outvertex[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(pAttribute);
//
//	glUseProgram(s_program[0]);
//	objColorLocation = glGetUniformLocation(s_program[0], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
//	lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
//	lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
//	viewPosLocation = glGetUniformLocation(s_program[0], "viewPos"); //--- viewPos 값 전달: 카메라 위치
//
//	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");//--- 모델 변환 설정
//	modelRotateLocation = glGetUniformLocation(s_program[0], "modelRotation");
//	viewLocation = glGetUniformLocation(s_program[0], "viewTransform"); //--- 뷰잉 변환 설정
//	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform"); //--- 투영 변환 값 설정
//}
//glm::mat4 view = glm::mat4(1.0f);
//glm::mat4 projection = glm::mat4(1.0f);
//float Distance(MyObj ob1, MyObj ob2) {
//	return sqrt(pow(ob2.Pos.x - ob1.Pos.x, 2) + pow(ob2.Pos.z - ob1.Pos.z, 2));
//}
//void drawWorld() {
//	if (remocon.light_on)
//		glUniform3f(lightColorLocation, light.Color.x, light.Color.y, light.Color.z); // 빛의 색
//	else
//		glUniform3f(lightColorLocation, 0.0, 0.0, 0.0); // 빛의 색
//	glUniform3f(lightPosLocation, light.Pos.x, light.Pos.y, light.Pos.z); // 빛의 위치
//	glUniform3f(viewPosLocation, myCamera[0].Pos.x, myCamera[0].Pos.y, myCamera[0].Pos.z); // 카메라 위치
//
//	glm::mat4 WorldConvert = glm::mat4(1.0f);//변환행렬 초기화
//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(WorldConvert));
//	glBindVertexArray(center_vao);//중앙 좌표 표시
//	glUniform3f(objColorLocation, 1.0, 0.0, 0.0);
//	glDrawArrays(GL_LINES, 0, 2);
//	glUniform3f(objColorLocation, 0.0, 1.0, 0.0);
//	glDrawArrays(GL_LINES, 2, 2);
//	glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
//	glDrawArrays(GL_LINES, 4, 2);
//	//glBindVertexArray(floor_vao);
//	//glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	GLUquadricObj* Circle = gluNewQuadric(); // 객체 생성하기
//	gluQuadricDrawStyle(Circle, GLU_SILHOUETTE); // 도형 스타일
//	glm::mat4 Circle_Convert = glm::mat4(1.0f);//변환행렬 초기화
//	Circle_Convert = glm::rotate(Circle_Convert, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Circle_Convert));
//	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
//	float distance = sqrt(pow(originBox.Pos.x - light.Pos.x, 2) + pow(originBox.Pos.z - light.Pos.z, 2));
//	gluDisk(Circle, 0.0, distance, 32, 1);  // 객체 만들기
//
//	//for (int i = 0; i < circle.vertex_size; i++) {
//	//	printf("%d ", circle.outvertex[i]);
//	//	printf("%d ", circle.outvertex[i+1]);
//	//	printf("%d \n", circle.outvertex[i+2]);
//	//}
//	glBindVertexArray(light_ball_vao);
//	glUniform3f(objColorLocation, light_ball.Color.x, light_ball.Color.y, light_ball.Color.z); // 물체 색
//	glm::mat4 Light_Trans = glm::mat4(1.0f);//변환행렬 초기화
//	Light_Trans = glm::translate(Light_Trans, glm::vec3(light.Pos.x, light.Pos.y, light.Pos.z));
//	light_ball.show(&modelLocation, Light_Trans);
//
//	if (remocon.cube) {
//		glBindVertexArray(VAO[0]);
//		glUniform3f(objColorLocation, originBox.Color.x, originBox.Color.y, originBox.Color.z); // 물체 색
//		glm::mat4 Rot = glm::mat4(1.0f);//변환행렬 초기화
//		glm::mat4 Trans = glm::mat4(1.0f);//변환행렬 초기화
//		Rot = glm::rotate(Rot, glm::radians(originBox.Rot.y), glm::vec3(0.0, 1.0, 0.0));
//		glUniformMatrix4fv(modelRotateLocation, 1, GL_FALSE, glm::value_ptr(Rot));
//		Trans = glm::translate(Trans, glm::vec3(originBox.Pos.x, originBox.Pos.y, originBox.Pos.z));
//		glm::mat4 Convert = Trans * Rot;
//		originBox.show(&modelLocation, Convert);
//	}
//	else {
//		glBindVertexArray(VAO[1]);
//		glUniform3f(objColorLocation, pyramid.Color.x, pyramid.Color.y, pyramid.Color.z); // 물체 색
//		glm::mat4 Rot = glm::mat4(1.0f);//변환행렬 초기화
//		glm::mat4 Trans = glm::mat4(1.0f);//변환행렬 초기화
//		Rot = glm::rotate(Rot, glm::radians(pyramid.Rot.y), glm::vec3(0.0, 1.0, 0.0));
//		glUniformMatrix4fv(modelRotateLocation, 1, GL_FALSE, glm::value_ptr(Rot));
//		Trans = glm::translate(Trans, glm::vec3(pyramid.Pos.x, pyramid.Pos.y, pyramid.Pos.z));
//		glm::mat4 Convert = Trans * Rot;
//		pyramid.show(&modelLocation, Convert);
//	}
//
//}
//
//void MyViewport0() {
//	glViewport(0, 0, g_window_w, g_window_h);
//
//	//MyCamera[0].Dir = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//카메라 보는 방향
//	if (remocon.perspective_projection)
//		projection = glm::perspective(glm::radians(45.0f), -1.0f, 0.1f, 200.0f);// 맨뒤 두개는 카메라로부터 근평면 원평면 거리
//	else
//		projection = glm::ortho(40.0f, -40.0f, -40.0f, 40.0f, 0.1f, 200.0f);
//
//	view = glm::lookAt(glm::vec3(myCamera[0].Pos), glm::vec3(myCamera[0].Dir), glm::vec3(myCamera[0].Up));
//	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
//	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
//	drawWorld();
//}
//
//void drawScene()
//{
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glUseProgram(s_program[0]);
//
//	if (remocon.polygon_mode)
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//면
//	else
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//외곽선
//	if (remocon.depth_test)
//		glEnable(GL_DEPTH_TEST);//깊이테스트
//	else
//		glDisable(GL_DEPTH_TEST);
//
//	//glEnable(GL_CULL_FACE);
//	//glFrontFace(GL_CW);
//
//	MyViewport0();
//	//MyViewport1();
//	glutSwapBuffers();
//}
//
//void Reshape(int w, int h)
//{
//	g_window_w = w;
//	g_window_h = h;
//	glViewport(0, 0, w, h);
//}
//
//void KeyDown(unsigned char key, int x, int y)
//{
//	switch (key) {
//	case 'Q':
//	case 'q':
//		exit(0);
//		break;
//	case '1':
//		remocon.cube = true;
//		break;
//	case '2':
//		remocon.cube = false;
//		break;
//	case '3':
//		if (remocon.light_on) {
//			remocon.light_on = false;
//			break;
//		}
//		remocon.light_on = true;
//		break;
//	case '\'':
//		for (int i = 0; i < 1; i++) {
//			glm::mat4 LR = glm::mat4(1.0f);
//			glm::mat4 LT = glm::mat4(1.0f);
//			glm::mat4 LRr = glm::mat4(1.0f);
//			LR = glm::rotate(LR, glm::radians(-light.Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			LT = glm::translate(LT, glm::vec3(0.0, 0.0, 0.01));
//			LRr = glm::rotate(LRr, glm::radians(light.Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			glm::mat4 LC = LRr * LT * LR;
//			light.Pos = LC * light.Pos;
//		}
//		break;
//	case '/':
//		for (int i = 0; i < 1; i++) {
//			glm::mat4 LR = glm::mat4(1.0f);
//			glm::mat4 LT = glm::mat4(1.0f);
//			glm::mat4 LRr = glm::mat4(1.0f);
//			LR = glm::rotate(LR, glm::radians(-light.Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			LT = glm::translate(LT, glm::vec3(0.0, 0.0, -0.01));
//			LRr = glm::rotate(LRr, glm::radians(light.Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			glm::mat4 LC = LRr * LT * LR;
//			light.Pos = LC * light.Pos;
//		}
//		break;
//	case 'Y':
//	case 'y':
//		originBox.Rot.y++;
//		pyramid.Rot.y++;
//		break;
//	case 'R':
//	case 'r':
//		for (int i = 0; i < 1; i++) {
//			glm::mat4 LT = glm::mat4(1.0f);
//			glm::mat4 LR = glm::mat4(1.0f);
//			glm::mat4 LTr = glm::mat4(1.0f);//변환행렬 초기화
//			float Dx, Dy, Dz;
//			Dx = originBox.Pos.x, Dy = originBox.Pos.y, Dz = originBox.Pos.z;
//			LT = glm::translate(LT, glm::vec3(-Dx, -Dy, -Dz));
//			LR = glm::rotate(LR, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
//			light.Rot.y += 1.0;
//			LTr = glm::translate(LTr, glm::vec3(Dx, Dy, Dz));
//			glm::mat4 LC = LTr * LR * LT;
//			light.Pos = LC * light.Pos;
//		}
//		break;
//	case 'I':
//	case 'i':
//		if (remocon.polygon_mode == true)
//			remocon.polygon_mode = false;
//		else
//			remocon.polygon_mode = true;
//		break;
//	case 'O':
//	case 'o':
//		if (remocon.depth_test == true)
//			remocon.depth_test = false;
//		else
//			remocon.depth_test = true;
//		break;
//	case 'P':
//	case 'p':
//		if (remocon.perspective_projection == true)
//			remocon.perspective_projection = false;
//		else
//			remocon.perspective_projection = true;
//		break;
//	case '[':
//		glm::mat4 CR;
//		glm::mat4 CT;
//		glm::mat4 CC;
//		glm::mat4 CTr;
//		glm::mat4 CRr;
//		float Px, Py, Pz;
//		float Dx, Dy, Dz;
//		for (int i = 0; i < 1; i++) {
//			CR = glm::mat4(1.0f);
//			CT = glm::mat4(1.0f);
//			CC = glm::mat4(1.0f);
//			CTr = glm::mat4(1.0f);//변환행렬 초기화
//			Dx = myCamera[i].Dir.x, Dy = myCamera[i].Dir.y, Dz = myCamera[i].Dir.z;
//			CT = glm::translate(CT, glm::vec3(-Dx, -Dy, -Dz));
//			CR = glm::rotate(CR, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
//			myCamera[i].Rot.y += 1.0;
//			CTr = glm::translate(CTr, glm::vec3(Dx, Dy, Dz));
//			CC = CTr * CR * CT;
//			myCamera[i].Pos = CC * myCamera[i].Pos;
//		}
//		break;
//	case ']':
//		for (int i = 0; i < 1; i++) {
//			CR = glm::mat4(1.0f);
//			CT = glm::mat4(1.0f);
//			CC = glm::mat4(1.0f);
//			CTr = glm::mat4(1.0f);
//			Dx = myCamera[i].Dir.x, Dy = myCamera[i].Dir.y, Dz = myCamera[i].Dir.z;
//			CT = glm::translate(CT, glm::vec3(-Dx, -Dy, -Dz));
//			CR = glm::rotate(CR, glm::radians(-1.0f), glm::vec3(0.0, 1.0, 0.0));
//			myCamera[i].Rot.y -= 1.0;
//			CTr = glm::translate(CTr, glm::vec3(Dx, Dy, Dz));
//			CC = CTr * CR * CT;
//			myCamera[i].Pos = CC * myCamera[i].Pos;
//		}
//		break;
//	case '9':
//		for (int i = 0; i < 1; i++) {
//			CR = glm::mat4(1.0f);
//			CT = glm::mat4(1.0f);
//			CC = glm::mat4(1.0f);
//			CTr = glm::mat4(1.0f);
//			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
//			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
//			CR = glm::rotate(CR, glm::radians(2.0f), glm::vec3(0.0, 1.0, 0.0));
//			myCamera[i].Rot.y += 2.0;
//			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
//			CC = CTr * CR * CT;
//			myCamera[i].Dir = CC * myCamera[i].Dir;
//		}
//		break;
//	case '7':
//		for (int i = 0; i < 1; i++) {
//			CR = glm::mat4(1.0f);
//			CT = glm::mat4(1.0f);
//			CC = glm::mat4(1.0f);
//			CTr = glm::mat4(1.0f);
//			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
//			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
//			CR = glm::rotate(CR, glm::radians(-2.0f), glm::vec3(0.0, 1.0, 0.0));
//			myCamera[i].Rot.y -= 2.0;
//			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
//			CC = CTr * CR * CT;
//			myCamera[i].Dir = CC * myCamera[i].Dir;
//		}
//		break;
//	case '8':
//		glm::mat4 CT1;
//		for (int i = 0; i < 1; i++) {
//			/*MyCamera[i].Pos.z += 0.1;
//			MyCamera[i].Dir.z += 0.1;*/
//			CT1 = glm::mat4(1.0f);
//			CR = glm::mat4(1.0f);
//			CT = glm::mat4(1.0f);
//			CC = glm::mat4(1.0f);
//			CTr = glm::mat4(1.0f);
//			CRr = glm::mat4(1.0f);
//			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
//			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
//			CR = glm::rotate(CR, glm::radians(-myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			CT1 = glm::translate(CT1, glm::vec3(0.0f, 0.0f, 0.5f));
//			CRr = glm::rotate(CRr, glm::radians(myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
//			CC = CTr * CRr * CT1 * CR * CT;
//			myCamera[i].Dir = CC * myCamera[i].Dir;
//			myCamera[i].Pos = CC * myCamera[i].Pos;
//		}
//		break;
//	case '5':
//		for (int i = 0; i < 1; i++) {
//			CT1 = glm::mat4(1.0f);
//			CR = glm::mat4(1.0f);
//			CT = glm::mat4(1.0f);
//			CC = glm::mat4(1.0f);
//			CTr = glm::mat4(1.0f);
//			CRr = glm::mat4(1.0f);
//			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
//			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
//			CR = glm::rotate(CR, glm::radians(-myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			CT1 = glm::translate(CT1, glm::vec3(0.0f, 0.0f, -0.5f));
//			CRr = glm::rotate(CRr, glm::radians(myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
//			CC = CTr * CRr * CT1 * CR * CT;
//			myCamera[i].Dir = CC * myCamera[i].Dir;
//			myCamera[i].Pos = CC * myCamera[i].Pos;
//		}
//		break;
//	case '4':
//		for (int i = 0; i < 1; i++) {
//			CT1 = glm::mat4(1.0f);
//			CR = glm::mat4(1.0f);
//			CT = glm::mat4(1.0f);
//			CC = glm::mat4(1.0f);
//			CTr = glm::mat4(1.0f);
//			CRr = glm::mat4(1.0f);
//			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
//			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
//			CR = glm::rotate(CR, glm::radians(-myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			CT1 = glm::translate(CT1, glm::vec3(-0.5f, 0.0f, 0.0f));
//			CRr = glm::rotate(CRr, glm::radians(myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
//			CC = CTr * CRr * CT1 * CR * CT;
//			myCamera[i].Dir = CC * myCamera[i].Dir;
//			myCamera[i].Pos = CC * myCamera[i].Pos;
//		}
//		break;
//	case '6':
//		for (int i = 0; i < 1; i++) {
//			CT1 = glm::mat4(1.0f);
//			CR = glm::mat4(1.0f);
//			CT = glm::mat4(1.0f);
//			CC = glm::mat4(1.0f);
//			CTr = glm::mat4(1.0f);
//			CRr = glm::mat4(1.0f);
//			Px = myCamera[i].Pos.x, Py = myCamera[i].Pos.y, Pz = myCamera[i].Pos.z;
//			CT = glm::translate(CT, glm::vec3(-Px, -Py, -Pz));
//			CR = glm::rotate(CR, glm::radians(-myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			CT1 = glm::translate(CT1, glm::vec3(0.5f, 0.0f, 0.0f));
//			CRr = glm::rotate(CRr, glm::radians(myCamera[i].Rot.y), glm::vec3(0.0, 1.0, 0.0));
//			CTr = glm::translate(CTr, glm::vec3(Px, Py, Pz));
//			CC = CTr * CRr * CT1 * CR * CT;
//			myCamera[i].Dir = CC * myCamera[i].Dir;
//			myCamera[i].Pos = CC * myCamera[i].Pos;
//		}
//		break;
//	}
//}
//
//void anim_play() {
//
//
//}
//
//void FPS100(int value) {
//	anim_play();
//	
//
//	glutPostRedisplay();
//	glutTimerFunc(10, FPS100, 1);
//}
//
//void enter() {
//	srand((unsigned int)time(NULL));
//	light_ball.loadObj_normalize_center("sphere_center.obj");
//	light_ball.Color = glm::vec3(1.0, 1.0, 1.0);
//	originBox.loadObj_normalize_center("cube_ground.obj");
//	pyramid.loadObj_normalize_center("pyramid_ground.obj");
//	randInitColor(&originBox);
//	randInitColor(&pyramid);
//	originBox.Scale = glm::vec3(3, 3, 3);
//	originBox.Pos = glm::vec3(0.0, -1.5, 0.0);
//	pyramid.Scale = glm::vec3(3, 3, 3);
//	pyramid.Pos = glm::vec3(0.0, -1.5, 0.0);
//	light.Pos = glm::vec4(0.0, 0.0, -5.0, 1.0);
//}
//
//void reset() {
//
//}
//
//void randInitColor(MyObj* Obj) {
//	glm::vec3 input;
//	double max = 32767;
//	input[0] = rand() / max;
//	input[1] = rand() / max;
//	input[2] = rand() / max;
//	Obj->Color = input;
//}