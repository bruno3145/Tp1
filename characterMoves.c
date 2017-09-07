#include "characterMoves.h"
#include <GL/glew.h>
#include<stdio.h>
#include <GL/freeglut.h>


void movimenta(int cima, int baixo, int esquerda, int direita, float *x, float *y){

	if(cima==1){
		glColor3f(0, 0, 1);
	*y+=0.5;
	}
	if(baixo==1){
		glColor3f(0, 1, 1);
	 *y-=0.5;
	}
	if(esquerda==1){
		glColor3f(1, 0, 1);
	 *x-=0.5;
	}
 if(direita==1){
				glColor3f(1, 0, 0);
	 *x+=0.5;
 }
}

void resetaMovimento(int *up, int *down, int *left, int *right){
	*up=0;
	*down=0;
	*left=0;
	*right=0;
}

void characterShape(int k, int l){

	glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(k, l, 0);
        glVertex3f(k+10, l, 0);
        glVertex3f(k+10, l+10, 0);
        glVertex3f(k, l+10, 0);
        glEnd();
}
