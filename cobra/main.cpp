/************************************************/
//
//	Exercicio de Computação gráfica
//
//	Programa Joguinho da Cobra
//
/************************************************/
//
// Alunos:	Giulianno G. Latorraca
//			Raiane Pereira Alcantara
//			Vinicius Henrique da Silva Souzas
//			Felipe Israel A C Barbosa
//
//-----------------------------------------------
//
//	Programa usado: Code Blocks
//	Bibliotecas usadas - GLUT, FREEGLUT e MinGW
//  Guia de instalação - http://wiki.codeblocks.org/index.php/Using_FreeGlut_with_Code::Blocks
//
//	A como inicia com o tamanho 3
//	A comida inicia junto com a cobra na tela
//	A tela não pode ser modificade por causa da função Corige_Tela()
//	Os controles de jogo padrões são - W,S,A,D
//	A cada 5 comidas que a cobrinha comer a velocidade irá aumentar
//	A função nova comida irá criar uma nova comida aleatóriamente
//	Ao fim do jogo o usuário irá receber um aviso de fim de jogo junto com a sua pontuação
//  Fim de jogo - Caso a cobra bater em alguma lateral ou em seu proprio corpo
//
//-----------------------------------------------
//
//Professor: Gustavo Vinhal
//			 Computação Gráfica
//
/************************************************/



/***************************************************/
//
//					INCLUDES
//
/***************************************************/
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include "windows.h"
#include <iostream>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <sstream>
#define GLUT_DISABLE_ATEXIT_HACK
#include <ctime>

//std
using namespace std;

/***************************************************/
//
//					VARIAVEIS UTILIZADAS
//
/***************************************************/

/****************/
//	NOTA
/****************/
//	ESQUERDA 0
//	DIREITA 1
//	CIMA 2
//	BAIXO 3
/****************/
int direcao_da_cobra=1;
//
int pontuacao=0;
int cobra_tamanho=3;

#define ESQUERDA 0
#define DIREITA 1
#define CIMA 2
#define BAIXO 3

#define FIM 999
bool cobra_colisao=false;
bool cobra_comeu=false;
float cobra_velocidade = 1;
int cobra_continua_velocidade=0;
list < pair<int,int> > corpo;
pair <int,int> comida;

/*
int random(int, int);
bool seedflag = false;
bool comida = false;
int linhas = 0, colunas = 0;
int comidax, comiday;
*/

/***************************************************/
//
//					MENSAGEM
//
/***************************************************/

void Escrever(float x, float y, void *font, const char* string, float R,float G, float B )
{
	glColor3f(R,G,B);
	glRasterPos2f(x, y);
	const unsigned char* t = reinterpret_cast<const unsigned char *>(string);
	glutBitmapString(font, t);
}


/***************************************************/
//
//					TELA DE JOGO
//
/***************************************************/
void Desenhar_Tela_de_Jogo(void)
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

		/***************************************************/
		//					FUNDO DA TELA
		/***************************************************/
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			glVertex2i(3,235);
		    glVertex2i(3,3);
		    glVertex2i(325,3);
		    glVertex2i(325,235);
	glEnd();

	glPushMatrix();

		/***************************************************/
		//					PRINCIPAL
		/***************************************************/
		string texto9 = "Jogo da Cobra  - Giulianno, Raiane, Vinicius e Felipe                Controles: W,S,A,D";
		char texto10[texto9.length() + 1];
		strcpy(texto10, texto9.c_str());
		Escrever(10.0f, 240.0f, GLUT_BITMAP_HELVETICA_12, texto10, 1.0f, 1.0f, 1.0f);

		/***************************************************/
		//					PONTUACAO
		/***************************************************/
		string texto2 = "Pontuacao: ";
		stringstream stream;
		stream << pontuacao;
		texto2 += stream.str();
		char texto3[texto2.length() + 1];
    	strcpy(texto3, texto2.c_str());
		Escrever(240.0f, 240.0f, GLUT_BITMAP_HELVETICA_12, texto3, 1.0f, 1.0f, 1.0f);
		/***************************************************/
		//					VELOCIDADE
		/***************************************************/
		string texto4 = "Velocidade: ";
		stringstream stream2;
		stream2 << cobra_velocidade;
		texto4 += stream2.str();
		char texto5[texto4.length() + 1];
    	strcpy(texto5, texto4.c_str());
		Escrever(290.0f, 240.0f, GLUT_BITMAP_HELVETICA_12, texto5, 1.0f, 1.0f, 1.0f);

	glPopMatrix();


	if(cobra_colisao==true)
	{
		/***************************************************/
		//					TELA DE FIM DE JOGO
		/***************************************************/
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			glVertex2i(50,225);
		    glVertex2i(50,50);
		    glVertex2i(275,50);
		    glVertex2i(275,225);
		glEnd();

		direcao_da_cobra=FIM;

		glColor3f(1.0, 0.0, 0.0);
		glRasterPos2f(128, 180);
		const unsigned char* texto = reinterpret_cast<const unsigned char *> ("FIM DE JOGO");
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,texto);
		//MessageBox(NULL, L"Seus pontos: ", L"Game Over", 0);
		string texto2 = "Pontuacao: ";
		stringstream stream;
		stream << pontuacao;
		texto2 += stream.str();
		char texto3[texto2.length() + 1];
    	strcpy(texto3, texto2.c_str());

		Escrever(133.0f, 120.0f, GLUT_BITMAP_TIMES_ROMAN_24, texto3, 0.0f, 0.0f, 0.0f);
	}
	else{
		/***************************************************/
		//					COMIDA
		/***************************************************/
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex2i(comida.first ,comida.second+10);
			glVertex2i(comida.first ,comida.second);
			glVertex2i(comida.first+10,comida.second);
			glVertex2i(comida.first+10,comida.second+10);
		glEnd();
		for (list< pair<int,int> >::iterator it=corpo.begin(); it != corpo.end(); ++it)
		{
			glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_QUADS);
				pair<int,int> aux = *it;
				glVertex2i(aux.first ,aux.second+10);
				glVertex2i(aux.first ,aux.second);
				glVertex2i(aux.first+10,aux.second);
				glVertex2i(aux.first+10,aux.second+10);
			glEnd();
		}
	}
    glutSwapBuffers();
}


/***************************************************/
//
//					NOVA COMIDA
//
/***************************************************/
void Nova_Comida(){
	int posX,posY;
	srand (time(NULL));
	posX = rand() % 26 + 1;
	posY = rand() % 20 + 1;
	int cont=0;
	bool pode = true;
	for (list< pair<int,int> >::iterator it=corpo.begin(); it != corpo.end(); ++it)
	{
		if(cont>0)
		{
			pair<int,int> aux = *it;
			if(posX==aux.first&&posY==aux.second)
			{
				pode=false;
				break;
			}
		}
		cont++;
	}
	if(pode==false)
	{
		Nova_Comida();
	}
	else{
		comida.first=posX*11;
		comida.second=posY*11;
	}
}

/*
void Nova_Comida()
{
	if (!comida)
	{
		comidax = random(2, colunas - 2);
		comiday = random(2, linhas - 2);
		std::cout << comidax << comiday << std::endl;
		comida = true;
	}
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2d(comidax, comiday); glVertex2d(comidax + 1, comiday); glVertex2d(comidax + 1, comiday + 1); glVertex2d(comidax, comiday + 1);
	glEnd();
}
*/

/*
int random(int _min, int _max)
{
	if (!seedflag)
	{
		srand(time(NULL));
		seedflag = true;
	}
	else
		seedflag = false;
	return _min + rand() % (_max - _min);
}
*/

/***************************************************/
//
//					COLISAO
//
/***************************************************/
void VerificaColisao()
{
		/***************************************************/
		//					A COBRA COMEU UMA COMIDA?
		/***************************************************/
	if(corpo.front().first==comida.first&&corpo.front().second==comida.second)
	{
		cobra_comeu=true;
		pontuacao++;
		cobra_tamanho++;
		cobra_continua_velocidade++;
		if(cobra_continua_velocidade==5)
		{
			cobra_velocidade++;
			cobra_continua_velocidade=0;
		}
		Nova_Comida();
	}
		/***************************************************/
		//					A COBRA COLIDIU NELA MESMA?
		/***************************************************/
	int cont=0;
	for (list< pair<int,int> >::iterator it=corpo.begin(); it != corpo.end(); ++it)
	{
		if(cont>0)
		{
			pair<int,int> aux = *it;
			if(corpo.front().first==aux.first&&corpo.front().second==aux.second)
			{
				cobra_colisao=true;
				break;
			}
		}
		cont++;
	}
		/***************************************************/
		//					A COBRA COLIDIU NA PAREDE?
		/***************************************************/
	switch(direcao_da_cobra)
	{
		case CIMA: // frente
			if(corpo.front().second>=230)
				cobra_colisao=true;
    		break;
		case BAIXO: // tras
		  	if(corpo.front().second<=10)
				cobra_colisao=true;
		    break;
		case ESQUERDA: // esquera
     		if(corpo.front().first<=10)
				cobra_colisao=true;
    		break;
		case DIREITA: // direita
     	 	 if(corpo.front().first>=310)
				cobra_colisao=true;
			break;
	}
}

/***************************************************/
//
//					CONTROLE DO JOGO
//
/***************************************************/
void Controle(unsigned char key, int a, int b)
{
	bool direcaoInvalida=false;
    switch (key)
    {
		/***************************************************/
		//					CONTROLES DO JOGO
		/***************************************************/
		//					W,A,S,D
		/***************************************************/
        case 'w': // cima
  			if(!(corpo.front().second>=230)&&direcao_da_cobra!=BAIXO){
				corpo.push_front(make_pair(corpo.front().first,corpo.front().second+11));
				direcao_da_cobra=CIMA;
			}
			if(direcao_da_cobra==BAIXO)
				direcaoInvalida=true;

            break;
        case 's': // baixo
        	if(!(corpo.front().second<=10)&&direcao_da_cobra!=CIMA){
				corpo.push_front(make_pair(corpo.front().first,corpo.front().second-11));
				direcao_da_cobra=BAIXO;
			}
			if(direcao_da_cobra==CIMA)
				direcaoInvalida=true;
            break;
        case 'a': // esquera
        	if(!(corpo.front().first<=10)&&direcao_da_cobra!=DIREITA){
				corpo.push_front(make_pair(corpo.front().first-11,corpo.front().second));
				direcao_da_cobra=ESQUERDA;
			}
			if(direcao_da_cobra==DIREITA)
				direcaoInvalida=true;
            break;
        case 'd': // direita
        	if(!(corpo.front().first>=310)&&direcao_da_cobra!=ESQUERDA){
				corpo.push_front(make_pair(corpo.front().first+11,corpo.front().second));
				direcao_da_cobra=DIREITA;
			}
			if(direcao_da_cobra==ESQUERDA)
				direcaoInvalida=true;
            break;
        default:
        	switch(direcao_da_cobra)
			{
				case CIMA: // frente
		  			if(!(corpo.front().second>=230)){
						corpo.push_front(make_pair(corpo.front().first,corpo.front().second+11));
						direcao_da_cobra=CIMA;
					}

		        case BAIXO: // tras
		        	if(!(corpo.front().second<=10)){
						corpo.push_front(make_pair(corpo.front().first,corpo.front().second-11));
						direcao_da_cobra=BAIXO;
					}
		            break;
		        case ESQUERDA: // esquera
		        	if(!(corpo.front().first<=10)){
						corpo.push_front(make_pair(corpo.front().first-11,corpo.front().second));
						direcao_da_cobra=ESQUERDA;
					}
		            break;
		        case DIREITA: // direita
		        	if(!(corpo.front().first>=310)){
						corpo.push_front(make_pair(corpo.front().first+11,corpo.front().second));
						direcao_da_cobra=DIREITA;
					}

				}
        	break;
    }

    /*void Controle(int key, int x, int y)
    {
	switch (key)
	{
	case GLUT_KEY_UP:
		if (direction != DOWN)
			direction = UP;
		break;
	case GLUT_KEY_DOWN:
		if (direction != UP)
			direction = DOWN;
		break;
	case GLUT_KEY_RIGHT:
		if (direction != LEFT)
			direction = RIGHT;
		break;
	case GLUT_KEY_LEFT:
		if (direction != RIGHT)
			direction = LEFT;
		break;
    }
    }*/

    VerificaColisao();
    if(cobra_comeu==false&&direcaoInvalida==false)
	{
		corpo.pop_back();
	}
	if(cobra_comeu==true)
	{
		cobra_comeu=false;
	}
    Desenhar_Tela_de_Jogo();
    Sleep(500/cobra_velocidade);

}

static void
Espera(void)
{
	switch(direcao_da_cobra)
	{
		case CIMA: // frente
		 	 Controle('w', 99999999, 99999999);
		 	 break;
		case BAIXO: // tras
     		Controle('s', 99999999, 99999999);
		    break;
		case ESQUERDA: // esquera
		   	Controle('a', 99999999, 99999999);
		    break;
    	case DIREITA: // direita
		   	Controle('d', 99999999, 99999999);
 			break;
 		case FIM:
 	 		break;
	}

}


/***************************************************/
//
//					INICIA COBRA e COMIDA
//
/***************************************************/
void Iniciar_Jogo(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    Nova_Comida();
	/***************************************************/
	//					COBRA INICIA COM TAMANHO 3
	/***************************************************/
    corpo.push_front(make_pair(3*11,5*11));
    corpo.push_front(make_pair(4*11,5*11));
    corpo.push_front(make_pair(5*11,5*11));
}

/***************************************************/
//
//					CORRIGIR TELA
//
/***************************************************/
void Corrige_Tela(GLsizei w, GLsizei h)
{
    // Não permitir que a tela seja modificada
    if(w < 800 || w> 800) glutReshapeWindow(800, h);
    if(h < 600 || h> 600) glutReshapeWindow(w, 600);

	// Evita a divisao por zero
	if(h == 0) h = 1;
	// Especifica as dimensões da Viewport
	glViewport(0, 0, w, h);
	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Estabelece a janela de seleção (left, right, bottom, top)
	if (w <= h)
        gluOrtho2D (0.0f, 250.0f, 0.0f, 250.0f*h/w);
   else
		gluOrtho2D (0.0f, 250.0f*w/h, 0.0f, 250.0f);

}


/***************************************************/
//
//					PRINCIPAL
//
/***************************************************/
int main(int argc, char*argv[])
//void main()
{
    glutInit (&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(150, 50);
    glutCreateWindow("Jogo da Cobra");
    glutDisplayFunc(Desenhar_Tela_de_Jogo);
    glutReshapeFunc(Corrige_Tela);
    glutKeyboardFunc(Controle);
    glutIdleFunc(Espera);
    Iniciar_Jogo();
    glutMainLoop();
}
