/**
 * 13/05/19 - 20/05/19 - 27/05/19
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
	Time *timeA;
	Time *timeB;
	int mesa;
	Data *data;
	int jaOcorreu;
	Placar *placar;
} Jogo;

// Rodada
typedef struct {
	int numero;
	int nJogos;
	Jogo *jogos;
} Rodada;

// Campeonato
typedef struct {
	int ano;
	int nRodadas;
	Rodada *rodadas;
	int nTimes;
	Time *times;
} Campeonato;

// Assinaturas
Data* criarData(int ano, int mes, int dia);
Data* criarDataHora(int ano, int mes, int dia, int hora, int minuto);
Time* criarTime(char nome[50], char resp[50], Data *fund);
Jogo* criarJogo(Time* timeA, Time* timeB, int mesa, Data* data);
Jogo* informarPlacar(Jogo* jogo, int golsTimeA, int golsTimeB);
Rodada* criarRodada(int numero);
Rodada* adicionarJogo(Rodada* rodada, Jogo* jogo);
Campeonato* criarCampeonato(int ano);
Campeonato* adicionarRodada(Campeonato* campeonato, Rodada* rodada);

// Helpers
Campeonato* criarCampeonatoFake();
Campeonato* autoCriarRodadas(Campeonato* campeonato);
void imprimeCampeonato(Campeonato *campeonato);
void imprimeRodada(Rodada* rodada);
void limparTela();

// Mensagem de aviso
char _mensagem_aviso[255];
void setMensagemAviso(char *msg);
void imprimeMensagemAviso();

// Funcao principal
int main() {

	limparTela();

	imprimeCampeonato(
		criarCampeonatoFake()
	);

	return 0;
}

// Declaracoes
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
Time* criarTime(char nome[50], char resp[50], Data *fund) {

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

	Campeonato *camp = (Campeonato*)malloc(sizeof(Campeonato));
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

Campeonato* criarCampeonatoFake() {

	Campeonato *campeonato;
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

	// Todo
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
						criarDataHora(2019, 6, 12, 13, 30)
				)
		);
		adicionarJogo(rd,
				criarJogo(
						&campeonato->times[1],
						&campeonato->times[3],
						2,
						criarDataHora(2019, 6, 12, 14, 00)
				)
		);
	adicionarRodada(campeonato, rd);

	rd = criarRodada(3);
		adicionarJogo(rd,
				criarJogo(
						&campeonato->times[0],
						&campeonato->times[3],
						1,
						criarDataHora(2019, 6, 13, 11, 15)
				)
		);
		adicionarJogo(rd,
				criarJogo(
						&campeonato->times[1],
						&campeonato->times[2],
						2,
						criarDataHora(2019, 6, 13, 11, 45)
				)
		);
	adicionarRodada(campeonato, rd);
	// Todo fim

	return campeonato;
}
void imprimeCampeonato(Campeonato *campeonato) {

	int i;

	printf("\t\t===== Campeonato %i =====\n", campeonato->ano);
	for (i=0; i<campeonato->nRodadas; i++) {
		imprimeRodada(&campeonato->rodadas[i]);
	}
	printf("\n");
}
void imprimeRodada(Rodada* rodada) {

	int i;

	printf("Rodada %i:\n", rodada->numero);
	for (i=0; i<rodada->nJogos; i++) {
		printf("\tJogo %i: %s [%i X %i] %s - %s \n",
				i+1,
				rodada->jogos[i].timeA->nome,
				rodada->jogos[i].placar->timeA,
				rodada->jogos[i].placar->timeB,
				rodada->jogos[i].timeB->nome,
				rodada->jogos[i].data->toString);
	}
	printf("\n");
}
void limparTela() {
	// TODO fazer um if para caso seja windows (ou nao)
	printf("\e[1;1H\e[2J");
}

void setMensagemAviso(char *msg) {
	strcpy(_mensagem_aviso, msg);
}
void imprimeMensagemAviso() {
	if(strcmp(_mensagem_aviso, "")) {
		printf("%s", _mensagem_aviso);
		strcpy(_mensagem_aviso, "");
	}
}







