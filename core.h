/*
 * core.h
 *
 *  Created on: May 15, 2019
 *      Author: nicolasanelli
 */

#ifndef CORE_H_
#define CORE_H_

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

// Linha
typedef struct {
	Time* time;
	int pontos;
	int vitorias;
	int golsPro;
	int saldo;
	int golsSofr;
} Linha;

// Tabela
typedef struct {
	int nLinhas;
	Linha* linhas;
} Tabela;

// Rodada
typedef struct {
	int numero;
	int nJogos;
	Jogo* jogos;
	Tabela* tabela;
} Rodada;

// Campeonato
typedef struct {
	int ano;
	int nRodadas;
	Rodada* rodadas;
	int nTimes;
	Time* times;
} Campeonato;

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
	sprintf(str, " as %2d:%02d", data->hora, data->minuto);
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
	rodada->tabela = (Tabela*)malloc(sizeof(Tabela));

	return rodada;
}
void adicionarJogo(Rodada* rodada, Jogo* jogo) {

	if(rodada->nJogos >= 10)
		return;

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

	if (campeonato->nTimes >= 20)
		return;

	campeonato->times[campeonato->nTimes] = *time;
	campeonato->nTimes += 1;
	free(time);
}
void adicionarRodada(Campeonato* campeonato, Rodada* rodada) {

	if (campeonato->nRodadas >= 38)
		return;

	campeonato->rodadas[campeonato->nRodadas] = *rodada;
	campeonato->nRodadas += 1;
	free(rodada);

	int i, j;
	Rodada aux;
	for (i=0; i<campeonato->nRodadas; i++) {
		for (j=i+1; j<campeonato->nRodadas; j++) {
			if (campeonato->rodadas[i].numero > campeonato->rodadas[j].numero) {
				aux = campeonato->rodadas[j];
				campeonato->rodadas[j] = campeonato->rodadas[i];
				campeonato->rodadas[i] = aux;
			}
		}
	}
}

Linha* criarLinha(Time* time, int pontos, int vitorias, int golsPro, int saldo, int golsSofridos) {

	Linha* linha = (Linha*)malloc(sizeof(Linha));
	linha->time = time;
	linha->pontos = pontos;
	linha->vitorias = vitorias;
	linha->golsPro = golsPro;
	linha->saldo = saldo;
	linha->golsSofr = golsSofridos;

	return linha;
}
Tabela* criarTabela() {

	Tabela* tabela = (Tabela*)malloc(sizeof(Tabela));
	tabela->nLinhas = 0;
	tabela->linhas = (Linha*)malloc(10*sizeof(Linha));

	return tabela;
}
void adicionarLinha(Tabela* tabela, Linha* linha) {

	tabela->linhas[tabela->nLinhas] = *linha;
	free(linha);
	tabela->nLinhas += 1;

	int i, j;
	Linha aux;
	for (i=0; i<tabela->nLinhas; i++) {
		for (j=i+1; j<tabela->nLinhas; j++) {
			if (tabela->linhas[i].pontos < tabela->linhas[j].pontos) {
				aux = tabela->linhas[j];
				tabela->linhas[j] = tabela->linhas[i];
				tabela->linhas[i] = aux;
			}
		}
	}
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

int obterPontosDoTimeNaRodada(Rodada* rodada, char* nomeDoTime) {

	int i;

	for (i=0; i<rodada->nJogos; i++) {
		if (!strcmp(rodada->jogos[i].timeA->nome, nomeDoTime)) {
			if (!rodada->jogos[i].jaOcorreu) {
				// Nao aconteceu ainda
				return 0;
			}
			if(rodada->jogos[i].placar->timeA > rodada->jogos[i].placar->timeB) {
				// Ganhou
				return 3;
			}
			if(rodada->jogos[i].placar->timeA == rodada->jogos[i].placar->timeB) {
				// Empatou
				return 1;
			}
			if(rodada->jogos[i].placar->timeA < rodada->jogos[i].placar->timeB) {
				// Perdeu
				return 0;
			}
		} else if(!strcmp(rodada->jogos[i].timeB->nome, nomeDoTime)) {
			if (!rodada->jogos[i].jaOcorreu) {
				// Nao aconteceu ainda
				return 0;
			}
			if(rodada->jogos[i].placar->timeB > rodada->jogos[i].placar->timeA) {
				// Ganhou
				return 3;
			}
			if(rodada->jogos[i].placar->timeB == rodada->jogos[i].placar->timeA) {
				// Empatou
				return 1;
			}
			if(rodada->jogos[i].placar->timeB < rodada->jogos[i].placar->timeA) {
				// Perdeu
				return 0;
			}
		}
	}

	return 0;
}
int calcularPontosDoTimeNoCampeonato(Campeonato* campeonato, char* nomeDoTime, int rodada) {

	int i, j, sum=0;

	for (i=0; i<campeonato->nRodadas; i++) {

		if(i>rodada)
			return sum;

		sum += obterPontosDoTimeNaRodada(&campeonato->rodadas[i], nomeDoTime);
	}

	return sum;
}
int verificarSeTimeGanhouNaRodada(Rodada* rodada, char* nomeDoTime) {

	int i;

	for (i=0; i<rodada->nJogos; i++) {
		if (!strcmp(rodada->jogos[i].timeA->nome, nomeDoTime)) {
			if (!rodada->jogos[i].jaOcorreu) {
				// Nao aconteceu ainda
				return 0;
			}
			if(rodada->jogos[i].placar->timeA > rodada->jogos[i].placar->timeB) {
				// Ganhou
				return 1;
			} else {
				return 0;
			}
		} else if(!strcmp(rodada->jogos[i].timeB->nome, nomeDoTime)) {
			if (!rodada->jogos[i].jaOcorreu) {
				// Nao aconteceu ainda
				return 0;
			}
			if(rodada->jogos[i].placar->timeB > rodada->jogos[i].placar->timeA) {
				// Ganhou
				return 1;
			} else {
				return 0;
			}
		}
	}

	return 0;
}
int calcularVitoriasDoTimeNoCamponato(Campeonato* campeonato, char* nomeDoTime, int rodada) {

	int i, j, sum=0;

	for (i=0; i<campeonato->nRodadas; i++) {

		if(i>rodada)
			return sum;

		sum += verificarSeTimeGanhouNaRodada(&campeonato->rodadas[i], nomeDoTime);
	}

	return sum;
}
int obterGolsDoTimeNaRodada(Rodada* rodada, char* nomeDoTime) {

	int i;

	for (i=0; i<rodada->nJogos; i++) {
		if (!strcmp(rodada->jogos[i].timeA->nome, nomeDoTime)) {
			return rodada->jogos[i].placar->timeA;
		} else if(!strcmp(rodada->jogos[i].timeB->nome, nomeDoTime)) {
			return rodada->jogos[i].placar->timeB;
		}
	}

	return 0;
}
int calcularGolsDoTimeNoCampeonato(Campeonato* campeonato, char* nomeDoTime, int rodada) {

	int i, j, sum=0;

	for (i=0; i<campeonato->nRodadas; i++) {

		if(i>rodada)
			return sum;

		sum += obterGolsDoTimeNaRodada(&campeonato->rodadas[i], nomeDoTime);
	}

	return sum;
}
int obterSaldoDoTimeNaRodada(Rodada* rodada, char* nomeDoTime) {

	int i;

	for (i=0; i<rodada->nJogos; i++) {
		if (!strcmp(rodada->jogos[i].timeA->nome, nomeDoTime)) {
			return (rodada->jogos[i].placar->timeA - rodada->jogos[i].placar->timeB);
		} else if(!strcmp(rodada->jogos[i].timeB->nome, nomeDoTime)) {
			return (rodada->jogos[i].placar->timeB - rodada->jogos[i].placar->timeA);
		}
	}

	return 0;
}
int calcularSaldoDoTimeNoCampeonato(Campeonato* campeonato, char* nomeDoTime, int rodada) {

	int i, j, sum=0;

	for (i=0; i<campeonato->nRodadas; i++) {

		if(i>rodada)
			return sum;

		sum += obterSaldoDoTimeNaRodada(&campeonato->rodadas[i], nomeDoTime);
	}

	return sum;
}
int obterGolsSofridosNaRodada(Rodada* rodada, char* nomeDoTime) {

	int i;

	for (i=0; i<rodada->nJogos; i++) {
		if (!strcmp(rodada->jogos[i].timeA->nome, nomeDoTime)) {
			return rodada->jogos[i].placar->timeB;
		} else if(!strcmp(rodada->jogos[i].timeB->nome, nomeDoTime)) {
			return rodada->jogos[i].placar->timeA;
		}
	}

	return 0;
}
int calcularGolsSofridosNoCampeonato(Campeonato* campeonato, char* nomeDoTime, int rodada) {

	int i, j, sum=0;

	for (i=0; i<campeonato->nRodadas; i++) {

		if(i>rodada)
			return sum;

		sum += obterGolsSofridosNaRodada(&campeonato->rodadas[i], nomeDoTime);
	}

	return sum;
}

int quantidadeJogos(Campeonato* campeonato) {

	int i, sum = 0;
	for (i=0; i<campeonato->nRodadas; i++) {
		sum += campeonato->rodadas[i].nJogos;
	}

	return sum;
}

#endif /* CORE_H_ */
