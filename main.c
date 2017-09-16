#include <GL/glew.h>
#include "characterMoves.h"
#include <GL/freeglut.h>
#include <stdio.h>
#include<string.h>
#include <math.h>
#include <SOIL/SOIL.h>

#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)
#define LARGURA_DO_MUNDO 896
#define ALTURA_DO_MUNDO 504

const float VELOCIDADE_PLAYER = 4.5f;
const int VELOCIDADE_TIRO=15.00f;
const int ENEMY_ISOLA = -1000;
const int TIRO_ISOLA = 4500;
const int MAX_ENEMIES = 1000;
const int RAIO_ENEMY =10;
const int RAIO_PLAYER=10;
float cordenadaY_menu1, cordenadaY_menu2, cordenadaY_menu3, cordenadaY_menu4, cordenadaX_menu1, cordenadaX_menu2, cordenadaX_menu3, cordenadaX_menu4 = 40;

GLuint texturaLifeBar1, texturaLifeBar2, texturaLifeBar3, texturaBackground, texturaBackground2, texturaInstrucoes, texturaPause , texturaMenu, texturaArrowMenu,texturaCredits, texturaGameOver;
GLuint texturaEnemyFront,texturaEnemyBack,texturaEnemyLeft,texturaEnemyRight, texturaShot, texturaBlood, texturaArma_Grande;
GLfloat orientacaoEmGraus = 0;
GLfloat velocidadeAngular = 0.05;
GLfloat x = 0, y = 0;
float velocidadeTangencial = 0.5;
int trueMouseX, trueMouseY;
int spawn=0,pontuacao=0, menu=1, gameOver=0, instrucoes=0, scoreBoard=0, credits=0;
int globalWidth=LARGURA_DO_MUNDO, globalHeight= ALTURA_DO_MUNDO;
int raioTiro=6, tiroGrande=0;
char stringPontuacao[100];
float axisx=LARGURA_DO_MUNDO/2, axisy=ALTURA_DO_MUNDO/2;
int up=0,down=0,left=0,right=0,r,enemyCount=0,level=10,pause = 0;
int atirou=0, tiroNaTela=0, vida=3;

struct ponto {
   float x, y;
};
struct Enemy{
  int isDead;
  float x,y;
};

struct Enemy enemyVector[1000];
struct ponto posicaoMouse;
struct ponto cliqueMouse;
struct ponto tiro;
struct ponto arma_TiroGrande;
struct Enemy enemy;
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

void loadArma_Grande() {
  texturaArma_Grande = SOIL_load_OGL_texture("texturas/armagrande.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaArma_Grande == 0) {
    printf("Erro carregando textura armagrande.png '%s'\n", SOIL_last_result());
  }
}

void loadEnemyFront() {
  texturaEnemyFront = SOIL_load_OGL_texture("texturas/frontwalker2.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaEnemyFront == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void desenhaTexto(float x, float y, int r, int g, int b, const char*string){
	int j = strlen(string);
  glColor3f(r,g,b);
	glRasterPos2f(x,y);
	int i;
	for(i=0;i<j;i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i] );
	}
  glColor3f(1,1,1);
}

void drawHud(){
    char shud[6]={'S','C','O','R','E',':'};
    sprintf(stringPontuacao, "%d", pontuacao);
    desenhaTexto((globalWidth/2)-70,globalHeight-20,0,0,0,stringPontuacao);
    sprintf(stringPontuacao, "%s", shud);
    desenhaTexto((globalWidth/2)-150,globalHeight-20,0,0,0,stringPontuacao);
}

void randomScreenSpawn(struct ponto* item_Coletavel){
   item_Coletavel->x = globalWidth-rand()%globalWidth;
   item_Coletavel->y = globalHeight-rand()%globalHeight;
} //SPAWNA UM ITEM EM UMA POSICAO ALEATORIA DA TELA

int pegaItem(struct ponto* item_Coletavel){ // VERIFICA DE O JOGADOR PASSOU PELAS COORDENADAS DO ITEM COLETAVEL
  if(axisx==item_Coletavel->x && axisy==item_Coletavel->y){
    return 1;
  }
  else
    return 0;
}
void drawItem(){ // DESENHA O ITEM COLETAVEL
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturaArma_Grande);
  glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0, 0); glVertex3f(0, 0,0);
      glTexCoord2f(1, 0); glVertex3f( 30, 0,0);
      glTexCoord2f(1, 1); glVertex3f( 30,  30,0);
      glTexCoord2f(0, 1); glVertex3f(0, 30,0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}
void loadBlood() {
  texturaBlood = SOIL_load_OGL_texture("texturas/blood.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaBlood == 0) {
    printf("Erro carregando textura: blood.png '%s'\n", SOIL_last_result());
  }
}
void loadMenu() {
  texturaMenu = SOIL_load_OGL_texture("texturas/StartMenu.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaMenu == 0) {
    printf("Erro carregando textura: StartMenu.png '%s'\n", SOIL_last_result());
  }
}

void loadShot() {
  texturaShot = SOIL_load_OGL_texture("texturas/arrow.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  if (texturaShot == 0) {
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
void drawScoreBoard(){

}

void drawInstrucoes(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaInstrucoes);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0,0);
        glTexCoord2f(1, 0); glVertex3f( globalWidth, 0,0);
        glTexCoord2f(1, 1); glVertex3f( globalWidth,  globalHeight,0);
        glTexCoord2f(0, 1); glVertex3f(0,  globalHeight,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawCredits(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaCredits);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0,0);
        glTexCoord2f(1, 0); glVertex3f( globalWidth, 0,0);
        glTexCoord2f(1, 1); glVertex3f( globalWidth,  globalHeight,0);
        glTexCoord2f(0, 1); glVertex3f(0,  globalHeight,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void drawMenu(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaMenu);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0,0);
        glTexCoord2f(1, 0); glVertex3f( globalWidth, 0,0);
        glTexCoord2f(1, 1); glVertex3f( globalWidth,  globalHeight,0);
        glTexCoord2f(0, 1); glVertex3f(0,  globalHeight,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawGameOver(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaGameOver);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0,0);
        glTexCoord2f(1, 0); glVertex3f( globalWidth, 0,0);
        glTexCoord2f(1, 1); glVertex3f( globalWidth,  globalHeight,0);
        glTexCoord2f(0, 1); glVertex3f(0,  globalHeight,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void backGround(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaBackground);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0,0);
        glTexCoord2f(1, 0); glVertex3f( globalWidth, 0,0);
        glTexCoord2f(1, 1); glVertex3f( globalWidth,  globalHeight,0);
        glTexCoord2f(0, 1); glVertex3f(0,  globalHeight,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void backGround2(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaBackground2);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0,0);
        glTexCoord2f(1, 0); glVertex3f( globalWidth, 0,0);
        glTexCoord2f(1, 1); glVertex3f( globalWidth,  globalHeight,0);
        glTexCoord2f(0, 1); glVertex3f(0,  globalHeight,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

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

void drawPauseScreen(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaPause);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(0, 0,0);
        glTexCoord2f(1, 0); glVertex3f( globalWidth, 0,0);
        glTexCoord2f(1, 1); glVertex3f( globalWidth,  globalHeight,0);
        glTexCoord2f(0, 1); glVertex3f(0,  globalHeight,0);
    glEnd();
  glDisable(GL_TEXTURE_2D);
}

void createShot(){
  tiro.x = axisx;
  tiro.y = axisy;
}

void drawShot(){ //CRIAR UM IF QUE DEPENDENDO DA VARIAVEL ARMA QUE ESTIVER TRUE, ALTERA O RAIO E TAMANHO DO DESENHO DO TIRO

  int tamanhoTiro=20;

  if(tiroGrande==1){
    tamanhoTiro=120;
    raioTiro=tamanhoTiro/3;
  }
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturaShot);
  glBegin(GL_POLYGON);
      glTexCoord2f(0, 0);  glVertex2f(0,-tamanhoTiro);
      glTexCoord2f(1, 0); glVertex2f(-tamanhoTiro,0);
      glTexCoord2f(1, 1); glVertex2f(0,tamanhoTiro);
      glTexCoord2f(0, 1); glVertex2f(tamanhoTiro,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void arrowMenu_converteCord(){
  cordenadaY_menu1 = 172*globalHeight/ALTURA_DO_MUNDO;
  cordenadaY_menu2 = 131*globalHeight/ALTURA_DO_MUNDO;
  cordenadaY_menu3 = 90*globalHeight/ALTURA_DO_MUNDO;
  cordenadaY_menu4 = 50*globalHeight/ALTURA_DO_MUNDO;
  cordenadaX_menu1 = 55*globalWidth/LARGURA_DO_MUNDO;
  cordenadaX_menu2 = 35*globalWidth/LARGURA_DO_MUNDO;
  cordenadaX_menu3 = 62*globalWidth/LARGURA_DO_MUNDO;
  cordenadaX_menu4 = 78*globalWidth/LARGURA_DO_MUNDO;
}


void arrowMenuTranslate(int posicao){

   if(posicao==1){
      glTranslatef(cordenadaX_menu1,cordenadaY_menu1,0);
    }
    else if(posicao==2){
      glTranslatef(cordenadaX_menu2,cordenadaY_menu2,0);
    }
    else if(posicao==3){
      glTranslatef(cordenadaX_menu3,cordenadaY_menu3,0);
    }
    else if(posicao==4){
          glTranslatef(cordenadaX_menu4,cordenadaY_menu4,0);
    }
}

void drawArrowMenu(){ //CRIAR UM IF QUE DEPENDENDO DA VARIAVEL ARMA QUE ESTIVER TRUE, ALTERA O RAIO E TAMANHO DO DESENHO DO TIRO

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

void drawBlood(int x, int y){

}

void clickMouse(int button, int state, int m, int n){

  if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN)){
    cliqueMouse.x=m;
    cliqueMouse.y=globalHeight-n;
    if(atirou==0 && tiroNaTela==0){
      atirou=1;
    }
    else atirou=0;
  }
}

void shotFollows(){

    float vetorx = (cliqueMouse.x - tiro.x);
    float vetory = (cliqueMouse.y - tiro.y);
    float  norma = sqrt(vetorx*vetorx + vetory*vetory);
    vetorx /= norma;
    vetory /= norma;
    tiro.x+= vetorx*VELOCIDADE_TIRO;
    tiro.y+= vetory*VELOCIDADE_TIRO;
}

void tiro_AvaliaPosicao(){  // DETERMINA QUANDO O TIRO SOME A PARTIR DA AVALIAÇÃO DA POSIÇÃO RELATIVA ENTRE O ALVO E O PLAYER

  if(axisx<cliqueMouse.x){ // player está a esquerda do mouse
    if(tiro.x<cliqueMouse.x){
      shotFollows();
    }
    else{
      tiroNaTela=0
      tiro.x = TIRO_ISOLA;         //COLOCA O TIRO EM UM LUGAR FORA DO MAPA, ONDE ELE NAO SERÁ RENDERIZADO, ATÉ O PROXIMO PODER SER DISPARADO
      tiro.y = TIRO_ISOLA;
    }
  }
  else if(axisx>cliqueMouse.x){ // player está a direita do mouse
    if(tiro.x>cliqueMouse.x){
      shotFollows();
    }
    else{
      tiroNaTela=0;
      tiro.x = TIRO_ISOLA;
      tiro.y = TIRO_ISOLA;
    }
  }
  else if(axisy>cliqueMouse.y){ //player está acima do mouse
    if(tiro.y>cliqueMouse.y){
      shotFollows();
    }
    else{
      tiroNaTela=0;
      tiro.x = TIRO_ISOLA;
      tiro.y = TIRO_ISOLA;;
    }
  }
  if(axisy<cliqueMouse.y){ // player está abaixo do mouse
    if(tiro.y<cliqueMouse.y){
    shotFollows();
    }
    else{
      tiroNaTela=0;
      tiro.x = TIRO_ISOLA;
      tiro.y = TIRO_ISOLA;
    }
  }
}

void enemySpawn(){
  if(enemyCount<MAX_ENEMIES && pause==0 && gameOver==0){
    switch(enemyCount){
      case 0 :
        enemyCount+=10;
        break;
      case 10 :
        enemyCount+=20;
        break;
      case 30 :
        enemyCount+=40;
        break;
      case 70 :
        enemyCount+=80;
        break;
      case 150 :
        enemyCount+=160;
        break;
      case 310 :
        enemyCount+=320;
        break;
      case 630:
        enemyCount+=370;
    }
  }
  glutTimerFunc(30000,enemySpawn,0);
}

void movimentoMouse(int x, int y) {     //callback do mouse       ELIMINAR FUNCAO CORRIGE MOUSE
    posicaoMouse.x = x;
    posicaoMouse.y = y;
}
void createEnemy(){
    for(int i=0;i<1000;i++){
    r = globalWidth+(rand()%2300-2600);
    enemyVector[i].x = r;
    r = globalHeight+(rand()%2300-2600);
    enemyVector[i].y = r;
    enemyVector[i].isDead = 0;
  }
}


int tiro_colisao (int i) {
  int distancia = sqrt(((tiro.x - enemyVector[i].x) * (tiro.x - enemyVector[i].x)) + ((tiro.y - enemyVector[i].y) * (tiro.y - enemyVector[i].y)));
  if (distancia <= raioTiro + RAIO_ENEMY){
    //drawBlood(enemyVector[i].x, enemyVector[i].y){
    //}
    enemyVector[i].x=ENEMY_ISOLA;
    enemyVector[i].y=ENEMY_ISOLA;
      tiro.x = TIRO_ISOLA;
      tiro.y = TIRO_ISOLA;
      tiroNaTela=0;
  return 1;
  }
  else
    return 0;
}

int colisao (int i) {
  int distancia = sqrt(((axisx - enemyVector[i].x) * (axisx - enemyVector[i].x)) + ((axisy - enemyVector[i].y) * (axisy - enemyVector[i].y)));
  if (axisx>=globalWidth){ //FAZER RETORNAR 1 PARA PODER TIRAR VIDA DO PLAYER
    axisx-=30;
    return 1;
  }
  if(axisy>=globalHeight){
    axisy-=30;
    return 1;
  }
  if (axisx<=0){
    axisx+=30;
    return 1;
  }
  if(axisy<=0){
    axisy+=30;
    return 1;
  }
  if (distancia <= RAIO_PLAYER + RAIO_ENEMY){
        if(axisy>enemyVector[i].y){
            axisy+=30;
        }
        else{
          axisy-=30;
}
        if(axisx<enemyVector[i].x){
          axisx-=30;
        }
        else{
          axisx+=30;
        }
      return 1;
  }
      return 0;
}

void drawEnemy(int i){

  glEnable(GL_TEXTURE_2D);

  float vetorx=(axisx-enemyVector[i].x);
  float vetory=(axisy-enemyVector[i].y);

  if(-vetory > vetorx && -vetory > -vetorx && -vetory > vetory){ // player está a direta do inimigo
        glBindTexture(GL_TEXTURE_2D, texturaEnemyFront);
    }
    else if(vetory > vetorx && vetory > -vetorx && vetory > -vetory){
        glBindTexture(GL_TEXTURE_2D, texturaEnemyBack);
    }
    else if(-vetorx > vetorx && -vetorx > vetory && -vetorx > -vetory){
        glBindTexture(GL_TEXTURE_2D, texturaEnemyLeft);
    } else
    {
        glBindTexture(GL_TEXTURE_2D, texturaEnemyRight);
    }

  glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0); glVertex3f(-15,-15 ,0);
    glTexCoord2f(1, 0); glVertex3f(15,-15,0);
    glTexCoord2f(1, 1); glVertex3f(15,15,0);
    glTexCoord2f(0, 1); glVertex3f(-15,15,0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void enemyFollows(int i){

  float vetorx = axisx - enemyVector[i].x;
  float vetory = axisy - enemyVector[i].y;
  int  norma = sqrt(vetorx*vetorx + vetory*vetory);
  vetorx /= norma;
  vetory /= norma;

  enemyVector[i].x += (vetorx)*(1.89f);
  enemyVector[i].y += (vetory)*(1.89f);
}

void desenhaCena(void){

    glClear(GL_COLOR_BUFFER_BIT);
    if(menu==0 && pause==0 && gameOver==0){
      if(spawn==0){            //SE NAO TINHA NADA SPAWNADO, SPAWNA UM ITEM
       randomScreenSpawn(&arma_TiroGrande);
        spawn=1;
      arma_TiroGrande.x=300;
      arma_TiroGrande.y=300;
      }
    else{                  // SE O ITEM JA ESTIVER SPAWNADO, DESENHA O ITEM.
        glPushMatrix();
          glTranslatef(arma_TiroGrande.x,arma_TiroGrande.y,0);
          drawItem();
        glPopMatrix();
        }

      backGround();
      glPushMatrix();
        glTranslatef(x, y, 0);
        //glRotatef(atan2(trueMouseY-axisy,trueMouseX-axisx)*180/M_PI, 0, 0, 1);       // Fazia o sprite do jogador se orientar pelo mouse
        characterShape(trueMouseX,trueMouseY,axisx,axisy);      // Desenha o personagem principal
      glPopMatrix();
      for(int i=0;i<enemyCount;i++){
        if(enemyVector[i].isDead==0){
          glPushMatrix();
            glTranslatef(enemyVector[i].x,enemyVector[i].y,0);
            drawEnemy(i);
          glPopMatrix();
        }
      }
      if(atirou==1){ //se atirou, o tiro está na tela e cancela o atirou
        tiroNaTela=1;
        atirou=0;
        createShot();
      }

      if(tiroNaTela==1){ //se o tiro está na tela, atualiza a posicao do tiro.
          glPushMatrix();
          glTranslatef(tiro.x,tiro.y,0);
          drawShot();
          glPopMatrix();
        }

      if(pause==1){
        drawPauseScreen();
      }
      backGround2();
      glPushMatrix();
      glTranslatef(globalWidth-80,globalHeight-15,0);
        drawLifeBar(vida);
      glPopMatrix();
      drawHud();
      glutSwapBuffers();
    }
}

// Inicia algumas variáveis de estado
void inicializa(void){
    // cor para limpar a tela
    glClearColor(0, 0, 0, 0);      // preto
    createEnemy();
    loadBackground();
    loadEnemyFront();
    loadEnemyBack();
    loadEnemyLeft();
    loadEnemyRight();
    loadPause();
    loadPlayerLeft();
    loadPlayerRight();
    loadPlayerFront();
    loadPlayerBack();
    loadShot();
    loadLifeBar1();
    loadLifeBar2();
    loadLifeBar3();
    loadBlood();
    loadArma_Grande();
    loadBackground2();
    loadArrowMenu();
    loadMenu();
    loadInstrucoes();
    loadGameOver();
    loadCredits();
    // imprime instruções
    printf("Instrucoes:\n");
}

// Callback de redimensionamento
void redimensiona(int w, int h){

  globalWidth = w;
  globalHeight = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void atualiza(int idx) {

  if(pause==0 && menu==0 && instrucoes==0 && scoreBoard==0 && credits==0 && gameOver==0){

    GLfloat orientacaoEmRadianos = grausParaRadianos(orientacaoEmGraus);
    corrigeMouse(posicaoMouse.x,posicaoMouse.y,&trueMouseX,&trueMouseY,globalHeight);
    movimenta(up,down,left,right,&axisx,&axisy,VELOCIDADE_PLAYER);
    if(pegaItem(&arma_TiroGrande)==1){
      tiroGrande=1;
    }
    if(tiroNaTela==1){
      tiro_AvaliaPosicao();
    }
    for(int i=0;i<enemyCount;i++){ //LOOP PARA CONFIGURAR OS INIMIGOS
      if(colisao(i)==1){          //verifica se houve colisao do player com inimigos
        if(vida>0){
          vida--;
        }
      }
      if(tiro_colisao(i)==1){
        enemyVector[i].isDead = 1;
      }          //Verifica se o inimigo foi atingido por um tiro.
      if(enemyVector[i].isDead!=1){
        enemyFollows(i);         //Faz os inimigos seguirem o jogador.
      }
    }
      x = axisx+ cos(orientacaoEmRadianos);
      y = axisy+ sin(orientacaoEmRadianos);

      orientacaoEmGraus += velocidadeAngular;
      glutPostRedisplay();
  }
  if(vida==0){
    gameOver=1;
  }

  if(pause==1 && menu ==0){
    drawPauseScreen();
    glutSwapBuffers();
  }
  else if(menu == 1 && pause==0){
      drawMenu();
      glutSwapBuffers();
  }
  else if(menu == 0 && scoreBoard == 1){
    drawScoreBoard();
    glutSwapBuffers();
  }
  else if(menu==0 && credits == 1){
    drawCredits();
    glutSwapBuffers();
  }
  else if(gameOver==1 && menu==0){
    drawGameOver();
    glutSwapBuffers();
  }
  else if(instrucoes==1 && menu == 0){
      drawInstrucoes();
      glutSwapBuffers();
  }

  if(menu!=0 && pause==0){      // DESENHA A SETA DO MENU
      drawMenu();
        glPushMatrix();
        arrowMenu_converteCord();
          arrowMenuTranslate(menu);
          drawArrowMenu();
        glPopMatrix();
      glutSwapBuffers();
  }

  glutTimerFunc(30, atualiza, 0);


}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y){
    switch(key){
        case 13:
          if(menu==1){
            menu=0;
          }
          else if(menu==2){
              menu=0;
              scoreBoard=1;
          }
          else if(menu==3){
              menu=0;
              instrucoes=1;
          }
          else if(menu==4){
            menu=0;
            credits=1;
          }
          break;
        // Tecla ESC
        case 8:
        if(gameOver==1){
          vida=3;
          enemyCount=0;
        }
          gameOver=0;
          menu=1;
          credits=0;
          scoreBoard=0;
          instrucoes=0;
          break;
        case 27:
          exit(0);
          break;
          case 'W':
          case 'w':
            if(menu >1){
              menu--;
            }
            resetaMovimento(&up,&down,&left,&right);
            up=1;
            break;
            case 'S':
            case 's':
              if(menu == 1 || menu==2 || menu==3){
                menu++;
              }
              resetaMovimento(&up,&down,&left,&right);
              down=1;
              break;
            case 'D':
            case 'd':
              resetaMovimento(&up,&down,&left,&right);
              right=1;
              break;
            case 'A':
            case 'a':
              resetaMovimento(&up,&down,&left,&right);
              left=1;
              break;
            case 'p':
              if(pause==0){
                pause=1;
              }
              else{
                pause=0;
              }
            default:
            break;
   }
}

// Rotina principal
int main(int argc, char **argv){
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(LARGURA_DO_MUNDO, ALTURA_DO_MUNDO);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("KILL THE WALKERS!");


    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutPassiveMotionFunc(movimentoMouse);
    glutMouseFunc(clickMouse);
    glutTimerFunc(0, atualiza, 0);
    inicializa();
    glutTimerFunc(0,enemySpawn,0);

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
