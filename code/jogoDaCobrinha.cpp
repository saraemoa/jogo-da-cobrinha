#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_TABULEIRO 20

typedef struct {
    int x, y;
} Segmento;

typedef struct {
    int x, y;
} Comida;

Segmento cobrinha[100];
int tamanhoCobrinha = 5;
int direcao = 1;
Comida comida;
int pontuacao = 0, recorde = 0;
char pontuacaoString[10], recordeString[50];
int gameOver = 0, win = 0;
int velocidade = 0, objetivo = 0;
int infinito, titulo = 1;
int melhorPontuacaoInfinito = 0;

void Inicializa() {
    tamanhoCobrinha = 2;
    for (int i = 0; i < tamanhoCobrinha; i++) {
        cobrinha[i].x = TAMANHO_TABULEIRO / 2 - i;
        cobrinha[i].y = TAMANHO_TABULEIRO / 2;
    }

    srand(time(NULL));
    comida.x = rand() % TAMANHO_TABULEIRO;
    comida.y = rand() % TAMANHO_TABULEIRO;

    pontuacao = 0;
    gameOver = 0;
    win = 0;

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, TAMANHO_TABULEIRO, 0, TAMANHO_TABULEIRO);
}

void desenharTabuleiro(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(73, 218, 73);
    glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(0, TAMANHO_TABULEIRO);
        glVertex2i(TAMANHO_TABULEIRO, TAMANHO_TABULEIRO);
        glVertex2i(TAMANHO_TABULEIRO, 0);
    glEnd();
}

void desenharCobrinha(){
    glColor3ub(78, 51, 250);
    glBegin(GL_QUADS);
    for (int i = 0; i < tamanhoCobrinha; i++) {
        glVertex2i(cobrinha[i].x, cobrinha[i].y);
        glVertex2i(cobrinha[i].x + 1, cobrinha[i].y);
        glVertex2i(cobrinha[i].x + 1, cobrinha[i].y + 1);
        glVertex2i(cobrinha[i].x, cobrinha[i].y + 1);
    }
    glEnd();
}

void desenharComida() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2i(comida.x, comida.y);
    glVertex2i(comida.x + 1, comida.y);
    glVertex2i(comida.x + 1, comida.y + 1);
    glVertex2i(comida.x, comida.y + 1);
    glEnd();
}

void DesenhaTexto(const char *string) {  
	glPushMatrix();         
	glRasterPos2f(7.25, TAMANHO_TABULEIRO / 2); 
	while(*string){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
	}  
	glPopMatrix();
}

void exibirTitulo(){
	glColor3ub(0, 0, 0);
	glPushMatrix(); 
	const char* titulo = "JOGO DA COBRINHA";
	glRasterPos2f(6.5, TAMANHO_TABULEIRO - 5); 
	while(*titulo){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *titulo++);
	} 
	glPopMatrix();
}

void exibirStatus(){
	if(win){
		const char* ganhou = "VOCE GANHOU!";
		DesenhaTexto(ganhou);
	}else if(gameOver){
		const char* perdeu = "  GAME OVER!";
		DesenhaTexto(perdeu);
		if(infinito){
		    char melhorPontuacao[50];
		    sprintf(melhorPontuacao, "Melhor pontuacao: %d", melhorPontuacaoInfinito);
		    glRasterPos2f(6.75, TAMANHO_TABULEIRO / 2 - 1); 
		    for (int i = 0; melhorPontuacao[i] != '\0'; i++) {
		        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, melhorPontuacao[i]);
		    }
		}
	}
}

void exibirPontuacao(int r, int g, int b) {
    glColor3ub(r, g, b);
    glRasterPos2f(1, TAMANHO_TABULEIRO - 2);
    for (int i = 0; pontuacaoString[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, pontuacaoString[i]);
    }
}

void atualizarJogo(int value){
    for (int i = tamanhoCobrinha - 1; i > 0; i--) {
        cobrinha[i].x = cobrinha[i - 1].x;
        cobrinha[i].y = cobrinha[i - 1].y;
    }

    switch (direcao) {
        case 0: cobrinha[0].y--; break;
        case 1: cobrinha[0].x++; break;
        case 2: cobrinha[0].y++; break;
        case 3: cobrinha[0].x--; break;
    }

    if (cobrinha[0].x == comida.x && cobrinha[0].y == comida.y) {
        tamanhoCobrinha++;
        comida.x = rand() % TAMANHO_TABULEIRO;
        comida.y = rand() % TAMANHO_TABULEIRO;
        pontuacao++;
        sprintf(pontuacaoString, "%d", pontuacao);
        if(pontuacao > recorde){
        	recorde = pontuacao;
        	sprintf(recordeString, "%d", recorde);
		}
    }

    if(pontuacao == objetivo){
        gameOver = 0;
        win = 1;
    }

    for (int i = 1; i < tamanhoCobrinha; i++) {
        if (cobrinha[0].x == cobrinha[i].x && cobrinha[0].y == cobrinha[i].y) {
            gameOver = 1;
            win = 0;
            if (infinito && pontuacao > melhorPontuacaoInfinito) {
                melhorPontuacaoInfinito = pontuacao;
            }
        }
    }

    if (cobrinha[0].x < 0 || cobrinha[0].x >= TAMANHO_TABULEIRO || cobrinha[0].y < 0 || cobrinha[0].y >= TAMANHO_TABULEIRO) {
        gameOver = 1;
        win = 0;
        if (infinito && pontuacao > melhorPontuacaoInfinito) {
            melhorPontuacaoInfinito = pontuacao;
        }
    }
    
    if(win || gameOver){
    	exibirStatus();
    	glutPostRedisplay();
    	return;
	}

    glutPostRedisplay();
    glutTimerFunc(velocidade, atualizarJogo, 0);
}

void teclasEspeciais(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN:
            if (direcao != 2) direcao = 0;
            break;
        case GLUT_KEY_RIGHT:
            if (direcao != 3) direcao = 1;
            break;
        case GLUT_KEY_UP:
            if (direcao != 0) direcao = 2;
            break;
        case GLUT_KEY_LEFT:
            if (direcao != 1) direcao = 3;
            break;
    }
}

void teclasNormais(unsigned char key, int x, int y){
	if(key == 27){
		exit(0);
	}
	if(key == 32){
		glutTimerFunc(velocidade, atualizarJogo, 0);
	}	
}

void iniciarJogo(int vel, int obj){
    velocidade = vel;
    objetivo = obj;
    Inicializa();
}

void menu(int escolha){
    switch(escolha){
        case 1:
            titulo = 0;
            infinito = 0;
            pontuacao = 0;
            sprintf(pontuacaoString, "%d", pontuacao);
            iniciarJogo(150, 5);
            break;
        case 2:
            titulo = 0;
            infinito = 0;
            pontuacao = 0;
            sprintf(pontuacaoString, "%d", pontuacao);
            iniciarJogo(125, 10);
            break;
        case 3:
            titulo = 0;
            infinito = 0;
            pontuacao = 0;
            sprintf(pontuacaoString, "%d", pontuacao);
            iniciarJogo(100, 15);
            break;
        case 4:
            titulo = 0;
            infinito = 0;
            pontuacao = 0;
            sprintf(pontuacaoString, "%d", pontuacao);
            iniciarJogo(75, 20);
            break;
        case 5:
            titulo = 0;
            infinito = 1;
            pontuacao = 0;
            sprintf(pontuacaoString, "%d", pontuacao);
            iniciarJogo(100, 9999999);
            break;
        default:
            break;
    }
}

void CriaMenu(){
    int submenu2 = glutCreateMenu(menu);
    glutAddMenuEntry("Fase 1", 1);
    glutAddMenuEntry("Fase 2", 2);
    glutAddMenuEntry("Fase 3", 3);
    glutAddMenuEntry("Fase 4", 4);

    int submenu1 = glutCreateMenu(menu);
    glutAddSubMenu("Fases", submenu2);
    glutAddMenuEntry("Infinito", 5);

    int menuPrincipal = glutCreateMenu(menu);
    glutAddSubMenu("Jogar", submenu1);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void desenhar(){
    desenharTabuleiro();
    if(titulo)
		exibirTitulo();
    if (!win && !gameOver){
        desenharCobrinha();
        desenharComida();
        exibirPontuacao(0, 0, 0);	
    }else{
        glClear(GL_COLOR_BUFFER_BIT);
        exibirStatus();
        exibirPontuacao(73, 218, 73);
    }
    glutSwapBuffers();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Jogo da Cobrinha");
    Inicializa();
    glutDisplayFunc(desenhar); 
    glutSpecialFunc(teclasEspeciais);
    glutKeyboardFunc(teclasNormais);
    CriaMenu();
    glutMainLoop();
    return 0;
}

