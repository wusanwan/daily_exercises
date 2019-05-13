#include<glut.h>
#include<gl/GL.h>
#include<stdio.h>
#include<windows.h>
#include<gl/GLU.h>
#include<vector>
#include<algorithm>
#include<iostream>
using namespace std;

int WinHeight = 400, WinWidth = 400, seed = 0;
int cx = WinHeight / 2, cy = WinWidth/2;
int iMode = 1;
struct Point{
	float x, y;
};
Point line[2];
Point window;	//�ü��������ĵ�
Point s0[4] = { 195.0f, 195.0f, 205.0f, 195.0f, 205.0f, 205.0f ,195.0f, 205.0f };
float square0[4] = { 195.0f, 195.0f, 205.0f, 205.0f };//��ʼ״̬
float square1[4];

void DrawSquare(){
	glClear(GL_COLOR_BUFFER_BIT); 
	if (iMode==1)glRectf(square0[0],square0[1], square0[2], square0[3]);    
	else glRectf(square1[0], square1[1], square1[2], square1[3]);
	glFlush();  
}
void Draw(){
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 4; i++){
		cout << s0[i].x << "  " << s0[i].y << endl;
		glVertex2f(s0[i].x, s0[i].y);
	}
	glEnd();
	glFlush();
}
void my_translation(){//ƽ��
	cout << "����ƽ��50����λ������ƽ��100����λ" << endl;
	float dx = 50,dy =100;//ƽ�Ƹ�����
	for (int i = 0; i < 4; i++){
		if (i == 0 || i == 2)square1[i] = square0[i] + dx;
		else square1[i] = square0[i] + dy;
	}
	DrawSquare();
}

void my_scale(){//����
	cout << "��ͼ������Ļ����,��������ԭ�����ηŴ�5��" << endl;
	float xmultiple = 5 , ymultiple = 5;		
	for (int i = 0; i < 4; i++){
		if (i == 0 || i == 2)square1[i] = (square0[i]-cx)*xmultiple + cx;
		else square1[i] = (square0[i] - cy)*ymultiple + cy;
	}
	DrawSquare();
}

void my_symmetry(){//�Գ�
	cout << "����ԭ���������½ǵĵ����ĶԳ�" << endl;
	for (int i = 0; i < 4; i++){
		if (i == 2)square1[i] = 2*square0[0]-square0[i];
		else if (i == 3) square1[i] = 2*square0[1] - square0[i];
		else square1[i] = square0[i];
	}
	if (square1[2]<square1[0])swap(square1[0], square1[2]);
	if (square1[3]<square1[1])swap(square1[1], square1[3]);
	DrawSquare();
}

void my_shear(){
	float c=0.5f;//��ˮƽ����Ĵ��У�����x�᷽�����y�Ĵ��У���>0������ + x�᷽����У���<0������ - x�᷽����С�
	cout << "ˮƽ������+x������У�����Ϊ0.5" << endl;
	for (int i = 0; i < 4; i++){
		s0[i].x = s0[i].x + c*(s0[i].y-s0[0].y);//��ȥs0[0].y����Ϊ��s0[0].y��Ϊy=0����
	}
	Draw();
}

//Liang-Barsky�㷨��ʼ
BOOL LBLineClipTest(float p, float q, float &u1, float&u2){
	float r = 0.0;
	if (p < -0.00001){//pС��0ʱ�Ƚ����ֵ
		r = q / p;
		if (r > u2)return false;//umaxС��umin�����ڴ����ڵĲ���
		else if (r > u1)u1 = r;
	}
	else if (p > 0.00001){	//p����0ʱ�Ƚ���Сֵ
		r = q / p;
		if (r < u1)return false;
		else if (r < u2)u2 = r;
	}
	else if (q < 0.0)return false;//����p����0�����
	return true;
}

void VBLineClip(float XL, float XR, float YB, float YT, float x1, float y1, float x2, float y2){
	float u1, u2, dx, dy;
	dx = x2 - x1;
	dy = y2 - y1;
	u1 = 0.0; u2 = 1.0;
	if (LBLineClipTest(-dx, x1 - XL, u1, u2)){//��߽罻��
		if (LBLineClipTest(dx, XR - x1, u1, u2)){//�ұ߽罻��
			if (LBLineClipTest(-dy, y1 - YB, u1, u2)){//�±߽罻��
				if (LBLineClipTest(dy, YT - y1, u1, u2)){//�ϱ߽罻��
					//u2����1˵���ߵ��Ҷ˵�����޽��㣨�˵��ڿ��ڣ�������ֱ���������
					if (u2< 1.0) {//˵���ߵ��Ҷ˵㳬�������ˣ���ʱ��Ҫ���У�����x2,y2Ϊ�߿򽻵㡣
						x2 = x1 + u2*dx;
						y2 = y1 + u2*dy;
					}
					if (u1>0.0) {//u1ͬ��
						x1 = x1 + u1*dx;
						y1 = y1 + u1*dy;
					}
				}
				glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex2i(x1, y1);
				glVertex2i(x2, y2);
				glEnd();
				glFlush();
			}
		}
	}
}

void Display(void){
	
	glColor3f(1.0f, 0.0f, 0.0f);	//ָ����ǰ�Ļ�ͼ��ɫ
	if (iMode == 1)DrawSquare();
	else if (iMode == 2)my_translation();
	else if (iMode == 3)my_scale();
	else if (iMode == 4)my_symmetry();
	else if (iMode == 5)my_shear();
	else{
		cout << "����ֱ�����Ҽ�����ѡ��liang-barsky�㷨" << endl;
		glClear(GL_COLOR_BUFFER_BIT);
		srand(seed);
		line[0].x = rand() % WinWidth;
		line[0].y = rand() % WinHeight;
		line[1].x = rand() % WinWidth;
		line[1].y = rand() % WinHeight;
		seed++;
		cout << line[0].x <<" "<< line[0].y << endl;
		cout << line[1].x <<" "<< line[1].y << endl;
		glBegin(GL_LINES);
		glVertex2i(line[0].x, line[0].y);
		glVertex2i(line[1].x, line[1].y);
		glEnd();
		glFlush();
		
	}
	glFlush();
}

void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){//ȷ���ü���������
	if (iMode==6&&button == GLUT_LEFT_BUTTON&&action == GLUT_DOWN){	//���ȷ���˵�
		window.x = xMouse;
		window.y = WinHeight - yMouse;
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2f(window.x-20, window.y-20);
		glVertex2f(window.x+20, window.y-20);
		glVertex2f(window.x+20, window.y+20);
		glVertex2f(window.x-20, window.y+20);
		glEnd();
		glFlush();
		VBLineClip(window.x - 20, window.x + 20, window.y - 20, window.y + 20, line[0].x, line[0].y, line[1].x, line[1].y);
	}
}

void Init(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void ProcessMenu(int value){
	iMode = value;
	Display();
}

void Reshape(int w, int h){
	int winWidth = w;
	int winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}
int main(int argc, char* argv[]){
	//��ʼ��
	glutInit(&argc, argv);							//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//���ô��ڵ���ʾģ��
	glutInitWindowPosition(100, 100);				//�趨����λ��
	glutInitWindowSize(400, 400);					//�趨���ڴ�С
	glutCreateWindow("�������α任");
	//�˵���
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("ԭʼ״̬", 1);
	glutAddMenuEntry("ƽ��", 2);
	glutAddMenuEntry("����", 3);
	glutAddMenuEntry("�Գ�", 4);
	glutAddMenuEntry("����", 5);
	glutAddMenuEntry("Liang-Barsky", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//����
	Init();
	glutDisplayFunc(Display);						//��myDisplayָ��Ϊ��ǰ���ڵ���ʾ���ݺ�����ÿ�δ����ػ�ʱ���ã�
	glutMouseFunc(MousePlot);						//�����Ӧ����
	glutReshapeFunc(Reshape);	
	glutMainLoop();
	return 0;
}

