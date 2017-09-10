#include <GL/glew.h>
#include <GL/freeglut.h>
#include "characterMoves.h"

void movimenta(int cima, int baixo, int esquerda, int direita, float *x, float *y){

	if(cima==1){
	*y+=8;
	}
	if(baixo==1){
	 *y-=8;
	}
	if(esquerda==1){
	 *x-=8;
	}
 if(direita==1){
	 *x+=8;
 }
}

void resetaMovimento(int *up, int *down, int *left, int *right){
	*up=0;
	*down=0;
	*left=0;
	*right=0;
}
void characterShape(int* colidou){
	if(*colidou==1){
		glBegin(GL_POLYGON);
		glColor3f(0,0,0);
				glVertex2f(0,-20);
				glVertex2f(-20,0);
				glVertex2f(0,20);
				glColor3f(0,1,1);
				glVertex2f(20,0);
		glEnd();
		*colidou = 0;
	}
	else
	glBegin(GL_POLYGON);
	glColor3f(1,1,1);
			glVertex2f(0,-20);
			glVertex2f(-20,0);
			glVertex2f(0,20);
			glColor3f(1,0,0);
			glVertex2f(20,0);
	glEnd();
	}

	void randomPosition(int* x, int* y){
		int r = (rand()%21-10);
		*x = r;
		r = (rand()%21-10);
		*y = r;
	}

void corrigeMouse(int x, int y,int* tMouseX, int* tMouseY, int gloHght){
/*trueMouseX=x*((float)LARGURA_DO_MUNDO/globalWidth);
  trueMouseY=(globalHeight-y)*((float)ALTURA_DO_MUNDO/globalHeight);*/
  *tMouseX = x;
  *tMouseY = (gloHght-y);
}
