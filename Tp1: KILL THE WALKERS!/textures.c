#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include"textures.h"

GLuint texturaPlayerF,texturaPlayerB,texturaPlayerR,texturaPlayerL;
GLuint texturaLifeBar1, texturaLifeBar2, texturaLifeBar3, texturaBackground, texturaBackground2, texturaInstrucoes, texturaPause , texturaMenu, texturaArrowMenu,texturaCredits, texturaGameOver;
GLuint texturaEnemyFront,texturaEnemyBack,texturaEnemyLeft,texturaEnemyRight;
GLuint texturaShot, texturaItem;

void loadPlayerFront() {
  texturaPlayerF = SOIL_load_OGL_texture("texturas/texturaPlayerFront.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  if (texturaPlayerF == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadPlayerBack() {
  texturaPlayerB = SOIL_load_OGL_texture("texturas/texturaPlayerBack.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  if (texturaPlayerB == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}


void loadPlayerLeft() {
  texturaPlayerL = SOIL_load_OGL_texture("texturas/texturaPlayerLeft.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  if (texturaPlayerL == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadPlayerRight() {
  texturaPlayerR = SOIL_load_OGL_texture("texturas/texturaPlayerRight.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  if (texturaPlayerR == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadMenu() {
  texturaMenu = SOIL_load_OGL_texture("texturas/StartMenu.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaMenu == 0) {
    printf("Erro carregando textura: blood.png '%s'\n", SOIL_last_result());
  }
}

void loadShot() {
  texturaShot = SOIL_load_OGL_texture("texturas/arrow.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  if (texturaShot == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadEnemyFront() {
  texturaEnemyFront = SOIL_load_OGL_texture("texturas/frontwalker2.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaEnemyFront == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadEnemyBack() {
  texturaEnemyBack = SOIL_load_OGL_texture("texturas/backwalker1.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  if (texturaEnemyBack == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadEnemyLeft() {
  texturaEnemyLeft = SOIL_load_OGL_texture("texturas/leftwalker1.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaEnemyLeft == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadEnemyRight() {
  texturaEnemyRight = SOIL_load_OGL_texture("texturas/rightwalker1.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaEnemyRight == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadBackground() {
  texturaBackground = SOIL_load_OGL_texture("texturas/BG.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaBackground == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadBackground2() {
  texturaBackground2 = SOIL_load_OGL_texture("texturas/ZombieBG.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaBackground2 == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadPause() {
  texturaPause = SOIL_load_OGL_texture("texturas/gamePaused2.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaPause == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadArrowMenu() {
  texturaArrowMenu = SOIL_load_OGL_texture("texturas/arrowMenu.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaArrowMenu == 0) {
    printf("Erro carregando textura arrowMenu.png '%s'\n", SOIL_last_result());
  }
}
void loadCredits() {
  texturaCredits= SOIL_load_OGL_texture("texturas/CreditsScreen.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaCredits == 0) {
    printf("Erro carregando textura CreditsScreen.png '%s'\n", SOIL_last_result());
  }
}

void loadLifeBar1() {
  texturaLifeBar1= SOIL_load_OGL_texture("texturas/barraDeVida1.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaLifeBar1 == 0) {
    printf("Erro carregando textura barraDeVida1.png '%s'\n", SOIL_last_result());
  }
}

void loadLifeBar2() {
  texturaLifeBar2= SOIL_load_OGL_texture("texturas/barraDeVida2.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaLifeBar2 == 0) {
    printf("Erro carregando textura barraDeVida2.png '%s'\n", SOIL_last_result());
  }
}

void loadLifeBar3() {
  texturaLifeBar3= SOIL_load_OGL_texture("texturas/barraDeVida3.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaLifeBar3 == 0) {
    printf("Erro carregando textura barraDeVida3.png '%s'\n", SOIL_last_result());
  }
}

void loadInstrucoes() {
  texturaInstrucoes= SOIL_load_OGL_texture("texturas/menuInstrucoes.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaInstrucoes == 0) {
    printf("Erro carregando textura menuInstrucoes.png '%s'\n", SOIL_last_result());
  }
}

void loadGameOver() {
  texturaGameOver= SOIL_load_OGL_texture("texturas/GameOver.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaGameOver == 0) {
    printf("Erro carregando textura GameOver.png '%s'\n", SOIL_last_result());
  }
}
void loadSecretItem() {
  texturaItem = SOIL_load_OGL_texture("texturas/secretItem.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaItem == 0) {
    printf("Erro carregando textura secretItem.png '%s'\n", SOIL_last_result());
  }
}
