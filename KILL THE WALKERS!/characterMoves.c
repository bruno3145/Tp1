#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include "characterMoves.h"

GLuint texturaPlayerF,texturaPlayerB,texturaPlayerR,texturaPlayerL;

void loadPlayerFront() {
  texturaPlayerF = SOIL_load_OGL_texture("texturaPlayerFront.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  if (texturaPlayerF == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadPlayerBack() {
  texturaPlayerB = SOIL_load_OGL_texture("texturaPlayerBack.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  if (texturaPlayerB == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadPlayerRight() {
  texturaPlayerR = SOIL_load_OGL_texture("texturaPlayerRight.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  if (texturaPlayerR == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadPlayerLeft() {
  texturaPlayerL = SOIL_load_OGL_texture("texturaPlayerLeft.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  if (texturaPlayerL == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void movimenta(int cima, int baixo, int esquerda, int direita, float *x, float *y, float velocidadePlayer){

	if(cima==1){
	*y+=velocidadePlayer;
	}
	if(baixo==1){
	 *y-=velocidadePlayer;
	}
	if(esquerda==1){
	 *x-=velocidadePlayer;
	}
 if(direita==1){
	 *x+=velocidadePlayer;
 }
}

void resetaMovimento(int *up, int *down, int *left, int *right){
	*up=0;
	*down=0;
	*left=0;
	*right=0;
}
void characterShape(int tMouseX, int tMouseY, float axix, float axiy){
  float vetorx=(tMouseX-axix);
  float vetory=(tMouseY-axiy);

    glEnable(GL_TEXTURE_2D);

  if(-vetory > vetorx && -vetory > -vetorx && -vetory > vetory){ // player está a direta do inimigo
        glBindTexture(GL_TEXTURE_2D, texturaPlayerF);
    }
    else if(vetory > vetorx && vetory > -vetorx && vetory > -vetory){
        glBindTexture(GL_TEXTURE_2D, texturaPlayerB);
    }
    else if(-vetorx > vetorx && -vetorx > vetory && -vetorx > -vetory){
        glBindTexture(GL_TEXTURE_2D, texturaPlayerL);
    } else
    {
        glBindTexture(GL_TEXTURE_2D, texturaPlayerR);
    }
    glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0, 0); glVertex3f(-15,-20 ,0);
      glTexCoord2f(1, 0); glVertex3f(15,-20,0);
      glTexCoord2f(1, 1); glVertex3f(15,20,0);
      glTexCoord2f(0, 1); glVertex3f(-15,20,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

	/*glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0, 0); glVertex3f(0,-20,0);
		glTexCoord2f(1, 0); glVertex3f(-20,0,0);
		glTexCoord2f(1, 1); glVertex3f(0,20,0);
		glTexCoord2f(0, 1); glVertex3f(20,0,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);*/
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
