#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdio_ext.h>

#define NMAXPLAY 20

#define FIFO_SERV "/tmp/fifo_serv"
#define FIFO_CLI "/tmp/fifo_cli"



typedef struct Jogador jogador, *pjogador;
struct Jogador{
	char username[50];	//username do jogador
	int score;		//pontos do jogador
	int n_bombas;		//número de bombas que pode largar
	int p_bombas;		//poder das bombas
	int vidas;		//vida do jogador
	int jid;		//id do processo do jogador
	pjogador prox;		//ponteiro para os outros jogadores
};

typedef struct Labirinto labirinto;
struct Labirinto{
	char **labirinto; 	//lista jogadores em jogo
	int monstros;		//mosntros em jogo
	int pdestr;		//paredes destrutiveis
	int n_object;		//numero de objetos por apanhar
	int n_bombas;		//numero bombas escondidas
	int nivel;		//nivel do jogo
};

typedef struct Mensagem msg;
struct Mensagem{
    char username[50];
    char password[20];
    int login;  //0-> nao existe username/ 1-> pass incorreta/ 2-> login correto;
    int situacao; //0-> nao esta logado // 1-> esta logado
    int pid;
};

typedef struct Resposta resp;
struct Resposta{
    char str[100];
    int log; //0->nao esta logado/ 1-> esta logado
    //int pid;
};
