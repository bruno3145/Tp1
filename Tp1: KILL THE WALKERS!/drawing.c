#include "textures.h"
#include "drawing.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include<string.h>
#include <SOIL/SOIL.h>


extern GLuint texturaItem, texturaShot;
extern GLuint texturaPause, texturaCredits, texturaInstrucoes, texturaGameOver, texturaMenu, texturaArrowMenu;
extern GLuint texturaLifeBar1, texturaLifeBar2, texturaLifeBar3, texturaBackground, texturaBackground2;
extern int pontuacao, scoreBoard, menu, pause, credits;
extern char stringPontuacao[100];

void drawLifeBar(int i){

    glEnable(GL_TEXTURE_2D);
    if(i==1){
      glBindTexture(GL_TEXTURE_2D, texturaLifeBar1);
    }
    else if(i==2){
      glBindTexture(GL_TEXTURE_2D, texturaLifeBar2);
    }
    else{
      glBindTexture(GL_TEXTURE_2D, texturaLifeBar3);
    }
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(-60, -20,0);
        glTexCoord2f(1, 0); glVertex3f( 60, -20,0);
        glTexCoord2f(1, 1); glVertex3f( 60,20,0);
        glTexCoord2f(0, 1); glVertex3f(-60, 20,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawScreenText(float x, float y, int r, int g, int b, const char*string){
	int j = strlen(string);
  glColor3f(r,g,b);
	glRasterPos2f(x,y);
	int i;
	for(i=0;i<j;i++){
    if(scoreBoard==1){
      	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
    else
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
	}
  glColor3f(1,1,1);
}

void drawHud(int* globalWidth, int* globalHeight){
    char shud[6]={'S','C','O','R','E',':'};
    sprintf(stringPontuacao, "%d", pontuacao);
    drawScreenText((*globalWidth/2)+20,*globalHeight-45,0,0,0,stringPontuacao);
    sprintf(stringPontuacao, "%s", shud);
    drawScreenText((*globalWidth/2)-57,*globalHeight-45,0,0,0,stringPontuacao);
}

void drawOnScreenSize(int opcao, int* globalWidth, int* globalHeight){
    glEnable(GL_TEXTURE_2D);

    switch (opcao) {
      case 1:
        glBindTexture(GL_TEXTURE_2D, texturaMenu);
        break;
      case 2:
        glBindTexture(GL_TEXTURE_2D, texturaInstrucoes);
        break;
      case 3:
        glBindTexture(GL_TEXTURE_2D, texturaCredits);
        break;
      case 4:
        glBindTexture(GL_TEXTURE_2D, texturaPause);
        break;
      case 5:
        glDisable(GL_TEXTURE_2D);
        glColor3f(0,0,0);
        break;
      case 6:
        glBindTexture(GL_TEXTURE_2D, texturaBackground);
        break;
      case 7:
        glBindTexture(GL_TEXTURE_2D, texturaBackground2);
        break;
      case 8:
        glBindTexture(GL_TEXTURE_2D, texturaGameOver);
        break;
      default:
        break;
    }

    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0,0);
        glTexCoord2f(1, 0); glVertex3f( *globalWidth, 0,0);
        glTexCoord2f(1, 1); glVertex3f( *globalWidth,  *globalHeight,0);
        glTexCoord2f(0, 1); glVertex3f(0,  *globalHeight,0);
    glEnd();
    if(opcao!=5){
        glDisable(GL_TEXTURE_2D);
    }
    else{
      glColor3f(1,1,1);
    }
}

void drawItem(){
    glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturaItem);
  glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0); glVertex3f(-15,-15 ,0);
    glTexCoord2f(1, 0); glVertex3f(15,-15,0);
    glTexCoord2f(1, 1); glVertex3f(15,15,0);
    glTexCoord2f(0, 1); glVertex3f(-15,15,0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void drawShot(int *raioTiro,int* tiroGrande, int* tiroVara){
  int tamanhoTiro=20;

  if(*tiroGrande==1){
    tamanhoTiro=120;
    *raioTiro=tamanhoTiro/3;
  }
  if(*tiroVara==1){
    glColor3f(0.6,0.8,0.196078);
  }
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturaShot);
  glBegin(GL_POLYGON);
      glTexCoord2f(0, 0);  glVertex2f(0,-tamanhoTiro);
      glTexCoord2f(1, 0); glVertex2f(-tamanhoTiro,0);
      glTexCoord2f(1, 1); glVertex2f(0,tamanhoTiro);
      glTexCoord2f(0, 1); glVertex2f(tamanhoTiro,0);
    glEnd();
    glColor3f(1,1,1);
    glDisable(GL_TEXTURE_2D);
}

void drawArrowMenu(){

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturaArrowMenu);
  glBegin(GL_POLYGON);
      glTexCoord2f(0, 0); glVertex2f(0,0);
      glTexCoord2f(1, 0); glVertex2f(20,0);
      glTexCoord2f(1, 1); glVertex2f(20,20);
      glTexCoord2f(0, 1); glVertex2f(0,20);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
