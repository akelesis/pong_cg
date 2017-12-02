/*******************************************

PONG - PROJETO DE COMPUTAÇÃO GRÁFICA
AUTOR: MARCUS TAVARES
DATA DE INICIO: 12.10.2017

********************************************/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_WINDOW 200
#define MIN_WINDOW -200
#define X_DIR 185
#define X_ESC -185

void init(void);
void display(void);
void drawCourt(void);
void drawChars(void);
void drawBall(void);
void drawScore(GLint scorea, GLint scoreb);
void keyboard(unsigned char key, int x, int y);
void updateBall(void);
void colideEsq(void);
void colideDir(void);
void aceleraY(void);
void setY(void);

// Variáveis
GLfloat ballPositionX = 0, ballPositionY = 0;
GLfloat acelera_y = 0;
GLint y_dir_max = 20, y_dir_min = -20, y_esq_max = 20, y_esq_min = -20;
GLint ballFlag = 0;
GLint scorea = 0, scoreb = 0;
GLint toggle = 1;


int main(int argc, char** argv){

    glutInit(&argc, argv); // Inicializa o GLUT e processa qualquer parâmetro passado pela linha de comandos. Deve ser chamada antes de qualqueroutra rotina GLUT.
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); // Indica se vamos usar cores nomodo RGBA ou Indexado. Também especificamos se usaremos um ou dois buffers para a geração das imagens e se vamos associar buffers de profundidade, estêncil e/ou acumulador à janela que estamos usando
    glutInitWindowSize (800, 600); // Especifica as dimensões da janela em pixels.
    glutInitWindowPosition (100, -100); // Especifica a coordenada superior esquerda dajanela
    glutCreateWindow ("Pong"); // Cria a janela e devolve um identificadorúnico para a janela. Até que o comando glutMainLoop sejachamado, a janela não será mostrada.
    init( );
    glutDisplayFunc(display); // Toda vez que o GLUT determinar que a janela tem deser desenhada, ele chamará a função aqui determinada.
    glutKeyboardFunc(keyboard); // Determinam as funções que usaremos para ler oteclado e o mouse respectivamente.
    glutMainLoop( ); // É o último comando que chamamos. Ele faz com que todas as janelas criadas sejam mostradas. Uma vez que entramos neste loop,só saímos quando o programa se encerra.
    return 0;
    }
void init(void){
    glClearColor(0.3, 0.5, 1.0, 1.0); // Fornece os valores para limpeza do buffer de cor nomodo RGBA
    glOrtho (MIN_WINDOW, MAX_WINDOW, MIN_WINDOW, MAX_WINDOW, -1, 1); // Seleciona o modo de projeção Ortogonal.
}
void display(void){
    glClear(GL_COLOR_BUFFER_BIT);    // Limpa toda a Janela para a cor do comando
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity( );
    drawCourt( );
    drawChars( );
    drawScore(scorea, scoreb);
    drawBall( );
    updateBall( );
    glutSwapBuffers( );
}

void drawCourt(void){
        glColor3f (1.0, 1.0, 1.0); // Seleciona a cor branca para a linha
        glLineWidth(4.0);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-190, -150, 1);
        glVertex3f(190, -150, 1);
        glVertex3f(190, 150, 1);
        glVertex3f(-190, 150, 1); // Fornece as coordenadas dos ponto siniciais e finais
        glEnd( );
        glBegin(GL_LINES);
        glVertex3d(0, 150, 1);
        glVertex3d(0, -150, 1);
        glEnd( );
        glBegin(GL_LINES);
        glVertex3d(-190, 100, 1);
        glVertex3d(190, 100, 1);
        glEnd( );
        glBegin(GL_LINES);
        glVertex3d(-190, -100, 1);
        glVertex3d(190, -100, 1);
        glEnd( );
        glBegin(GL_LINES);
        glVertex3d(-80, 0, 1);
        glVertex3d(80, 0, 1);
        glEnd( );
        glBegin(GL_LINES);
        glVertex3d(-80, -100, 1);
        glVertex3d(-80, 100, 1);
        glEnd( );
        glBegin(GL_LINES);
        glVertex3d(80, -100, 1);
        glVertex3d(80, 100, 1);
        glEnd( );
}

void drawChars(void){
        glColor3f(0.8, 0.1, 0.1);
        glLineWidth(4.0);
        glBegin(GL_LINES);
        glVertex3f(X_ESC, y_esq_min, 1);
        glVertex3f(X_ESC, y_esq_max, 1);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(X_DIR, y_dir_min, 1);
        glVertex3f(X_DIR, y_dir_max, 1);
        glEnd();
}

void drawBall(){
        glColor3f(0.8, 0.9, 0.3);
        glBegin(GL_QUADS);
        glVertex3f(ballPositionX - 4.0, ballPositionY + 4.0, 1);
        glVertex3f(ballPositionX + 4.0, ballPositionY + 4.0, 1);
        glVertex3f(ballPositionX + 4.0, ballPositionY - 4.0, 1);
        glVertex3f(ballPositionX - 4.0, ballPositionY - 4.0, 1);
        glEnd();
}

void updateBall(void){
    if(ballFlag == 0){
        if(ballPositionX > -200){
            ballPositionX -= 1;
            colideEsq();
            aceleraY();
        }
        else{
            ballFlag = 1;
            ballPositionX = 0;
            acelera_y = 0;
            ballPositionY = 0;
            scoreb++;
        }
    }
    if(ballFlag == 1){
        if(ballPositionX < 200){
            ballPositionX += 1;
            colideDir();
            aceleraY();
        }
        else{
            ballFlag = 0;
            ballPositionX = 0;
            acelera_y = 0;
            ballPositionY = 0;
            scorea++;
        }
    }
    glutPostRedisplay();
}

void colideEsq(void){
    if(ballPositionX <= X_ESC + 5 && ballPositionX >= X_ESC - 5){
        if(ballPositionY >= y_esq_min && ballPositionY <= y_esq_max){
            ballFlag = 1;
            setY();
        }
    }
}

void colideDir(void){
    if(ballPositionX <= X_DIR + 5 && ballPositionX >= X_DIR - 5){
        if(ballPositionY >= y_dir_min && ballPositionY <= y_dir_max){
            ballFlag = 0;
            setY();
        }
    }
}

void aceleraY(void){
    ballPositionY += acelera_y;
    if(ballPositionY >= MAX_WINDOW || ballPositionY <= MIN_WINDOW){
        acelera_y *= (-1);
    }
}

void setY(void){
    float div = RAND_MAX / 2;
    if(toggle == 1){
        toggle = -1;
        acelera_y = toggle*(rand() / div);
        return;
    }
    else{
        toggle = 1;
        acelera_y = toggle*(rand() / div);
        return;
    }
}

void drawScore(GLint scorea, GLint scoreb){
        switch(scorea){
        case 0:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINE_LOOP);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 30, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 1:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 35, MAX_WINDOW -30, 1);
            glVertex3f(MIN_WINDOW + 35, MAX_WINDOW -10, 1);
            glEnd( );
            break;
        case 2:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 30, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 3:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 30, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glEnd( );
            break;
        case 4:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 5:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 30, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 6:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 30, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 7:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 8:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINE_LOOP);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 30, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glEnd( );
            break;
        case 9:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 20, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 10, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MIN_WINDOW + 30, MAX_WINDOW - 30, 1);
            glVertex3f(MIN_WINDOW + 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        }

        switch(scoreb){
        case 0:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINE_LOOP);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 1:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 35, MAX_WINDOW -30, 1);
            glVertex3f(MAX_WINDOW - 35, MAX_WINDOW -10, 1);
            glEnd( );
            break;
        case 2:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 30, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 3:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glEnd( );
            break;
        case 4:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 5:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 6:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 7:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        case 8:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINE_LOOP);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glEnd( );
            break;
        case 9:
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 20, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 20, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 10, 1);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glEnd( );
            glBegin(GL_LINES);
            glVertex3f(MAX_WINDOW - 30, MAX_WINDOW - 30, 1);
            glVertex3f(MAX_WINDOW - 40, MAX_WINDOW - 30, 1);
            glEnd( );
            break;
        }
    }



    void keyboard(unsigned char key, int x, int y){
        switch (key) {
        case 'w':
            if(y_esq_max < MAX_WINDOW){
                y_esq_max = (y_esq_max) + 5;
                y_esq_min = (y_esq_min) + 5;
            }
            break;
        case 's':
            if(y_esq_min > MIN_WINDOW){
                y_esq_max = y_esq_max - 5;
                y_esq_min = y_esq_min - 5;
            }
            break;
        case 'o':
            if(y_dir_max < MAX_WINDOW){
                y_dir_max = (y_dir_max) + 5;
                y_dir_min = (y_dir_min) + 5;
            }
            break;
        case 'l':
            if(y_dir_min > MIN_WINDOW){
                y_dir_max = y_dir_max - 5;
                y_dir_min = y_dir_min - 5;
            }
            break;
            case 27:
                exit(0);
                break;
        }
        glutPostRedisplay();
    }
