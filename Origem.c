#include "lab.h"
#include <stdbool.h>
#include <windows.h>

#define PISTA_SIZE 14
#define NUM_PERSONAGENS 5
#define NUM_PODIO 3

void ecraInicial() {

	setColor(MY_COLOR_LIGTH_GREEN, MY_COLOR_BLACK);
	setFullScreen(true);
	system("cls");
	showRectAt(44, 12, 20, 4);
	showRectAt(38, 8, 32, 4);
	gotoxy(40, 10); printf("Jogo da lebre e da Tartaruga!\n");
	gotoxy(50, 14); printf("by Pinhao");

	gotoxy(10, 22); system("pause");
}

typedef struct BARALHO {
	int maxsize;	// Tamanho máximo do baralho novo
	char cartas[81];	// Estrutura para representar o baralho total
	int size;	// Tamanho atual do baralho
	char descarte[81];	// Estrutura para representar o baralho de descarte
	char apostasIniciais[5];

}baralho;

typedef struct Apostasdejogo {
	char cartas[8];  // Estrutura para representar o baralho de aposta dentro do jogo
	int size;        // Tamanho atual do baralho de aposta
	int maxsize;     // Tamanho máximo do baralho de aposta
} baralho_aposta;

typedef struct Jogador {

	char nome[50];
	int type;
	baralho mao;
	char apostaI;//O que cada jogador vai apostar
	char apostafixa;
	int pontos;// Para a pontua��o final
	char jogo;

}jogador;

typedef struct BOT {

	char nome[50];
	int type;
	baralho mao;
	char apostaI;//O que cada jogador vai apostar
	char apostafixa;
	int pontos;// Para a pontua��o final


}nothuman;

typedef struct pistaepodio {
	char tipo;     // Tipo de personagem: C, L, T, W, R
	int posicao;
	bool no_podio;   // Indica se o personagem está no pódio
} Personagem;

void comecarbaralho(baralho* myB) {

	//L,T,W,w,R,C
	myB->maxsize = 81;
	myB->size;
	int i = 0;
	myB->size = 0;
	for (i = 0; i < 18; i++) {

		myB->cartas[i] = 'L';
		myB->size++;
		if (i == 17) {
			myB->apostasIniciais[0] = 'L';
		}
	}

	for (i = 18; i < 35; i++) {

		myB->cartas[i] = 'T';
		myB->size++;
		if (i == 34) {
			myB->apostasIniciais[1] = 'T';
		}
	}


	for (i = 35; i < 48; i++) {

		myB->cartas[i] = 'W';
		myB->size++;
		if (i == 37) {
			myB->apostasIniciais[2] = 'W';
		}
	}

	for (i = 48; i < 51; i++) {

		myB->cartas[i] = 'w';
		myB->size++;
		if (i == 50) {

		}
	}

	for (i = 51; i < 66; i++) {

		myB->cartas[i] = 'R';
		myB->size++;
		if (i == 65) {
			myB->apostasIniciais[3] = 'R';
		}
	}

	for (i = 66; i < 81; i++) {

		myB->cartas[i] = 'C';
		myB->size++;
		if (i == 80) {
			myB->apostasIniciais[4] = 'C';
		}
	}


}

void printbaralho(baralho myB) {

	int i = 0;
	for (i = 0; i < 81; i++) //80 --> myB.size??
	{
		if (i == 80) {
			printf("%c", myB.cartas[i]);
		}
		else
			printf("%c,", myB.cartas[i]);


	}

	printf("\n");
}

void scrambledeck(baralho* myB) {

	int i = 0;
	for (i = 0; i < myB->size; i++) {

		if (myB->size == 0) { return; }
		trocaChars(&(myB->cartas[i]), &(myB->cartas[rand() % myB->size]));

	}

}

void embaralharapostainicial(baralho* myB, int tamanho, jogador* P1, nothuman* P2) {
	for (int i = tamanho - 1; i > 0; i--) {
		int j = rand() % (i + 1);

		// Troca as cartas i e j
		char temp = myB->apostasIniciais[i];
		myB->apostasIniciais[i] = myB->apostasIniciais[j];
		myB->apostasIniciais[j] = temp;
	}
	P1->apostafixa = myB->apostasIniciais[0];
	P2->apostafixa = myB->apostasIniciais[1];
}

void nomep1(jogador* P1) {
	gotoxy(40, 14);
	printf("Introduza o nome do Player 1: \n");
	gotoxy(40, 16);
	(void)scanf("%s´", P1->nome);

}

void nomep2(nothuman* P2) {
	gotoxy(40, 14);
	printf("Introduza o nome do Player 2: \n");
	gotoxy(40, 16);
	(void)scanf("%s", P2->nome);

}

void p1mao(jogador* P1, baralho* myB) {
	int i;
	P1->mao.maxsize = 7;
	P1->mao.size = 0;
	for (i = 0; i < P1->mao.maxsize; i++) {
		P1->mao.size++;
		P1->mao.cartas[i] = myB->cartas[i];
		myB->cartas[i] = 'x';
		if (i == P1->mao.maxsize) {
			printf("%c", P1->mao.cartas[i]);

		}
		else
			printf("%c,", P1->mao.cartas[i]);
	}

}

void p2mao(jogador* P1, nothuman* P2, baralho* myB) {
	int i;
	int posicao_maozinha = 0;;
	P2->mao.maxsize = 7;
	P2->mao.size = 0;
	for (i = 0; i < P1->mao.maxsize + P2->mao.maxsize; i++) {
		if (myB->cartas[i] != 'x') {
			P2->mao.cartas[posicao_maozinha] = myB->cartas[i];
			myB->cartas[i] = 'x';
			posicao_maozinha++;
			P2->mao.size++;
			if (posicao_maozinha == P2->mao.maxsize) {
				break;
			}
		}
	}
	for (int i = 0; i < P2->mao.maxsize; i++) {
		if (i == P2->mao.maxsize) {
			printf("%c", P2->mao.cartas[i]);

		}
		else
			printf("%c,", P2->mao.cartas[i]);
	}


}

void removerElemento(char vetor[], int tamanho, char elemento) {
	int i, j;

	// Procura o elemento no vetor
	for (i = 0; i < tamanho; i++) {
		if (vetor[i] == elemento) {
			// Move os elementos à direita do elemento a ser removido uma posiçao para a esquerda
			for (j = i; j < tamanho - 1; j++) {
				vetor[j] = vetor[j + 1];
			}

			// Reduz o tamanho do vetor
			tamanho--;

			// Decrementa o �ndice para evitar pular um elemento
			i--;
		}
	}
}

void saveBaralho(FILE* fp, baralho* myB) {

	int i = 0;
	fprintf(fp, "(");
	for (i = 0; i < 81; i++) {
		if (i == 80) {
			fprintf(fp, "%c", myB->cartas[i]);
		}
		else
			fprintf(fp, "%c,", myB->cartas[i]);
	}
	fprintf(fp, ")");
	fclose(fp);
}

void savedeckplusp1(FILE* fp, baralho* myB, jogador* P1, nothuman* P2) { //Funçao serve para verificar se est� a guardar tudo bem no ficheiro txt

	int i = 0;
	fprintf(fp, "(");
	for (i = 0; i < 81; i++) {
		if (i == 80) {
			fprintf(fp, "%c", myB->cartas[i]);
		}
		else
			fprintf(fp, "%c,", myB->cartas[i]); //Imprimir tudo igual hà funcao anterior e normal saveBaralho
	}
	fprintf(fp, ")");
	fprintf(fp, "\n %s -> ", P1->nome);
	for (i = 0; i < 7; i++) {
		if (i == 6) {
			fprintf(fp, " %c ", P1->mao.cartas[i]);
		}
		else
			fprintf(fp, " %c,", P1->mao.cartas[i]);
	}

	fprintf(fp, "\n %s -> ", P2->nome);
	for (i = 0; i < 7; i++) {
		if (i == 6) {
			fprintf(fp, " %c ", P2->mao.cartas[i]);
		}
		else
			fprintf(fp, " %c,", P2->mao.cartas[i]);
	}
	fclose(fp);
	printf(" \n");
}

void readBaralho(FILE* fp, baralho* myB) {

	int i = 0;
	fgetc(fp); //descarta '('
	for (i = 0; i < 80; i++) {

		myB->cartas[i] = fgetc(fp);
	}
	fgetc(fp); //descarta ','
}

// Funcão para verificar se uma carta está na mao
bool cartaNaMao(char mao[], int tamanhoDaMao, char cartaProcurada) {
	for (int i = 0; i < tamanhoDaMao; i++) {
		if (mao[i] == cartaProcurada) {
			return true; // Carta encontrada na m�o
		}
	}
	return false; // Carta n�o encontrada na m�o
}

void apostaInicial(jogador* P1, baralho* myB) {

	int verdadeiro = 0;
	printf("\nTens estas cartas na tua mão:");
	for (int i = 0; i < 7; i++) {
		if (i == 6) {
			printf("%c", P1->mao.cartas[i]);
		}
		else
			printf("%c,", P1->mao.cartas[i]);
	}

	do {
		// Solicita uma carta para fazer de aposta Inicial
		printf("\nEscolhe agora a tua carta da aposta inicial: ");
		(void)scanf(" %c", &P1->apostaI);
		if (P1->apostaI == 'l') {
			P1->apostaI = 'L';
		}
		else {
			if (P1->apostaI == 'r') {
				P1->apostaI = 'R';
			}
			else {
				if (P1->apostaI == 'c') {
					P1->apostaI = 'C';
				}
				else
				{
					if (P1->apostaI == 't') {
						P1->apostaI = 'T';
					}
				}
			}
		}

		if (cartaNaMao(P1->mao.cartas, 7, P1->apostaI))
		{
			printf("Carta encontrada na mao!\n");
			verdadeiro = 1;
			switch (P1->apostaI) {

			case 'L': {
				int j = 0;
				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;
			}

			case 'l': {
				int j = 0;
				P1->apostaI = 'L';
				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;
			}
			case 'W': {
				int j = 0;
				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;
			}
			case 'w': {
				int j = 0;
				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;
			}
			case 'R': {
				int j = 0;
				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;
			}
			case 'r': {
				int j = 0;
				P1->apostaI = 'R';
				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;
			}
			case 'C': {
				int j = 0;
				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;
			}
			case 'c': {
				int j = 0;
				P1->apostaI = 'C';
				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;
			}
			case 'T': {

				int j = 0;

				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;
			}
			case 't': {
				int j = 0;
				P1->apostaI = 'T';
				for (j = 0; j < 7; j++) {

					if (P1->apostaI == P1->mao.cartas[j]) {

						P1->mao.cartas[j] = 'x';
						break;
					}
				}

				break;

			}
			}


			printf("Aposta principal: %c\n", P1->apostaI);
			printf("Ficaste com estas cartas no teu deck :");
			for (int i = 0; i < 7; i++) {
				if (i == 6) {
					printf("%c", P1->mao.cartas[i]);
				}
				else
					printf("%c,", P1->mao.cartas[i]);
			}
		}
		else {
			printf("Carta nao encontrada na mao.\n");

		}
	} while (verdadeiro != 1);
}

void apostaInicialBot(nothuman* P2, baralho* myB) {

	int indicedecartas = rand() % 7;
	P2->apostaI = P2->mao.cartas[indicedecartas];
	P2->mao.cartas[indicedecartas] = 'x';

}

//Adicionar ao baralho de aposta as cartas jogadas
baralho_aposta adicionar_carta_ao_baralho(char carta, baralho_aposta aposta) {
	aposta.maxsize = 8;
	if (aposta.size < aposta.maxsize) {
		for (int i = 0; i < aposta.size; i++) {
			aposta.cartas[i] = carta;
		}

	}
	else {
		printf("O baralho de aposta está cheio. Não é possível adicionar mais cartas.\n");
	}

	return aposta;
}

jogador remover_cartas_da_mao(char carta, int quantidade, jogador* P1) {


	int removidas = 0;
	for (int j = 0; j < P1->mao.size; j++) {
		if (carta == P1->mao.cartas[j]) {
			P1->mao.cartas[j] = 'x';
			removidas++;

			if (removidas == quantidade) {
				break;  // Sai do loop se o número desejado de cartas iguais foi removido
			}
		}
	}

	return *P1;
}

// Verificar se a mao do jogador realmente tem cartas iguais para se jogar
int contar_cartas_na_mao(char carta, jogador* P1) {
	int contador = 0;
	for (int i = 0; i < P1->mao.maxsize; i++) {
		if (P1->mao.cartas[i] == carta) {
			contador++;
		}
	}
	return contador;
}

void escolher_cartajogo(jogador* P1, baralho* myB, baralho_aposta aposta) {


	int verdadeiro = 0;
	int quantidade;

	do {
		// Solicita uma carta para fazer de aposta Inicial
		printf("\nEscolhe agora a tua carta da aposta inicial: ");
		(void)scanf(" %c", &P1->jogo);
		if (P1->jogo == 'l') {
			P1->jogo = 'L';
		}
		else {
			if (P1->jogo == 'r') {
				P1->jogo = 'R';
			}
			else {
				if (P1->jogo == 'c') {
					P1->jogo = 'C';
				}
				else
				{
					if (P1->jogo == 't') {
						P1->jogo = 'T';
					}
				}
			}
		}

		if (cartaNaMao(P1->mao.cartas, 7, P1->jogo))
		{
			printf("Carta encontrada na mao!\n");
			verdadeiro = 1;
			printf("Quantas cartas iguais queres jogar: ");
			(void)scanf(" %d", &quantidade);

			int quantidade_na_mao = contar_cartas_na_mao(P1->jogo, P1);

			if (quantidade_na_mao < quantidade) {
				printf("Você não tem cartas suficientes na mão.\n");
				verdadeiro = 0;
				continue;  // Reinicia o loop para nova escolha
			}

			remover_cartas_da_mao(P1->jogo, quantidade, P1);
			aposta.size = quantidade;
			aposta = adicionar_carta_ao_baralho(P1->jogo, aposta);


			for (int i = 0; i < quantidade; i++) {
				if (quantidade == 1) {
					printf(" Jogaste: %c\n", P1->jogo);
				}
				else
				{
					if (i == quantidade - 1) {
						printf(" %c\n", P1->jogo);
					}
					else
					{
						printf("Jogaste: %c,", P1->jogo);
					}

				}
			}

			printf("\nFicaste com estas cartas no teu deck :");
			for (int i = 0; i < 6; i++) {
				if (i == 5) {
					printf("%c", P1->mao.cartas[i]);
				}
				else
					printf("%c,", P1->mao.cartas[i]);
			}
			printf("\nBaralho de aposta atual: ");
			for (int i = 0; i < aposta.size; i++) {
				if (i == aposta.size - 1) {
					printf("%c", aposta.cartas[i]);
				}
				else {
					printf("%c, ", aposta.cartas[i]);
				}
			}
			printf("\n");
		}
		else {
			printf("Carta nao encontrada na mao.\n");

		}
	} while (verdadeiro != 1);
}

void printcartasparaapostaI(jogador P1) {
	printf("\n%s cartas na mão: ", P1.nome);
	for (int i = 0; i < 7; i++) {
		if (i == 6) {
			printf("%c ", P1.mao.cartas[i]);
		}
		else {
			printf("%c, ", P1.mao.cartas[i]);
		}
	}
	printf("\n");
}

void imprimirCartasNaMaoparajogada(jogador P1) {
	printf("\n%s cartas na mão: ", P1.nome);
	for (int i = 0; i < 6; i++) {
		if (i == 5) {
			printf("%c ", P1.mao.cartas[i]);
		}
		else {
			printf("%c, ", P1.mao.cartas[i]);
		}
	}
	printf("\n");
	printf("Cartas de aposta: %c, %c", P1.apostaI, P1.apostafixa);
}

void imprimirCartasNaMaoparajogadabot(nothuman P2) {
	printf("\n%s cartas na mão: ", P2.nome);
	for (int i = 0; i < 6; i++) {
		if (i == 5) {
			printf("%c ", P2.mao.cartas[i]);
		}
		else {
			printf("%c, ", P2.mao.cartas[i]);
		}
	}
	printf("\n");
	printf("Cartas de aposta: %c, %c", P2.apostaI, P2.apostafixa);
}


Personagem pista[PISTA_SIZE]; // Pista com tamanho fixo

void inicializarPista() {
	for (int i = 0; i < PISTA_SIZE; i++) {
		pista[i].tipo = ' ';
		pista[i].posicao = i + 1;
		pista[i].no_podio = 0;
	}
	pista[0].tipo = 'S';  // Saída no início da pista
	pista[PISTA_SIZE - 1].tipo = 'E';  // Chegada no final da pista
}

void imprimirPista() {

	for (int i = 0; i < PISTA_SIZE; i++) {
		if (pista[i].no_podio) {
			printf("%c ", pista[i].tipo);
		}
		else if (pista[i].tipo == ' ') {
			printf("|----| ");
		}
		else {
			if (i < PISTA_SIZE - 1 && pista[i].tipo == pista[i + 1].tipo) {
				printf("%c%c ", pista[i].tipo, pista[i + 1].tipo);
				i++;  // Pula para a próxima posição para evitar impressão duplicada
			}
			else {
				printf("%c ", pista[i].tipo);
			}
		}
	}
	printf("\n");
}

void moverPersonagem(jogador* P1, baralho_aposta* aposta) {

	char carta = P1->jogo;
	int countL = 0, avanco = 0;

	// Verifica se a carta é 'L' para definir o avanço
	if (carta == 'L') {
		// Verifica se já existem duas cartas 'L' jogadas
		for (int i = 0; i < aposta->size; i++) {
			if (pista[i].tipo == 'L' && !pista[i].no_podio) {
				countL++;
			}
		}

		if (countL < 2) {
			// Se ainda não foram jogadas duas cartas 'L', avança 1 casa
			avanco = 1;
		}
		else {
			// Se já foram jogadas duas cartas 'L', avança 2 casas
			avanco = 2;
		}
	}
	else if (carta == 'W') {
		// Verifica se já existem duas cartas 'W' jogadas
		for (int i = 0; i < aposta->size; i++) {
			if (pista[i].tipo == 'W' && !pista[i].no_podio) {
				countL++;
			}
		}

		if (countL < 2) {
			// Se ainda não foram jogadas duas cartas 'W', avança 1 casa
			avanco = 1;
		}
		else {
			// Se já foram jogadas duas cartas 'W', avança 2 casas
			avanco = 2;
		}
	}
	else if (carta == 'T') {
		// Verifica se já existem duas cartas 'T' jogadas
		for (int i = 0; i < aposta->size; i++) {
			if (pista[i].tipo == 'T' && !pista[i].no_podio) {
				countL++;
			}
		}

		if (countL < 2) {
			// Se ainda não foram jogadas duas cartas 'T', avança 1 casa
			avanco = 1;
		}
		else {
			// Se já foram jogadas duas cartas 'T', avança 2 casas
			avanco = 2;
		}
	}
	else if (carta == 'C') {
		// Verifica se já existem duas cartas 'C' jogadas
		for (int i = 0; i < aposta->size; i++) {
			if (pista[i].tipo == 'C' && !pista[i].no_podio) {
				countL++;
			}
		}

		if (countL < 2) {
			// Se ainda não foram jogadas duas cartas 'C', avança 1 casa
			avanco = 1;
		}
		else {
			// Se já foram jogadas duas cartas 'C', avança 2 casas
			avanco = 2;
		}
	}
	else if (carta == 'R') {
		// Verifica se já existem duas cartas 'R' jogadas
		for (int i = 0; i < aposta->size; i++) {
			if (pista[i].tipo == 'R' && !pista[i].no_podio) {
				countL++;
			}
		}

		if (countL < 2) {
			// Se ainda não foram jogadas duas cartas 'R', avança 1 casa
			avanco = 1;
		}
		else {
			// Se já foram jogadas duas cartas 'R', avança 2 casas
			avanco = 2;
		}
	}

	for (int i = 0; i < NUM_PERSONAGENS; i++) {
		if (pista[i].tipo == carta && !pista[i].no_podio) {
			int novaPosicao = pista[i].posicao + avanco;

			// Garante que não ultrapassa o final da pista sem transpor "E"
			if (novaPosicao >= PISTA_SIZE - 1) {
				// Verifica se está no "E" (chegada) e ajusta a posição
				if (pista[i].tipo == 'E') {
					pista[i].posicao = PISTA_SIZE - 1;
					pista[i].no_podio = true;  // Chegou ao final e entra no pódio
				}
				else {
					// Se não está no "E", ajusta a posição sem ultrapassar "S"
					pista[i].posicao = PISTA_SIZE - 2;
				}
			}
			else if (novaPosicao < 0) {
				// Garante que não ultrapassa "S" (posição inicial)
				pista[i].posicao = 0;
			}
			else {
				// Se não ultrapassou o final nem "S", atualiza a posição normalmente
				pista[i].posicao = novaPosicao;
			}
		}
	}

	// Verifica se há mais de um personagem na mesma posição
	for (int j = 0; j < PISTA_SIZE; j++) {
		int count = 0;
		for (int h = 0; h < NUM_PERSONAGENS; h++) {
			if (pista[h].posicao == j && !pista[h].no_podio) {
				count++;
			}
		}
		if (count > 1) {
			// Imprime os personagens que estão na mesma posição
			printf("Posição %d: ", j);
			for (int h = 0; h < NUM_PERSONAGENS; h++) {
				if (pista[h].posicao == j && !pista[h].no_podio) {
					printf("%c ", pista[h].tipo);
				}
			}
			printf("\n");
		}
	}
}

void atualizarPosicoes(Personagem pista[], int tamanhoPista) {
	for (int i = 0; i < tamanhoPista; i++) {
		// Se o personagem não estiver no pódio, atualize a posição
		if (!pista[i].no_podio) {
			int novaPosicao = pista[i].posicao - 1;

			// Garante que não ultrapassa o final da pista sem transpor "E"
			if (novaPosicao >= tamanhoPista - 1) {
				// Verifica se está no "E" (chegada) e ajusta a posição
				if (pista[i].tipo == 'E') {
					pista[i].posicao = tamanhoPista - 1;
					pista[i].no_podio = true;  // Chegou ao final e entra no pódio
				}
				else {
					// Se não está no "E", ajusta a posição sem ultrapassar "S"
					pista[i].posicao = tamanhoPista - 2;
				}
			}
			else if (novaPosicao < 0) {
				// Garante que não ultrapassa "S" (posição inicial)
				pista[i].posicao = 0;
			}
			else {
				// Se não ultrapassou o final nem "S", atualiza a posição normalmente
				pista[i].posicao = novaPosicao;
			}
		}
	}
}
void verificarChegadaAoPodio() {
	for (int i = 0; i < PISTA_SIZE; i++) {
		if (pista[i].posicao <= NUM_PODIO) {
			pista[i].no_podio = 1;
		}
	}
}

void novoJogo() {

	baralho myB;
	jogador P1;
	nothuman P2;
	baralho_aposta aposta = { .size = 0,.maxsize = 8 };

	comecarbaralho(&myB);

	scrambledeck(&myB);

	embaralharapostainicial(&myB, 5, &P1, &P2);

	nomep1(&P1);
	system("cls");
	nomep2(&P2);
	system("cls");

	FILE* fp = NULL;
	fopen_s(&fp, "maindeck.txt", "w");
	if (fp) {
		saveBaralho(fp, &myB);
		fclose(fp);
	}
	gotoxy(40, 15);
	printf("Baralho inicial:\n");
	printbaralho(myB);
	system("pause");

	p1mao(&P1, &myB);
	p2mao(&P1, &P2, &myB);

	fopen_s(&fp, "maindeck.txt", "w");
	if (fp) {
		savedeckplusp1(fp, &myB, &P1, &P2);

		fclose(fp);
	}
	printbaralho(myB);
	system("pause");
	system("cls");

	printcartasparaapostaI(P1);
	apostaInicial(&P1, &myB);
	apostaInicialBot(&P2, &myB);

	removerElemento(&P1.mao.cartas, 7, 'x');
	removerElemento(&P2.mao.cartas, 7, 'x');


	imprimirCartasNaMaoparajogada(P1);
	imprimirCartasNaMaoparajogadabot(P2);
	system("cls");

	inicializarPista();
	imprimirPista();

	system("pause");
	imprimirCartasNaMaoparajogada(P1);
	escolher_cartajogo(&P1, &myB, aposta);
	moverPersonagem(&P1, &aposta);
	system("pause");
	system("cls");
	printf("Posições após mover Personagem:\n");

	atualizarPosicoes(pista, PISTA_SIZE);
	imprimirPista();
}

int main(void) {

	srand(time(NULL));
	ecraInicial();

	setlocale(LC_ALL, "Portuguese");
	int escolha_menu;
	char content[1000];
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, 0);


	FILE* Rules;

	do {
		system("cls");
		gotoxy(10, 8); printf("Escolha agora o que deseja fazer:\n");
		gotoxy(10, 10);	printf("1-Jogar uma Partida do Jogo ""A Lebre e a Tartaruga""\n");//Escolha A
		gotoxy(10, 11);	printf("2-Carregar uma partida a partir de um ficheiro e continuar o jogo\n");//Escolha B
		gotoxy(10, 12);	printf("3-Descrição do jogo\n");//Escolha C | Op��o configurada e concluida :CHECKMARK:
		gotoxy(10, 13); printf("0-Sair\n");//Sair do jogo | Op��o configurada e concluida :CHECKMARK:

		gotoxy(10, 15); printf("Escolha agora o que deseja fazer:");
		(void)scanf("%d", &escolha_menu);//Escolher a op��o do menu

		switch (escolha_menu) {

		case 1: {

			system("cls");//Limpar o texto anterior 
			novoJogo();

			printf("\nO programa foi bem executado ate aqui\n");//Ponto de verifica��o
			system("pause");//Parar o programa para verificar
			break;//N�o esquecer deste malandro
		}

		case 2: {
			setBackColor(MY_COLOR_DARK_GRAY);

			system("cls"); //Limpar o texto anterior 

			//carregarjogo();
			int i = 0;
			for (i = 0; i < 15; i++) {
				setForeColor(0 + i, i + 1);
				showRectAt(3 + i * 4, 5, 3, 3);

			}


			gotoxy(20, 20);
			printf("O programa foi bem executado ate aqui\n");//Ponto de verifica��o
			system("pause");//Parar o programa para verificar
			break;//N�o esquecer deste malandro
		}

		case 3: {
			setlocale(LC_ALL, "pt_PT.UTF-8");// UTF-8 para ler ficheiros .txt para pt-pt
			system("cls");
			printf("Descricao do jogo:");
			Rules = fopen("regras.txt", "r");
			while (fgets(content, sizeof(content), Rules) != NULL) {
				printf("%s", content);
			}
			fclose(Rules);
			system("pause");
			break;//N�o esquecer deste malandro

		}

		case 0: {
			printf("Saiu do programa\n");//Sair do programa
			break;
		}
		default: {
			//Default para voltar ao ciclo e escolher novamente um n�mero

			printf("Escolha um numero pertencente ha lista!\n");
			system("pause");
			break;

		}
		}

	} while (escolha_menu != 0);//Sair do jogo e fechar o programa



}

