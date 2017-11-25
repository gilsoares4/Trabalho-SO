#include "dados.h"

int fr;
int fw;
int pid_cli[NMAXPLAY];
int n_log=0;

void  termina(){
   int i;

   for(i=0; i<NMAXPLAY; i++)
        kill(pid_cli[i], SIGUSR1);
}

void desligaServidor(int sig){

    if(sig == SIGINT){
        printf("\nA terminar Servidor...\n");
        termina();
        close(fr);
        unlink(FIFO_SERV);
        exit(EXIT_SUCCESS);
        }
}

/*void desligaCliente(int sig){
    int i, j;
    int pid_aux[NMAXPLAY];
    int pid_eli;


    for(i=0; i<NMAXPLAY; i++){
        if(pid_cli[i] == ){
            kill(pid_cli[i], SIGUSR1);
            n_log--;
            }
        else{
            pid_cli[i]=pid_aux[j];
            j++;
        }
    }
    for(i=0; i<NMAXPLAY; i++);
        pid_cli[i] = pid_aux[i];
}*/

int verifica_ficheiro(char *nome_fich){
	int fr = open(nome_fich, O_RDONLY);
	if(fr == -1)
		return 0;
	else
        close(fr);
    return 1;
}

int verifica_jogador(char *n, char *nome_fich){
	char c;
	char utilizador[50];
	char nome[20];
	int lido, i;

	int fr = open(nome_fich, O_RDONLY);
	if(fr == -1){
		perror("No ficheiro para leitura");
		exit(EXIT_FAILURE);
		}
    do{
        i = 0;

        do{
            lido = read(fr, &c, 1);
            if(lido < 0){
                perror("No ficheiro para escrita");
                exit (0);
            }
            utilizador[i] = c;
            i++;
        }
        while(c != '\n');
        utilizador[i] = '\0';

        i = 0;

        while(utilizador[i] != ' '){
            nome[i] = utilizador[i];
            i++;
            }
            nome[i] = '\0';

        if(strcmp(nome, n) == 0){
            close(fr);
            return 0;   //mesmo nome
        }
	}
	while(lido>0);

	close(fr);
	return 1;	//nome diferente

}

void primeiro_jogador(char *n, char *pass, char *nome_fich){
    int i = 0;
    char c = ' ';
    char p = '\n';
	int fw = open(nome_fich, O_WRONLY | O_CREAT, 0744);
	if(fw == -1){
		perror("No ficheiro para escrita");
		close(fw);
		exit(EXIT_FAILURE);
	}

	while(n[i] != '\0')
        i++;
    write(fw, n, i);
	write(fw, &c, 1);

	i = 0;
	while(pass[i] != '\0')
        i++;
    write(fw, pass, i);
	write(fw, &p, 1);

	close(fw);

}

void novo_jogador(char n[], char pass[], char *nome_fich){
    int i = 0;
    char c = ' ';
    char p = '\n';
	int fw = open(nome_fich, O_WRONLY | O_APPEND, 0744);
	if(fw == -1){
		perror("No ficheiro para escrita");
		exit(EXIT_FAILURE);
	}

	while(n[i] != '\0')
        i++;
    write(fw, n, i);
	write(fw, &c, 1);

	i = 0;
	while(pass[i] != '\0')
        i++;
    write(fw, pass, i);
	write(fw, &p, 1);

	close(fw);
}

void mostra_jogadores(char *nome_fich){
	int i, j, lido;
	char c;
	char linha[100];
	char nome[50];
	char password[20];

	int fr = open(nome_fich, O_RDONLY);
	if(fr == -1){
		perror("No ficheiro para leitura");
		exit(EXIT_FAILURE);
	}
	printf("\nNome\tPassword\n");

	do{
        i = 0;

        do{
            lido = read(fr, &c, 1);
            if(lido < 0){
                printf("No ficheiro para leitura");
                exit(EXIT_FAILURE);
            }
            linha[i] = c;
            i++;
        }
        while(c != '\n');
        linha[i] = '\0';

        i = 0;

        while(linha[i] != ' '){
            nome[i] = linha[i];
            i++;
            }
            nome[i] = '\0';

            i++;
            j=0;

        while(linha[i] != '\0'){
            password[j] = linha[i];
            i++;
            j++;
            }
            password[j] = '\0';
            if(nome[0]!= '\n')
                printf("%s\t%s", nome, password);
	}
	while(lido > 0);
	close(fr);

}

char **crialab(char **labirinto, char*nome_fich){
    char c, num[3];
    int lin, col, i, j, lido;
    int fr = open(nome_fich, O_RDONLY);
	if(fr == -1){
		perror("No ficheiro para leitura");
		exit(EXIT_FAILURE);
	}
    i = 0;

    do{
        lido = read(fr, &c, 1);
        if(lido < 0){
                    perror("No ficheiro para leitura");
                    exit(EXIT_FAILURE);
                }
        num[i] = c;
        i++;
    }while(c!=' ');
    lin = atoi(num);

    i = 0;
    do{
        lido = read(fr, &c, 1);
        if(lido < 0){
                    perror("No ficheiro para leitura");
                    exit(EXIT_FAILURE);
                }
        num[i] = c;
        i++;
    }while(c!='\n');
    col = atoi(num);

	printf("Linhas: %d\t Colunas: %d\n\t", lin, col);

	labirinto = malloc(lin * sizeof(char));

	if(labirinto != NULL){
        for(i=0; i < lin; i++){
            labirinto[i] = malloc(sizeof(char) * col);
            if(labirinto[i] == NULL){
                perror("Na alocação de memória");
                exit(EXIT_FAILURE);
                }
            }
        }
	else{
        printf("Erro a alocar memoria!\n Labirinto nao foi criado");
        exit(EXIT_FAILURE);
        }

    read(fr, labirinto, lin*col);

    printf("\nMapa carregado\n\n");

    printf("%s\n", labirinto);
    close(fr);
	return labirinto;

}

void info_jogo(){

	printf("Deverá imprimir toda a informação de jogo...\n");
}

int login(char *username, char *password, char *nomefich){
    char utilizador[50];
    char pass[20];
    char c;
    int lido, i, j, flag=-1;
	char linha[100];

	int fr = open(nomefich, O_RDONLY);
	if(fr == -1){
		perror("No ficheiro para leitura");
		exit(EXIT_FAILURE);
		}

    do{
        i = 0;

        do{
            lido = read(fr, &c, 1);
            if(lido < 0){
                perror("No ficheiro para leitura");
                exit(EXIT_FAILURE);
            }
            linha[i] = c;
            i++;
        }
        while(c != '\n');
        linha[i] = '\0';

        i = 0;

        while(linha[i] != ' '){
            utilizador[i] = linha[i];
            i++;
            }
            utilizador[i] = '\0';
            i++;
            j=0;

        while(linha[i] != '\0'){
            pass[j] = linha[i];
            i++;
            j++;
            }
            pass[j-1] = '\0';
            if(utilizador[0]!= '\n'){

                if(strcmp(utilizador, username) == 0){
                    if(strcmp(pass, password) == 0){
                        close(fr);
                        return 2;
                    }else{
                        close(fr);
                        return 1;
                    }

                }
             }

    }while(lido>0);

	close(fr);

	return 0;
}

int main(int argc,char *argv[]){
    msg men;
    resp res;
	char cmd[20];
	char arg1[20];
	char arg2[20];
	char **labirinto = NULL;

	signal(SIGINT, desligaServidor);    //terminar servidor quando faz CTRL+C
//	signal(SIGUSR1,desligaCliente);    //terminar cliente quando faz CTRL+C

	mkfifo(FIFO_SERV, 0777);    //criar fifo servidor

	if(argc != 2){
		printf("[ERRO] Numero de argumentos inválido...\n");
		kill(getpid(), SIGINT);
	}

	if(strcmp(argv[1], "login.txt") != 0){
		printf("[ERRO] O ficheiro %s não existe...\n", argv[1]);
		kill(getpid(), SIGINT);
	}

    fr = open(FIFO_SERV, O_RDONLY);     //Abrir fifo cliente para leitura
    if(fr == -1){
        perror("A abrir o fifo servidor");
        unlink(FIFO_SERV);
        exit(EXIT_FAILURE);
    }

    fw = open(FIFO_CLI, O_WRONLY);      //Abrir fifo servidor para resposta
    if(fw == -1){
        perror("A abrir o fifo cliente");
        unlink(FIFO_CLI);
        exit(EXIT_FAILURE);
    }
    printf("Em espera...\n");
	read(fr, &men, sizeof(men));

    do{
        if(men.situacao == 0){
            men.login = login(men.username, men.password, argv[1]);
            if(men.login == 0 || men.login == 1)
                res.log = 0;
            else
                res.log = 1;

            if(men.login == 0){
                strcpy(res.str, "Username introduzido não existe...\n");
                write(fw, &res, sizeof(res));
                }
            else if(men.login == 1){
                strcpy(res.str, "Password introduzida está incorreta...\n");
                write(fw, &res, sizeof(res));
            }else if(men.login == 2){
                strcpy(res.str, "Login efetuado com sucesso...\n");
                write(fw, &res, sizeof(res));
                men.situacao = 1;
                pid_cli[n_log] = men.pid;
                n_log++;

                printf("Jogador %s entrou no jogo...\n", men.username);
            }
        }
        else{
            strcpy(res.str, "O username introduzido já se encontra logado...\n");
            write(fw, &res, sizeof(res));
        }

        read(fr, &men, sizeof(men));
    }while(1);


	do{
		printf("\nComando >> ");
		scanf("%s", cmd);

		if(strcmp(cmd, "add") == 0){
            if(stdin == NULL)
                printf("Argumentos insuficientes\n");
				scanf("%s %s", arg1, arg2);
				if(arg1 == NULL || arg2 == NULL)
					printf("Argumentos insuficientes\n");
				else{
					if(verifica_ficheiro(argv[1]) == 0){
						primeiro_jogador(arg1, arg2, argv[1]);
						printf("Primeiro Jogador adicionado com sucesso...\n");
					}
					else{
						if(verifica_jogador(arg1, argv[1])){
							novo_jogador(arg1, arg2, argv[1]);
							printf("Jogador adicionado com sucesso...\n");
						}else
							printf("[ERRO] Username já existe...\n");
					    }
				    }
		}else if(strcmp(cmd, "users") == 0){
			mostra_jogadores(argv[1]);

		}else if(strcmp(cmd, "kick") == 0){
			scanf("%s", arg1);
			if(arg1 == NULL)
				printf("Argumentos insuficientes\n");
			else
				printf("Falta implementar...\n");

		}else if(strcmp(cmd, "game") == 0){
			info_jogo();

		}else if(strcmp(cmd, "shutdown") == 0){
			kill(getpid(), SIGINT);
        }else if(strcmp(cmd, "map") == 0){
            scanf("%s", arg1);
			if(arg1 == NULL)
				printf("Argumentos insuficientes\n");
            else{
                if(verifica_ficheiro(arg1)==1)
					labirinto = crialab(labirinto, arg1);
				else
					printf("Ficheiro invalido!\n");
            }

		}else
			printf("[ERRO] Comando inválido...\n");
		__fpurge(stdin);

	}while(1);

    free(labirinto);

	exit(0);
}
