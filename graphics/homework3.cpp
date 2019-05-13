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

//-------------------------ȫ�ֱ���-----------------------------------------------
int WinHeight = 300, WinWidth = 300;
int xcenter = WinHeight / 2, ycenter = WinWidth / 2;
int iMode = 1;

typedef float Color[3];
Color FillColor = { 0.0, 1.0, 0.0 };//�����ɫ 
Color BorderColor = { 0.0, 0.0, 1.0 };//�߽���ɫ

struct Point{
	float x, y;
};

vector<Point>pointset;		//���ڴ�ŵ㼯
vector<int>vector_line_ymin;		//������űߵ�ymin

//---------------------��Ч�߱��㷨------------------------------------------------
//���
struct Tnode{
	float x;		//��ǰɨ��������Ч�ߵĽ����x����
	int ymax;		//��Ч�����ڵ����ɨ����
	float reciprocalK;	//��ǰ��Ч�ߵ�б�ʵ���
	Tnode* next;		//��������һ������ָ��
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

void  LineYmin(){//���������vector�У������ظ�����
	for (int i = 0; i < pointset.size(); i++)
		pointset[i].y < pointset[(i + 1) % pointset.size()].y ? vector_line_ymin.push_back(pointset[i].y) : vector_line_ymin.push_back(pointset[(i + 1) % pointset.size()].y);
}

bool cmp(ETnode a, ETnode b){
	if (a.x == b.x)return a.reciprocalK < b.reciprocalK;
	else return a.x < b.x;
}

void My_AET_Algorithm(){
	//����ET
	int start = ceil(Ymin());
	int end = floor(Ymax());
	ETnode* list[400];
	for (int i = 0; i <= end - start; i++){
		list[i] = new ETnode;		//Ͱ
		list[i]->next = NULL;
	}
	LineYmin();//�������бߵ�ymin
	//ɨ��
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
				//�������
				ETnode *head = list[h-start],*q=list[h-start]->next;
				while (q!=NULL){
					if (cmp(*(q), *(p))){//������Ҫ��
						q = q->next;
						head = head->next;
					}
					else{//����
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
	//�߱����
	cout << "�߱����" << endl;
	for (int h = 0; h <= end - start; h++){
		if (!list[h]->next)cout << "";
		else {
			AETnode *p = list[h]->next;
			cout << h << " ";
			while (p!=NULL){
				cout << "��� ";
				p = p->next;
			}
			cout << endl;
		}
	}
	//����AET��
	AETnode *AET = new AETnode;
	AET->next = NULL;
	for (int h = 0; h <= end-start; h++){
		//�ϲ�	
		AETnode *p = AET, *q = list[h]->next;		
		while (p->next != NULL &&q != NULL){
			if (cmp(*(q), *(p->next))){
				cout << "ĿǰΪ��" << h << "�У�һ��" << end - start << "��" << endl;
				cout << "����ڵ� "<<endl;
				AETnode *temp = q;
				q = q->next;
				temp->next = p->next;
				p->next = temp;
			}
			p = p->next;
		}
		if (p->next == NULL&&q!=NULL)p->next = q;
		//����
		/*
		p = AET->next;
		if (p==NULL)cout<<h << "ָ��� "<<endl;
		else{
			int i = 0;
			while (p!=NULL){
				i++;
				cout << i << " " << p->x << endl;//Ұָ��
				p = p->next;
			}
		}*/
		//ɾ��y=ymax�ı�
		p = AET;
		q = AET->next;
		while (q!=NULL){
			if (h+start > q->ymax){
				cout << "ĿǰΪ��" << h << "�У�һ��" << end - start << "��" << endl;
				cout << "ɾ����" << endl;				//���Ե�
				p->next = q->next;
				delete(q);
				q = p->next;
			}
			else{
				p = p->next;
				q = q->next;
			}
		}
		//���
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POINTS);
		p = AET->next;
		while (p != NULL&&p->next != NULL){
			//cout << p->x << " " << p->next->x << endl;
			for (float f = p->x; f <= p->next->x; f++){//�������
				if (p == AET->next)glVertex2i(ceil(f), h + start);
				else glVertex2i(int(f), h + start);
			}
			p = p->next->next;
		}
		glEnd();
		glFlush();
		//����
		p = AET->next;
		while (p!=NULL){
			p->x = p->x + p->reciprocalK;
			p = p->next;
		}
	}
}

//---------------------��������㷨����---------------------------------------------

void DrawBorder(){
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glEnd();
	glFlush();
	glLineWidth(10.0f);
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 1.0f);	//ָ����ǰ�Ļ�ͼ��ɫ
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
	if (!equal(InteriorColor,BorderColor) && !equal(InteriorColor,FillColor))//û���߽�Ҳû�����
	{
		glPointSize(3);
		glColor3f(0.0f, 1.0f, 0.0f);	//ָ����ǰ�Ļ�ͼ��ɫ
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
//�߽�����㷨-8��ͨ��
void BoundaryFill8(int x, int y)
{
	Color InteriorColor;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &InteriorColor);
	if (!equal(InteriorColor, BorderColor) && !equal(InteriorColor, FillColor))
	{
		glPointSize(3);
		glColor3f(0.0f, 1.0f, 0.0f);	//ָ����ǰ�Ļ�ͼ��ɫ
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

//---------------------���岿��------------------------------------------------
void DrawSeedPoint(){	//���ڳ�ʼ����Ļ������Ļ������ʾ���ӵ㣬���AET�㷨�е�vector
	pointset.clear();
	vector_line_ymin.clear();
	glPointSize(1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2f(xcenter, ycenter);
	glEnd();
	glFlush();
	cout << "�밴��ʱ��˳��ѡ���������ϵĵ㣬D��ȷ����C�����" << endl;
}

void Display(void){
	glClear(GL_COLOR_BUFFER_BIT);				//�õ�ǰ����ɫ��䴰��
	DrawSeedPoint();
	glFlush();
	if (iMode == 1)cout << "AET�㷨" << endl;
	else if (iMode == 2)cout << "4��ͨ����" << endl;
	else cout << "8��ͨ����" << endl;
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
	//���̽�����������ȷ�������
	switch (key){
	case'D':   //ȷ�����������
	case 'd':
		if (pointset.size() < 3)cout << "����ѡ��" << 3 - pointset.size() << "����" << endl;
		else  submit();
		break;
	case'C':   //�����Ļ
	case'c':
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POINTS);
		glEnd();
		glFlush();
		DrawSeedPoint();
		break;
	}
}

void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){//ȷ������εĶ����
	if (button == GLUT_LEFT_BUTTON&&action == GLUT_DOWN){	//���ȷ���˵�
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
	//��ʼ��
	glutInit(&argc, argv);							//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);	//���ô��ڵ���ʾģ��
	glutInitWindowPosition(100, 100);				//�趨����λ��
	glutInitWindowSize(WinHeight,WinWidth);					//�趨���ڴ�С
	glutCreateWindow("�������");
	//�˵���
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("AET", 1);
	glutAddMenuEntry("4��ͨ�������", 2);
	glutAddMenuEntry("8��ͨ�������", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//����
	Init();
	glutDisplayFunc(Display);						//��myDisplayָ��Ϊ��ǰ���ڵ���ʾ���ݺ�����ÿ�δ����ػ�ʱ���ã�
	glutReshapeFunc(Reshape);
	glutMouseFunc(MousePlot);						//�����Ӧ����
	glutKeyboardFunc(Keyboard);						//����ע��ص�����
	glutMainLoop();
	glutPostRedisplay();
	return 0;
}
