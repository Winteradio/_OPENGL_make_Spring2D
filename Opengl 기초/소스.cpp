#include "GL/freeglut.h"
#include <windows.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <list>
#include <string>

//사용할 라이브러리를 지정해줍니다.
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32s.lib")

using namespace std;

// Position
float Pos[4][2];
// Velocity
float Vel[4][2];
// Force
float For[4][2];

// 음수 부여하는 난수
float R;

// 기타 변수들
float G = -0; // G 중력 상수
float Ks =5.0f; // Ks 감쇠 개수
float Kd =1.0f; // Kd 감쇠 계수
int num = 4; // num 점들의 개수

float mass = 5.00000f; // 점들의 무게
float L =0.50000f; // 용수철 원래 길이

GLuint Time = 10;
float time_step = 5.0000f;

void Reset() {
	for (int i = 0; i < num; i++) {
		for (int j = 0; j <= 1; j++) {
			For[i][j] = 0;
		}
	}
}

void Start() {
	for (int i = 0; i < num; i++) {
		Pos[i][0] = (float)((rand() % GLUT_WINDOW_WIDTH) - GLUT_WINDOW_WIDTH*0.5) / GLUT_WINDOW_WIDTH;
		Pos[i][1] = (float)((rand() % GLUT_WINDOW_HEIGHT) - GLUT_WINDOW_HEIGHT * 0.5) / GLUT_WINDOW_HEIGHT;
		for (int j = 0; j <= 1; j++) {
			Vel[i][j] = (float)((rand() % 10) -10) /10;

			cout << "[" << i << "," << j << "]" << Vel[i][j] << endl;
		}
	}
}

float Delta(float X, float Y) {
	return sqrt(pow(X, 2) + pow(Y, 2));
}

float Dot(float P[2], float V[2]) {
	return P[0] * V[0] + P[1] * V[1];
}

void Force(float pos[4][2], float vel[4][2]) {
	double F_Delta;
	float P_Delta[2];
	float V_Delta[2];
	for (int i = 0; i < num - 1; i++) {
		for (int j = i + 1; j < num; j++) {
			for (int t = 0; t <= 1; t++) {
				P_Delta[t] = pos[i][t] - pos[j][t];
				V_Delta[t] = pos[i][t] - pos[j][t];
			}
			F_Delta = -(Ks * ((Delta(P_Delta[0], P_Delta[1]) - L) + Kd * Dot(P_Delta, V_Delta) / Delta(P_Delta[0], P_Delta[1])))/mass;
			for (int t = 0; t <= 1; t++) {
				For[i][t] += F_Delta * P_Delta[t] / Delta(P_Delta[0], P_Delta[1]);
				For[j][t] += -For[i][t];
			}
		}
	}
}

void velocity() {
	for (int i = 0; i < num; i++) {
		for (int j = 0; j <= 1; j++) {
			if (j == 1) {
				For[i][j] += G;
			}
			Vel[i][j] += time_step/1000 *  For[i][j];
		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	{
		for (int i = 0; i < num; i++) {
			glVertex2f(Pos[i][0], Pos[i][1]);
		}
	}
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1.0);
	glBegin(GL_LINES);
	{for (int i = 0; i < num-1; i++) {
		for (int j = i + 1; j < num; j++) {
			glVertex2f(Pos[i][0], Pos[i][1]);
			glVertex2f(Pos[j][0], Pos[j][1]);
		}
	}}
	glEnd();
	glFlush();
}
void TimerFunc(int value) {
	for (int i = 0; i < num; i++) {
		for (int j = 0; j <= 1; j++) {
			Force(Pos, Vel);
			velocity();
			if (pow(Pos[i][j], 2) >= 0.8) {
				Vel[i][j] = -Vel[i][j];
			}
			Pos[i][j] += Vel[i][j] * time_step / 1000;
			Reset();
		}
	}
	glutPostRedisplay();
	glutTimerFunc(Time, TimerFunc, 1);
}

int main(int argc, char** argv) {
	Start();
	glutInit(&argc, argv);
	glutInitWindowSize(300, 500);
	glutInitWindowPosition(120, 50);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(display);
	glClearColor(0.0, 0.0, 0.0, 0);
	glutTimerFunc(0, TimerFunc, 1);
	glutMainLoop();
}