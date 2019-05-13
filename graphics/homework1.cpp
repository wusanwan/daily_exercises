#include<glut.h>
#include<gl/GL.h>
#include<stdio.h>
#include<windows.h>
#include<graphics.h>
#include<gl/GLU.h>

float x0 = 200, y0 = 200;		//Cλ
float x1 = 250, y1 = 150;		//��ʼλ��
int iMode = 1;
int pointcount = 0;
int WinHeight = 400, WinWidth = 400;

void DDALine(){
	int dx, dy, epsl,type;
	float x, y, xIncre, yIncre,k;
	dx = x1 - x0;dy = y1 - y0;			//ȷ��λ��
	x = x0;y = y0;						//ȷ����ʼ��
	if (abs(dx) > abs(dy))epsl = abs(dx); //ѡ��������
	else epsl = abs(dy);
	xIncre = (float)dx / (float)epsl;		//ȷ������
	yIncre = (float)dy / (float)epsl;
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(1);
	for (k = 0; k < epsl; k++){
		glBegin(GL_POINTS);
		glVertex2i(int(x + 0.5), (int)(y + 0.5));	//�������뻭��
		glEnd();
		x += xIncre;	
		y += yIncre;
	}
	glFlush();
}

void MidBresenhamLine(){
	int dx, dy, UpIncre, DownIncre, x, y, d, flag=0;
	if (x0>x1){
		int temp;
		temp = x0; x0 = x1; x1 = temp;
		temp = y0; y0 = y1; y1 = temp;
	}
	x = x0; y = y0;
	dx = x1 - x0;
	dy = y1 - y0;
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(1);
	if (dx > abs(dy)){//б�ʣ�0,1��
		flag = 0;
		if (dy < 0){//б��Ϊ�������
			dy = -dy;
			flag = 1;
		}
		d = dx - 2 * dy;
		UpIncre = 2 * dx - 2 * dy;		
		DownIncre = -2 * dy;			
		while (x <= x1){
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();
			x++;
			if (d < 0){
				if (flag) y--;
				else y++;
				d += UpIncre;
			}
			else d += DownIncre;
		}
	}
	else{//��ȫб��>1�����
		flag = 0;
		if (dy < 0){//б��Ϊ�������
			dy = -dy;
			flag = 1;
		}
		d = dy - 2 * dx;
		UpIncre = 2 * dy - 2 * dx;		
		DownIncre = -2 * dx;			
		
		if (flag) {
			while (y >= y1){
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				y--;
				if (d < 0){
					x++;
					d += UpIncre;
				}
				else d += DownIncre;
			}
		}
		else {
			while (y <= y1){
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				y++;
				if (d < 0){
					x++;
					d += UpIncre;
				}
				else d += DownIncre;
			}
		}
	}
}

void BresenhamLine(){
	int dx, dy, e, x, y, flag = 0;
	if (x0 > x1){
		int temp;
		temp = x0; x0 = x1; x1 = temp;
		temp = y0; y0 = y1; y1 = temp;
	}
	x = x0; y = y0;
	dx = x1 - x0; dy = y1 - y0;
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(1);
	if (dx > abs(dy)){//б�ʣ�0,1��
		e = -dx;
		flag = 0;
		if (dy < 0){ //б��Ϊ�������
			dy = -dy;
			flag = 1;
		}
		while (x <= x1){
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();
			x++;
			e = e + 2 * dy;
			if (e > 0){
				if (flag)y--;
				else y++;
				e = e - 2 * dx;
			}
		}
	}
	else{ //��ȫб��>1�����
		flag = 0;
		if (dy < 0)
		{ //б��Ϊ�������
			dy = -dy;
			flag = 1;
		}
		e = -dy;
		if (flag){
			while (y >= y1){
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				y--;
				e = e + 2 * dx;
				if (e > 0){
					x++;
					e = e - 2 * dy;
				}
			}
		}
		else{//б��Ϊ�������
			while (y <= y1){
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				y++;
				e = e + 2 * dx;
				if (e > 0){
					x++;
					e = e - 2 * dy;
				}
			}
		}
	}
}

void Display(void){
	glClear(GL_COLOR_BUFFER_BIT);				//�õ�ǰ����ɫ��䴰��
	glColor3f(1.0f, 0.0f, 0.0f);	//ָ����ǰ�Ļ�ͼ��ɫ
	if (iMode == 1)DDALine();
	else if (iMode == 2) MidBresenhamLine();
	else BresenhamLine();
	glFlush();					//ִ��ǰ���OPENGL����
}
void Keyboard(unsigned char key, int x, int y){//���̽�������
	switch (key){
		case'W':   //����
		case 'w':
			y1 += 10;
			Display();
			break;
		case'S':   //����
		case's':
			y1 -= 10;
			Display();
			break;
		case'A':  //����
		case'a':
			x1 -= 10;
			Display();
			break;
		case'D':   //����
		case'd':
			x1 += 10;
			Display();
			break;
	}
}

void Init(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void Reshape(int w, int h){
	int winWidth = w;
	int winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}
void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){//ȷ��ֱ�ߵ�����
	if (button == GLUT_LEFT_BUTTON&&action == GLUT_DOWN){	//���ȷ���˵�
		if (pointcount == 0 || pointcount == 2){
			pointcount = 1;
			x0 = xMouse;
			y0 = WinHeight - yMouse;
		}
		else{
			pointcount = 2;
			x1 = xMouse;
			y1 = WinWidth - yMouse;
			glutPostRedisplay();			//ָ���������»���
		}
	}
}
void ProcessMenu(int value){
	iMode = value;
	glutPostRedisplay();
}
int main(int argc, char* argv[]){
	//��ʼ��
	glutInit(&argc, argv);							//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//���ô��ڵ���ʾģ��
	glutInitWindowPosition(100, 100);				//�趨����λ��
	glutInitWindowSize(400, 400);					//�趨���ڴ�С
	glutCreateWindow("ֱ�ߵ�ɨ��ת��");
	//�˵���
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("DDA�㷨", 1);
	glutAddMenuEntry("�е�Bresenham�㷨", 2);
	glutAddMenuEntry("Bresenham�㷨", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//����
	Init();				
	glutDisplayFunc(&Display);						//��myDisplayָ��Ϊ��ǰ���ڵ���ʾ���ݺ�����ÿ�δ����ػ�ʱ���ã�
	glutReshapeFunc(Reshape);
	glutMouseFunc(MousePlot);						//�����Ӧ����
	//�ƶ���Ӧ������ΪЧ�����ⱻɾȥ
	glutKeyboardFunc(Keyboard);						//����ע��ص�����
	glutMainLoop();
	glutPostRedisplay();
	return 0;
}

