#include <GL/glew.h>
#include "characterMoves.h"
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <SOIL/SOIL.h>

#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)
#define LARGURA_DO_MUNDO 896
#define ALTURA_DO_MUNDO 504

const float VELOCIDADE_PLAYER = 4.5f;
const int VELOCIDADE_TIRO=15.00f;
const int ENEMY_ISOLA = -50;
const int TIRO_ISOLA = -100;
const int MAX_ENEMIES = 1000;
const int RAIO_ENEMY =10;
const int RAIO_PLAYER=10;
float CORDENADAY_MENU1 = 157;
float CORDENADAY_MENU2 = 117;
float CORDENADAY_MENU3 = 77;
float CORDENADAX_MENU1 = 60;
float CORDENADAX_MENU2 = 40;
float CORDENADAX_MENU3 = 82;

GLuint texturaBackground, texturaBackground2, texturaPause , texturaMenu, texturaArrowMenu,texturaCredits;
GLuint texturaEnemyFront,texturaEnemyBack,texturaEnemyLeft,texturaEnemyRight, texturaShot, texturaBlood, texturaArma_Grande;
GLfloat orientacaoEmGraus = 0;
GLfloat velocidadeAngular = 0.05;
GLfloat x = 0, y = 0;
float velocidadeTangencial = 0.5;
int trueMouseX, trueMouseY;
int colidiu=0, spawn=0 , menu=1 , scoreBoard=0, credits=0;
int globalWidth=LARGURA_DO_MUNDO, globalHeight= ALTURA_DO_MUNDO;
int raioTiro=6, tiroGrande=0;

float axisx=0, axisy=0;
int up=0,down=0,left=0,right=0,r,enemyCount=0,level=10,pause = 0;
int atirou=0, tiroNaTela=0;;

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
  texturaArrowMenu = SOIL_load_OGL_texture("arrowMenu.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaArrowMenu == 0) {
    printf("Erro carregando textura arrowMenu.png '%s'\n", SOIL_last_result());
  }
}
void loadCredits() {
  texturaCredits= SOIL_load_OGL_texture("CreditsScreen.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaCredits == 0) {
    printf("Erro carregando textura CreditsScreen.png '%s'\n", SOIL_last_result());
  }
}
void loadArma_Grande() {
  texturaArma_Grande = SOIL_load_OGL_texture("armagrande.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaArma_Grande == 0) {
    printf("Erro carregando textura armagrande.png '%s'\n", SOIL_last_result());
  }
}

void loadEnemyFront() {
  texturaEnemyFront = SOIL_load_OGL_texture("frontwalker2.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaEnemyFront == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
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
  texturaBlood = SOIL_load_OGL_texture("blood.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaBlood == 0) {
    printf("Erro carregando textura: blood.png '%s'\n", SOIL_last_result());
  }
}
void loadMenu() {
  texturaMenu = SOIL_load_OGL_texture("StartMenu.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaMenu == 0) {
    printf("Erro carregando textura: blood.png '%s'\n", SOIL_last_result());
  }
}

void loadShot() {
  texturaShot = SOIL_load_OGL_texture("arrow.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  if (texturaShot == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadEnemyBack() {
  texturaEnemyBack = SOIL_load_OGL_texture("backwalker1.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
  if (texturaEnemyBack == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadEnemyLeft() {
  texturaEnemyLeft = SOIL_load_OGL_texture("leftwalker1.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaEnemyLeft == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadEnemyRight() {
  texturaEnemyRight = SOIL_load_OGL_texture("rightwalker1.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaEnemyRight == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}

void loadBackground() {
  texturaBackground = SOIL_load_OGL_texture("BG.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaBackground == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadBackground2() {
  texturaBackground2 = SOIL_load_OGL_texture("ZombieBG.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaBackground2 == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadPause() {
  texturaPause = SOIL_load_OGL_texture("gamePaused2.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaPause == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void drawScoreBoard(){

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

/*float arrowMenu_converteCord(float cordenada, char componente){
  if(componente=='y'){
    return cordenada*globalHeight/ALTURA_DO_MUNDO;
  }
  else{
    return cordenada*globalWidth/LARGURA_DO_MUNDO;
  }
}*/

void arrowMenuTranslate(int posicao){
    if(posicao==1){
      glTranslatef(CORDENADAX_MENU1,CORDENADAY_MENU1,0);
    }
    else if(posicao==2){
      glTranslatef(CORDENADAX_MENU2,CORDENADAY_MENU2,0);
    }
    else if(posicao==3){
      glTranslatef(CORDENADAX_MENU3,CORDENADAY_MENU3,0);
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
      tiroNaTela=0;
      tiro.x = TIRO_ISOLA;         //COLOCA O TIRO EM UM LUGAR FORA DO MAPA, ONDE ELE NAO SERÁ RENDERIZADO, ATÉ O PROXIMO PODER SER DISPARADO
      tiro.y = TIRO_ISOLA;
    }
  }
  if(axisx>cliqueMouse.x){ // player está a direita do mouse
    if(tiro.x>cliqueMouse.x){
      shotFollows();
    }
    else{
      tiroNaTela=0;
      tiro.x = TIRO_ISOLA;
      tiro.y = TIRO_ISOLA;
    }
  }
  if(axisy>cliqueMouse.y){ //player está acima do mouse
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
  if(enemyCount<MAX_ENEMIES && pause==0){
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
    enemyVector[i].x=-60;
    enemyVector[i].y=-60;
      tiro.x = -100;
      tiro.y = -100;
  return 1;
  }
  else
    return 0;
}

int colisao (int i) {
  int distancia = sqrt(((axisx - enemyVector[i].x) * (axisx - enemyVector[i].x)) + ((axisy - enemyVector[i].y) * (axisy - enemyVector[i].y)));
  if (axisx>=globalWidth){ //FAZER RETORNAR 1 PARA PODER TIRAR VIDA DO PLAYER
    axisx-=30;
  }
  if(axisy>=globalHeight){
    axisy-=30;
  }
  if (axisx<=0){
    axisx+=30;
  }
  if(axisy<=0){
    axisy+=30;
  }
  if (distancia <= RAIO_PLAYER + RAIO_ENEMY){
        if(axisy>enemyVector[i].y){
            axisy+=30;
        }
        else
          axisy-=30;

        if(axisx<enemyVector[i].x){
          axisx-=30;
        }
        else
          axisx+=30;

      return 1;
  }
  else
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
    if(menu==0 && pause==0){
      if(spawn==0){            //SE NAO TINHA NADA SPAWNADO, SPAWNA UM ITEM
       randomScreenSpawn(&arma_TiroGrande);
        spawn=1;
      arma_TiroGrande.x=300;
      arma_TiroGrande.y=300;
        printf("\nGerou arma em x=%f e y=%f\n",arma_TiroGrande.x,arma_TiroGrande.y);
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
        else{
          glPushMatrix();
            glTranslatef(ENEMY_ISOLA,ENEMY_ISOLA,0);
            drawEnemy(i);
          glPopMatrix();
        }
      }
      if(atirou==1){ //se atirou, o tiro está na tela e cancela o atirou
        createShot();
        atirou=0;
        tiroNaTela=1;
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
    loadBlood();
    loadArma_Grande();
    loadBackground2();
    loadArrowMenu();
    loadMenu();
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

  if(pause==0 && menu==0 && scoreBoard==0 && credits == 0){
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
        colidiu=1;
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

  if(menu>=1 && pause==0){      // DESENHA A SETA DO MENU
      drawMenu();
      CORDENADAY_MENU1 = 157*globalHeight/ALTURA_DO_MUNDO;
      CORDENADAY_MENU2 = 117*globalHeight/ALTURA_DO_MUNDO;
      CORDENADAY_MENU3 = 77*globalHeight/ALTURA_DO_MUNDO;
      CORDENADAX_MENU1 = 60*globalWidth/LARGURA_DO_MUNDO;
      CORDENADAX_MENU2 = 40*globalWidth/LARGURA_DO_MUNDO;
      CORDENADAX_MENU3 = 82*globalWidth/LARGURA_DO_MUNDO;
        glPushMatrix();
        arrowMenuTranslate(menu);
        drawArrowMenu();
        glPopMatrix();
      glutSwapBuffers();
  }

  glutTimerFunc(33, atualiza, 0);


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
              credits=1;
          }
          break;
        // Tecla ESC
        case 8:
          menu =1;
          credits=0;
          scoreBoard=0;
          break;
        case 27:
          exit(0);
          break;
          case 'W':
          case 'w':
            if(menu == 3){
              menu=2;
            }
            else if(menu == 2){
              menu --;
            }
            resetaMovimento(&up,&down,&left,&right);
            up=1;
            break;
            case 'S':
            case 's':
              if(menu == 1 || menu==2){
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
