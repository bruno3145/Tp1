#include <GL/glew.h>
#include<stdio.h>
#include <GL/freeglut.h>
int x=0, y=0, ww=0, dd=0, aa=0, ss=0;

void square(int k, int l){

	glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(k, l, 0);
        glVertex3f(k+10, l, 0);
        glVertex3f(k+10, l+10, 0);
        glVertex3f(k, l+10, 0);
        glEnd();
}

void reBuffer(){
	if(x>100){
		x=0;
	}
	if(y>100){
		y=0;
	}
	if(y<0){
		y+=100;
	}
	if(x<0){
		x+=100;
	}
	 if(ww==1){
		y++;
	 }
	 if(ss==1){
		y--;
	 }
	 if(aa==1){
		x--;
	 }
	if(dd==1){
		x++;
	}
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1);
	square(x,y);
	glutSwapBuffers();
}

// Inicia algumas variáveis de estado
void inicializa(void)
{
    // cor para limpar a tela
    glClearColor(1, 1, 1, 0);      // branco
}

// Callback de redimensionamento
void redimensiona(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 100, 0, 100, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Callback de evento de teclado
void teclado(unsigned char key)
{
   switch(key)
   {
      // Tecla ESC
      case 27:
         exit(0);
         break;
      case 'w':
	aa=0;
	ss=0;
	dd=0;
	if(ww==1){
		ww=0;
	}
	else
	ww=1;
	glutPostRedisplay();
	break;
      case 's':
	aa=0;
	ww=0;
	dd=0;
	if(ss==1){
		ss=0;
	}
	else
	ss=1;
	glutPostRedisplay();
	break;
      case 'd':
	aa=0;
	ss=0;
	ww=0;
	if(dd==1){
		dd=0;
	}
	else
	dd=1;
	glutPostRedisplay();
	break;
      case 'a':
	ww=0;
	ss=0;
	dd=0;
	if(aa==1){
		aa=0;
	}
	else
	aa=1;
	glutPostRedisplay();
	break;
      default:
         break;
   }
}


// Rotina principal
int main(int argc, char **argv)
{
    // Acordando o GLUT
    glutInit(&argc, argv);

    // Definindo a versão do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    // Configuração inicial da janela do GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("tp1");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(reBuffer);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutIdleFunc(glutPostRedisplay);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
