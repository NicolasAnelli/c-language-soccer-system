/**
 * 13/05/19 - 20/05/19 - 27/05/19
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "layout.h"
#include "core.h"

#define ARQ_TIMES_DEFAULT "times.dat"
#define ARQ_JOGOS_DEFAULT "jogos.dat"

#define NOME_SISTEMA "Sistema de Controle de Jogos"

// Funcoes de persistencia
void lerTimesDoArquivo(Campeonato* campeonato, char* filename);
void lerJogosDoArquivo(Campeonato* campeonato, char* filename);
void salvarTimesNoArquivo(Campeonato* campeonato, char* filename);

// Funcoes de aviso
char _mensagem_aviso[255];
void setMensagemAviso(char *msg);
void imprimeMensagemAviso();

// Funcoes de tela
void cabecalho();

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
void telaCadastroJogo();
void imprimeCadastroJogo(char* nomeTimeA, char* nomeTimeB, Data* data, int mesa, int rodada);
void menuTabela();
void imprimeMenuTabela(int nRodada);

// Funcoes de debug
Campeonato* criarCampeonatoFake();
void imprimeCampeonato(Campeonato* campeonato);
void imprimeRodada(Rodada* rodada);
void imprimeTime(Time* time);
void imprimeTabela(Tabela* tabela);

Campeonato* campeonato_;
int main() {

	campeonato_ = criarCampeonato(2019);
	menuPrincipal();

	clearScreen();
	return EXIT_SUCCESS;
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

void cabecalho(char* menu) {
	filledLine();
	line(NOME_SISTEMA, 'C');
	line(menu, 'C');
	filledLine();
	imprimeMensagemAviso();
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

		// Test with files
		case 't':
		case 'T':
			lerTimesDoArquivo(campeonato_, ARQ_TIMES_DEFAULT);
			lerJogosDoArquivo(campeonato_, ARQ_JOGOS_DEFAULT);
			break;

		case '0':
			free(opcao);
			return;
		}
	}
}
void imprimeMenuPrincipal() {
	cabecalho("Menu Principal");
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

	char *aux = (char*)malloc(80*sizeof(char));

	cabecalho("Campeonatos");
	sprintf(aux, "Campeonato %d", campeonato_->ano);
	line(aux, 'C');
	emptyLine();
	sprintf(aux, "   Rodadas: %d", campeonato_->nRodadas);
	line(aux, 'L');
	emptyLine();
	sprintf(aux, "   Times: %d", campeonato_->nTimes);
	line(aux, 'L');
	emptyLine();
	sprintf(aux, "   Jogos: %d", quantidadeJogos(campeonato_));
	line(aux, 'L');
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

	free(aux);
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

	cabecalho("Times");
	sprintf(aux, "1. Carregar do arquivo: %s", ARQ_TIMES_DEFAULT);
	line(aux, 'L');
	emptyLine();
	line("2. Cadastrar novo time", 'L');
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

	cabecalho("Times - Cadastro");
	sprintf(aux, "1. Nome do time:  %s", nome);
	line(aux, 'L');
	emptyLine();
	sprintf(aux, "2. Nome do responsavel:  %s", resp);
	line(aux, 'L');
	emptyLine();
	sprintf(aux, "3. Data de criacao:  %s", data->toString);
	line(aux, 'L');
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	line("4. Salvar", 'L');
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

		case '2':
			telaCadastroJogo();
			break;

		case '0':
			free(opcao);
			return;
		}
	}
}
void imprimeMenuJogos() {

	char *aux = (char*)malloc(80*sizeof(char));

	cabecalho("Jogos");
	sprintf(aux, "1. Carregar do arquivo: %s", ARQ_JOGOS_DEFAULT);
	line(aux, 'L');
	emptyLine();
	line("2. Cadastrar novo jogo", 'L');
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	emptyLine();
	sprintf(aux, "Jogos cadastrados: %d", quantidadeJogos(campeonato_));
	line(aux, 'C');
	emptyLine();
	emptyLine();
	line("0. Voltar", 'L');
	emptyLine();
	filledLine();

	free(aux);
}
void telaCadastroJogo() {

	char *opcao = (char*)malloc(sizeof(char));
	char *msg;
	char *nomeTimeA;
	char *nomeTimeB;
	int ano, mes, dia, hora, minuto;
	int mesa = 0, rodada = 0;
	Data* data;

	nomeTimeA = malloc(50*sizeof(char));
	nomeTimeB = malloc(50*sizeof(char));
	msg = malloc(50*sizeof(char));

	strcpy(nomeTimeA, "");
	strcpy(nomeTimeB, "");
	data = criarDataHora(0, 0, 0, 0, 0);

	while (1) {

		clearScreen();
		imprimeCadastroJogo(nomeTimeA, nomeTimeB, data, mesa, rodada);
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", opcao);

		switch(*opcao) {

		case '1':
			clearScreen();
			imprimeCadastroJogo(nomeTimeA, nomeTimeB, data, mesa, rodada);
			printf("Digite o nome do time A: ");
			scanf(" %[^\n]", nomeTimeA);
			if (!buscarTimePorNome(campeonato_, nomeTimeA)) {
				sprintf(msg, "O time '%s' nao foi encontrado!", nomeTimeA);
				setMensagemAviso(msg);
				strcpy(nomeTimeA, "");
			}
			break;

		case '2':
			clearScreen();
			imprimeCadastroJogo(nomeTimeA, nomeTimeB, data, mesa, rodada);
			printf("Digite o nome do time B: ");
			scanf(" %[^\n]", nomeTimeB);
			if (!buscarTimePorNome(campeonato_, nomeTimeB)) {
				sprintf(msg, "O time '%s' nao foi encontrado!", nomeTimeB);
				setMensagemAviso(msg);
				strcpy(nomeTimeB, "");
			}
			break;

		case '3':
			clearScreen();
			imprimeCadastroJogo(nomeTimeA, nomeTimeB, data, mesa, rodada);
			printf("Digite a data (dd/mm/aaaa hh:mm): ");
			scanf(" %d/%d/%d %d:%d", &dia, &mes, &ano, &hora, &minuto);
			data = criarDataHora(ano, mes, dia, hora, minuto);
			break;

		case '4':
			clearScreen();
			imprimeCadastroJogo(nomeTimeA, nomeTimeB, data, mesa, rodada);
			printf("Digite o numero da mesa: ");
			scanf(" %d", &mesa);
			break;

		case '5':
			clearScreen();
			imprimeCadastroJogo(nomeTimeA, nomeTimeB, data, mesa, rodada);
			printf("Digite o numero da rodada: ");
			scanf(" %d", &rodada);
			break;

		case '6':
			setMensagemAviso("Ainda nao implementado");
			criarJogo(
					buscarTimePorNome(campeonato_, nomeTimeA),
					buscarTimePorNome(campeonato_, nomeTimeB),
					mesa,
					data);
			break;


		case '0':
			free(opcao);
			free(nomeTimeA);
			free(nomeTimeB);
			free(data);
			free(msg);
			return;
		}
	}
}
void imprimeCadastroJogo(char* nomeTimeA, char* nomeTimeB, Data* data, int mesa, int rodada) {

	char *aux = (char*)malloc(80*sizeof(char));

	cabecalho("Jogos - Cadastro");
	sprintf(aux, "1. Nome do time A:  %s", nomeTimeA);
	line(aux, 'L');
	emptyLine();
	sprintf(aux, "2. Nome do Time B: %s", nomeTimeB);
	line(aux, 'L');
	emptyLine();
	sprintf(aux, "3. Data: %s", data->toString);
	line(aux, 'L');
	emptyLine();
	sprintf(aux, "4. Mesa: %d", mesa);
	line(aux, 'L');
	emptyLine();
	sprintf(aux, "5. Rodada: %d", rodada);
	line(aux, 'L');
	emptyLine();
	emptyLine();
	emptyLine();
	line("6. Salvar", 'L');
	emptyLine();
	emptyLine();
	line("0. Voltar", 'L');
	emptyLine();
	filledLine();

	free(aux);
}
void menuTabela() {

	int nRodada = 0;
	char *opcao = (char*)malloc(sizeof(char));

	while (1) {

		clearScreen();
		imprimeMenuTabela(nRodada);
		printf("Digite sua opcao: ");
		scanf(" %[^\n]", opcao);

		switch(*opcao) {

		case '1':
			if(nRodada > 0)
				nRodada--;
			break;

		case '2':
			if(nRodada < (campeonato_->nRodadas-1))
				nRodada++;
			break;

		case '0':
			free(opcao);
			return;
		}
	}
}
void imprimeMenuTabela(int nRodada) {

	int i;
	char* aux = (char*)malloc(80*sizeof(char));

	Tabela* tabela = criarTabela();
	for (i=0; i<campeonato_->nTimes; i++) {
		adicionarLinha(tabela,
				criarLinha(&campeonato_->times[i],
				calcularPontosDoTimeNoCampeonato(campeonato_,campeonato_->times[i].nome, nRodada)));
	}

	cabecalho("Tabela de classificacao");
	sprintf(aux, "Rodada %d", nRodada+1);
	line(aux, 'C');
	emptyLine();
	for (i=0; i<10; i++) {
		if (campeonato_->nTimes > i) {
			sprintf(aux, "  %02d | %02d .....  %s", i+1, tabela->linhas[i].pontos, tabela->linhas[i].time->nome);
			line(aux, 'L');
		} else {
			emptyLine();
		}
	}
	emptyLine();
	line("1 - Anterior | Proxima - 2", 'C');
	emptyLine();
	line("0. Voltar", 'L');
	emptyLine();
	filledLine();

	free(aux);
	free(tabela);
}

// Fake||Debug
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

	rd = criarRodada(3);
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

	rd = criarRodada(2);
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
void imprimeTabela(Tabela* tabela) {

	int i;
	printf("Tabela: \n");
	for (i=0; i<tabela->nLinhas; i++) {
		printf("\t %s - %d\n", tabela->linhas[i].time->nome, tabela->linhas[i].pontos);
	}
	printf("\n");
}





