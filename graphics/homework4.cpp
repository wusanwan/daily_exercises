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
Point window;	//裁剪窗口中心点
Point s0[4] = { 195.0f, 195.0f, 205.0f, 195.0f, 205.0f, 205.0f ,195.0f, 205.0f };
float square0[4] = { 195.0f, 195.0f, 205.0f, 205.0f };//初始状态
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
void my_translation(){//平移
	cout << "向右平移50个单位，向上平移100个单位" << endl;
	float dx = 50,dy =100;//平移更改量
	for (int i = 0; i < 4; i++){
		if (i == 0 || i == 2)square1[i] = square0[i] + dx;
		else square1[i] = square0[i] + dy;
	}
	DrawSquare();
}

void my_scale(){//比例
	cout << "绘图点是屏幕中心,按比例将原正方形放大5倍" << endl;
	float xmultiple = 5 , ymultiple = 5;		
	for (int i = 0; i < 4; i++){
		if (i == 0 || i == 2)square1[i] = (square0[i]-cx)*xmultiple + cx;
		else square1[i] = (square0[i] - cy)*ymultiple + cy;
	}
	DrawSquare();
}

void my_symmetry(){//对称
	cout << "按照原正方形左下角的点中心对称" << endl;
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
	float c=0.5f;//是水平方向的错切，即沿x轴方向关于y的错切，若>0，则沿 + x轴方向错切，若<0，则沿 - x轴方向错切。
	cout << "水平方向沿+x方向错切，参数为0.5" << endl;
	for (int i = 0; i < 4; i++){
		s0[i].x = s0[i].x + c*(s0[i].y-s0[0].y);//减去s0[0].y是因为把s0[0].y作为y=0的轴
	}
	Draw();
}

//Liang-Barsky算法开始
BOOL LBLineClipTest(float p, float q, float &u1, float&u2){
	float r = 0.0;
	if (p < -0.00001){//p小于0时比较最大值
		r = q / p;
		if (r > u2)return false;//umax小于umin才有在窗口内的部分
		else if (r > u1)u1 = r;
	}
	else if (p > 0.00001){	//p大于0时比较最小值
		r = q / p;
		if (r < u1)return false;
		else if (r < u2)u2 = r;
	}
	else if (q < 0.0)return false;//处理p等于0的情况
	return true;
}

void VBLineClip(float XL, float XR, float YB, float YT, float x1, float y1, float x2, float y2){
	float u1, u2, dx, dy;
	dx = x2 - x1;
	dy = y2 - y1;
	u1 = 0.0; u2 = 1.0;
	if (LBLineClipTest(-dx, x1 - XL, u1, u2)){//左边界交点
		if (LBLineClipTest(dx, XR - x1, u1, u2)){//右边界交点
			if (LBLineClipTest(-dy, y1 - YB, u1, u2)){//下边界交点
				if (LBLineClipTest(dy, YT - y1, u1, u2)){//上边界交点
					//u2大于1说明线的右端点与框无交点（端点在框内），所以直接输出即可
					if (u2< 1.0) {//说明线的右端点超出框外了，此时需要剪切，更新x2,y2为线框交点。
						x2 = x1 + u2*dx;
						y2 = y1 + u2*dy;
					}
					if (u1>0.0) {//u1同理
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
	
	glColor3f(1.0f, 0.0f, 0.0f);	//指定当前的绘图颜色
	if (iMode == 1)DrawSquare();
	else if (iMode == 2)my_translation();
	else if (iMode == 3)my_scale();
	else if (iMode == 4)my_symmetry();
	else if (iMode == 5)my_shear();
	else{
		cout << "换条直线请右键重新选择liang-barsky算法" << endl;
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

void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){//确定裁剪窗口中心
	if (iMode==6&&button == GLUT_LEFT_BUTTON&&action == GLUT_DOWN){	//左键确定端点
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
	//初始化
	glutInit(&argc, argv);							//初始化GLUT库
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//设置窗口的显示模型
	glutInitWindowPosition(100, 100);				//设定窗口位置
	glutInitWindowSize(400, 400);					//设定窗口大小
	glutCreateWindow("基本几何变换");
	//菜单项
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("原始状态", 1);
	glutAddMenuEntry("平移", 2);
	glutAddMenuEntry("比例", 3);
	glutAddMenuEntry("对称", 4);
	glutAddMenuEntry("错切", 5);
	glutAddMenuEntry("Liang-Barsky", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//运行
	Init();
	glutDisplayFunc(Display);						//将myDisplay指定为当前窗口的显示内容函数（每次窗口重绘时调用）
	glutMouseFunc(MousePlot);						//鼠标响应函数
	glutReshapeFunc(Reshape);	
	glutMainLoop();
	return 0;
}

