#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	Time timeA;
	Time timeB;
	int mesa;
	Data data;
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
	sprintf(data->toString, "%d/%d/%d", data->dia, data->mes, data->ano);

	return data;
}
Data* criarDataHora(int ano, int mes, int dia, int hora, int minuto) {

	Data* data = criarData(ano, mes, dia);
	data->temHora = 1;
	data->hora = hora;
	data->minuto = minuto;

	char str[10];
	sprintf(str, " as %d:%d", data->hora, data->minuto);
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
	jogo->timeA = *timeA;
	jogo->timeB = *timeB;
	jogo->mesa = mesa;
	jogo->data = *data;
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

	return camp;
}
Campeonato* adicionarRodada(Campeonato* campeonato, Rodada* rodada) {

	campeonato->rodadas[campeonato->nRodadas] = *rodada;
	campeonato->nRodadas += 1;

	return campeonato;
}

Campeonato* criarCampeonatoFake() {

	Campeonato *campeonato;
	campeonato = criarCampeonato(2019);

	Rodada *rd = criarRodada(1);
	adicionarJogo(rd,
			criarJogo(
					criarTime("Time A", "Jogador A",
							criarData(2010, 05, 10)
					),
					criarTime("Time B", "Jogador B",
							criarData(2008, 07, 07)
					),
					1,
					criarDataHora(2019, 5, 10, 22, 30)
			)
	);
	adicionarJogo(rd,
			criarJogo(
					criarTime("Time C", "Jogador C",
							criarData(2010, 03, 03)
					),
					criarTime("Time D", "Jogador D",
							criarData(2008, 04, 07)
					),
					2,
					criarDataHora(2019, 5, 10, 22, 45)
			)
	);
	adicionarRodada(campeonato, rd);

	rd = criarRodada(2);
		adicionarJogo(rd,
				criarJogo(
						criarTime("Time A", "Jogador A",
								criarData(2010, 05, 10)
						),
						criarTime("Time C", "Jogador C",
								criarData(2008, 07, 07)
						),
						1,
						criarDataHora(2019, 6, 10, 19, 30)
				)
		);
		adicionarJogo(rd,
				criarJogo(
						criarTime("Time B", "Jogador B",
								criarData(2010, 03, 03)
						),
						criarTime("Time D", "Jogador D",
								criarData(2008, 04, 07)
						),
						2,
						criarDataHora(2019, 6, 10, 19, 45)
				)
		);
	adicionarRodada(campeonato, rd);

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
				rodada->jogos[i].timeA.nome,
				rodada->jogos[i].placar->timeA,
				rodada->jogos[i].placar->timeB,
				rodada->jogos[i].timeB.nome,
				rodada->jogos[i].data.toString);
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







