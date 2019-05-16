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
void informarPlacar(Jogo* jogo, int golsTimeA, int golsTimeB);
Jogo* criarJogoComPlacar(Time* timeA, Time* timeB, int mesa, Data* data, int golsTimeA, int golsTimeB);
Rodada* criarRodada(int numero);
void adicionarJogo(Rodada* rodada, Jogo* jogo);
Campeonato* criarCampeonato(int ano);
void adicionarTime(Campeonato* campeonato, Time* time);
void adicionarRodada(Campeonato* campeonato, Rodada* rodada);

// Funcoes de buscas
Time* buscarTimePorNome(Campeonato* campeonato, char* nome);
Rodada* buscarRodadaPeloNumero(Campeonato* campeonato, int numero);

// Funcoes de leituras
void lerTimesDoArquivo(Campeonato* campeonato, char* filename);
void lerJogosDoArquivo(Campeonato* campeonato, char* filename);

// Funcoes de escrita
void salvarTimesNoArquivo(Campeonato* campeonato, char* filename);

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
void telaCadastroTime();
void imprimeCadastroTime(char* nome, char* resp, Data* data);
void menuJogos();
void imprimeMenuJogos();
void menuTabela();
void imprimeMenuTabela();

// Funcoes de debug
Campeonato* criarCampeonatoFake();
void imprimeCampeonato(Campeonato* campeonato);
void imprimeRodada(Rodada* rodada);
void imprimeTime(Time* time);


Campeonato* campeonato_;

// Funcao principal
int main() {

	campeonato_ = criarCampeonato(2019);
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
void informarPlacar(Jogo* jogo, int golsTimeA, int golsTimeB) {

	jogo->jaOcorreu = 1;
	jogo->placar->timeA = golsTimeA;
	jogo->placar->timeB = golsTimeB;
}
Jogo* criarJogoComPlacar(Time* timeA, Time* timeB, int mesa, Data* data, int golsTimeA, int golsTimeB) {

	Jogo* jogo = criarJogo(timeA, timeB, mesa, data);
	informarPlacar(jogo, golsTimeA, golsTimeB);

	return jogo;
}
Rodada* criarRodada(int numero) {

	Rodada* rodada = (Rodada*)malloc(sizeof(Rodada));
	rodada->numero = numero;
	rodada->nJogos = 0;
	rodada->jogos = (Jogo*)malloc(10*sizeof(Jogo));

	return rodada;
}
void adicionarJogo(Rodada* rodada, Jogo* jogo) {

	rodada->jogos[rodada->nJogos] = *jogo;
	rodada->nJogos += 1;

	free(jogo);
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
void adicionarTime(Campeonato* campeonato, Time* time) {
	// todo limitar quantidade de times (20)
	campeonato->times[campeonato->nTimes] = *time;
	campeonato->nTimes += 1;
	free(time);
}
void adicionarRodada(Campeonato* campeonato, Rodada* rodada) {
	// todo limitar quantidade de rodadas (38)
	campeonato->rodadas[campeonato->nRodadas] = *rodada;
	campeonato->nRodadas += 1;
	free(rodada); // todo parece que posso fazer
}

Time* buscarTimePorNome(Campeonato* campeonato, char* nome) {

	int i;
	for (i=0; i<campeonato->nTimes; i++) {
		if(!strcmp(campeonato->times[i].nome, nome)) {
			return &campeonato->times[i];
		}
	}

	return NULL;
}
Rodada* buscarRodadaPeloNumero(Campeonato* campeonato, int numero) {

	int i;
	for (i=0; i<campeonato->nRodadas; i++) {
		if(campeonato->rodadas[i].numero == numero) {
			return &campeonato->rodadas[i];
		}
	}

	return NULL;
}

void lerTimesDoArquivo(Campeonato* campeonato, char* filename) {

	FILE* arquivo;
	char *msg;
	int res = 0;

	char *nomeTime;
	char *resp;
	int ano, mes, dia;

	arquivo = fopen(filename, "r");
	if (!arquivo) {
		msg = malloc(80*sizeof(char));
		sprintf(msg, "Nao foi possivel abrir %s para leitura dos times.", filename);
		setMensagemAviso(msg);
		free(msg);
		return;
	}

	nomeTime = malloc(50*sizeof(char));
	resp = malloc(50*sizeof(char));

	while (1) {

		res = fscanf(arquivo, " %[^,],%[^,],%d-%d-%d\n", nomeTime, resp, &ano, &mes, &dia);

		if (res == EOF)
			break;

		if(!buscarTimePorNome(campeonato, nomeTime)){
			adicionarTime(campeonato,
					criarTime(nomeTime, resp,
							criarData(ano, mes, dia)));
		}
	};

	free(nomeTime);
	free(resp);

	fclose(arquivo);
}
void lerJogosDoArquivo(Campeonato* campeonato, char* filename) {

	FILE* arquivo;
	char *msg;
	int res = 0;

	Rodada *rodada;
	Jogo *jogo;

	Time* timeA;
	Time* timeB;

	char *nomeTimeA;
	char *nomeTimeB;
	int numero, mesa, ano, mes, dia, hora, minuto, golsTimeA, golsTimeB;

	arquivo = fopen(filename, "r");
	if (!arquivo) {
		msg = malloc(80*sizeof(char));
		sprintf(msg, "Nao foi possivel abrir %s para leitura dos jogos.", filename);
		setMensagemAviso(msg);
		free(msg);
		return;
	}

	nomeTimeA = malloc(50*sizeof(char));
	nomeTimeB = malloc(50*sizeof(char));

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

		timeA = buscarTimePorNome(campeonato, nomeTimeA);
		timeB = buscarTimePorNome(campeonato, nomeTimeB);

		if (!timeA) {
			msg = malloc(80*sizeof(char));
			sprintf(msg, "Nao foi possivel encontrar time '%s'.", nomeTimeA);
			setMensagemAviso(msg);
			free(msg);
			free(nomeTimeA);
			free(nomeTimeB);
			return;
		}
		if (!timeB) {
			msg = malloc(80*sizeof(char));
			sprintf(msg, "Nao foi possivel encontrar time '%s'.", nomeTimeB);
			setMensagemAviso(msg);
			free(msg);
			free(nomeTimeA);
			free(nomeTimeB);
			return;
		}

		adicionarJogo(rodada,
			criarJogoComPlacar(
				timeA,
				timeB,
				mesa,
				criarDataHora(ano, mes, dia, hora, minuto),
				golsTimeA,
				golsTimeB));
	}
	free(nomeTimeA);
	free(nomeTimeB);

	fclose(arquivo);
}

void salvarTimesNoArquivo(Campeonato* campeonato, char* filename) {

	FILE* arquivo;
	char *msg;
	int i;

	arquivo = fopen(filename, "w");
	if (!arquivo) {
		msg = malloc(80*sizeof(char));
		sprintf(msg, "Nao foi possivel abrir %s para salvar os times.", filename);
		setMensagemAviso(msg);
		free(msg);
		return;
	}

	for (i=0; i<campeonato->nTimes; i++) {
		fprintf(arquivo, "%s,%s,%d-%d-%d\n",
				campeonato->times[i].nome, campeonato->times[i].responsavel,
				campeonato->times[i].fundacao->ano, campeonato->times[i].fundacao->mes, campeonato->times[i].fundacao->dia);
	}

	fclose(arquivo);
}

void setMensagemAviso(char* msg) {
	strcpy(_mensagem_aviso, msg);
}
void imprimeMensagemAviso() {
	if(strcmp(_mensagem_aviso, "")) {
		line(_mensagem_aviso, 'C');
		strcpy(_mensagem_aviso, "");
	} else {
		emptyLine();
	}
}

void menuPrincipal() {

	char *opcao = (char*)malloc(sizeof(char));

	while (1) {
		clearScreen();
		imprimeMenuPrincipal();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", opcao);

		switch(*opcao) {

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

		case '8':
			free(campeonato_);
			campeonato_ = criarCampeonato(2019);
			break;

		// debug mode
		case 'd':
		case 'D':
			clearScreen();
			printf("- Debug Mode\n");
			imprimeCampeonato(campeonato_);
			printf("Digite qualquer caractere para voltar: ");
			scanf(" %[^\n]", opcao);
			break;

		// fake data
		case 'f':
		case 'F':
			free(campeonato_);
			campeonato_ = criarCampeonatoFake();
			break;

		case '0':
			free(opcao);
			return;
		}
	}
}
void imprimeMenuPrincipal() {
	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Menu Principal", 'C');
	filledLine();
	imprimeMensagemAviso();
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

	char *opcao = (char*)malloc(sizeof(char));

	while (1) {

		clearScreen();
		imprimeMenuCampeonatos();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", opcao);

		switch(*opcao) {

		case '1':
			break;

		case '0':
			free(opcao);
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
	line("0. Voltar", 'L');
	emptyLine();
	filledLine();
}
void menuTimes() {

	char *opcao = (char*)malloc(sizeof(char));

	while (1) {

		clearScreen();
		imprimeMenuTimes();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", opcao);

		switch(*opcao) {

		case '1':
			lerTimesDoArquivo(campeonato_, ARQ_TIMES_DEFAULT);
			break;

		case '2':
			telaCadastroTime();
			break;

		case '0':
			free(opcao);
			return;
		}
	}
}
void imprimeMenuTimes() {

	char *aux = (char*)malloc(50*sizeof(char));

	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Times", 'C');
	filledLine();
	emptyLine();
	sprintf(aux, "1. Carregar do arquivo: %s", ARQ_TIMES_DEFAULT);
	line(aux, 'L');
	emptyLine();
	line("2. Criar novo time", 'L');
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	sprintf(aux, "Times cadastrados: %d", campeonato_->nTimes);
	line(aux, 'C');
	emptyLine();
	emptyLine();
	line("0. Voltar", 'L');
	emptyLine();
	filledLine();

	free(aux);
}
void telaCadastroTime() {

	char *opcao = (char*)malloc(sizeof(char));
	char *nome;
	char *resp;
	int ano, mes, dia;
	Data* data;

	nome = malloc(50*sizeof(char));
	resp = malloc(50*sizeof(char));

	strcpy(nome, "");
	strcpy(resp, "");
	data = criarData(0, 0, 0);

	while (1) {

		clearScreen();
		imprimeCadastroTime(nome, resp, data);
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", opcao);

		switch(*opcao) {

		case '1':
			clearScreen();
			imprimeCadastroTime(nome, resp, data);
			printf("Digite o nome: ");
			scanf(" %[^\n]", nome);
			break;

		case '2':
			clearScreen();
			imprimeCadastroTime(nome, resp, data);
			printf("Digite o responsavel: ");
			scanf(" %[^\n]", resp);
			break;

		case '3':
			clearScreen();
			imprimeCadastroTime(nome, resp, data);
			printf("Digite a data (dd/mm/aaaa): ");
			scanf(" %d/%d/%d", &dia, &mes, &ano);
			data = criarData(ano, mes, dia);
			break;

		case '4':
			if (!strcmp(nome, "") || !strcmp(resp, "")) {

				setMensagemAviso("Informe o nome do time e o responsavel!");

			} else {
				adicionarTime(campeonato_,
									criarTime(nome, resp, data));

				strcpy(nome, "");
				strcpy(resp, "");
				data = criarData(0, 0, 0);

				setMensagemAviso("Time cadastrado com sucesso!");
			}

			break;

		case '0':
			free(opcao);
			free(nome);
			free(resp);
			free(data);
			return;
		}
	}
}
void imprimeCadastroTime(char* nome, char* resp, Data* data) {

	char *aux = (char*)malloc(80*sizeof(char));

	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Times - Cadastro", 'C');
	filledLine();
	imprimeMensagemAviso();
	line("1. Nome do time:", 'L');
	sprintf(aux, "  %s", nome);
	line(aux, 'L');
	emptyLine();
	line("2. Nome do responsavel:", 'L');
	sprintf(aux, "  %s", resp);
	line(aux, 'L');
	emptyLine();
	line("3. Data de criacao:", 'L');
	sprintf(aux, "  %s", data->toString);
	line(aux, 'L');
	emptyLine();
	line("4. Salvar", 'L');
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	line("0. Voltar", 'L');
	emptyLine();
	filledLine();

	free(aux);
}
void menuJogos() {

	char *opcao = (char*)malloc(sizeof(char));

	while (1) {

		clearScreen();
		imprimeMenuJogos();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", opcao);

		switch(*opcao) {

		case '1':
			lerJogosDoArquivo(campeonato_, ARQ_JOGOS_DEFAULT);
			break;

		case '0':
			free(opcao);
			return;
		}
	}
}
void imprimeMenuJogos() {

	char *aux = (char*)malloc(80*sizeof(char));

	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Jogos", 'C');
	filledLine();
	imprimeMensagemAviso();
	sprintf(aux, "1. Carregar do arquivo: %s", ARQ_JOGOS_DEFAULT);
	line(aux, 'L');
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
	sprintf(aux, "Rodadas cadastradas: %d", campeonato_->nRodadas);
	line(aux, 'C');
	emptyLine();
	emptyLine();
	line("0. Voltar", 'L');
	emptyLine();
	filledLine();

	free(aux);
}
void menuTabela() {

	char *opcao = (char*)malloc(sizeof(char));

	while (1) {

		clearScreen();
		imprimeMenuTabela();
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", opcao);

		switch(*opcao) {

		case '1':
			break;

		case '0':
			free(opcao);
			return;
		}
	}
}
void imprimeMenuTabela() {

	filledLine();
	line(NOME_SISTEMA, 'C');
	line("Tabela de classificacao", 'C');
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
	line("0. Voltar", 'L');
	emptyLine();
	filledLine();
}


Campeonato* criarCampeonatoFake() {

	Campeonato* campeonato = criarCampeonato(2019);

	adicionarTime(campeonato, criarTime("Time A", "Jogador A",
			criarData(2007, 2, 5)
	));
	adicionarTime(campeonato, criarTime("Time B", "Jogador B",
			criarData(2008, 3, 6)
	));
	adicionarTime(campeonato, criarTime("Time C", "Jogador C",
			criarData(2009, 4, 7)
	));
	adicionarTime(campeonato, criarTime("Time D", "Jogador D",
			criarData(2010, 5, 8)
	));
//	adicionarTime(campeonato, criarTime("Time E", "Jogador E",
//			criarData(2010, 5, 10)
//	));
//	adicionarTime(campeonato, criarTime("Time F", "Jogador F",
//			criarData(2010, 4, 10)
//	));
//	adicionarTime(campeonato, criarTime("Time G", "Jogador G",
//			criarData(2002, 7, 5)
//	));
//	adicionarTime(campeonato, criarTime("Time H", "Jogador H",
//			criarData(2004, 12, 5)
//	));
//	adicionarTime(campeonato, criarTime("Time I", "Jogador I",
//			criarData(2005, 5, 25)
//	));
//	adicionarTime(campeonato, criarTime("Time J", "Jogador J",
//			criarData(2017, 11, 15)
//	));

	Rodada *rd = criarRodada(1);
	adicionarJogo(rd,
			criarJogo(
					&campeonato->times[0],
					&campeonato->times[1],
					1,
					criarDataHora(2019, 5, 11, 12, 00)
			)
	);
	adicionarJogo(rd,
			criarJogo(
					&campeonato->times[2],
					&campeonato->times[3],
					2,
					criarDataHora(2019, 5, 11, 12, 30)
			)
	);
	adicionarRodada(campeonato, rd);

	rd = criarRodada(2);
	adicionarJogo(rd,
			criarJogo(
					&campeonato->times[0],
					&campeonato->times[2],
					1,
					criarDataHora(2019, 5, 12, 13, 30)
			)
	);
	adicionarJogo(rd,
			criarJogo(
					&campeonato->times[1],
					&campeonato->times[3],
					2,
					criarDataHora(2019, 5, 12, 14, 00)
			)
	);
	adicionarRodada(campeonato, rd);

	rd = criarRodada(3);
	adicionarJogo(rd,
			criarJogo(
					&campeonato->times[0],
					&campeonato->times[3],
					1,
					criarDataHora(2019, 5, 13, 11, 15)
			)
	);
	adicionarJogo(rd,
			criarJogo(
					&campeonato->times[1],
					&campeonato->times[2],
					2,
					criarDataHora(2019, 5, 13, 11, 45)
			)
	);
	adicionarRodada(campeonato, rd);

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






