#include <GL/glew.h>
#include <GL/freeglut.h>
#include "characterMoves.h"

void movimenta(int cima, int baixo, int esquerda, int direita, float *x, float *y){

	if(cima==1){
	*y+=3;
	}
	if(baixo==1){
	 *y-=3;
	}
	if(esquerda==1){
	 *x-=3;
	}
 if(direita==1){
	 *x+=3;
 }
}

void resetaMovimento(int *up, int *down, int *left, int *right){
	*up=0;
	*down=0;
	*left=0;
	*right=0;
}
void characterShape(){

	glBegin(GL_POLYGON);
	glColor3f(1,1,1);
			glVertex2f(0,-20);
			glVertex2f(-20,0);
			glVertex2f(0,20);
			glColor3f(1,0,0);
			glVertex2f(20,0);
	glEnd();
}
void corrigeMouse(int x, int y,int* tMouseX, int* tMouseY, int gloHght){
/*trueMouseX=x*((float)LARGURA_DO_MUNDO/globalWidth);
  trueMouseY=(globalHeight-y)*((float)ALTURA_DO_MUNDO/globalHeight);*/
  *tMouseX = x;
  *tMouseY = (gloHght-y);
}
