/**
 * 13/05/19 - 20/05/19 - 27/05/19
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "layout.h"

#define ARQ_TIMES_DEFAULT "times.dat"
#define ARQ_JOGOS_DEFAULT "jogos.dat"

#define NOME_SISTEMA "Sistema de Controle de Jogos"

// Data
typedef struct {
	int dia;
	int mes;
	int ano;
	int temHora;
	int hora;
	int minuto;
	char toString[21];
} Data;

// Time
typedef struct {
	char nome[50];
	char responsavel[50];
	Data* fundacao;
} Time;

// Placar
typedef struct {
	int timeA;
	int timeB;
} Placar;

// Jogo
typedef struct {
	Time* timeA;
	Time* timeB;
	int mesa;
	Data* data;
	int jaOcorreu;
	Placar* placar;
} Jogo;

// Rodada
typedef struct {
	int numero;
	int nJogos;
	Jogo* jogos;
} Rodada;

// Campeonato
typedef struct {
	int ano;
	int nRodadas;
	Rodada* rodadas;
	int nTimes;
	Time* times;
} Campeonato;

// Funcoes principais
Data* criarData(int ano, int mes, int dia);
Data* criarDataHora(int ano, int mes, int dia, int hora, int minuto);
Time* criarTime(char* nome, char* resp, Data* fund);
Jogo* criarJogo(Time* timeA, Time* timeB, int mesa, Data* data);
Jogo* informarPlacar(Jogo* jogo, int golsTimeA, int golsTimeB);
Rodada* criarRodada(int numero);
Rodada* adicionarJogo(Rodada* rodada, Jogo* jogo);
Campeonato* criarCampeonato(int ano);
Campeonato* adicionarRodada(Campeonato* campeonato, Rodada* rodada);

// Funcoes de buscas
Time* buscarTimePorNome(Campeonato* campeonato, char* nome);
Rodada* buscarRodadaPeloNumero(Campeonato* campeonato, int numero);

// Funcoes de leituras
Campeonato* lerTimesDoArquivo(Campeonato* campeonato, char* filename);
Campeonato* lerJogosDoArquivo(Campeonato* campeonato, char* filename);

// Funcoes de escrita
Campeonato* salvarTimesNoArquivo(Campeonato* campeonato, char* filename);

// Funcoes de aviso
char _mensagem_aviso[255];
void setMensagemAviso(char *msg);
void imprimeMensagemAviso();

// Funcoes de tela
void menuPrincipal();
void imprimeMenuPrincipal();
void menuCampeonatos();
void imprimeMenuCampeonatos();
void menuTimes();
void imprimeMenuTimes();
void menuJogos();
void imprimeMenuJogos();
void menuTabela();
void imprimeMenuTabela();

// Funcoes helpers
Campeonato* criarCampeonatoFake();
Campeonato* autoCriarRodadas(Campeonato* campeonato);
void imprimeCampeonato(Campeonato* campeonato);
void imprimeRodada(Rodada* rodada);
void imprimeTime(Time* time);

// Funcao principal
int main() {

	menuPrincipal();

	clearScreen();

	return EXIT_SUCCESS;
}


Data* criarData(int ano, int mes, int dia){

	Data* data = (Data*)malloc(sizeof(Data));
	data->ano = ano;
	data->mes = mes;
	data->dia = dia;
	data->temHora = 0;
	data->hora = 0;
	data->minuto = 0;
	sprintf(data->toString, "%02d/%02d/%04d", data->dia, data->mes, data->ano);

	return data;
}
Data* criarDataHora(int ano, int mes, int dia, int hora, int minuto) {

	Data* data = criarData(ano, mes, dia);
	data->temHora = 1;
	data->hora = hora;
	data->minuto = minuto;

	char str[10];
	sprintf(str, " as %02d:%02d", data->hora, data->minuto);
	strcat(data->toString, str);

	return data;
}
Time* criarTime(char* nome, char* resp, Data* fund) {

	Time* time = (Time*)malloc(sizeof(Time));
	strcpy(time->nome, nome);
	strcpy(time->responsavel, resp);
	time->fundacao = fund;

	return time;
}
Jogo* criarJogo(Time* timeA, Time* timeB, int mesa, Data* data) {

	Jogo* jogo = (Jogo*)malloc(sizeof(Jogo));
	jogo->timeA = timeA;
	jogo->timeB = timeB;
	jogo->mesa = mesa;
	jogo->data = data;
	jogo->jaOcorreu = 0;
	jogo->placar = (Placar*)malloc(sizeof(Placar));
	jogo->placar->timeA = 0;
	jogo->placar->timeB = 0;

	return jogo;
}
Jogo* informarPlacar(Jogo* jogo, int golsTimeA, int golsTimeB) {

	jogo->jaOcorreu = 1;
	jogo->placar->timeA = golsTimeA;
	jogo->placar->timeB = golsTimeB;

	return jogo;
}
Jogo* criarJogoComPlacar(Time* timeA, Time* timeB, int mesa, Data* data, int golsTimeA, int golsTimeB) {

	Jogo* jogo = criarJogo(timeA, timeB, mesa, data);
	jogo = informarPlacar(jogo, golsTimeA, golsTimeB);

	return jogo;
}
Rodada* criarRodada(int numero) {

	Rodada* rodada = (Rodada*)malloc(sizeof(Rodada));
	rodada->numero = numero;
	rodada->nJogos = 0;
	rodada->jogos = (Jogo*)malloc(10*sizeof(Jogo));

	return rodada;
}
Rodada* adicionarJogo(Rodada* rodada, Jogo* jogo) {

	rodada->jogos[rodada->nJogos] = *jogo;
	rodada->nJogos += 1;

	return rodada;
}
Campeonato* criarCampeonato(int ano) {

	Campeonato* camp = (Campeonato*)malloc(sizeof(Campeonato));
	camp->ano = ano;
	camp->nRodadas = 0;
	camp->rodadas = (Rodada*)malloc(38*sizeof(Rodada));
	camp->nTimes = 0;
	camp->times = (Time*)malloc(20*sizeof(Time));

	return camp;
}
Campeonato* adicionarTime(Campeonato* campeonato, Time* time) {

	campeonato->times[campeonato->nTimes] = *time;
	campeonato->nTimes += 1;

	return campeonato;
}
Campeonato* adicionarRodada(Campeonato* campeonato, Rodada* rodada) {

	campeonato->rodadas[campeonato->nRodadas] = *rodada;
	campeonato->nRodadas += 1;

	return campeonato;
}

Time* buscarTimePorNome(Campeonato* campeonato, char* nome) {

	int i;
	for (i=0; i<campeonato->nTimes; i++) {
		if(!strcmp(campeonato->times[i].nome, nome)) {
			return &campeonato->times[i];
		}
	}

	// todo nenhum time encontrado, devo setar mensagem?
	return NULL;
}
Rodada* buscarRodadaPeloNumero(Campeonato* campeonato, int numero) {

	int i;

	for (i=0; i<campeonato->nRodadas; i++) {
		if(campeonato->rodadas[i].numero == numero) {
			return &campeonato->rodadas[i];
		}
	}

	// todo nenhuma rodada encontrada, devo setar mensagem?
	return NULL;
}

Campeonato* lerTimesDoArquivo(Campeonato* campeonato, char* filename) {

	FILE* arquivo;
	char msg[128];
	int res = 0;

	char nomeTime[50];
	char resp[50];
	int ano, mes, dia;

	arquivo = fopen(filename, "r");
	if (!arquivo) {
		sprintf(msg, "Nao foi possivel abrir %s para leitura dos times.", filename);
		setMensagemAviso(msg);
		return campeonato;
	}

	while (1) {

		res = fscanf(arquivo, " %[^,],%[^,],%d-%d-%d\n", nomeTime, resp, &ano, &mes, &dia);

		if (res == EOF)
			break;

		campeonato = adicionarTime(campeonato,
				criarTime(nomeTime, resp,
						criarData(ano, mes, dia)));
	};

	fclose(arquivo);

	return campeonato;
}
Campeonato* lerJogosDoArquivo(Campeonato* campeonato, char* filename) {

	FILE* arquivo;
	char msg[128];
	int res = 0;

	Rodada *rodada;
	Jogo *jogo;

	char nomeTimeA[50];
	char nomeTimeB[50];
	int numero, mesa, ano, mes, dia, hora, minuto, golsTimeA, golsTimeB;

	arquivo = fopen(filename, "r");
	if (!arquivo) {
		sprintf(msg, "Nao foi possivel abrir %s para leitura dos jogos.", filename);
		setMensagemAviso(msg);
		return campeonato;
	}

	while (1) {

		res = fscanf(arquivo, " %d,%d-%d-%d,%d:%d,%d,%[^,],%[^,],%d,%d\n",
				&numero, &ano, &mes, &dia, &hora, &minuto, &mesa, nomeTimeA, nomeTimeB, &golsTimeA, &golsTimeB);

		if(res == EOF)
			break;

		rodada = buscarRodadaPeloNumero(campeonato, numero);

		if (!rodada) {
			rodada = criarRodada(numero);
			adicionarRodada(campeonato, rodada);
			rodada = buscarRodadaPeloNumero(campeonato, numero);
		}

		adicionarJogo(rodada,
			criarJogoComPlacar(
				buscarTimePorNome(campeonato, nomeTimeA), //todo verificar se encontrou o time
				buscarTimePorNome(campeonato, nomeTimeB), //todo verificar se encontrou o time
				mesa,
				criarDataHora(ano, mes, dia, hora, minuto),
				golsTimeA,
				golsTimeB));
	}

	fclose(arquivo);

	return campeonato;
}

Campeonato* salvarTimesNoArquivo(Campeonato* campeonato, char* filename) {

	FILE* arquivo;
	char msg[128];
	int i;

	arquivo = fopen(filename, "w");
	if (!arquivo) {
		sprintf(msg, "Nao foi possivel abrir %s para salvar os times.", filename);
		setMensagemAviso(msg);
		return campeonato;
	}

	for (i=0; i<campeonato->nTimes; i++) {
		fprintf(arquivo, "%s,%s,%d-%d-%d\n",
				campeonato->times[i].nome, campeonato->times[i].responsavel,
				campeonato->times[i].fundacao->ano, campeonato->times[i].fundacao->mes, campeonato->times[i].fundacao->dia);
	}

	fclose(arquivo);

	return campeonato;
}

void setMensagemAviso(char* msg) {
	strcpy(_mensagem_aviso, msg);
}
void imprimeMensagemAviso() {
	if(strcmp(_mensagem_aviso, "")) {
		printf("%s", _mensagem_aviso);
		strcpy(_mensagem_aviso, "");
	}
}

void menuPrincipal() {

	char opcao;

	while (1) {
		clearScreen();
		imprimeMenuPrincipal();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", &opcao);

		switch(opcao) {

		case '1':
			menuCampeonatos();
			break;

		case '2':
			menuTimes();
			break;

		case '3':
			menuJogos();
			break;

		case '4':
			menuTabela();
			break;

		case '0':
			return;
		}
	}
}
void imprimeMenuPrincipal() {
	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Menu Principal", 'C');
	filledLine();
	emptyLine();
	line("1. Campeonatos", 'L');
	emptyLine();
	line("2. Times", 'L');
	emptyLine();
	line("3. Jogos", 'L');
	emptyLine();
	line("4. Tabela de classificacao", 'L');
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	line("8. Restaurar/Redefinir dados", 'L');
	emptyLine();
	line("0. Sair", 'L');
	emptyLine();
	filledLine();
}
void menuCampeonatos() {

	char opcao;

	while (1) {

		clearScreen();
		imprimeMenuCampeonatos();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", &opcao);

		switch(opcao) {

		case '1':

		case '0':
			return;
		}
	}
}
void imprimeMenuCampeonatos() {
	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Campeonatos", 'C');
	filledLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	line("0. Menu Principal", 'L');
	emptyLine();
	filledLine();
}
void menuTimes(Campeonato* campeonato) {

	char opcao;

	while (1) {

		clearScreen();
		imprimeMenuTimes();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", &opcao);

		switch(opcao) {

		case '1':

		case '0':
			return;
		}
	}
}
void imprimeMenuTimes() {

	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Times", 'C');
	filledLine();
	emptyLine();
	line("1. Carregar do arquivo: times.dat", 'L');
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	line("0. Menu Principal", 'L');
	emptyLine();
	filledLine();
}
void menuJogos() {

	char opcao;

	while (1) {

		clearScreen();
		imprimeMenuJogos();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", &opcao);

		switch(opcao) {

		case '1':

		case '0':
			return;
		}
	}
}
void imprimeMenuJogos() {

	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Jogos", 'C');
	filledLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	line("0. Menu Principal", 'L');
	emptyLine();
	filledLine();
}
void menuTabela() {

	char opcao;

	while (1) {

		clearScreen();
		imprimeMenuTabela();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", &opcao);

		switch(opcao) {

		case '1':

		case '0':
			return;
		}
	}
}
void imprimeMenuTabela() {

	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Tabela", 'C');
	filledLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	line("0. Menu Principal", 'L');
	emptyLine();
	filledLine();
}


Campeonato* criarCampeonatoFake() {

	Campeonato* campeonato;
	campeonato = criarCampeonato(2019);

	campeonato = adicionarTime(campeonato, criarTime("Time A", "Jogador A",
			criarData(2007, 2, 5)
	));
	campeonato = adicionarTime(campeonato, criarTime("Time B", "Jogador B",
			criarData(2008, 3, 6)
	));
	campeonato = adicionarTime(campeonato, criarTime("Time C", "Jogador C",
			criarData(2009, 4, 7)
	));
	campeonato = adicionarTime(campeonato, criarTime("Time D", "Jogador D",
			criarData(2010, 5, 8)
	));
//	campeonato = adicionarTime(campeonato, criarTime("Time E", "Jogador E",
//			criarData(2010, 5, 10)
//	));
//	campeonato = adicionarTime(campeonato, criarTime("Time F", "Jogador F",
//			criarData(2010, 4, 10)
//	));
//	campeonato = adicionarTime(campeonato, criarTime("Time G", "Jogador G",
//			criarData(2002, 7, 5)
//	));
//	campeonato = adicionarTime(campeonato, criarTime("Time H", "Jogador H",
//			criarData(2004, 12, 5)
//	));
//	campeonato = adicionarTime(campeonato, criarTime("Time I", "Jogador I",
//			criarData(2005, 5, 25)
//	));
//	campeonato = adicionarTime(campeonato, criarTime("Time J", "Jogador J",
//			criarData(2017, 11, 15)
//	));

	campeonato = autoCriarRodadas(campeonato);

	return campeonato;
}
Campeonato* autoCriarRodadas(Campeonato* campeonato) {

	// Todo gerar rodadas a partir de times
//	Rodada *rd = criarRodada(1);
//	adicionarJogo(rd,
//			criarJogo(
//					&campeonato->times[0],
//					&campeonato->times[1],
//					1,
//					criarDataHora(2019, 5, 11, 12, 00)
//			)
//	);
//	adicionarJogo(rd,
//			criarJogo(
//					&campeonato->times[2],
//					&campeonato->times[3],
//					2,
//					criarDataHora(2019, 5, 11, 12, 30)
//			)
//	);
//	adicionarRodada(campeonato, rd);
//
//	rd = criarRodada(2);
//		adicionarJogo(rd,
//				criarJogo(
//						&campeonato->times[0],
//						&campeonato->times[2],
//						1,
//						criarDataHora(2019, 5, 12, 13, 30)
//				)
//		);
//		adicionarJogo(rd,
//				criarJogo(
//						&campeonato->times[1],
//						&campeonato->times[3],
//						2,
//						criarDataHora(2019, 5, 12, 14, 00)
//				)
//		);
//	adicionarRodada(campeonato, rd);
//
//	rd = criarRodada(3);
//		adicionarJogo(rd,
//				criarJogo(
//						&campeonato->times[0],
//						&campeonato->times[3],
//						1,
//						criarDataHora(2019, 5, 13, 11, 15)
//				)
//		);
//		adicionarJogo(rd,
//				criarJogo(
//						&campeonato->times[1],
//						&campeonato->times[2],
//						2,
//						criarDataHora(2019, 5, 13, 11, 45)
//				)
//		);
//	adicionarRodada(campeonato, rd);

	return campeonato;
}

void imprimeCampeonato(Campeonato* campeonato) {

	int i;

	printf("\t\t===== Campeonato %i =====\n", campeonato->ano);
	for (i=0; i<campeonato->nRodadas; i++) {
		imprimeRodada(&campeonato->rodadas[i]);
	}
	printf("\n");

	printf("==== Times (%d) ====\n\n", campeonato->nTimes);
	for (i=0; i<campeonato->nTimes; i++) {
		imprimeTime(&campeonato->times[i]);
	}
	printf("\n");

}
void imprimeRodada(Rodada* rodada) {

	int i;

	printf("Rodada %i:\n", rodada->numero);
	for (i=0; i<rodada->nJogos; i++) {
		printf("\tJogo %i (Mesa %d):\n\t- %s [%i X %i] %s - %s \n",
				i+1,
				rodada->jogos[i].mesa,
				rodada->jogos[i].timeA->nome,
				rodada->jogos[i].placar->timeA,
				rodada->jogos[i].placar->timeB,
				rodada->jogos[i].timeB->nome,
				rodada->jogos[i].data->toString);
	}
	printf("\n");
}
void imprimeTime(Time* time) {

	printf("Time: %s \n", time->nome);
	printf("Fundacao: %s \n", time->fundacao->toString);
	printf("Responsavel: %s \n", time->responsavel);

	printf("\n");
}






