#include<glut.h>
#include<gl/GL.h>
#include<stdio.h>
#include<windows.h>
#include<graphics.h>
#include<gl/GLU.h>
#include<cmath>
int WinHeight = 400, WinWidth = 400;
float centerx = WinHeight/2, centery = WinWidth/2;		//C位
float rx = 250, ry = 150;		//初始位置
int iMode = 1;
int a = 80, b = 45;

void CirclePoint(int x,int y){
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex2i(centerx +x, centery+y);
	glVertex2i(centerx +x, centery-y);
	glVertex2i(centerx -x, centery+y);
	glVertex2i(centerx -x, centery-y);
	glVertex2i(centerx +y, centery+x);
	glVertex2i(centerx -y, centery+x);
	glVertex2i(centerx +y, centery-x);
	glVertex2i(centerx -y, centery-x);
	glEnd();
	glFlush();
}

void MidBresenhamCircle(){
	int x, y, d, r;
	r = int(sqrt((centerx - rx)*(centerx - rx) + (centery - ry)*(centery - ry)));
	x = 0; y =r; d = 1 - r;
	while (x <= y){
		CirclePoint(x, y);
		if (d < 0)d += 2 * x + 3;
		else{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
}
void CirclePoint4(int x, int y){
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex2i(centerx + x, centery + y);
	glVertex2i(centerx + x, centery - y);
	glVertex2i(centerx - x, centery + y);
	glVertex2i(centerx - x, centery - y);
	glEnd();
	glFlush();
}
void MidBresenhamEllipse(){
	int x, y;
	float d1, d2;
	x = 0; y = b;
	d1 = b*b + a*a*(-b + 0.25);
	CirclePoint4(x, y);
	while (b*b*(x + 1) < a*a*(y - 0.5)){
		if (d1 <= 0){
			d1 += b*b*(2 * x + 3);
			x++;
		}
		else{
			d1 += b*b*(2 * x + 3) + a*a*(-2 * y + 2);
			x++; 
			y--;
		}
		CirclePoint4(x, y);
	}
	d2 = b*b*(0.5 + x)*(x + 0.5) + a*a*(y - 1)*(y - 1) - a*a*b*b;
	while (y > 0){
		if (d2 <= 0){
			d2 += b*b*(2 * x + 2) + a*a*(-2 * y + 3);
			x++; y--;
		}
		else{
			d2 += a*a*(-2 * y + 3);
			y--;
		}
		CirclePoint4(x, y);
	}
}


void Display(void){
	glClear(GL_COLOR_BUFFER_BIT);				//用当前背景色填充窗口
	glColor3f(1.0f, 0.0f, 0.0f);	//指定当前的绘图颜色
	if (iMode == 1)MidBresenhamCircle();
	else if (iMode == 2) MidBresenhamEllipse();
	glFlush();					//执行前面的OPENGL命令
}
void Init(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void Keyboard(unsigned char key,int x,int y){
	//键盘交互函数
	if (iMode == 2){
		switch (key){
			case'W':   //上移
			case 'w':
				b += 5;
				glutPostRedisplay();
				break;
			case'S':   //下移
			case's':
				b -= 5;
				glutPostRedisplay();
				break;
			case'A':  //左移
			case'a':
				a -= 5;
				glutPostRedisplay();
				break;
			case'D':   //右移
			case'd':
				a += 5;
				glutPostRedisplay();
				break;
			}
	}
	
}
void Reshape(int w, int h){
	int winWidth = w;
	int winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}
void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){//确定半径
	if (iMode==1 && button == GLUT_LEFT_BUTTON&&action == GLUT_DOWN){	//左键确定端点
			rx = xMouse;
			ry = WinWidth - yMouse;
			glutPostRedisplay();			//指定窗口重新绘制
	}
}
void PassiveMouseMove(GLint xMouse, GLint yMouse){
	if (iMode == 1){
		rx = xMouse;
		ry = WinWidth - yMouse;
		glutPostRedisplay();
	}
}

void ProcessMenu(int value){
	iMode = value;
	glutPostRedisplay();
}
int main(int argc, char* argv[]){
	//初始化
	glutInit(&argc, argv);							//初始化GLUT库
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//设置窗口的显示模型
	glutInitWindowPosition(100, 100);				//设定窗口位置
	glutInitWindowSize(400, 400);					//设定窗口大小
	glutCreateWindow("圆、椭圆的扫描转换");
	//菜单项
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("圆", 1);
	glutAddMenuEntry("椭圆", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//运行
	Init();
	glutDisplayFunc(&Display);						//将myDisplay指定为当前窗口的显示内容函数（每次窗口重绘时调用）
	glutReshapeFunc(Reshape);
	glutMouseFunc(MousePlot);						//鼠标响应函数
	glutPassiveMotionFunc(PassiveMouseMove);      //鼠标移动响应函数
	glutKeyboardFunc(Keyboard);						//键盘注册回调函数
	glutMainLoop();
	glutPostRedisplay();
	return 0;
}

