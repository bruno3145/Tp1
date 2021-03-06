#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include "maths.h"

#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)

/** Vetor - Calcula ângulo entre vetores
* @param v1 Vetor 1
* @param v2 Vetor 2
* @return ângulo entre os vetores v1 e v2
**/

Vetor vetor_criaVetor(int x,int xx, int y, int yy){
  struct Vetor v;

  v.x=(xx-x);
  v.y=(yy-y);

  return struct v;
}

double vetor_calculaAngulo(struct Vetor v1, struct Vetor v2){
    // Normaliza os 2 vetores
    vetor_normalizaVetor(&v1);
    vetor_normalizaVetor(&v2);

    double _angulo = acos(vetor_produtoEscalar(v1,v2));
    if(v1.y < 0) _angulo=2*M_PI-_angulo; // A função normalmente é definida de 0 a 180, com isso passa a ser de 0 a 360
    return radianoParaGraus(_angulo);
}

/** Vetor - Calcula produto escalar entre vetores
* @param v1 Vetor 1
* @param v2 Vetor 2
* @return o produto escalar entre os vetores v1 e v2
**/
double vetor_produtoEscalar(struct Vetor v1, struct Vetor v2){
    return (v1.x*v2.x)+(v2.y+v2.y);
}

/** Vetor - Normaliza vetor
* @param v Vetor a ser normalizado
**/
void vetor_normalizaVetor(struct Vetor *v){
    double _moduloVetor = vetor_calculaModulo(*v);
    v->x = v->x/_moduloVetor;
    v->y = v->y/_moduloVetor;
}

/** Vetor - Calcula módulo
* @param v Vetor a ter módulo calculado
* @return o módulo de v
**/
double vetor_calculaModulo(struct Vetor v){
    double _moduloVetor = sqrt(pow(v.x,2)+pow(v.y,2));
    return _moduloVetor;
}

/** Geral - Escreve texto
* @param font Fonte de texto
* @param s String a ser escrita
* @param x posição x
* @param y posição y
* @param z posição z
**/
void escreveTexto(void * font, char *s, float x, float y, float z){
    int i;
    glRasterPos3f(x, y, z);

    for (i=0; i < strlen(s); i++)
       glutBitmapCharacter(font, s[i]);
}
