#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include "characterMoves.h"

GLuint texturaCharacter;

void loadCharacter() {
  texturaCharacter = SOIL_load_OGL_texture("snow.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  if (texturaCharacter == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void movimenta(int cima, int baixo, int esquerda, int direita, float *x, float *y, float* v_Player){

	if(cima==1){
	*y+=*v_Player;
	}
	if(baixo==1){
	 *y-=*v_Player;
	}
	if(esquerda==1){
	 *x-=*v_Player;
	}
 if(direita==1){
	 *x+=*v_Player;
 }
}

void resetaMovimento(int *up, int *down, int *left, int *right){
	*up=0;
	*down=0;
	*left=0;
	*right=0;
}
void characterShape(int* colidou){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaCharacter);
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0, 0); glVertex3f(0,-20,0);
		glTexCoord2f(1, 0); glVertex3f(-20,0,0);
		glTexCoord2f(1, 1); glVertex3f(0,20,0);
		glTexCoord2f(0, 1); glVertex3f(20,0,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
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
