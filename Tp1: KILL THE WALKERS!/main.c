#include <GL/glew.h>
#include "characterMoves.h"
#include "drawing.h"
#include"textures.h"
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include<time.h>
#include<string.h>
#include <SOIL/SOIL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

//Conversao de valores
#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)

//Dimensoes do mundo
#define LARGURA_DO_MUNDO 896
#define ALTURA_DO_MUNDO 504

//Variaveis de textura
extern GLuint texturaLifeBar1, texturaLifeBar2, texturaLifeBar3;
extern GLuint texturaBackground, texturaBackground2;
extern GLuint texturaInstrucoes, texturaPause , texturaMenu, texturaArrowMenu,texturaCredits, texturaGameOver;
extern GLuint texturaEnemyFront,texturaEnemyBack,texturaEnemyLeft,texturaEnemyRight, texturaShot, texturaItem;

//Constantes
const float VELOCIDADE_PLAYER = 4.5f;
const int VELOCIDADE_TIRO=22.00f;
const int ENEMY_ISOLA = -120;
const int TIRO_ISOLA = -60;
const int MAX_ENEMIES = 1000;
const int RAIO_PLAYER=10;
const int MAX_ITEMS = 50;

//Variaveis de menu principal
float cordenadaY_menu1, cordenadaY_menu2, cordenadaY_menu3, cordenadaY_menu4;
float cordenadaX_menu1, cordenadaX_menu2, cordenadaX_menu3, cordenadaX_menu4;
int gameOverControl=0;

//Variaveis de controle da tela
int menu=1, gameOver=0, instrucoes=0, scoreBoard=0, credits=0,pause = 0;

//Variaveis de dimensão da tela
int globalWidth=LARGURA_DO_MUNDO, globalHeight= ALTURA_DO_MUNDO;

//Variaveis de itens
int tiroGrande=0, tiroVara=0,bigEnemies=0,itemCount,coletou=0, vida=3, pickupControl=0;

//Variaveis de controle do personagem
GLfloat orientacaoEmGraus = 0;
GLfloat velocidadeAngular = 0.05;
GLfloat x = 0, y = 0;
float axisx=LARGURA_DO_MUNDO/2, axisy=ALTURA_DO_MUNDO/2;
int trueMouseX, trueMouseY;
float velocidadeTangencial = 0.5;
int up=0,down=0,left=0,right=0,r,enemyCount=0;

//Variaveis de controle do sistema de tiro
int atirou=0, tiroNaTela=0;
int f_highscore, raio_enemy =10, raioTiro=6;

//Variaveis de controle da pontuação
int pontuacao=0;
char stringPontuacao[100];
FILE *f_record = NULL;

//Variaveis de som
Mix_Music *backGroundMusic = NULL;
Mix_Chunk *itemPickup = NULL;
Mix_Chunk *shotSound = NULL;
Mix_Chunk *dyingEnemySound = NULL;
Mix_Chunk *gameOverSound = NULL;

struct ponto {
   float x, y;
};
struct Enemy{
  int isDead;
  float x,y;
};
struct Item{
  int id;
  float x,y;
};

struct Item item_Coletavel[50];
struct Enemy enemyVector[10000];
struct ponto posicaoMouse;
struct ponto cliqueMouse;
struct ponto tiro;
struct ponto arma_TiroGrande;
struct Enemy enemy;

int itemColisao(){      //AVALIA SE O PLAYER PEGOU O ITEM
  int distancia = sqrt(((axisx - item_Coletavel[itemCount].x) * (axisx - item_Coletavel[itemCount].x)) + ((axisy - item_Coletavel[itemCount].y) * (axisy - item_Coletavel[itemCount].y)));
  if (distancia <= RAIO_PLAYER + raio_enemy){
    return 1;
  }
  else
    return 0;
}

void createItem(){   // GERA UM ITEM EM UMA POSICAO ALEATORIA DENTRO DA TELA
  for(int i=0;i<MAX_ITEMS;i++){
    item_Coletavel[i].id = rand()%3;
    item_Coletavel[i].x = 250+rand()%250;
    item_Coletavel[i].y = 250+rand()%250;
  }
}
void gastaItem(){   // FAZ COM QUE O EFEITO DO ITEM EXPIRE
  tiroGrande=0;
  tiroVara=0;
  bigEnemies=0;
  pickupControl=0;
}

void itemSpawn(){    // CONTROLA O TEMPO DE SPAWN DOS ITENS
  if(itemCount<MAX_ITEMS && pause==0){
      itemCount++;
      coletou=0;
    }
  glutTimerFunc(10000,itemSpawn,0);
}

void arrowMenu_converteCord(){     // CONVERTE AS COORDENADAS DA SETA DO MENU INICIAL PARA UM REDIMENSIONAMENTO
  cordenadaY_menu1 = 172*globalHeight/ALTURA_DO_MUNDO;
  cordenadaY_menu2 = 131*globalHeight/ALTURA_DO_MUNDO;
  cordenadaY_menu3 = 90*globalHeight/ALTURA_DO_MUNDO;
  cordenadaY_menu4 = 50*globalHeight/ALTURA_DO_MUNDO;
  cordenadaX_menu1 = 55*globalWidth/LARGURA_DO_MUNDO;
  cordenadaX_menu2 = 35*globalWidth/LARGURA_DO_MUNDO;
  cordenadaX_menu3 = 62*globalWidth/LARGURA_DO_MUNDO;
  cordenadaX_menu4 = 78*globalWidth/LARGURA_DO_MUNDO;
}

void arrowMenuTranslate(int posicao){     // DETERMINA A POSICAO DA SETA DO MENU INICIAL PARA CADA ITEM
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

void createShot(){    // QUANDO O JOGADOR ATIRA, CRIA UM TIRO NAS COORDENADAS DO JOGADOR
  tiro.x = axisx;
  tiro.y = axisy;
}

void clickMouse(int button, int state, int m, int n){  //CONTROLA O USO DO MOUSE PARA ATIRAR

  if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN)){
    cliqueMouse.x=m;
    cliqueMouse.y=globalHeight-n;
    if(atirou==0 && tiroNaTela==0){
      atirou=1;
      Mix_PlayChannel(-1,shotSound,0);
    }
  }
}

void shotFollows(){      // FAZ O TIRO SEGUIR ATÉ A POSICAO EM QUE O MOUSE CLICOU, USANDO VETORES

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
    if(tiro.x<cliqueMouse.x && tiroNaTela==1){
      shotFollows();
    }
    else{
      tiro.x = TIRO_ISOLA;         //COLOCA O TIRO EM UM LUGAR FORA DO MAPA, ONDE ELE NAO SERÁ RENDERIZADO, ATÉ O PROXIMO PODER SER DISPARADO
      tiro.y = TIRO_ISOLA;
      tiroNaTela=0;
    }
  }
  if(axisx>cliqueMouse.x){ // player está a direita do mouse
    if(tiro.x>cliqueMouse.x && tiroNaTela==1){
      shotFollows();
    }
    else{
      tiro.x = TIRO_ISOLA;
      tiro.y = TIRO_ISOLA;
      tiroNaTela=0;
    }
  }
  if(axisy>cliqueMouse.y&& tiroNaTela==1){ //player está acima do mouse
    if(tiro.y>cliqueMouse.y){
      shotFollows();
    }
    else{
      tiro.x = TIRO_ISOLA;
      tiro.y = TIRO_ISOLA;;
      tiroNaTela=0;
    }
  }
  if(axisy<cliqueMouse.y){ // player está abaixo do mouse
    if(tiro.y<cliqueMouse.y && tiroNaTela==1){
    shotFollows();
    }
    else{
      tiro.x = TIRO_ISOLA;
      tiro.y = TIRO_ISOLA;
      tiroNaTela=0;
    }
  }
}

void enemySpawn(){      // CONTROLA AS WAVES DE WHITEWALKERS
  if(enemyCount<MAX_ENEMIES && pause==0){
    tiroGrande=0;
    switch(enemyCount){
      case 0 :
        enemyCount+=100;
        break;
      case 100 :
        enemyCount+=200;
        break;
      case 300 :
        enemyCount+=400;
        break;
      case 700 :
        enemyCount+=800;
        break;
      case 1500 :
        enemyCount+=1600;
        break;
      case 3100 :
        enemyCount+=3200;
        break;
      case 6300:
        enemyCount+=3700;
    }
  }
  glutTimerFunc(30000,enemySpawn,0);
}

void movimentoMouse(int x, int y) {     //callback do mouse
    posicaoMouse.x = x;
    posicaoMouse.y = y;
}
void createEnemy(){ // GERA UM INIMIGO FORA DA TELA
    for(int i=0;i<1000;i++){
    r = globalWidth+(rand()%2300-2600);
    enemyVector[i].x = r;
    r = globalHeight+(rand()%2300-2600);
    enemyVector[i].y = r;
    enemyVector[i].isDead = 0;
  }
}
int tiro_colisao (int i) {  // VERIFICA SE UM INIMIGO FOI ATINGIDO POR UM TIRO
  int distancia = sqrt(((tiro.x - enemyVector[i].x) * (tiro.x - enemyVector[i].x)) + ((tiro.y - enemyVector[i].y) * (tiro.y - enemyVector[i].y)));
  if (distancia <= raioTiro + raio_enemy){
    enemyVector[i].x=ENEMY_ISOLA;
    enemyVector[i].y=ENEMY_ISOLA;
    if(tiroVara==0){
      tiro.x = TIRO_ISOLA;
      tiro.y = TIRO_ISOLA;
    }
    if(tiroNaTela==1){
      pontuacao+=15;
    }
  return 1;
  }
  else
    return 0;
}

int colisao (int i) {  //VERIFICA SE O PLAYER ENCOSTOU EM UM WHITEWALKER OU NAS BORDAS DA TELA E TIRA VIDA.
  int distancia = sqrt(((axisx - enemyVector[i].x) * (axisx - enemyVector[i].x)) + ((axisy - enemyVector[i].y) * (axisy - enemyVector[i].y)));
  if (axisx>=globalWidth){
    axisx-=30;
    vida-=1;
  }
  if(axisy>=globalHeight){
    axisy-=30;
    vida-=1;
  }
  if (axisx<=0){
    axisx+=30;
    vida-=1;
  }
  if(axisy<=0){
    axisy+=30;
    vida-=1;
  }
  if (distancia <= RAIO_PLAYER + raio_enemy){
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

void drawEnemy(int i){ // DESENHA UM INIMIGO USANDO TEXTURAS DIFERENTES DEPENDENDO DA POSICAO EM RELACAO AO PLAYER(SEMPRE ESTÁ OLHANDO PARA O PLAYER)

  int size=15;
  glEnable(GL_TEXTURE_2D);

  float vetorx=(axisx-enemyVector[i].x);
  float vetory=(axisy-enemyVector[i].y);

  if(-vetory > vetorx && -vetory > -vetorx && -vetory > vetory){ //player está abaixo do inimigo
        glBindTexture(GL_TEXTURE_2D, texturaEnemyFront);
    }
    else if(vetory > vetorx && vetory > -vetorx && vetory > -vetory){ //player esta acima do inimigo
        glBindTexture(GL_TEXTURE_2D, texturaEnemyBack);
    }
    else if(-vetorx > vetorx && -vetorx > vetory && -vetorx > -vetory){  // player esta a esquerda do inimigo
        glBindTexture(GL_TEXTURE_2D, texturaEnemyLeft);
    }
    else{                                                                 // player esta a direita do inimigo
        glBindTexture(GL_TEXTURE_2D, texturaEnemyRight);
    }
    if(bigEnemies==1){
      size=45;
      raio_enemy=50;
    }

  glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0); glVertex3f(-size,-size ,0);
    glTexCoord2f(1, 0); glVertex3f(size,-size,0);
    glTexCoord2f(1, 1); glVertex3f(size,size,0);
    glTexCoord2f(0, 1); glVertex3f(-size,size,0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void enemyFollows(int i){        // USA A MESMA LÓGICA DE shotFollows, PORÉM O VETOR DIRECIONAL ESTA SE
                                // MODIFICANDO JUNTO COM A POSICAO DO ZUMBI EM RELAÇÃO AO PLAYER
  float vetorx = axisx - enemyVector[i].x;
  float vetory = axisy - enemyVector[i].y;
  int  norma = sqrt(vetorx*vetorx + vetory*vetory);
  vetorx /= norma;
  vetory /= norma;

  enemyVector[i].x += (vetorx)*(0.89f);
  enemyVector[i].y += (vetory)*(0.89f);
}

void desenhaCena(void){

    glClear(GL_COLOR_BUFFER_BIT);
    if(menu==0 && pause==0){               // SE O ITEM JA ESTIVER SPAWNADO, DESENHA O ITEM.
      drawOnScreenSize(6,&globalWidth,&globalHeight);
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
            glTranslatef(ENEMY_ISOLA,ENEMY_ISOLA,-50);
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
          drawShot(&raioTiro,&tiroGrande,&tiroVara);
          glPopMatrix();
      }
     if(itemColisao()!=1 && coletou==0){
        glPushMatrix();
          glTranslatef(item_Coletavel[itemCount].x,item_Coletavel[itemCount].y,0);
          drawItem();
        glPopMatrix();
    }
    if(itemColisao()==1){
      coletou=1;
      if(pickupControl==0){
          Mix_PlayChannel( -1, itemPickup, 0 );
          pickupControl=1;
      }
            if(item_Coletavel[itemCount].id==0){
              tiroGrande=1;
            }
            if(item_Coletavel[itemCount].id==1){
              tiroVara=1;
            }
            if(item_Coletavel[itemCount].id==2){
              bigEnemies=1;
            }
      glutTimerFunc(5000,gastaItem,0);
    }
      if(pause==1){
        drawOnScreenSize(4,&globalWidth,&globalHeight);
      }
      drawOnScreenSize(7,&globalWidth,&globalHeight);
      glPushMatrix();
      glTranslatef(globalWidth-(460*globalWidth/LARGURA_DO_MUNDO),globalHeight-15,0);
        drawLifeBar(vida);
      glPopMatrix();
      drawHud(&globalWidth, &globalHeight);
      glutSwapBuffers();
    }
}

// Inicia algumas variáveis de estado
void inicializa(void){
    // cor para limpar a tela
    srand(time(NULL));
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
    createItem();
    loadSecretItem();
    loadBackground2();
    loadArrowMenu();
    loadMenu();
    loadInstrucoes();
    loadGameOver();
    loadCredits();
    f_record = fopen("highscore.txt","r");
    if(f_record==NULL){
        f_highscore=0;
    }
    else{
  fscanf(f_record,"%d",&f_highscore);
  fclose(f_record);
    }

    // Efeitos sonoros
 if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ){
     printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
 }

 //Initialize SDL_mixer
 if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
     printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
 }
 Mix_Music *backGroundMusic = Mix_LoadMUS("soundfx/backGroundMusic.mp3");
 if( backGroundMusic == NULL ){
     printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
 }
 shotSound = Mix_LoadWAV( "soundfx/shotSound.wav" );
 if( shotSound == NULL ){
     printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
 }
 itemPickup = Mix_LoadWAV( "soundfx/itemPickup.wav" );
 if( itemPickup == NULL ){
     printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
 }
 dyingEnemySound = Mix_LoadWAV( "soundfx/dyingEnemySound.wav" );
 if( dyingEnemySound == NULL ){
     printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
 }
 gameOverSound = Mix_LoadWAV( "soundfx/gameOverSound.wav" );
 if( gameOverSound == NULL ){
     printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
 }

 Mix_PlayMusic( backGroundMusic, -1 );

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
    if(tiroNaTela==1){
      tiro_AvaliaPosicao();
    }
    if(bigEnemies==0){
      raio_enemy=10;
    }

    for(int i=0;i<enemyCount;i++){ //LOOP PARA CONFIGURAR OS INIMIGOS
        if(colisao(i)==1){          //verifica se houve colisao do player com inimigos
          if(vida>0){
            vida--;
          }
        }
      if(tiro_colisao(i)==1){
        enemyVector[i].isDead = 1;
        if(bigEnemies==0){
          Mix_PlayChannel(-1,dyingEnemySound,0);
        }
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
    if(gameOverControl==0){
      Mix_PlayChannel(-1,gameOverSound,0);
      gameOverControl=1;
    }
  }

  if(pause==1 && menu ==0){
    drawOnScreenSize(4,&globalWidth,&globalHeight);
    glutSwapBuffers();
  }
  else if(menu == 1 && pause==0){
      vida=3;
      pontuacao=0;
      drawOnScreenSize(1,&globalWidth,&globalHeight);
      glutSwapBuffers();
  }
  else if(menu == 0 && scoreBoard == 1){
    drawOnScreenSize(5,&globalWidth,&globalHeight);
    f_record=fopen("Record.txt","r");
    fscanf(f_record,"%d",&f_highscore);
    sprintf(stringPontuacao, "%d", f_highscore);
    drawScreenText((globalWidth/2),globalHeight/2,1,1,1,stringPontuacao);
    glutSwapBuffers();
  }
  else if(menu==0 && credits == 1){
    drawOnScreenSize(3,&globalWidth,&globalHeight);
    glutSwapBuffers();
  }
  else if(gameOver==1 && menu==0){
    if(pontuacao>f_highscore){
  f_highscore=pontuacao;
  f_record=fopen("Record.txt","w");
  fprintf(f_record,"%d",f_highscore);
  fclose(f_record);
    }
    drawOnScreenSize(8,&globalWidth,&globalHeight);
    glutSwapBuffers();
  }
  else if(instrucoes==1 && menu == 0){
      drawOnScreenSize(2,&globalWidth,&globalHeight);
      glutSwapBuffers();
  }

  if(menu!=0 && pause==0){      // DESENHA A SETA DO MENU
      drawOnScreenSize(1,&globalWidth,&globalHeight);
        glPushMatrix();
        arrowMenu_converteCord();
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
        case 13:        // Tecla ENTER
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
        case 8:        // Tecla BACKSPACE
          if(gameOver==1){
            enemyCount=0;
          }
            gameOver=0;
            menu=1;
            credits=0;
            scoreBoard=0;
            instrucoes=0;
            break;
        case 27:         // Tecla ESC
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
    glutTimerFunc(0,itemSpawn,0);
    if(coletou==1){
      glutTimerFunc(5000,gastaItem,0);
    }
    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
