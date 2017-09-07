#include <GL/glew.h>
#include<stdio.h>
#include <GL/freeglut.h>
#include "characterMoves.h"

#define LARGURA_DO_MUNDO 160
#define ALTURA_DO_MUNDO 90

float axisx=0, axisy=0;
int up=0,down=0,left=0,right=0;

/*void worldShape(){

	glColor3f(0, .5, 0);  // verde escuro
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	desenhaRetangulo(1, 1, LARGURA_DO_MUNDO-2, ALTURA_DO_MUNDO-2);
}*/

void reBuffer(){
	glClear(GL_COLOR_BUFFER_BIT);
	movimenta(up,down,left,right,&axisx,&axisy);
	characterShape(axisx,axisy);
	glutSwapBuffers();
}

void inicializa(void){
	// cor para limpar a tela
	glClearColor(1, 1, 1, 0);      // branco
}

// Callback de redimensionamento
/*void redimensiona(int w, int h){

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 100, 0, 100, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}*/

void redimensiona(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARGURA_DO_MUNDO, 0, ALTURA_DO_MUNDO, -1, 1);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float) LARGURA_DO_MUNDO)/ ALTURA_DO_MUNDO;
    // se a janela está menos larga do que o mundo (16:9)...
    if (razaoAspectoJanela < razaoAspectoMundo) {
        // vamos colocar barras verticais (acima e abaixo)
        float hViewport = width / razaoAspectoMundo;
        float yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    // se a janela está mais larga (achatada) do que o mundo (16:9)...
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        // vamos colocar barras horizontais (esquerda e direita)
        float wViewport = ((float)height) * razaoAspectoMundo;
        float xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Callback de evento de teclado
void teclado(unsigned char key, int A, int B){

	switch(key){

			case 27:
				exit(0);
				break;
			case 'w':
				resetaMovimento(&up,&down,&left,&right);
				up=1;
				glutPostRedisplay();
				break;
			case 's':
				resetaMovimento(&up,&down,&left,&right);
				down=1;
				glutPostRedisplay();
				break;
			case 'd':
				resetaMovimento(&up,&down,&left,&right);
				right=1;
				glutPostRedisplay();
				break;
			case 'a':
				resetaMovimento(&up,&down,&left,&right);
				left=1;
				glutPostRedisplay();
				break;
			default:
				break;
	}
}


// Rotina principal
int main(int argc, char **argv){

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
	glutCreateWindow("Tp1");

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
