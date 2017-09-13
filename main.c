#include <GL/glew.h>
#include "characterMoves.h"
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <SOIL/SOIL.h>

#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)
#define LARGURA_DO_MUNDO 800
#define ALTURA_DO_MUNDO 600

const float velocidadeTiro=15.00f;
const int MAX_ENEMIES = 1000;
GLuint texturaBackground;
GLuint texturaEnemy;
GLuint texturaPause;
GLfloat orientacaoEmGraus = 0;
GLfloat velocidadeAngular = 0.05;
GLfloat x = 0, y = 0;
float velocidadeTangencial = 0.5;
int trueMouseX, trueMouseY;
int colidiu =0;
int globalWidth=LARGURA_DO_MUNDO, globalHeight= ALTURA_DO_MUNDO;
const int raioEnemy =10;
const int raioPlayer=10;
const int raioTiro=6;


float axisx=0, axisy=0;
int up=0,down=0,left=0,right=0,r,enemyCount=0,level=10,pause = 0;
int atirou=0, tiroNaTela=0;;

struct ponto {
   float x, y;
};
struct ponto enemyVector[1000];
struct ponto posicaoMouse;
struct ponto cliqueMouse;
struct ponto tiro;
struct ponto enemy;


void loadEnemy() {
  texturaEnemy = SOIL_load_OGL_texture("frontwalker2.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaEnemy == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadBackground() {
  texturaBackground = SOIL_load_OGL_texture("backgroundzumbis.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaBackground == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void loadPause() {
  texturaPause = SOIL_load_OGL_texture("texturaPause.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaPause == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
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

void drawShot(){
  glBegin(GL_POLYGON);
      glVertex2f(0,-4);
      glVertex2f(-4,0);
      glVertex2f(0,4);
      glVertex2f(4,0);
  glEnd();
}

void clickMouse(int button, int state, int m, int n){

  if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN)){
    cliqueMouse.x=m;
    cliqueMouse.y=globalHeight-n;
    if(atirou==0 && tiroNaTela==0){
      atirou=1;
    }
  }
  //if((button==GLUT_LEFT_BUTTON) && (state==GLUT_UP)){
    //clicou=0;
  //}
}

void tiro_AvaliaPosicao(){  // DETERMINA QUANDO O TIRO SOME A PARTIR DA AVALIAÇÃO DA POSIÇÃO RELATIVA ENTRE O ALVO E O PLAYER
  if(axisx<cliqueMouse.x){ // player está a esquerda do mouse
    if(tiro.x<cliqueMouse.x){
      shotFollows();
    }
    else{
      tiroNaTela=0;
    }
  }
  if(axisx>cliqueMouse.x){ // player está a direita do mouse
    if(tiro.x>cliqueMouse.x){
      shotFollows();
    }
    else{
      tiroNaTela=0;
    }
  }
  if(axisy>cliqueMouse.y){ //player está acima do mouse
    if(tiro.y>cliqueMouse.y){
      shotFollows();
    }
    else{
      tiroNaTela=0;
    }
  }
  if(axisy<cliqueMouse.y){ // player está abaixo do mouse
    if(tiro.y<cliqueMouse.y){
    shotFollows();
    }
    else{
      tiroNaTela=0;
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
  	r = globalWidth+(rand()%3300-2600);
  	enemyVector[i].x = r;
  	r = globalHeight+(rand()%3300-2600);
  	enemyVector[i].y = r;
  }
}


int tiro_colisao (int i) {
  int distancia = sqrt(((tiro.x/2 - enemyVector[i].x/2) * (tiro.x/2 - enemyVector[i].x/2)) + ((tiro.y/2 - enemyVector[i].y/2) * (tiro.y/2 - enemyVector[i].y/2)));
  if (distancia <= raioTiro + raioEnemy){
    if(tiro.y>enemyVector[i].y){
        enemyVector[i].y-=30;
    }
    else
      enemyVector[i].y+=30;

    if(tiro.x<enemyVector[i].x){
    enemyVector[i].x+=30;
    }
    else
      enemyVector[i].x-=30;

  return 1;
}
  else
    return 0;
}

int colisao (int i) {
  int distancia = sqrt(((axisx/2 - enemyVector[i].x/2) * (axisx/2 - enemyVector[i].x/2)) + ((axisy/2 - enemyVector[i].y/2) * (axisy/2 - enemyVector[i].y/2)));
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
  if (distancia <= raioPlayer + raioEnemy){
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

void drawEnemy(){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturaEnemy);
  glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0); glVertex3f(0,0 ,0);
    glTexCoord2f(1, 0); glVertex3f(30,0,0);
    glTexCoord2f(1, 1); glVertex3f(30,30,0);
    glTexCoord2f(0, 1); glVertex3f(0,30,0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void shotFollows(){

    float vetorx = (cliqueMouse.x - tiro.x);
    float vetory = (cliqueMouse.y - tiro.y);
    float  norma = sqrt(vetorx*vetorx + vetory*vetory);
    vetorx /= norma;
    vetory /= norma;
    tiro.x+= vetorx*velocidadeTiro;
    tiro.y+= vetory*velocidadeTiro;
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
    backGround();
    // Começa a usar a cor azul
    corrigeMouse(posicaoMouse.x,posicaoMouse.y,&trueMouseX,&trueMouseY,globalHeight);
	  movimenta(up,down,left,right,&axisx,&axisy);
    glPushMatrix();
      // Faz o objeto se orientar pelo mouse
      glTranslatef(x, y, 0);
      glRotatef(atan2(trueMouseY-axisy,trueMouseX-axisx)*180/M_PI, 0, 0, 1);
      // Desenha o personagem principal
      characterShape(&colidiu);
    glPopMatrix();

    for(int i=0;i<enemyCount;i++){
      glPushMatrix();
        glTranslatef(enemyVector[i].x,enemyVector[i].y,0);
        drawEnemy();
      glPopMatrix();
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
    glutSwapBuffers();
}

// Inicia algumas variáveis de estado
void inicializa(void){
    // cor para limpar a tela
    glClearColor(0, 0, 0, 0);      // preto
    createEnemy();
    loadBackground();
    loadEnemy();
    loadPause();
    loadCharacter();
    // imprime instruções
    printf("Instrucoes:\n");
    printf("\t+: gira no sentido horario\n");
    printf("\t-: gira no sentido anti-horario\n\n");
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
  if(pause==0){
    GLfloat orientacaoEmRadianos = grausParaRadianos(orientacaoEmGraus);

    if(tiroNaTela==1){
      tiro_AvaliaPosicao();
    }
    for(int i=0;i<enemyCount;i++){ //LOOP PARA CONFIGURAR OS INIMIGOS
      if(colisao(i)==1){          //colisao com o player
        colidiu=1;
      }
      tiro_colisao(i);          //Verifica se o inimigo foi atingido por um tiro.
      enemyFollows(i);         //Faz os inimigos seguirem o jogador.

    }
      x = axisx+ cos(orientacaoEmRadianos);
      y = axisy+ sin(orientacaoEmRadianos);

      orientacaoEmGraus += velocidadeAngular;
      glutPostRedisplay();
  }
  else{
    drawPauseScreen();
    glutSwapBuffers();
  }
  glutTimerFunc(33, atualiza, 0);


}
// Callback de evento de teclado
void teclado(unsigned char key, int x, int y){
  switch(key){
    case 'W':
      key='w';
      break;
    case 'S':
      key='s';
      break;
    case 'A':
      key='a';
      break;
    case 'D':
      key='d';
      break;
  }
    switch(key){
        // Tecla ESC
        case 27:
            exit(0);
            break;
  			case 'w':
  				resetaMovimento(&up,&down,&left,&right);
  				up=1;
  				break;
  			case 's':
  				resetaMovimento(&up,&down,&left,&right);
  				down=1;
  				break;
  			case 'd':
  				resetaMovimento(&up,&down,&left,&right);
  				right=1;
  				break;
  			case 'a':
  				resetaMovimento(&up,&down,&left,&right);
  				left=1;
  				break;
  			case '+':
            velocidadeTangencial+=5;
            break;
        case '=':
            velocidadeTangencial += 0.5;
            break;
        case '-':
              velocidadeTangencial -= 5;
              break;
        case '_':
            velocidadeAngular -= 0.5;
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
