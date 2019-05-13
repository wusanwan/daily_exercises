#include<glut.h>
#include<gl/GL.h>
#include<stdio.h>
#include<windows.h>
#include<gl/GLU.h>
#include<vector>
#include<algorithm>
#include<cfloat>
#include<iostream>
using namespace std;

//-------------------------全局变量-----------------------------------------------
int WinHeight = 300, WinWidth = 300;
int xcenter = WinHeight / 2, ycenter = WinWidth / 2;
int iMode = 1;

typedef float Color[3];
Color FillColor = { 0.0, 1.0, 0.0 };//填充颜色 
Color BorderColor = { 0.0, 0.0, 1.0 };//边界颜色

struct Point{
	float x, y;
};

vector<Point>pointset;		//用于存放点集
vector<int>vector_line_ymin;		//用来存放边的ymin

//---------------------有效边表算法------------------------------------------------
//结点
struct Tnode{
	float x;		//当前扫描线与有效边的交点的x坐标
	int ymax;		//有效边所在的最高扫描线
	float reciprocalK;	//当前有效边的斜率倒数
	Tnode* next;		//链表中下一个结点的指针
};
typedef Tnode AETnode;
typedef Tnode ETnode;


float Ymax(){
	int result = pointset[0].y;
	for (int i = 1; i < pointset.size(); i++){
		if (result < pointset[i].y)result = pointset[i].y;
	}
	return result;
}

float Ymin(){
	int result = pointset[0].y;
	for (int i = 1; i < pointset.size(); i++)
	if (result > pointset[i].y)result = pointset[i].y;
	return result;
}

void  LineYmin(){//计算完放在vector中，避免重复计算
	for (int i = 0; i < pointset.size(); i++)
		pointset[i].y < pointset[(i + 1) % pointset.size()].y ? vector_line_ymin.push_back(pointset[i].y) : vector_line_ymin.push_back(pointset[(i + 1) % pointset.size()].y);
}

bool cmp(ETnode a, ETnode b){
	if (a.x == b.x)return a.reciprocalK < b.reciprocalK;
	else return a.x < b.x;
}

void My_AET_Algorithm(){
	//构造ET
	int start = ceil(Ymin());
	int end = floor(Ymax());
	ETnode* list[400];
	for (int i = 0; i <= end - start; i++){
		list[i] = new ETnode;		//桶
		list[i]->next = NULL;
	}
	LineYmin();//计算所有边的ymin
	//扫描
	for (int h = start; h <= end; h++){
		for (int i = 0; i < pointset.size(); i++){
			if (h == vector_line_ymin[i]){
				ETnode *p = new ETnode;
				if (pointset[i].y == pointset[(i + 1) % pointset.size()].y)continue;
				else if (pointset[i].y < pointset[(i + 1) % pointset.size()].y){
					p->x = pointset[i].x;
					p->ymax = pointset[(i + 1) % pointset.size()].y;
				}
				else{
					p->x = pointset[(i + 1) % pointset.size()].x;
					p->ymax = pointset[i].y;
				}
				p->next = NULL;
				p->reciprocalK = (pointset[i].x - pointset[(i + 1) % pointset.size()].x) / (pointset[i].y - pointset[(i + 1) % pointset.size()].y);
				//排序插入
				ETnode *head = list[h-start],*q=list[h-start]->next;
				while (q!=NULL){
					if (cmp(*(q), *(p))){//不符合要求
						q = q->next;
						head = head->next;
					}
					else{//插入
						head->next = p;
						p->next = q;
						break;
					}
				}
				if (q==NULL){
					head->next = p;
					p->next = NULL;
				}
			}
		}		
	}
	//边表测试
	cout << "边表测试" << endl;
	for (int h = 0; h <= end - start; h++){
		if (!list[h]->next)cout << "";
		else {
			AETnode *p = list[h]->next;
			cout << h << " ";
			while (p!=NULL){
				cout << "结点 ";
				p = p->next;
			}
			cout << endl;
		}
	}
	//构造AET表
	AETnode *AET = new AETnode;
	AET->next = NULL;
	for (int h = 0; h <= end-start; h++){
		//合并	
		AETnode *p = AET, *q = list[h]->next;		
		while (p->next != NULL &&q != NULL){
			if (cmp(*(q), *(p->next))){
				cout << "目前为第" << h << "行，一共" << end - start << "行" << endl;
				cout << "插入节点 "<<endl;
				AETnode *temp = q;
				q = q->next;
				temp->next = p->next;
				p->next = temp;
			}
			p = p->next;
		}
		if (p->next == NULL&&q!=NULL)p->next = q;
		//测试
		/*
		p = AET->next;
		if (p==NULL)cout<<h << "指针空 "<<endl;
		else{
			int i = 0;
			while (p!=NULL){
				i++;
				cout << i << " " << p->x << endl;//野指针
				p = p->next;
			}
		}*/
		//删除y=ymax的边
		p = AET;
		q = AET->next;
		while (q!=NULL){
			if (h+start > q->ymax){
				cout << "目前为第" << h << "行，一共" << end - start << "行" << endl;
				cout << "删除边" << endl;				//测试点
				p->next = q->next;
				delete(q);
				q = p->next;
			}
			else{
				p = p->next;
				q = q->next;
			}
		}
		//填充
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POINTS);
		p = AET->next;
		while (p != NULL&&p->next != NULL){
			//cout << p->x << " " << p->next->x << endl;
			for (float f = p->x; f <= p->next->x; f++){//填充区间
				if (p == AET->next)glVertex2i(ceil(f), h + start);
				else glVertex2i(int(f), h + start);
			}
			p = p->next->next;
		}
		glEnd();
		glFlush();
		//更新
		p = AET->next;
		while (p!=NULL){
			p->x = p->x + p->reciprocalK;
			p = p->next;
		}
	}
}

//---------------------种子填充算法部分---------------------------------------------

void DrawBorder(){
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glEnd();
	glFlush();
	glLineWidth(10.0f);
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 1.0f);	//指定当前的绘图颜色
	for (int i = 0; i < pointset.size(); i++){
		glVertex2f(pointset[i].x, pointset[i].y);
	}
	glEnd();
	glFlush();
}

bool equal(Color c1, Color c2){
	bool flag = true;
	for (int i = 0;  i < 3; i++){
		if (fabs(c1[i] - c2[i])>1e-6){
			flag = false;
			break;
		}
	}
	return flag;
}

void BoundaryFill4(int x, int y)
{
	Color InteriorColor;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &InteriorColor);
	if (!equal(InteriorColor,BorderColor) && !equal(InteriorColor,FillColor))//没到边界也没被填充
	{
		glPointSize(3);
		glColor3f(0.0f, 1.0f, 0.0f);	//指定当前的绘图颜色
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
		glFlush();
		BoundaryFill4(x + 3, y);
		BoundaryFill4(x - 3, y);
		BoundaryFill4(x, y + 3);
		BoundaryFill4(x, y - 3);
	}
}
//边界填充算法-8连通域
void BoundaryFill8(int x, int y)
{
	Color InteriorColor;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &InteriorColor);
	if (!equal(InteriorColor, BorderColor) && !equal(InteriorColor, FillColor))
	{
		glPointSize(3);
		glColor3f(0.0f, 1.0f, 0.0f);	//指定当前的绘图颜色
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
		glFlush();
		BoundaryFill8(x + 3, y);
		BoundaryFill8(x - 3, y);
		BoundaryFill8(x, y + 3);
		BoundaryFill8(x, y - 3);
		BoundaryFill8(x - 3, y - 3);
		BoundaryFill8(x - 3, y + 3);
		BoundaryFill8(x + 3, y - 3);
		BoundaryFill8(x + 3, y + 3);
	}
}

//---------------------主体部分------------------------------------------------
void DrawSeedPoint(){	//用于初始化屏幕并在屏幕中心显示种子点，清空AET算法中的vector
	pointset.clear();
	vector_line_ymin.clear();
	glPointSize(1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2f(xcenter, ycenter);
	glEnd();
	glFlush();
	cout << "请按逆时针顺序选择三个以上的点，D键确定，C键清空" << endl;
}

void Display(void){
	glClear(GL_COLOR_BUFFER_BIT);				//用当前背景色填充窗口
	DrawSeedPoint();
	glFlush();
	if (iMode == 1)cout << "AET算法" << endl;
	else if (iMode == 2)cout << "4连通种子" << endl;
	else cout << "8连通种子" << endl;
}

void submit(){
	DrawBorder();
	if (iMode == 2)BoundaryFill4(xcenter,ycenter);
	else if (iMode == 3)BoundaryFill8(xcenter, ycenter);
	else if (iMode == 1)My_AET_Algorithm();
}

void Init(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void Keyboard(unsigned char key, int x, int y){
	//键盘交互函数——确定，清空
	switch (key){
	case'D':   //确定，进行填充
	case 'd':
		if (pointset.size() < 3)cout << "还需选择" << 3 - pointset.size() << "个点" << endl;
		else  submit();
		break;
	case'C':   //清空屏幕
	case'c':
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POINTS);
		glEnd();
		glFlush();
		DrawSeedPoint();
		break;
	}
}

void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){//确定多边形的多个点
	if (button == GLUT_LEFT_BUTTON&&action == GLUT_DOWN){	//左键确定端点
		Point temp;
		temp.x = xMouse;
		temp.y = WinHeight - yMouse;
		pointset.push_back(temp);
		glColor3f(1.0f, 1.0f, 1.0f);
		glPointSize(10);
		glBegin(GL_POINTS);
		glVertex2i(temp.x, temp.y);
		glEnd();
		glFlush();
	}
}

void ProcessMenu(int value){
	iMode = value;
	glutPostRedisplay();
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
	glutInitWindowSize(WinHeight,WinWidth);					//设定窗口大小
	glutCreateWindow("区域填充");
	//菜单项
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("AET", 1);
	glutAddMenuEntry("4连通种子填充", 2);
	glutAddMenuEntry("8连通种子填充", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//运行
	Init();
	glutDisplayFunc(Display);						//将myDisplay指定为当前窗口的显示内容函数（每次窗口重绘时调用）
	glutReshapeFunc(Reshape);
	glutMouseFunc(MousePlot);						//鼠标响应函数
	glutKeyboardFunc(Keyboard);						//键盘注册回调函数
	glutMainLoop();
	glutPostRedisplay();
	return 0;
}
