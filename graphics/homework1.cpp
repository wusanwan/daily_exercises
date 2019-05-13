#include<glut.h>
#include<gl/GL.h>
#include<stdio.h>
#include<windows.h>
#include<graphics.h>
#include<gl/GLU.h>

float x0 = 200, y0 = 200;		//C位
float x1 = 250, y1 = 150;		//初始位置
int iMode = 1;
int pointcount = 0;
int WinHeight = 400, WinWidth = 400;

void DDALine(){
	int dx, dy, epsl,type;
	float x, y, xIncre, yIncre,k;
	dx = x1 - x0;dy = y1 - y0;			//确定位移
	x = x0;y = y0;						//确定起始点
	if (abs(dx) > abs(dy))epsl = abs(dx); //选择主方向
	else epsl = abs(dy);
	xIncre = (float)dx / (float)epsl;		//确定步长
	yIncre = (float)dy / (float)epsl;
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(1);
	for (k = 0; k < epsl; k++){
		glBegin(GL_POINTS);
		glVertex2i(int(x + 0.5), (int)(y + 0.5));	//四舍五入画点
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
	if (dx > abs(dy)){//斜率（0,1）
		flag = 0;
		if (dy < 0){//斜率为负的情况
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
	else{//补全斜率>1的情况
		flag = 0;
		if (dy < 0){//斜率为负的情况
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
	if (dx > abs(dy)){//斜率（0,1）
		e = -dx;
		flag = 0;
		if (dy < 0){ //斜率为负的情况
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
	else{ //补全斜率>1的情况
		flag = 0;
		if (dy < 0)
		{ //斜率为负的情况
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
		else{//斜率为负的情况
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
	glClear(GL_COLOR_BUFFER_BIT);				//用当前背景色填充窗口
	glColor3f(1.0f, 0.0f, 0.0f);	//指定当前的绘图颜色
	if (iMode == 1)DDALine();
	else if (iMode == 2) MidBresenhamLine();
	else BresenhamLine();
	glFlush();					//执行前面的OPENGL命令
}
void Keyboard(unsigned char key, int x, int y){//键盘交互函数
	switch (key){
		case'W':   //上移
		case 'w':
			y1 += 10;
			Display();
			break;
		case'S':   //下移
		case's':
			y1 -= 10;
			Display();
			break;
		case'A':  //左移
		case'a':
			x1 -= 10;
			Display();
			break;
		case'D':   //右移
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
void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){//确定直线的两点
	if (button == GLUT_LEFT_BUTTON&&action == GLUT_DOWN){	//左键确定端点
		if (pointcount == 0 || pointcount == 2){
			pointcount = 1;
			x0 = xMouse;
			y0 = WinHeight - yMouse;
		}
		else{
			pointcount = 2;
			x1 = xMouse;
			y1 = WinWidth - yMouse;
			glutPostRedisplay();			//指定窗口重新绘制
		}
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
	glutCreateWindow("直线的扫描转换");
	//菜单项
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("DDA算法", 1);
	glutAddMenuEntry("中点Bresenham算法", 2);
	glutAddMenuEntry("Bresenham算法", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//运行
	Init();				
	glutDisplayFunc(&Display);						//将myDisplay指定为当前窗口的显示内容函数（每次窗口重绘时调用）
	glutReshapeFunc(Reshape);
	glutMouseFunc(MousePlot);						//鼠标响应函数
	//移动响应函数因为效果问题被删去
	glutKeyboardFunc(Keyboard);						//键盘注册回调函数
	glutMainLoop();
	glutPostRedisplay();
	return 0;
}

