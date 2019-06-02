#define _USE_MATH_DEFINES
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<gl/GLU.h>
#include<glut.h>
#include<math.h>
#include<gl/GL.h>
#include<vector>
#include<iostream>
#include"SOIL.h"

using namespace std;
const GLdouble FRUSTDIM = 100.0f;
GLfloat door = 0.0;
GLfloat sticker = 45.0;
GLfloat screen = 0.0;
GLfloat arm = 10.0;

//spongebob location
GLfloat x0 = -60.0;
GLfloat y00 = -30.0;
GLfloat z0 = -30.0;

//walk status
GLint orient = 0; //0是面朝屏幕，顺时针0-3
GLfloat angle[4] = { 0, 90, 180, 270 };

//face
GLint face_type = 0;

int option = 0;
int power = 20;
bool light = true;
float l1 = 0.25;
float ll = 1.0;

void init(void) 
{
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glFrontFace(GL_CCW);
	glEnable(GL_COLOR_MATERIAL);
}

void music()
{
	MCI_OPEN_PARMS op; //播放背景音乐
	op.dwCallback = NULL;
	op.lpstrAlias = NULL;
	op.lpstrDeviceType = "MPEGAudio";
	op.lpstrElementName = "BGM0.mp3";
	op.wDeviceID = NULL;
	UINT rs;
	rs = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&op);
	if (rs == 0)
	{
		MCI_PLAY_PARMS pp;
		pp.dwCallback = NULL;
		pp.dwFrom = 0;
		mciSendCommand(op.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&pp);
	}
}

int n = 80;     //这里的n表示用多边形绘制圆的精度，可以考虑增大精度
void draw_circle(float R){
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)     //通过数学计算来画多边形的点
	{
		glVertex2f(R*cos(2 * M_PI * i / n), R*sin(2 * M_PI*i / n));
	}
	glEnd();
	glFlush();
}
void draw_eye(){
	glPushMatrix();
	//眼睫毛
	glTranslated(0.0, 3.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(-0.5f, 0.0f);
	glVertex2f(-2.3f, 1.8f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.2f, 2.2f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(1.8f, 1.9f);
	glEnd();
	glPopMatrix();
	if (face_type == 0){//正常眼
		//眼白
		glColor3f(1.0, 1.0, 1.0);
		draw_circle(3.8);
		//虹膜
		glColor3f(float(104 / 256.0), float(179 / 256.0), float(210 / 256.0));
		glTranslatef(0.0, 0.3, 0.1);
		draw_circle(2.0);
		//眼珠
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(0.0, 0.3, 0.1);
		draw_circle(1.0);
	}
	else if (face_type == 1){//星星眼
		//眼白
		glColor3f(1.0, 1.0, 1.0);
		draw_circle(3.8);
		//虹膜
		glColor3f(float(104 / 256.0), float(179 / 256.0), float(210 / 256.0));
		glTranslatef(0.0, 0.3, 0.1);
		draw_circle(3.5);
		//眼珠
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(0.0, 0.5, 0.1);
		draw_circle(2.5);
		//里面的光点
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0, 0.5, 0.1);
		draw_circle(1.0);
		glTranslatef(-0.3, -2.0, 0.0);
		draw_circle(0.6);
		glTranslatef(1.5, -0.2, 0.0);
		draw_circle(0.6);
		glTranslatef(-1.2, 1.7, -0.1);//回到原状态
	}

}
void display()
{
	music();
	glPushMatrix();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	GLfloat lightpos[] = { 0.f, 100.f, -450.f, ll };
	GLfloat light0_mat1[] = { 0.2, 0.2, 0.2, 0.3f };
	GLfloat light0_diff[] = { 1.0, 1.0, 1.0, 0.3 };
	GLfloat lightpos2[] = { 0.0, 0.0, 0.0, ll };
	GLfloat light1_mat1[] = { 0.1, 0.1, 0.1, 0.3 };
	GLfloat light1_diff[] = { 0.1, 0.1, 0.1, 0.3 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_mat1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff);

	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat high_shininess[] = { 20.0 };
	GLfloat high_mat[] = { l1, l1, l1, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, high_mat);

	glTranslatef(0.0, 0.0, -300.0);

	// SpongeBob
	glPushMatrix();
	glTranslatef(x0, y00, z0);
	glRotatef(angle[orient], 0.0, 1.0, 0.0);
	// arms
	glColor3f(float(249 / 256.0), float(236 / 256.0), float(59 / 256.0));
	glPushMatrix();
	glTranslatef(-11, -32.0, 0.0);
	glRotatef(arm, 0.0, 0.0, -1.0);
	glTranslatef(0.0, -15.0, 0.0);
	glScalef(2.0, 18.0, 2.0);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(11, -32.0, 0.0);
	glRotatef(arm, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -15.0, 0.0);
	glScalef(2.0, 18.0, 2.0);
	glutSolidCube(1.0);
	glPopMatrix();
	// body
	glColor3f(float(249 / 256.0), float(236 / 256.0), float(59 / 256.0));
	glTranslatef(0.0, -30.0, 0.0);
	glScalef(25.0, 25.0, 15.0);
	glutSolidCube(1.0);
	//face
	glPushMatrix();
	//eye
	glScalef(1.0 / 25.0, 1.0 / 25.0, 1.0 / 15.0);
	glTranslatef(-4.0, 0.3, 8.0);
	glPushMatrix();
	draw_eye();
	glPopMatrix();
	glTranslatef(7.0, 0.0, 0.0);
	draw_eye();
	//nose
	glTranslatef(-3.5, -3.0, 0.5);
	glColor3f(float(209 / 256.0), float(186 / 256.0), float(49 / 256.0));
	draw_circle(1.5);
	//mouse
	glColor3f(float(209 / 256.0), float(186 / 256.0), float(49 / 256.0));
	glTranslatef(0.0, 2.0, -0.5);
	glBegin(GL_LINE_STRIP);
	float mouseR = 6;
	for (int i = 0; i < n; i++)
	{
		if (sin(2 * M_PI*i / n)<-0.5)glVertex2f(mouseR*cos(2 * M_PI * i / n), mouseR*sin(2 * M_PI*i / n));
	}
	glEnd();
	glFlush();
	//tooth
	if (face_type == 0){
		glTranslatef(-1.5, -6.0, 0.0);
		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
		glVertex2f(-1.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, -3.0f);
		glVertex2f(-1.0f, -3.0f);
		glEnd();
		glTranslatef(2.4, 0.0, 0.0);
		glBegin(GL_POLYGON);
		glVertex2f(-1.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, -2.5f);
		glVertex2f(-1.0f, -2.5f);
		glEnd();
	}
	// pants
	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glScalef(1.0 / 25.0, 1.0 / 25.0, 1.0 / 15.0);
	glTranslatef(0.0, -15.0, 0.0);
	glScalef(25.0, 10.0, 15.0);
	glutSolidCube(1.0);
	// legs and shoes
	glColor3f(float(249 / 256.0), float(236 / 256.0), float(59 / 256.0));
	glScalef(1.0 / 30.0, 1.0 / 35.0, 1.0 / 15.0);
	glTranslatef(-8.0, -35.0, 0.0);
	glScalef(2.0, 35.0, 2.0);
	glutSolidCube(1.0);//left leg
	glPushMatrix();
	glScalef(1.0 / 2.0, 1.0 / 35.0, 1.0 / 15.0);
	glTranslatef(-0.5, -20.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glScalef(4.5, 10.0, 22.0);
	glutSolidCube(1.0);//left shoe
	glPopMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glScalef(1.0 / 7.0, 1.0 / 35.0, 1.0 / 15.0);
	glTranslatef(48.0, 0.0, 0.0);
	glScalef(7.0, 35.0, 15.0);
	glutSolidCube(1.0);//right leg
	glScalef(1.0 / 2.0, 1.0 / 35.0, 1.0 / 15.0);
	glTranslatef(-0.5, -20.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glScalef(4.5, 10.0, 22.0);
	glutSolidCube(1.0);//right shoe
	glPopMatrix();

	

	// top light
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.5);
	glTranslatef(0.0, 94.0, -150.0);
	glScalef(80.0, 4.0, 80.0);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.6, 0.5, 0.5);
	glTranslatef(0.0, 99.0, -150.0);
	glScalef(100.0, 2.0, 100.0);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}


void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-FRUSTDIM, FRUSTDIM, -FRUSTDIM, FRUSTDIM, 300., 800.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{
	switch (key)
	{
	case'\033':
		exit(0);
		break;
	case'W':
	case'w':
		orient = 2;
		Sleep(5);
		display();
		if (z0 >= -235){
			Sleep(5);
			z0 -= 3;
			display();
		}
		else cout << "要撞墙了，不能再走啦" << endl;
		break;
	case'S':
	case's':
		orient = 0;
		Sleep(5);
		display();
		if (z0 <= -11){
			Sleep(5);
			z0 += 3;
			display();
		}
		else cout << "要撞墙了，不能再走啦" << endl;
		break;
	case'A':
	case'a':
		orient = 3;
		Sleep(5);
		display();
		if (x0 >= -78){
			Sleep(5);
			x0 -= 3;
			display();
		}
		else cout << "要撞墙了，不能再走啦" << endl;
		break;
	case'D':
	case'd':
		orient = 1;
		Sleep(5);
		display();
		if (x0 <= 78){
			Sleep(5);
			x0 += 3;
			display();
		}
		else cout << "要撞墙了，不能再走啦" << endl;
		break;
	case'P':
	case'p':
		if (power <= 160){
			power += 4;
			cout << "蓄力中当前力量为" << power << endl;
		}
		else{
			cout << "蓄力完毕" << endl;
		}
		break;
	case'J':
	case'j':

		Sleep(30);
		y00 += (power / 14 * 9);
		display();
		Sleep(30);
		y00 += (power / 14 * 4);
		display();
		Sleep(30);
		y00 += (power / 14 * 1);
		display();
		Sleep(30);
		//跳跃判定
		if (power >= 140 && z0 >= -120 && z0 <= -90 && x0 <= 12 && x0 >= -18){
			if (light){
				l1 = 0.0;
				ll = 0.0;
				light = false;
				display();
			}
			else{
				l1 = 0.25;
				ll = 1.0;
				light = true;
				display();
			}
		}
		y00 -= (power / 14 * 1);
		display();
		Sleep(30);
		y00 -= (power / 14 * 4);
		display();
		Sleep(30);
		y00 -= (power / 14 * 9);
		display();
		Sleep(30);
		power = 20;
		break;
	case'F':
	case'f':
		face_type++;
		face_type = face_type % 2;
		display();
		break;
	default:
		printf("error\n");
	}
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("太平洋比基尼海滩比奇堡镇贝壳街124号的波萝屋");

	init();	
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}
